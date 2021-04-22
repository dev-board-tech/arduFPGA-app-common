
#ifdef __AVR_MEGA__
#include <avr/pgmspace.h>
const char BASE58_ALPHABET[59] PROGMEM = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
#else
const char BASE58_ALPHABET[59] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
#endif



#ifdef __AVR_MEGA__
const char BASE58_ALPHABET_MAP[256] PROGMEM =
#else
const char BASE58_ALPHABET_MAP[256] =
#endif
{
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1,
    -1,  9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
    -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

const double iFactor = 1.36565823730976103695740418120764243208481439700722980119458355862779176747360903943915516885072037696111192757109;

// reslen is the allocated length for result, feel free to overallocate
int EncodeBase58(const unsigned char *source, int len, unsigned char result[], int reslen) {
    int zeros = 0, length = 0, pbegin = 0, pend;
    if (!(pend = len)) 
		return 0;
    while (pbegin != pend && !source[pbegin]) 
		pbegin = ++zeros;
    int size = 1 + iFactor * (double)(pend - pbegin);
    unsigned char b58[size];
    for (int i = 0; i < size; i++) 
		b58[i] = 0;
    while (pbegin != pend) {
        unsigned int carry = source[pbegin];
        int i = 0;
        for (int it1 = size - 1; (carry || i < length) && (it1 != -1); it1--,i++) {
            carry += 256 * b58[it1];
            b58[it1] = carry % 58;
            carry /= 58;
        }
        if (carry) 
			return 0;
        length = i;
        pbegin++;
    }
    int it2 = size - length;
    while ((it2-size) && !b58[it2]) 
		it2++;
	if ((zeros + size - it2 + 1) > reslen) 
		return 0;
    int ri = 0;
    while(ri < zeros) 
		result[ri++] = '1';
	for (; it2 < size; ++it2) 
#ifdef __AVR_MEGA__
		result[ri++] = pgm_read_byte(&BASE58_ALPHABET[b58[it2]]);
#else
		result[ri++] = BASE58_ALPHABET[b58[it2]];
#endif
    result[ri] = 0;
    return ri;
}

// result must be declared (for the worst case): char result[len * 2];
int DecodeBase58(const unsigned char *str, int len, unsigned char *result) {
    result[0] = 0;
    int resultlen = 1;
    for (int i = 0; i < len; i++) {
#ifdef __AVR_MEGA__
        unsigned int carry = (unsigned int) pgm_read_byte(&BASE58_ALPHABET_MAP[str[i]]);
#else
        unsigned int carry = (unsigned int) BASE58_ALPHABET_MAP[str[i]];
#endif
        if (carry == -1) 
			return 0;
        for (int j = 0; j < resultlen; j++) {
            carry += (unsigned int) (result[j]) * 58;
            result[j] = (unsigned char) (carry & 0xff);
            carry >>= 8;
        }
        while (carry > 0) {
            result[resultlen++] = carry & 0xff;
            carry >>= 8;
        }
    }

    for (int i = 0; i < len && str[i] == '1'; i++)
        result[resultlen++] = 0;

    // Poorly coded, but guaranteed to work.
    for (int i = resultlen - 1, z = (resultlen >> 1) + (resultlen & 1);
        i >= z; i--) {
        int k = result[i];
        result[i] = result[resultlen - i - 1];
        result[resultlen - i - 1] = k;
    }
    return resultlen;
}