#ifndef DRIVE_OPERATIONS_H
#define DRIVE_OPERATIONS_H

extern unsigned long long getSDCardSize();
extern unsigned long long getSDCardFree();
extern unsigned long long getFlashcardFree();
extern unsigned long long getFlashcardSize();

extern u8 stored_SCFG_MC;

extern bool sdMounted;
extern bool sdMountedDone;				// true if SD mount is successful once
extern bool flashcardMounted;
extern bool nitroMounted;

extern bool secondaryDrive;			// false == SD card, true == Flashcard
extern bool nitroSecondaryDrive;		// false == SD card, true == Flashcard

extern char sdLabel[12];
extern char fatLabel[12];

extern bool sdFound(void);
extern bool flashcardFound(void);
extern bool bothSDandFlashcard(void);
extern bool sdMount(void);
extern void sdUnmount(void);
extern bool flashcardMount(void);
extern void flashcardUnmount(void);

#endif //DRIVE_OPERATIONS_H