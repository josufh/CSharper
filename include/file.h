#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "strings.h"
#include "bytes.h"

typedef struct FileStream FileStream;
struct FileStream
{
    FILE *handler;
};

typedef enum FileMode {
    CreateNew = 1,
    Create,
    Open,
    OpenOrCreate,
    Append
} FileMode;


typedef struct FileStruct FileStruct;
typedef void (*CopyFn)(String sourceFileName, String destFileName);
typedef FileStream (*CreateFn)(String path);
typedef void (*DeleteFn)(String path);
typedef char (*ExistsFn)(String path);
// typedef void (*MoveFn)(String sourceFileName, String destFileName);
typedef FileStream (*OpenFn)(String path, FileMode mode);
typedef Bytes (*ReadAllBytesFn)(String path);
typedef String* (*ReadAllLinesFn)(String path);
typedef String (*ReadAllTextFn)(String path);
// typedef void (*ReplaceFn)(String sourceFileName, String destinationFileName, String destinationBackupFileName);
typedef void (*WriteAllBytesFn)(String path, Bytes bytes);
typedef void (*WriteAllLinesFn)(String path, String* contents);
typedef void (*WriteAllTextFn)(String path, String contents);

struct FileStruct {
    CopyFn Copy;
    CreateFn Create;
    DeleteFn Delete;
    ExistsFn Exists;
    // MoveFn Move;
    OpenFn Open;
    ReadAllBytesFn ReadAllBytes;
    ReadAllLinesFn ReadAllLines;
    ReadAllTextFn ReadAllText;
    // ReplaceFn Replace;
    WriteAllBytesFn WriteAllBytes;
    WriteAllLinesFn WriteAllLines;
    WriteAllTextFn WriteAllText;
};

extern const FileStruct File;

#endif // FILE_H