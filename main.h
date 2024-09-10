#ifndef CODE_REVIEW_MAIN_H
#define CODE_REVIEW_MAIN_H

#include <stdio.h>  // Include stdio.h for FILE type

/* Base64 functions */
char *Base64Encode(const void *data, int size);
char *Base64Decode(const char *str);

/* UUencode functions */
void Encode(FILE *fp);

/* UUdecode functions */
int Decode(char *filename, FILE *fpin);

#endif /* CODE_REVIEW_MAIN_H */
