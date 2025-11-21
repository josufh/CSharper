#include "file.h"

#include <sys/stat.h>
#include "gc.h"

#define FILE_BUFFER 4096

static FileStream MakeFileStream(FILE *f) {
    FileStream fs = {f};
    return fs;
}

static void File_Copy(String sourceFileName, String destFileName) {
    FILE *src = fopen(sourceFileName.Value, "rb");
    if (!src) {
        return;
    }

    FILE *dst = fopen(destFileName.Value, "wb");
    if (!dst) {
        fclose(src);
        return;
    }

    unsigned char buffer[FILE_BUFFER];
    size_t readCount;

    while ((readCount = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t written = fwrite(buffer, 1, readCount, dst);
        if (written != readCount) {
            break; // TODO error message, or "exception"
        }
    }

    fclose(src);
    fclose(dst);
}

static FileStream File_Create(String path) {
    FILE *f = fopen(path.Value, "wb+");
    return MakeFileStream(f);
}

static void File_Delete(String path) {
    remove(path.Value);
}

static char File_Exists(String path) {
    struct stat st;
    return stat(path.Value, &st) == 0;
}

static FileStream File_Open(String path, FileMode mode) {
    FILE *f = NULL;

    switch (mode) {
        case CreateNew:
            if (File_Exists(path)) {
                f = NULL;
            } else {
                f = fopen(path.Value, "wb+");
            }
            break;
        
        case Create:
            f = fopen(path.Value, "wb+");
            break;

        case Open:
            f = fopen(path.Value, "rb+");
            if (!f) {
                f = fopen(path.Value, "rb");
            }
            break;
            
        case OpenOrCreate:
            f = fopen(path.Value, "rb+");
            if (!f) {
                f = fopen(path.Value, "wb+");
            }
            break;

        case Append:
            f = fopen(path.Value, "ab+");
            break;
    }

    return MakeFileStream(f);
}

#define READ_ALL_BYTES__RETURN_RESULT(value, length) \
    Bytes result = {value, length}; \
    return result;

static Bytes File_ReadAllBytes(String path) {
    FILE *f = fopen(path.Value, "rb");
    if (!f) {
        READ_ALL_BYTES__RETURN_RESULT(NULL, 0);
    }
    
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        READ_ALL_BYTES__RETURN_RESULT(NULL, 0);
    }
    
    long size = ftell(f);
    if (size < 0) {
        fclose(f);
        READ_ALL_BYTES__RETURN_RESULT(NULL, 0);
    }

    unsigned char *buf = (unsigned char*)GC_Alloc((size_t)size);
    // TODO maybe pool of FILE handlers

    size_t readCount = fread(buf, 1, (size_t)size, f);
    fclose(f);

    READ_ALL_BYTES__RETURN_RESULT(buf, readCount);
}

