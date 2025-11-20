#include "console.h"

#include <stdio.h>
#include <stdarg.h>
#include "strings.h"

static void Console_WriteLine(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    vfprintf(stdout, format, args);

    // for (const char* p = format; *p != '\0'; p++) {
    //     if (*p != '%') {
    //         putchar(*p);
    //         continue;
    //     }

    //     ++p;
    //     if (*p == '\0') {
    //         putchar('%');
    //         break;
    //     }

    //     switch (*p) {
    //         case 'S': {
    //             String s = va_arg(args, String);
    //             if (s.Value) {
    //                 fputs(s.Value, stdout);
    //             }
    //             break;
    //         }
    //         case 's': {
    //             const char* s = va_arg(args, const char*);
    //             if (s != NULL) {
    //                 fputs(s, stdout);
    //             }
    //             break;
    //         }
    //         case 'd': {
    //             int v = va_arg(args, int);
    //             printf("%d", v);
    //             break;
    //         }
    //         case 'f': {
    //             double v = va_arg(args, double);
    //             printf("%f", v);
    //             break;
    //         }
    //         case '%': {
    //             putchar('%');
    //             break;
    //         }
    //         default:
    //             putchar('%');
    //             putchar(*p);
    //             break;
    //     }
    // }

    va_end(args);

    putchar('\n');
}

static void Console_LineBreak() {
    putchar('\n');
}

const ConsoleStruct Console = {
    .WriteLine = Console_WriteLine,
    .LineBreak = Console_LineBreak
};