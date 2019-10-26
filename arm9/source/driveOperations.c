#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <nds.h>
#include <fat.h>
#include "../term256/term256.h"
#include "../term256/term256ext.h"

u8 stored_SCFG_MC = 0;

bool sdMounted = false;
bool sdMountedDone = false;				// true if SD mount is successful once
bool flashcardMounted = false;
bool nitroMounted = false;
bool arm7SCFGLocked = false;
bool isRegularDS = true;
bool secondaryDrive = false;				// false == SD card, true == Flashcard
bool nitroSecondaryDrive = false;			// false == SD card, true == Flashcard

char sdLabel[12];
char fatLabel[12];

int sdSize = 0;
int fatSize = 0;

void fixLabel(bool fat) {
	if (fat) {
		for (int i = 0; i < 12; i++) {
			if (((fatLabel[i] == ' ') && (fatLabel[i+1] == ' ') && (fatLabel[i+2] == ' '))
			|| ((fatLabel[i] == ' ') && (fatLabel[i+1] == ' '))
			|| (fatLabel[i] == ' ')) {
				fatLabel[i] = '\0';
				break;
			}
		}
	} else {
		for (int i = 0; i < 12; i++) {
			if (((sdLabel[i] == ' ') && (sdLabel[i+1] == ' ') && (sdLabel[i+2] == ' '))
			|| ((sdLabel[i] == ' ') && (sdLabel[i+1] == ' '))
			|| (sdLabel[i] == ' ')) {
				sdLabel[i] = '\0';
				break;
			}
		}
	}
}

bool sdFound(void) {
	if (access("sd:/", F_OK) == 0) {
		return true;
	} else {
		return false;
	}
}

bool flashcardFound(void) {
	if (access("fat:/", F_OK) == 0) {
		return true;
	} else {
		return false;
	}
}

bool bothSDandFlashcard(void) {
	if (sdMounted && flashcardMounted) {
		return true;
	} else {
		return false;
	}
}

TWL_CODE bool sdMount(void) {
	fatMountSimple("sd", get_io_dsisd());
	if (sdFound()) {
		sdMountedDone = true;
		fatGetVolumeLabel("sd", sdLabel);
		fixLabel(false);
		/*struct statvfs st;
		if (statvfs("sd:/", &st) == 0) {
			sdSize = st.f_bsize * st.f_blocks;
		}*/
		return true;
	}
	return false;
}

TWL_CODE void sdUnmount(void) {
	fatUnmount("sd");
	sdLabel[0] = '\0';
	sdSize = 0;
	sdMounted = false;
}

bool flashcardMount(void) {
	if ((!isDSiMode()) || (arm7SCFGLocked && !sdMountedDone)) {
		fatInitDefault();
		if (flashcardFound()) {
			fatGetVolumeLabel("fat", fatLabel);
			fixLabel(true);
			/*struct statvfs st;
			if (statvfs("fat:/", &st) == 0) {
				fatSize = st.f_bsize * st.f_blocks;
			}*/
			return true;
		}
		return false;
	} else {
		return false;
		//return twl_flashcardMount();
	}
}

void flashcardUnmount(void) {
	fatUnmount("fat");
	fatLabel[0] = '\0';
	fatSize = 0;
	flashcardMounted = false;
}

void printBytes(unsigned long long bytes) {
	if  (bytes == 1)
		iprtf("%d Byte", (unsigned int)bytes);

	else if (bytes < 1024)
		iprtf("%d Bytes", (unsigned int)bytes);

	else if (bytes < 1024 * 1024)
		iprtf("%.2f KB", (float)bytes / 1024.f);

	else if (bytes < 1024 * 1024 * 1024)
		iprtf("%.2f MB", (float)bytes / 1024.f / 1024.f);

	else
		iprtf("%.2f GB", (float)bytes / 1024.f / 1024.f / 1024.f);
}

unsigned long long getSDCardSize() {
	if (sdMounted) {
		struct statvfs st;
		if (statvfs("sd:/", &st) == 0)
			return st.f_bsize * st.f_blocks;
	}

	return 0;
}

unsigned long long getSDCardFree() {
	if (sdMounted) {
		struct statvfs st;
		if (statvfs("sd:/", &st) == 0)
			return st.f_bsize * st.f_bavail;
	}

	return 0;
}

unsigned long long getFlashcardSize() {
	if (flashcardMounted) {
		struct statvfs st;
		if (statvfs("fat:/", &st) == 0)
			return st.f_bsize * st.f_blocks;
	}

	return 0;
}

unsigned long long getFlashcardFree() {
	if (flashcardMounted) {
		struct statvfs st;
		if (statvfs("fat:/", &st) == 0)
			return st.f_bsize * st.f_bavail;
	}

	return 0;
}
