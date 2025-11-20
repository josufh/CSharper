#include "convert.h"

#include <stdlib.h>
#include <stdint.h>
#include "gc.h"

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
    char* out = (char*)GC_Alloc(out_len + 1);

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

    String string = {out, out_len};
    return string;
}

static const int8_t BASE64_REVERSE[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

Bytes Convert_FromBase64Fn(String string) {
    const char* b64 = string.Value;

    size_t len = 0;
    while (b64[len] != '\0') len++;

    if (len % 4 != 0) {
        Bytes bytes = {0};
        return bytes;
    }

    size_t pad = 0;
    if (len >= 2 && b64[len-1] == '=') pad++;
    if (len >= 3 && b64[len-2] == '=') pad++;

    size_t decoded_len = (len / 4) * 3 - pad;

    unsigned char* out = GC_Alloc(decoded_len);
    if (!out) {
        Bytes bytes = {0};
        return bytes;
    }

    size_t o = 0;

    for (size_t i = 0; i < len; i += 4) {
        int8_t c1 = BASE64_REVERSE[(unsigned char)b64[i]];
        int8_t c2 = BASE64_REVERSE[(unsigned char)b64[i+1]];
        int8_t c3 = b64[i+2] == '=' ? 0 : BASE64_REVERSE[(unsigned char)b64[i+2]];
        int8_t c4 = b64[i+3] == '=' ? 0 : BASE64_REVERSE[(unsigned char)b64[i+3]];

        uint32_t triple =
            (c1 << 18) |
            (c2 << 12) |
            (c3 << 6)  |
            (c4);

        if (o < decoded_len) out[o++] = (triple >> 16) & 0xFF;
        if (o < decoded_len) out[o++] = (triple >> 8) & 0xFF;
        if (o < decoded_len) out[o++] = triple & 0xFF;
    }

    Bytes bytes = {out, decoded_len};
    return bytes;
}

const ConvertStruct Convert = {
    .ToBase64 = Convert_ToBase64Fn,
    .FromBase64 = Convert_FromBase64Fn
};