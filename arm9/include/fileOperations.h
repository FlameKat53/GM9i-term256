#include <nds.h>

#ifndef FILE_COPY
#define FILE_COPY

extern char clipboard[256];
extern char clipboardFilename[256];
extern bool clipboardFolder;
extern bool clipboardOn;
extern bool clipboardUsed;
extern bool clipboardDrive;	// false == SD card, true == Flashcard
extern bool clipboardInNitro;

extern void printBytes(unsigned long long bytes);

extern off_t getFileSize(const char *fileName);
extern int fcopy(const char *sourcePath, const char *destinationPath);

#endif // FILE_COPY