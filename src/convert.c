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
    [0 ... 255] = -1,

    ['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,  ['E'] = 4,
    ['F'] = 5,  ['G'] = 6,  ['H'] = 7,  ['I'] = 8,  ['J'] = 9,
    ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14,
    ['P'] = 15, ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23, ['Y'] = 24,
    ['Z'] = 25,

    ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29, ['e'] = 30,
    ['f'] = 31, ['g'] = 32, ['h'] = 33, ['i'] = 34, ['j'] = 35,
    ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39, ['o'] = 40,
    ['p'] = 41, ['q'] = 42, ['r'] = 43, ['s'] = 44, ['t'] = 45,
    ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49, ['y'] = 50,
    ['z'] = 51,

    ['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55, ['4'] = 56,
    ['5'] = 57, ['6'] = 58, ['7'] = 59, ['8'] = 60, ['9'] = 61,

    ['+'] = 62,
    ['/'] = 63
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