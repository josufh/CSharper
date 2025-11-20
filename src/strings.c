#include "../include/strings.h" // for some reason vscode is not getting strings.h

#include <string.h>
#include "../include/gc.h"

static String Strings_NewFn(const char* text) {
    size_t len = strlen(text);
    
    char* buf = (char*)GC_Alloc(len + 1);
    if (buf) {
        memcpy(buf, text, len + 1);
    }
    
    String string = {buf, len};
    return string;
}

const StringsStruct Strings = {
    .New = Strings_NewFn
};