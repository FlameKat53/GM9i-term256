extern "C" {
#include "../term256/term256.h"
#include "../term256/term256ext.h"
}
#include "nds_loader_arm9.h"
#include "driveMenu.h"
#include "driveOperations.h"
#include "file_browse.h"
#include "fileOperations.h"

#include "gm9i_logo.h"
#include "driveMenu.h"
#include <vector>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <nds.h>
#include <fat.h>

term_t t0;
term_t t1;

#define Cls "\x1b[2J"
#define Rst "\x1b[0m"
#define BlkOnWht "\x1b[30;47m"
#define CyanOnBlk "\x1b[32;1;40m"
#define Red "\x1b[31;1m"
#define BlkOnRed "\x1b[31;1;7;30m"

char titleName[32] = {" "};

int screenMode = 0;

bool appInited = false;

bool arm7SCFGLocked = false;
bool isRegularDS = true;
bool is3DS = true;

bool applaunch = false;

static int bg3;

using namespace std;

bool extension(const std::string& filename, const char* ext) {
	if(strcasecmp(filename.c_str() + filename.size() - strlen(ext), ext)) {
		return false;
	} else {
		return true;
	}
}
void set_scroll_callback(int x, int y, void *param) {
	bgSetScroll(*(int*)param, x, y);
	bgUpdate();
}

void stop() {
	while (1) {
		swiWaitForVBlank();
	}
}

int main(int argc, char **argv) {

	// overwrite reboot stub identifier
	extern u64 *fake_heap_end;
	*fake_heap_end = 0;

	defaultExceptionHandler();

	int pathLen;
	std::string filename;

	bool yHeld = false;

	snprintf(titleName, sizeof(titleName), "GodMode9i v%i.%i.%i", 2, 0, 0);

	videoSetModeSub(MODE_3_2D);
	videoSetMode(MODE_3_2D);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankA(VRAM_A_MAIN_BG);
	int bg0id = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
	int bg1id = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
	u16 *bg0 = bgGetGfxPtr(bg0id);
	u16 *bg1 = bgGetGfxPtr(bg1id);
	generate_ansi256_palette(BG_PALETTE_SUB);
	dmaCopy(BG_PALETTE_SUB, BG_PALETTE, 256 * 2);
	term_init(&t0, bg0, set_scroll_callback, &bg0id);
	term_init(&t1, bg1, set_scroll_callback, &bg1id);

	cpuStartTiming(0);
	// Display GM9i logo
	bg3 = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 1, 0);
	bgSetScroll(bg3, 0, 0);
	decompress(gm9i_logoBitmap, bgGetGfxPtr(bg3), LZ77Vram);

	select_term(&t0);
	prt("\x1b[5;1H");
	iprtf("\t%s", titleName);
	prt("\x1b[6;1H");
	prt("\t--------------------------------");
	prt("\x1b[7;1H");
	prt("\thttps:/github.com/\n");
	prt("\t\t\t\tRocketRobz/GodMode9i");
	if (isDSiMode()) {
		prt("\x1b[22;1H");
		prt("\taaHold Y - Disable cart access");
	}

	// Display for 2 seconds
	for (int i = 0; i < 60*2; i++) {
		swiWaitForVBlank();
	}

	fifoWaitValue32(FIFO_USER_06);
	if (fifoGetValue32(FIFO_USER_03) == 0) arm7SCFGLocked = true;
	u16 arm7_SNDEXCNT = fifoGetValue32(FIFO_USER_07);
	if (arm7_SNDEXCNT != 0) isRegularDS = false;	// If sound frequency setting is found, then the console is not a DS Phat/Lite
	fifoSendValue32(FIFO_USER_07, 0);

	if (isDSiMode()) {
		prt("\x1b[22;1H");
		prt("\t                              ");	// Clear "Y Held" text
	}
	prt("\x1b[22;11H");
	prt("Mounting drives...");

	sysSetCartOwner (BUS_OWNER_ARM9);	// Allow arm9 to access GBA ROM

	if (isDSiMode()) {
		scanKeys();
		if (keysHeld() & KEY_Y) {
			yHeld = true;
		}
		sdMounted = sdMount();
	}
	if (!isDSiMode() || !sdMounted || (access("sd:/Nintendo 3DS", F_OK) != 0)) {
		is3DS = false;
	}
	if (!isDSiMode() || !yHeld) {
		flashcardMounted = flashcardMount();
		flashcardMountSkipped = false;
	}

	videoSetMode(MODE_0_2D);
	// initialize all the VRAM banks
	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_MAIN_BG_0x06000000);
	vramSetBankE(VRAM_E_TEX_PALETTE);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT4);
	vramSetBankG(VRAM_G_TEX_PALETTE_SLOT5);
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

	keysSetRepeat(25,5);

	appInited = true;

	while(1) {

		if (screenMode == 0) {
			driveMenu();
		} else {
			filename = browseForFile();
		}

		if (applaunch) {
		prt("not implemented ");
		}
	}

	return 0;
}