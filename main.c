#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "main.h"

#define MAX_FILE_SIZE 5000

// print usage line
static void usage() {
    printf("usage myencode <e|d> <uu|base64> <input filename>\n");
}

/* Take the contents of a file and convert it to base64 or uuencoded format
 * and also convert from base64 or uuencoded formate back to the original */
int
main (int argc, char **argv)
{
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

    /* check number command line args, exit if there is an error */
    if (argc != 4) {
       usage();
       exit(0);
    }

    direction = argv[1][0];
    type = argv[2];
    filename = argv[3];
  
    if ((fp = fopen(filename, "r")) == NULL) {
        perror("Error");
        exit(1);
    }

    if (strcmp(type, "uu") == 0) {
        fstat (fileno (fp), &sb);
        mode = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

        switch (direction) {
        case 'e':
            printf ("begin %o %s\n", mode, filename);
            encode (fp);
            printf ("end\n");
            break;
        case 'd':
            decode (filename, fp);
            break;
        default:
            usage();
            exit(1);
        }
    } else if (strcmp(type, "base64") == 0) {
        for (i = 0; i < MAX_FILE_SIZE; i++)
        {
            int c = getc(fp);

            if (c == EOF)
            {
                buffer[i] = 0;
                break;
            }

            buffer[i] = c;
        }
        switch (direction) {
        case 'e':
            base64_encoded = base64_encode(buffer, strlen(buffer));
            printf("%s\n", base64_encoded);
            break;
        case 'd':
            base64_decoded = base64_decode(buffer);
            printf("%s\n", base64_decoded);
            break;
        }
    }

    /* Exit 0 on success */
    exit (0);
}
