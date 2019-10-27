extern "C" {
#include "../term256/term256.h"
#include "../term256/term256ext.h"
#include "driveOperations.h"
}

#include "gm9i_logo.h"
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

static int bg3;

void set_scroll_callback(int x, int y, void *param) {
	bgSetScroll(*(int*)param, x, y);
	bgUpdate();
}

void stop() {
	while (1) {
		swiWaitForVBlank();
	}
}

int main(int argc, const char * const argv[]) {
	defaultExceptionHandler();

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
	prt("\tGodMode9i v2.0.0");
	prt("\x1b[6;1H");
	prt("\t--------------------------------");
	prt("\x1b[7;1H");
	prt("\thttps:/github.com/\n");
	prt("\t\t\t\tRocketRobz/GodMode9i");
	if (isDSiMode()) {
		prt("\x1b[22;1H");
		prt("\taaHold Y - Disable cart access");
	}
		stop();
	// Display for 2 seconds
	for (int i = 0; i < 60*2; i++) {
		swiWaitForVBlank();
	}

	fifoWaitValue32(FIFO_USER_06);
}
