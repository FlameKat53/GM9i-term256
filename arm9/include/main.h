#ifndef MAIN_H
#define MAIN_H

#define POWERTEXT_DS	"POWER - Poweroff"
#define POWERTEXT		"POWER - Reboot/[+held] Poweroff"
#define POWERTEXT_3DS	"POWER - Sleep Mode screen"
#define HOMETEXT 		"HOME - HOME Menu prompt"
#define SCREENSHOTTEXT 	"R+L - Make a screenshot"

// For Console
#define Cls "\x1b[2J"
#define Rst "\x1b[0m"
#define BlkOnWht "\x1b[30;47m"
#define CyanOnBlk "\x1b[32;1;40m"
#define Red "\x1b[31;1m"
#define BlkOnRed "\x1b[31;1;7;30m"

extern char titleName[32];

extern int screenMode;

extern bool appInited;
extern bool arm7SCFGLocked;
extern bool isRegularDS;
extern bool is3DS;
extern bool applaunch;


#endif //MAIN_H
