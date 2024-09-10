/*
 * Copyright (c) 1995-2001 Kungliga Tekniska Högskolan
 * (Royal Institute of Technology, Stockholm, Sweden).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the Institute nor the names of its contributors
 *  may be used to endorse or promote products derived from this software
 *  without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>

#include "main.h"

static char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* return the position in the base64_chars array of the matching character */ 
static int
pos(char c) {
  char *p;
  for (p = base64_chars; *p; p++) {
    if (*p == c) {
      return p - base64_chars;
    }
  }
  return -1;
}


/* encode a chunck of data in base64 format. return a pointer to the
 * encoded base64 string, which is NULL terminated.
 */
char *
base64_encode(const void *data, int size)
{
  char *s, *p;
  int i;
  int c;
  const unsigned char *q;

  p = s = (char *) malloc(size * 4 / 3 + 4);
  if (p == NULL) {
    return NULL;
  }
  q = (const unsigned char *) data;
  i = 0;
  for (i = 0; i < size;) {
    c = q[i++];
    c *= 256;
    if (i < size) {
      c += q[i];
    }
    i++;
    c *= 256;
    if (i < size) {
      c += q[i];
    }
    i++;
    p[0] = base64_chars[(c & 0x00fc0000) >> 18];
    p[1] = base64_chars[(c & 0x0003f000) >> 12];
    p[2] = base64_chars[(c & 0x00000fc0) >> 6];
    p[3] = base64_chars[(c & 0x0000003f) >> 0];
    if (i > size) {
      p[3] = '=';
    }
    if (i > size + 1) {
      p[2] = '=';
    }
    p += 4;
  }
  *p = 0;
  return s;
}

#define DECODE_ERROR 0xffffffff

/*
 * Decode the tokens.
 */
static unsigned int
token_decode(const char *TokenString)
{
  int i;
  unsigned int val = 0;
  int marker = 0;
  if (strlen(TokenString) < 4) {
    return DECODE_ERROR;
  }
  for (i = 0; i < 4; i++) {
    val *= 64;
    if (TokenString[i] == '=') {
      marker++;
    } else if (marker > 0) {
      return DECODE_ERROR;
    } else {
      val += pos(TokenString[i]);
    }
  }
  if (marker > 2) {
    return DECODE_ERROR;
  }
  return (marker << 24) | val;
}

/*
 * Decoode the base64 string return allocated string it has the decoded string
 */
char *
base64_decode(const char *str)
{
  const char *p;
  char *q;
  char *data;

  data = (char *)malloc((strlen(str)*3)/4+1);

  q = data;
  for (p = str; *p && (*p == '=' || strchr(base64_chars, *p)); p += 4) {
    unsigned val = token_decode(p);
    unsigned marker = (val >> 24) & 0xff;
    if (val == DECODE_ERROR) {
      return 0;
    }
    *q++ = (val >> 16) & 0xff;
    if (marker < 2) {
      *q++ = (val >> 8) & 0xff;
    }
    if (marker < 1) {
      *q++ = val & 0xff;
    }
  }
  return data;
}

