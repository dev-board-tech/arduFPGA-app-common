#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdint.h>
#include <stdbool.h>
#include "delay.h"

#include "driver/spi.h"
#include "gfx/draw.h"

#include "fat_fs/inc/ff.h"

typedef uint16_t textEditorPtr;

typedef struct textEditorFile_s{
	textEditorPtr cursorPos;
	textEditorPtr winY;
	int16_t winX;
	uint16_t lineCnt;
	char *fileName;
	bool edited;
}textEditorFile_t;

typedef enum {
	TEXT_EDITOR_ERR_OK,
	TEXT_EDITOR_ERR_OPEN_ASM_FILE,
	TEXT_EDITOR_ERR_CREATE_TMP_FILE,
	TEXT_EDITOR_ERR_ALLOC_CONVERT_LINE,
	TEXT_EDITOR_ERR_READ,
	TEXT_EDITOR_ERR_WRITE,
	TEXT_EDITOR_ERR_INVALID_FORMAT,
	TEXT_EDITOR_ERR_UNABLE_TO_WRITE_TMP_FILE,
	TEXT_EDITOR_ERR_OUT_OF_MEMORY
}textEditor_Err_e;

typedef struct textEditor_s{
	bool menuOpen;
	bool textEditMode;
	enum {
		EDITOR_STATE_EDIT,
		EDITOR_STATE_KBD,
		EDITOR_STATE_MENU
	}editorState;
	textEditorFile_t **files;
	textEditorPtr filePtr;
	textEditorPtr maxFileLen;
	textEditorPtr cursorPos;
	int16_t winX;
	textEditorPtr winY;
	bool edited;
	bool cursorState;
	timer_t timer;
	gfxString_t gfxString;
	box_t box;
	uint8_t activeFile;
	uint16_t lineCnt;
	FIL file;
	bool busy;
}textEditor_t;

void textEditor_Init(textEditor_t *inst);
void textEditor_DeInit(textEditor_t *inst);

void textEditor_Idle(textEditor_t *inst);

textEditor_Err_e addFile(textEditor_t *inst, char *fileName);
textEditor_Err_e changeFile(textEditor_t *inst, uint8_t fileNr);

void textEditor_edit(textEditor_t *inst, uint8_t c);
void textEditor_goLeft(textEditor_t *inst);
void textEditor_goRight(textEditor_t *inst);
void textEditor_goUp(textEditor_t *inst);
void textEditor_goDown(textEditor_t *inst);

#endif // TEXT_EDITOR_H
