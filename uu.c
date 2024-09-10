/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pwd.h>

 
#include "main.h"
 
#define ENC(c) ((c) ? ((c) & 077) + ' ': '`')
#define DEC(c)    (((c) - ' ') & 077) /* single character decode */

 
/*
 * Encode a file using uuencoding so it can be mailed to a remote system.
 */
void
encode (FILE *fp)
{
    register int ch, n;
    register char *p;
    char buf[80];
 
    while ((n = fread (buf, 1, 45, fp)) != 0)
      {
        ch = ENC (n);
        if (putchar (ch) == EOF) {
            break;
        }
        for (p = buf; n > 0; n -= 3, p += 3)
          {
        ch = *p >> 2;
        ch = ENC (ch);
        if (putchar (ch) == EOF) {
            break;
        }
        ch = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
        ch = ENC (ch);
        if (putchar (ch) == EOF) {
            break;
        }
        ch = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
        ch = ENC (ch);
        if (putchar (ch) == EOF) {
            break;
        }
        ch = p[2] & 077;
        ch = ENC (ch);
        if (putchar (ch) == EOF) {
            break;
        }
          }
        if (putchar ('\n') == EOF) {
            break;
        }
      }
    if (ferror (fp)) {
        fprintf (stderr, "uuencode: read error\n");
        exit(1);
    }

    if (ferror (fp)) {
        fprintf (stderr, "uuencode: read error\n");
        exit(1);
    }
    ch = ENC ('\0');
    putchar (ch);
    putchar ('\n');
}
 
 
/* 
 * uudecode a file.
 */
int
decode (char *filename, FILE *fpin)
{
    struct passwd *pw;
    int n;
    char ch, *p;
    int mode, n1;
    char buf[2 * BUFSIZ];
    char *outname;
 
    /* search for header line */
    do {
        if (fgets (buf, sizeof (buf), fpin) == NULL) {
            fprintf (stderr,
                    "no \"begin\" line\n");
            return 1;
        }
    }
    while (strncmp (buf, "begin ", 6) != 0);
 
    sscanf (buf, "begin %o %s", &mode, buf);
 
    /* handle ~user/file format */
    if ( buf[0] != '~' ) {
        outname = buf;
    } else {
        p = buf + 1;
        while (*p != '/') {
            ++p;
        }
        if (*p == '\0') {
            fprintf (stderr, "%s illegal ~user\n", filename);
            return 1;
        }
        *p++ = '\0';
        pw = getpwnam (buf + 1);
        if (pw == NULL) {
            fprintf (stderr, "%s: no user %s\n", filename, buf + 1);
            return 1;
        }
        n = strlen (pw->pw_dir);
        n1 = strlen (p);
        outname = (char *) malloc (n + n1 + 2);
        memcpy (outname + n + 1, p, n1 + 1);
        memcpy (outname, pw->pw_dir, n);
        outname[n] = '/';
    }
 
    /* create output file, set mode */
    if (freopen (outname, "w", stdout) == NULL
                 || fchmod (fileno (stdout),
                 mode & (S_IRWXU | S_IRWXG | S_IRWXO))) {
        fprintf (stderr, "%s:", outname);
        perror (filename);
        return 1;
    }
 
    /* for each input line */
    while (1) {
        if (fgets (buf, sizeof(buf), fpin) == NULL) {
            fprintf (stderr, "%s: short file.\n", filename);
            return 1;
        }
        p = buf;
        /*
         * `n' is used to avoid writing out all the characters
         * at the end of the file.
         */
        n = DEC (*p);
        if (n <= 0)
        break;
        for (++p; n > 0; p += 4, n -= 3) {
            if (n >= 3) {
                ch = DEC (p[0]) << 2 | DEC (p[1]) >> 4;
                putchar (ch);
                ch = DEC (p[1]) << 4 | DEC (p[2]) >> 2;
                putchar (ch);
                ch = DEC (p[2]) << 6 | DEC (p[3]);
                putchar (ch);
            }
            else {
                if (n >= 1) {
                    ch = DEC (p[0]) << 2 | DEC (p[1]) >> 4;
                    putchar (ch);
                }
                if (n >= 2) {
                    ch = DEC (p[1]) << 4 | DEC (p[2]) >> 2;
                    putchar (ch);
                }
            }
        }
    }
 
    if (fgets (buf, sizeof(buf), fpin) == NULL || strcmp (buf, "end\n")) {
        fprintf (stderr, "%s: no \"end\" line.\n", filename);
        return 1;
    }
 
    return (0);
}
 
