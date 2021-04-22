/*
 * base58.h
 *
 * Created: 04.12.2020 20:18:16
 *  Author: MorgothCreator
 */ 


#ifndef BASE58_H_
#define BASE58_H_


int EncodeBase58(const unsigned char *source, int len, unsigned char result[], int reslen);
int DecodeBase58(const unsigned char *str, int len, unsigned char *result);



#endif /* BASE58_H_ */