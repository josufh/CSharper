#include "convert.h"

#include <stdlib.h>
#include <stdint.h>

static const char BASE64_ALPHABET[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static String Convert_ToBase64Fn(Bytes bytes) {
    const unsigned char* data = bytes.Data;
    size_t len = bytes.Length;

    if (!data && len != 0) {
        exit(69);
    }

    size_t out_len = 4 * ((len + 2) / 3);
    char* out = (char*)malloc(out_len + 1);
    if (!out) {
        exit(69);
    }

    size_t i = 0;
    size_t o = 0;

    while (i + 2 < len) {
        uint32_t triple = ((uint32_t)data[i]     << 16) |
                          ((uint32_t)data[i + 1] << 8 ) |
                          ((uint32_t)data[i + 2]);

        out[o++] = BASE64_ALPHABET[(triple >> 18) & 0x3F];
        out[o++] = BASE64_ALPHABET[(triple >> 12) & 0x3F];
        out[o++] = BASE64_ALPHABET[(triple >> 6)  & 0x3F];
        out[o++] = BASE64_ALPHABET[triple & 0x3F];

        i += 3;
    }

    if (i < len) {
        uint32_t triple = 0;
        size_t remaining = len - i;

        triple |= (uint32_t)data[i] << 16;
        if (remaining == 2) {
            triple |= (uint32_t)data[i + 1] << 8;
        }

        out[o++] = BASE64_ALPHABET[(triple >> 18) & 0x3F];
        out[o++] = BASE64_ALPHABET[(triple >> 12) & 0x3F];

        if (remaining == 2) {
            out[o++] = BASE64_ALPHABET[(triple >> 6) & 0x3F];
            out[o++] = '=';
        } else {
            out[o++] = '=';
            out[o++] = '=';
        }
    }

    out[o] = '\0';

    return Strings.New(out);
}

const ConvertStruct Convert = {
    .ToBase64 = Convert_ToBase64Fn
};