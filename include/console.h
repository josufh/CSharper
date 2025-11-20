#ifndef CONSOLE_H
#define CONSOLE_H

typedef struct ConsoleStruct ConsoleStruct;

typedef void (*WriteLineFn)(const char* format, ...);
typedef void (*LineBreakFn)();

struct ConsoleStruct {
    WriteLineFn WriteLine;
    LineBreakFn LineBreak;
};

extern const ConsoleStruct Console;

#endif // CONSOLE_H