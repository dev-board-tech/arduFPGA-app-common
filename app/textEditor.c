
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"
#include "textEditor.h"
#include DISPLAY_DRIVER_FILE
#include "util.h"


static int _drawStringWindowed4x6(textEditor_t *inst, box_t *box, textEditorPtr string, int16_t x, int16_t y, int16_t cursorPos, bool cursorState);

void textEditor_Init(textEditor_t *inst) {
    inst->files = (textEditorFile_t **)calloc(1, sizeof(textEditorFile_t *));
    inst->busy = false;
    inst->winX = 0;
    inst->winY = 0;
    inst->cursorPos = 0;
    inst->edited = true;
    inst->cursorState = false;
    inst->box.x_min = 0;
    inst->box.y_min = 0;
    inst->box.x_max = DISPLAY_FUNC_GET_X();
    inst->box.y_max = DISPLAY_FUNC_GET_Y();
    inst->timer.value = 700;
    inst->lineCnt = 0;
    timer_enable(&inst->timer, true);
}

void textEditorDeInit(textEditor_t *inst) {
    f_close(&inst->file);
    if(inst->files) {
        textEditorFile_t *ptr = inst->files[0];
        for(; ptr != NULL; ptr++) {
            free(ptr);
            ptr = NULL;
        }
        free(inst->files);
        inst->files = NULL;
    }
}

static bool write(textEditor_t *inst, textEditorPtr addr, uint8_t data) {
    return ((uint8_t *)inst->filePtr)[addr] = data;
}

static void writeChunk(textEditor_t *inst, textEditorPtr addr, uint8_t len, char *buf) {
    memcpy(((uint8_t *)inst->filePtr) + addr, buf, len);
}

static uint8_t read(textEditor_t *inst, textEditorPtr addr) {
    return ((uint8_t *)inst->filePtr)[addr];
}

static void readChunk(textEditor_t *inst, textEditorPtr addr, uint8_t len, char *buf) {
    memcpy(buf, ((uint8_t *)inst->filePtr) + addr, len);
}

static void resetCursorState(textEditor_t *inst) {
    inst->edited = true;
    inst->cursorState = true;
    timer_enable(&inst->timer, true);
}

static textEditorPtr getNextLine(textEditor_t *inst, textEditorPtr cursorPos);
static textEditorPtr getPreviousLine(textEditor_t *inst, textEditorPtr cursorPos);
static void moveString(textEditor_t *inst, textEditorPtr from, textEditorPtr to);

static void paintWindow(textEditor_t *inst) {
    DISPLAY_FUNC_CLEAR(NULL, inst->gfxString.vram, 0);
    inst->gfxString.transparent = true;
    box_t bTmp = inst->box;
    uint8_t lineNrSpace = 0;
    if(!inst->textEditMode) {
        bTmp.x_min += getCharWidth4x6() * 6;
        lineNrSpace = 6;
    }
    _drawStringWindowed4x6(inst, &bTmp, inst->winY, inst->winX + (lineNrSpace * getCharWidth4x6()), 0, inst->cursorPos - inst->winY, inst->cursorState);
    if((inst->gfxString.edgeTouch & EDGE_OUTSIDE_LEFT) && inst->winX + getCharWidth4x6() <= 0) {
        inst->winX += getCharWidth4x6();
        inst->edited = true;
    }
    if(inst->gfxString.edgeTouch & EDGE_OUTSIDE_RIGHT) {
        inst->winX -= getCharWidth4x6();
        inst->edited = true;
    }
    if(inst->gfxString.edgeTouch & EDGE_OUTSIDE_UP || inst->cursorPos <= inst->winY) {
        inst->winY = getPreviousLine(inst, inst->winY);
        inst->lineCnt--;
        inst->edited = true;
    }
    if(inst->gfxString.edgeTouch & EDGE_OUTSIDE_DOWN) {
        inst->winY = getNextLine(inst, inst->winY);
        inst->lineCnt++;
        inst->edited = true;
    }
    if(!inst->winY) {
        inst->lineCnt = 0;
    }
    inst->gfxString.transparent = false;
    _drawStringWindowed4x6(inst, &bTmp, inst->winY, inst->winX + (lineNrSpace * getCharWidth4x6()), 0, inst->cursorPos - inst->winY, inst->cursorState);

    if(!inst->textEditMode) {
        bTmp = inst->box;
        bTmp.x_max = getCharWidth4x6() * 6;
        char mpS[7];
        int t = inst->lineCnt + 1;
        for(uint16_t cnt = 0; cnt < DISPLAY_FUNC_GET_Y() ; cnt += getCharHeight4x6()) {
            char *mpSp = mpS;
            //sprintf(mpS, "%05d|", t);
            if(t < 10000)
                *mpSp++ = '0';
            if(t < 1000)
                *mpSp++ = '0';
            if(t < 100)
                *mpSp++ = '0';
            if(t < 10)
                *mpSp++ = '0';
            util_utoa(t, mpSp);
            mpS[5] = '|';
            mpS[6] = '\0';
            drawStringWindowed4x6(&inst->gfxString, &bTmp, mpS, 0, cnt, -1, false);
            t++;
        }
    }

    DISPLAY_REFRESH(NULL, inst->gfxString.vram);
    inst->busy = false;
}

void textEditor_Idle(textEditor_t *inst) {
    if(inst->edited) {
        inst->edited = false;
        paintWindow(inst);
    }
    if(timer_tik(&inst->timer)) {
        if(inst->cursorState)
            inst->cursorState = false;
        else
            inst->cursorState = true;
        inst->edited = true;
    }
}

textEditor_Err_e addFile(textEditor_t *inst, char *fileName) {
    textEditorFile_t **ptr = inst->files;
    int fileCnt = 0;
    for(; ptr[fileCnt] != NULL; fileCnt++) {}
    ptr = (textEditorFile_t **)malloc(sizeof(textEditorFile_t *) * (fileCnt + 1));
    if(!ptr)
        return TEXT_EDITOR_ERR_OUT_OF_MEMORY;
    memcpy(ptr, inst->files, sizeof(textEditorFile_t *) * fileCnt);
    //free(inst->files);
    inst->files = ptr;
    ptr[fileCnt] = NULL;
    ptr[fileCnt + 1] = NULL;
    textEditorFile_t *fil = (textEditorFile_t *)calloc(1, sizeof(textEditorFile_t));
    if(!fil)
        return TEXT_EDITOR_ERR_OUT_OF_MEMORY;
    ptr[fileCnt] = fil;
    char *fName = (char *)calloc(1, /*strlen(fileName) + 1*/36);
    if(!fName)
        return TEXT_EDITOR_ERR_OUT_OF_MEMORY;
    //strncpy(fName, fileName, 17);
    memcpy(fName, fileName, 34);
    fil->fileName = fName;
    return TEXT_EDITOR_ERR_OK;
}

textEditor_Err_e changeFile(textEditor_t *inst, uint8_t fileNr) {
    char chunk[16];
    UINT bw;
    FRESULT res = FR_OK;
    if(inst->activeFile != (uint8_t)-1 && inst->activeFile != fileNr && inst->files[inst->activeFile]->edited) {
        // Restore the asm file from the temporary file in order to be compatible with PC compilers and be readable.
        res = f_open(&inst->file, (TCHAR *)inst->files[inst->activeFile]->fileName, FA_WRITE | FA_OPEN_ALWAYS);
        if(res != FR_OK)
            return TEXT_EDITOR_ERR_OPEN_ASM_FILE;
        // Search the end of the text in the temporary file.
        int32_t p = inst->maxFileLen;
        char *pp;
        do {
            p -= sizeof(chunk);
            readChunk(inst, p, sizeof(chunk), chunk);
            pp = chunk + sizeof(chunk);
            for(; pp > chunk; ) {
                pp--;
                if(*pp != '\0')
                    break;
            }
        } while(pp == chunk && p > 0);
        // Copy the content of the temporary file to the open file.
        int32_t end = p + sizeof(chunk);
        for(p = 0; p < end; p+= sizeof(chunk)) {
            readChunk(inst, p, sizeof(chunk), chunk);
            res = f_write(&inst->file, chunk, strnlen(chunk, sizeof(chunk)), &bw);
            if(res != FR_OK) {
				f_close(&inst->file);
                return TEXT_EDITOR_ERR_WRITE;
            }
        }
        f_truncate(&inst->file);
		f_close(&inst->file);
        inst->files[fileNr]->cursorPos = inst->cursorPos;// Save the cursor and window position.
        inst->files[fileNr]->winX= inst->winX;
        inst->files[fileNr]->winY = inst->winY;
        inst->files[fileNr]->lineCnt = inst->lineCnt;
        inst->files[inst->activeFile]->edited = false;
    }
    if(inst->activeFile != (uint8_t)-1 || inst->activeFile != fileNr) {
        // Copy the edited file to a temporary file and make each line "maxTmpLineLen" characters long with the difference filled with space characters.
        // This way we will avoid shifting the right part of the rest of the file to make space for new characters, but the maximum line len will be limited by "maxTmpLineLen".
        if(f_open(&inst->file, (TCHAR *)inst->files[fileNr]->fileName, FA_READ | FA_OPEN_ALWAYS) != FR_OK)
            return TEXT_EDITOR_ERR_OPEN_ASM_FILE;
        memset(chunk, 0, sizeof(chunk));
        for(textEditorPtr p = 0; p < inst->maxFileLen; p += sizeof(chunk))
            writeChunk(inst, p, sizeof(chunk), chunk);
        inst->activeFile = fileNr;
        UINT br;
        textEditorPtr p = 0;
        do {
            res = f_read(&inst->file, chunk, sizeof(chunk), &br);
            if(res != FR_OK) {
				f_close(&inst->file);
                return TEXT_EDITOR_ERR_READ;
            }
            if(!br)
                break;
            writeChunk(inst, p, br, chunk);
            p += br;
        } while(p < inst->maxFileLen);
		f_close(&inst->file);
        inst->cursorPos = inst->files[fileNr]->cursorPos; // Restore the cursor and window position.
        inst->winX = inst->files[fileNr]->winX;
        inst->winY = inst->files[fileNr]->winY;
        inst->lineCnt = inst->files[fileNr]->edited;
    }
    return TEXT_EDITOR_ERR_OK;
}

static bool isNewLine(textEditor_t *inst, textEditorPtr ptr) {
    if(read(inst, ptr) == '\n')
        return true;
    return false;
}

static void moveString(textEditor_t *inst, textEditorPtr from, textEditorPtr to) {
    char chunk[16];
    if(from < to) {
	    int8_t l = sizeof(chunk);
	    textEditorPtr sp = inst->maxFileLen - (to - from);
	    textEditorPtr dp = inst->maxFileLen;
	    do {
		    if(sp < from + sizeof(chunk))
				l = sp - from;
		    if(l == 0)
				break;
		    sp -= l; dp -= l;
		    readChunk(inst, sp, l, chunk);
		    writeChunk(inst, dp, l, chunk);
	    }while (1);
		inst->files[inst->activeFile]->edited = true;
	} else if(from > to) {
	    uint8_t l = sizeof(chunk);
	    textEditorPtr sp = from, dp = to;
	    do {
		    if(sp + l > inst->maxFileLen - 1)
				l = inst->maxFileLen - 1 - sp;
		    if(l == 0)
				break;
		    readChunk(inst, sp, l, chunk);
		    writeChunk(inst, dp, l, chunk);
		    sp += l; dp += l;
	    } while(1);
		inst->files[inst->activeFile]->edited = true;
   }
}

void textEditor_edit(textEditor_t *inst, uint8_t c) {
    if(!inst->busy) {
        inst->busy = true;
        switch(c) {
            case 0x08://Backspace 0x08
                if(inst->cursorPos == 0)
                    return;
                inst->cursorPos -= 1;
                moveString(inst, inst->cursorPos + 1, inst->cursorPos);
                resetCursorState(inst);
                break;
            case 0x07://Delete 0x07
                moveString(inst, inst->cursorPos + 1, inst->cursorPos);
                resetCursorState(inst);
                break;
            default://The rest
                moveString(inst, inst->cursorPos, inst->cursorPos + 1);
                write(inst, inst->cursorPos, c);
                inst->cursorPos += 1;
                resetCursorState(inst);
                break;
        }
    }
}

static textEditorPtr getNextLine(textEditor_t *inst, textEditorPtr cursorPos) {
    for (textEditorPtr cnt = cursorPos; cnt < inst->maxFileLen; cnt++) {
        if(isNewLine(inst, cnt))
            return cnt + 1;
    }
    for (textEditorPtr cnt = cursorPos; cnt > 0; cnt--) {
        if(isNewLine(inst, cnt))
            return cnt + 1;
    }
    return 0;
}

static textEditorPtr getPreviousLine(textEditor_t *inst, textEditorPtr cursorPos) {
    textEditorPtr cnt;
    for (cnt = cursorPos; cnt > 0; cnt--) {
        if(isNewLine(inst, cnt))
            break;
    }
    if(!cnt)
        return 0;
    if(cnt == cursorPos)
        cnt--;
    for (cnt = cnt - 1; cnt > 0; cnt--) {
        if(isNewLine(inst, cnt))
            return cnt + 1;
    }
    return 0;
}

void textEditor_goLeft(textEditor_t *inst) {
    if(!inst->busy) {
        inst->busy = true;
        if(inst->cursorPos > 0)
            inst->cursorPos--;
        else
            return;
        resetCursorState(inst);
    }
}

void textEditor_goRight(textEditor_t *inst) {
    if(!inst->busy) {
        inst->busy = true;
        if(inst->cursorPos < inst->maxFileLen && read(inst, inst->cursorPos) != 0)
            inst->cursorPos++;
        else
            return;
        resetCursorState(inst);
    }
}

void textEditor_goUp(textEditor_t *inst) {
    if(!inst->busy) {
        inst->busy = true;
        textEditorPtr prevLine = getPreviousLine(inst, inst->cursorPos);
        inst->cursorPos = prevLine;
        resetCursorState(inst);
    }
}

void textEditor_goDown(textEditor_t *inst) {
    if(!inst->busy) {
        inst->busy = true;
        textEditorPtr nextLine = getNextLine(inst, inst->cursorPos);
        inst->cursorPos = nextLine;
        resetCursorState(inst);
    }
}

static int _drawStringWindowed4x6(textEditor_t *inst, box_t *box, textEditorPtr string, int16_t x, int16_t y, int16_t cursorPos, bool cursorState) {
    box_t box__;
    if(box) {
        box__.x_min = box->x_min;
        box__.x_max = box->x_max;
        box__.y_min = box->y_min;
        box__.y_max = box->y_max;
    } else {
        box__.x_min = 0;
        box__.x_max = 128;
        box__.y_min = 0;
        box__.y_max = 64;
    }
    int16_t cX = x;
    int16_t cY = y;
    int16_t cCnt = 0;
    inst->gfxString.edgeTouch = 0;
    textEditorPtr str = string;
    do {
        if(cCnt == inst->gfxString.maxLen && inst->gfxString.maxLen)
            return cCnt;
        if(cursorPos == cCnt) {
            if(cX < box__.x_min)
                inst->gfxString.edgeTouch |= EDGE_OUTSIDE_LEFT;
            if(cY < box__.y_min)
                inst->gfxString.edgeTouch |= EDGE_OUTSIDE_UP;
            if(cX >= box__.x_max)
                inst->gfxString.edgeTouch |= EDGE_OUTSIDE_RIGHT;
            if(cY >= box__.y_max - 6)
                inst->gfxString.edgeTouch |= EDGE_OUTSIDE_DOWN;
        }
        char C, c = C = read(inst, str);
        if(cursorPos == cCnt) {
            if((cursorState) || (cursorState && C == 0))
                C = (uint8_t)(96+32);
        }
        if(!inst->gfxString.transparent)
            drawChar4x6(inst->gfxString.spi, box, inst->gfxString.vram, cX, cY, C, inst->gfxString.inkColor);
        if (c == 0)
            return cCnt - 1;
        switch (c) {
        case '\n':
            if(cY > box__.y_max)
                return cCnt;
            cX = x;
            cY += 6;
            break;
        case 0x09:
            cX = (4 * inst->gfxString.tabSpaces) + ((cX / (4 * inst->gfxString.tabSpaces)) * (4 * inst->gfxString.tabSpaces));
            break;
        default:
            cX += 4;
            break;
        }
        str++;
        cCnt++;
    } while (1);
}
