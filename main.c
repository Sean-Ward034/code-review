#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "main.h"

#define MAX_FILE_SIZE 5000

/* Print the usage line for the program */
static void Usage() {
    printf("Usage: myencode <e|d> <uu|base64> <input filename>\n");
}

/* 
 * Main function:
 * This program converts the contents of a file to either base64 or uuencoded format 
 * and also decodes base64 or uuencoded formats back to the original data.
 */
int main(int argc, char **argv) {
    int i;
    struct stat sb;
    int mode;
    FILE *fp;
    char direction;
    char *type;
    char *filename;
    char *base64_encoded;
    char *base64_decoded;
    char buffer[MAX_FILE_SIZE];

    /* Check number of command line arguments, exit if there is an error */
    if (argc != 4) {
        Usage();
        exit(0);
    }

    direction = argv[1][0];
    type = argv[2];
    filename = argv[3];

    if ((fp = fopen(filename, "r")) == NULL) {
        perror("Error opening file");
        exit(1);
    }

    /* Handling UUencode and UUdecode */
    if (strcmp(type, "uu") == 0) {
        fstat(fileno(fp), &sb);
        mode = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

        switch (direction) {
            case 'e':
                printf("begin %o %s\n", mode, filename);
                Encode(fp);
                printf("end\n");
                break;
            case 'd':
                Decode(filename, fp);
                break;
            default:
                Usage();
                exit(1);
        }
    } 
    /* Handling Base64 encode and decode */
    else if (strcmp(type, "base64") == 0) {
        for (i = 0; i < MAX_FILE_SIZE; i++) {
            int c = getc(fp);
            if (c == EOF) {
                buffer[i] = '\0';
                break;
            }
            buffer[i] = c;
        }

        switch (direction) {
            case 'e':
                base64_encoded = Base64Encode(buffer, strlen(buffer));
                printf("%s\n", base64_encoded);
                break;
            case 'd':
                base64_decoded = Base64Decode(buffer);
                printf("%s\n", base64_decoded);
                break;
            default:
                Usage();
                exit(1);
        }
    }

    exit(0);
}
