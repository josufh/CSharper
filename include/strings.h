#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

typedef struct String String;

struct String {
    const char* Value;
    const size_t Length;
};

typedef struct StringsStruct StringsStruct;
typedef String (*NewFn)(const char* string);

struct StringsStruct
{
    NewFn New;
};

extern const StringsStruct Strings;

#endif // STRINGS_H