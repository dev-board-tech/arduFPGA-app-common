/*
 * lib/crypto/aes.h
 */ 


#ifndef AES_H_
#define AES_H_

//#define CBC 1

unsigned char aes128_getSBoxValue(unsigned char num);
unsigned char aes128_getSBoxInvert(unsigned char num);
void aes128_KeyExpansion(void);
void aes128_AddRoundKey(unsigned char round);
void aes128_SubBytes(void);
void aes128_ShiftRows(void);
unsigned char aes128_xtime(unsigned char x);
void aes128_MixColumns(void);
unsigned char aes128_Multiply(unsigned char x, unsigned char y);
void aes128_InvMixColumns(void);
void aes128_InvSubBytes(void);
void aes128_InvShiftRows(void);
void aes128_Cipher(void);
void aes128_InvCipher(void);
void aes128_BlockCopy(unsigned char* output, unsigned char* input);
void aes128_XorWithIv(unsigned char* buf);
unsigned char* aes128_Iv;

void AES128_ECB_encrypt(unsigned char* input, unsigned char* key, unsigned char* output);
void AES128_ECB_decrypt(unsigned char* input, unsigned char* key, unsigned char *output);
void AES128_CBC_encrypt_buffer(unsigned char* output, unsigned char* input, unsigned int length, unsigned char* key, unsigned char* iv);
void AES128_CBC_decrypt_buffer(unsigned char* output, unsigned char* input, unsigned int length, unsigned char* key, unsigned char* iv);

/*#####################################################*/
#endif /* AES_H_ */
