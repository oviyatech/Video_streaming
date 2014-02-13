/*
 * $Id: evtest.c,v 1.23 2005/02/06 13:51:42 vojtech Exp $
 *
 *  Copyright (c) 1999-2000 Vojtech Pavlik
 *
 *  Event device test program
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */

#include <stdint.h>
#include <stdlib.h>
#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <assert.h>

#include <getopt.h>             /* getopt_long() */
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include<sys/time.h>
#include<time.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>

/** Header files for Read config file **/
#include "ftplib.h"
#include "logger.h"
#include <signal.h>

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "video_capture.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

#ifndef EV_SYN
#define EV_SYN 0
#endif


/********* VARIABLE DECLARATION *************/

/* Variables for FIle read*/
	FILE *file;
	char *line = NULL;
	size_t len = 0;
	char Read;
	char **buf[20];
	int lineIndex = 0; 
/* Variables for Taking Video Width & Height*/
	char *width, *height;
	static int WIDTH,HEIGHT;

/* Variables for Taking Image Width & Height */
	char *i_width,*i_height;
        static int I_WIDTH,I_HEIGHT;

/* Variables for Taking FPS Value*/
	unsigned char *fps;
	int i, Fps;

/*Varialbles for Taking FIle name*/
	char *f_name;
	static char *F_name;

/* Variables for taking FTP deatils*/

	char *ftp_link;
	static char *FTP_LINK;
	char *ftp_uname;
	static char *FTP_UNAME;
	char *ftp_pwd;
	static char *FTP_PWD;
/*Variables for taking APN NAME*/
	char *apn; 
	static char *APN;

/* Variables for taking Video size and capture interval*/
	char *v_sz, *v_intr;
	int V_SIZE, V_IVAL;

/* Variables for taking Image Capture Interval */
	char *i_intr;
	int I_IVAL;

/* Variables for taking Image File Name */
	char *i_name, *I_name;

/*Variables for Taking Capture mode*/
	int CAP_MODE;

/*Variables for Getting IP*/
	char *ip, *IP;

/* ADDED BY SHAN */
static gint capture_time = 10;
static gint capture_count = 0;
GTimer *timer = NULL;
char n[1024];
sem_t lock;
char res[1024];

static int f_cnt = 0;
int factory_make(char* location);
/* ENDEDD BY SHAN */

char *events[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = "Sync",			[EV_KEY] = "Key",
	[EV_REL] = "Relative",			[EV_ABS] = "Absolute",
	[EV_MSC] = "Misc",			[EV_LED] = "LED",
	[EV_SND] = "Sound",			[EV_REP] = "Repeat",
	[EV_FF] = "ForceFeedback",		[EV_PWR] = "Power",
	[EV_FF_STATUS] = "ForceFeedbackStatus",
};

char *keys[KEY_MAX + 1] = {
	[0 ... KEY_MAX] = NULL,
	[KEY_RESERVED] = "Reserved",		[KEY_ESC] = "Esc",
	[KEY_1] = "1",				[KEY_2] = "2",
	[KEY_3] = "3",				[KEY_4] = "4",
	[KEY_5] = "5",				[KEY_6] = "6",
	[KEY_7] = "7",				[KEY_8] = "8",
	[KEY_9] = "9",				[KEY_0] = "0",
	[KEY_MINUS] = "Minus",			[KEY_EQUAL] = "Equal",
	[KEY_BACKSPACE] = "Backspace",		[KEY_TAB] = "Tab",
	[KEY_Q] = "Q",				[KEY_W] = "W",
	[KEY_E] = "E",				[KEY_R] = "R",
	[KEY_T] = "T",				[KEY_Y] = "Y",
	[KEY_U] = "U",				[KEY_I] = "I",
	[KEY_O] = "O",				[KEY_P] = "P",
	[KEY_LEFTBRACE] = "LeftBrace",		[KEY_RIGHTBRACE] = "RightBrace",
	[KEY_ENTER] = "Enter",			[KEY_LEFTCTRL] = "LeftControl",
	[KEY_A] = "A",				[KEY_S] = "S",
	[KEY_D] = "D",				[KEY_F] = "F",
	[KEY_G] = "G",				[KEY_H] = "H",
	[KEY_J] = "J",				[KEY_K] = "K",
	[KEY_L] = "L",				[KEY_SEMICOLON] = "Semicolon",
	[KEY_APOSTROPHE] = "Apostrophe",	[KEY_GRAVE] = "Grave",
	[KEY_LEFTSHIFT] = "LeftShift",		[KEY_BACKSLASH] = "BackSlash",
	[KEY_Z] = "Z",				[KEY_X] = "X",
	[KEY_C] = "C",				[KEY_V] = "V",
	[KEY_B] = "B",				[KEY_N] = "N",
	[KEY_M] = "M",				[KEY_COMMA] = "Comma",
	[KEY_DOT] = "Dot",			[KEY_SLASH] = "Slash",
	[KEY_RIGHTSHIFT] = "RightShift",	[KEY_KPASTERISK] = "KPAsterisk",
	[KEY_LEFTALT] = "LeftAlt",		[KEY_SPACE] = "Space",
	[KEY_CAPSLOCK] = "CapsLock",		[KEY_F1] = "F1",
	[KEY_F2] = "F2",			[KEY_F3] = "F3",
	[KEY_F4] = "F4",			[KEY_F5] = "F5",
	[KEY_F6] = "F6",			[KEY_F7] = "F7",
	[KEY_F8] = "F8",			[KEY_F9] = "F9",
	[KEY_F10] = "F10",			[KEY_NUMLOCK] = "NumLock",
	[KEY_SCROLLLOCK] = "ScrollLock",	[KEY_KP7] = "KP7",
	[KEY_KP8] = "KP8",			[KEY_KP9] = "KP9",
	[KEY_KPMINUS] = "KPMinus",		[KEY_KP4] = "KP4",
	[KEY_KP5] = "KP5",			[KEY_KP6] = "KP6",
	[KEY_KPPLUS] = "KPPlus",		[KEY_KP1] = "KP1",
	[KEY_KP2] = "KP2",			[KEY_KP3] = "KP3",
	[KEY_KP0] = "KP0",			[KEY_KPDOT] = "KPDot",
	[KEY_ZENKAKUHANKAKU] = "Zenkaku/Hankaku", [KEY_102ND] = "102nd",
	[KEY_F11] = "F11",			[KEY_F12] = "F12",
	[KEY_RO] = "RO",			[KEY_KATAKANA] = "Katakana",
	[KEY_HIRAGANA] = "HIRAGANA",		[KEY_HENKAN] = "Henkan",
	[KEY_KATAKANAHIRAGANA] = "Katakana/Hiragana", [KEY_MUHENKAN] = "Muhenkan",
	[KEY_KPJPCOMMA] = "KPJpComma",		[KEY_KPENTER] = "KPEnter",
	[KEY_RIGHTCTRL] = "RightCtrl",		[KEY_KPSLASH] = "KPSlash",
	[KEY_SYSRQ] = "SysRq",			[KEY_RIGHTALT] = "RightAlt",
	[KEY_LINEFEED] = "LineFeed",		[KEY_HOME] = "Home",
	[KEY_UP] = "Up",			[KEY_PAGEUP] = "PageUp",
	[KEY_LEFT] = "Left",			[KEY_RIGHT] = "Right",
	[KEY_END] = "End",			[KEY_DOWN] = "Down",
	[KEY_PAGEDOWN] = "PageDown",		[KEY_INSERT] = "Insert",
	[KEY_DELETE] = "Delete",		[KEY_MACRO] = "Macro",
	[KEY_MUTE] = "Mute",			[KEY_VOLUMEDOWN] = "VolumeDown",
	[KEY_VOLUMEUP] = "VolumeUp",		[KEY_POWER] = "Power",
	[KEY_KPEQUAL] = "KPEqual",		[KEY_KPPLUSMINUS] = "KPPlusMinus",
	[KEY_PAUSE] = "Pause",			[KEY_KPCOMMA] = "KPComma",
	[KEY_HANGUEL] = "Hanguel",		[KEY_HANJA] = "Hanja",
	[KEY_YEN] = "Yen",			[KEY_LEFTMETA] = "LeftMeta",
	[KEY_RIGHTMETA] = "RightMeta",		[KEY_COMPOSE] = "Compose",
	[KEY_STOP] = "Stop",			[KEY_AGAIN] = "Again",
	[KEY_PROPS] = "Props",			[KEY_UNDO] = "Undo",
	[KEY_FRONT] = "Front",			[KEY_COPY] = "Copy",
	[KEY_OPEN] = "Open",			[KEY_PASTE] = "Paste",
	[KEY_FIND] = "Find",			[KEY_CUT] = "Cut",
	[KEY_HELP] = "Help",			[KEY_MENU] = "Menu",
	[KEY_CALC] = "Calc",			[KEY_SETUP] = "Setup",
	[KEY_SLEEP] = "Sleep",			[KEY_WAKEUP] = "WakeUp",
	[KEY_FILE] = "File",			[KEY_SENDFILE] = "SendFile",
	[KEY_DELETEFILE] = "DeleteFile",	[KEY_XFER] = "X-fer",
	[KEY_PROG1] = "Prog1",			[KEY_PROG2] = "Prog2",
	[KEY_WWW] = "WWW",			[KEY_MSDOS] = "MSDOS",
	[KEY_COFFEE] = "Coffee",		[KEY_DIRECTION] = "Direction",
	[KEY_CYCLEWINDOWS] = "CycleWindows",	[KEY_MAIL] = "Mail",
	[KEY_BOOKMARKS] = "Bookmarks",		[KEY_COMPUTER] = "Computer",
	[KEY_BACK] = "Back",			[KEY_FORWARD] = "Forward",
	[KEY_CLOSECD] = "CloseCD",		[KEY_EJECTCD] = "EjectCD",
	[KEY_EJECTCLOSECD] = "EjectCloseCD",	[KEY_NEXTSONG] = "NextSong",
	[KEY_PLAYPAUSE] = "PlayPause",		[KEY_PREVIOUSSONG] = "PreviousSong",
	[KEY_STOPCD] = "StopCD",		[KEY_RECORD] = "Record",
	[KEY_REWIND] = "Rewind",		[KEY_PHONE] = "Phone",
	[KEY_ISO] = "ISOKey",			[KEY_CONFIG] = "Config",
	[KEY_HOMEPAGE] = "HomePage",		[KEY_REFRESH] = "Refresh",
	[KEY_EXIT] = "Exit",			[KEY_MOVE] = "Move",
	[KEY_EDIT] = "Edit",			[KEY_SCROLLUP] = "ScrollUp",
	[KEY_SCROLLDOWN] = "ScrollDown",	[KEY_KPLEFTPAREN] = "KPLeftParenthesis",
	[KEY_KPRIGHTPAREN] = "KPRightParenthesis", [KEY_F13] = "F13",
	[KEY_F14] = "F14",			[KEY_F15] = "F15",
	[KEY_F16] = "F16",			[KEY_F17] = "F17",
	[KEY_F18] = "F18",			[KEY_F19] = "F19",
	[KEY_F20] = "F20",			[KEY_F21] = "F21",
	[KEY_F22] = "F22",			[KEY_F23] = "F23",
	[KEY_F24] = "F24",			[KEY_PLAYCD] = "PlayCD",
	[KEY_PAUSECD] = "PauseCD",		[KEY_PROG3] = "Prog3",
	[KEY_PROG4] = "Prog4",			[KEY_SUSPEND] = "Suspend",
	[KEY_CLOSE] = "Close",			[KEY_PLAY] = "Play",
	[KEY_FASTFORWARD] = "Fast Forward",	[KEY_BASSBOOST] = "Bass Boost",
	[KEY_PRINT] = "Print",			[KEY_HP] = "HP",
	[KEY_CAMERA] = "Camera",		[KEY_SOUND] = "Sound",
	[KEY_QUESTION] = "Question",		[KEY_EMAIL] = "Email",
	[KEY_CHAT] = "Chat",			[KEY_SEARCH] = "Search",
	[KEY_CONNECT] = "Connect",		[KEY_FINANCE] = "Finance",
	[KEY_SPORT] = "Sport",			[KEY_SHOP] = "Shop",
	[KEY_ALTERASE] = "Alternate Erase",	[KEY_CANCEL] = "Cancel",
	[KEY_BRIGHTNESSDOWN] = "Brightness down", [KEY_BRIGHTNESSUP] = "Brightness up",
	[KEY_MEDIA] = "Media",			[KEY_UNKNOWN] = "Unknown",
	[BTN_0] = "Btn0",			[BTN_1] = "Btn1",
	[BTN_2] = "Btn2",			[BTN_3] = "Btn3",
	[BTN_4] = "Btn4",			[BTN_5] = "Btn5",
	[BTN_6] = "Btn6",			[BTN_7] = "Btn7",
	[BTN_8] = "Btn8",			[BTN_9] = "Btn9",
	[BTN_LEFT] = "LeftBtn",			[BTN_RIGHT] = "RightBtn",
	[BTN_MIDDLE] = "MiddleBtn",		[BTN_SIDE] = "SideBtn",
	[BTN_EXTRA] = "ExtraBtn",		[BTN_FORWARD] = "ForwardBtn",
	[BTN_BACK] = "BackBtn",			[BTN_TASK] = "TaskBtn",
	[BTN_TRIGGER] = "Trigger",		[BTN_THUMB] = "ThumbBtn",
	[BTN_THUMB2] = "ThumbBtn2",		[BTN_TOP] = "TopBtn",
	[BTN_TOP2] = "TopBtn2",			[BTN_PINKIE] = "PinkieBtn",
	[BTN_BASE] = "BaseBtn",			[BTN_BASE2] = "BaseBtn2",
	[BTN_BASE3] = "BaseBtn3",		[BTN_BASE4] = "BaseBtn4",
	[BTN_BASE5] = "BaseBtn5",		[BTN_BASE6] = "BaseBtn6",
	[BTN_DEAD] = "BtnDead",			[BTN_A] = "BtnA",
	[BTN_B] = "BtnB",			[BTN_C] = "BtnC",
	[BTN_X] = "BtnX",			[BTN_Y] = "BtnY",
	[BTN_Z] = "BtnZ",			[BTN_TL] = "BtnTL",
	[BTN_TR] = "BtnTR",			[BTN_TL2] = "BtnTL2",
	[BTN_TR2] = "BtnTR2",			[BTN_SELECT] = "BtnSelect",
	[BTN_START] = "BtnStart",		[BTN_MODE] = "BtnMode",
	[BTN_THUMBL] = "BtnThumbL",		[BTN_THUMBR] = "BtnThumbR",
	[BTN_TOOL_PEN] = "ToolPen",		[BTN_TOOL_RUBBER] = "ToolRubber",
	[BTN_TOOL_BRUSH] = "ToolBrush",		[BTN_TOOL_PENCIL] = "ToolPencil",
	[BTN_TOOL_AIRBRUSH] = "ToolAirbrush",	[BTN_TOOL_FINGER] = "ToolFinger",
	[BTN_TOOL_MOUSE] = "ToolMouse",		[BTN_TOOL_LENS] = "ToolLens",
	[BTN_TOUCH] = "Touch",			[BTN_STYLUS] = "Stylus",
	[BTN_STYLUS2] = "Stylus2",		[BTN_TOOL_DOUBLETAP] = "Tool Doubletap",
	[BTN_TOOL_TRIPLETAP] = "Tool Tripletap", [BTN_GEAR_DOWN] = "WheelBtn",
	[BTN_GEAR_UP] = "Gear up",		[KEY_OK] = "Ok",
	[KEY_SELECT] = "Select",		[KEY_GOTO] = "Goto",
	[KEY_CLEAR] = "Clear",			[KEY_POWER2] = "Power2",
	[KEY_OPTION] = "Option",		[KEY_INFO] = "Info",
	[KEY_TIME] = "Time",			[KEY_VENDOR] = "Vendor",
	[KEY_ARCHIVE] = "Archive",		[KEY_PROGRAM] = "Program",
	[KEY_CHANNEL] = "Channel",		[KEY_FAVORITES] = "Favorites",
	[KEY_EPG] = "EPG",			[KEY_PVR] = "PVR",
	[KEY_MHP] = "MHP",			[KEY_LANGUAGE] = "Language",
	[KEY_TITLE] = "Title",			[KEY_SUBTITLE] = "Subtitle",
	[KEY_ANGLE] = "Angle",			[KEY_ZOOM] = "Zoom",
	[KEY_MODE] = "Mode",			[KEY_KEYBOARD] = "Keyboard",
	[KEY_SCREEN] = "Screen",		[KEY_PC] = "PC",
	[KEY_TV] = "TV",			[KEY_TV2] = "TV2",
	[KEY_VCR] = "VCR",			[KEY_VCR2] = "VCR2",
	[KEY_SAT] = "Sat",			[KEY_SAT2] = "Sat2",
	[KEY_CD] = "CD",			[KEY_TAPE] = "Tape",
	[KEY_RADIO] = "Radio",			[KEY_TUNER] = "Tuner",
	[KEY_PLAYER] = "Player",		[KEY_TEXT] = "Text",
	[KEY_DVD] = "DVD",			[KEY_AUX] = "Aux",
	[KEY_MP3] = "MP3",			[KEY_AUDIO] = "Audio",
	[KEY_VIDEO] = "Video",			[KEY_DIRECTORY] = "Directory",
	[KEY_LIST] = "List",			[KEY_MEMO] = "Memo",
	[KEY_CALENDAR] = "Calendar",		[KEY_RED] = "Red",
	[KEY_GREEN] = "Green",			[KEY_YELLOW] = "Yellow",
	[KEY_BLUE] = "Blue",			[KEY_CHANNELUP] = "ChannelUp",
	[KEY_CHANNELDOWN] = "ChannelDown",	[KEY_FIRST] = "First",
	[KEY_LAST] = "Last",			[KEY_AB] = "AB",
	[KEY_NEXT] = "Next",			[KEY_RESTART] = "Restart",
	[KEY_SLOW] = "Slow",			[KEY_SHUFFLE] = "Shuffle",
	[KEY_BREAK] = "Break",			[KEY_PREVIOUS] = "Previous",
	[KEY_DIGITS] = "Digits",		[KEY_TEEN] = "TEEN",
	[KEY_TWEN] = "TWEN",			[KEY_DEL_EOL] = "Delete EOL",
	[KEY_DEL_EOS] = "Delete EOS",		[KEY_INS_LINE] = "Insert line",
	[KEY_DEL_LINE] = "Delete line",
};

char *absval[5] = { "Value", "Min  ", "Max  ", "Fuzz ", "Flat " };

char *relatives[REL_MAX + 1] = {
	[0 ... REL_MAX] = NULL,
	[REL_X] = "X",			[REL_Y] = "Y",
	[REL_Z] = "Z",			[REL_HWHEEL] = "HWheel",
	[REL_DIAL] = "Dial",		[REL_WHEEL] = "Wheel", 
	[REL_MISC] = "Misc",	
};

char *absolutes[ABS_MAX + 1] = {
	[0 ... ABS_MAX] = NULL,
	[ABS_X] = "X",			[ABS_Y] = "Y",
	[ABS_Z] = "Z",			[ABS_RX] = "Rx",
	[ABS_RY] = "Ry",		[ABS_RZ] = "Rz",
	[ABS_THROTTLE] = "Throttle",	[ABS_RUDDER] = "Rudder",
	[ABS_WHEEL] = "Wheel",		[ABS_GAS] = "Gas",
	[ABS_BRAKE] = "Brake",		[ABS_HAT0X] = "Hat0X",
	[ABS_HAT0Y] = "Hat0Y",		[ABS_HAT1X] = "Hat1X",
	[ABS_HAT1Y] = "Hat1Y",		[ABS_HAT2X] = "Hat2X",
	[ABS_HAT2Y] = "Hat2Y",		[ABS_HAT3X] = "Hat3X",
	[ABS_HAT3Y] = "Hat 3Y",		[ABS_PRESSURE] = "Pressure",
	[ABS_DISTANCE] = "Distance",	[ABS_TILT_X] = "XTilt",
	[ABS_TILT_Y] = "YTilt",		[ABS_TOOL_WIDTH] = "Tool Width",
	[ABS_VOLUME] = "Volume",	[ABS_MISC] = "Misc",
};

char *misc[MSC_MAX + 1] = {
	[ 0 ... MSC_MAX] = NULL,
	[MSC_SERIAL] = "Serial",	[MSC_PULSELED] = "Pulseled",
	[MSC_GESTURE] = "Gesture",	[MSC_RAW] = "RawData",
	[MSC_SCAN] = "ScanCode",
};

char *leds[LED_MAX + 1] = {
	[0 ... LED_MAX] = NULL,
	[LED_NUML] = "NumLock",		[LED_CAPSL] = "CapsLock", 
	[LED_SCROLLL] = "ScrollLock",	[LED_COMPOSE] = "Compose",
	[LED_KANA] = "Kana",		[LED_SLEEP] = "Sleep", 
	[LED_SUSPEND] = "Suspend",	[LED_MUTE] = "Mute",
	[LED_MISC] = "Misc",
};

char *repeats[REP_MAX + 1] = {
	[0 ... REP_MAX] = NULL,
	[REP_DELAY] = "Delay",		[REP_PERIOD] = "Period"
};

char *sounds[SND_MAX + 1] = {
	[0 ... SND_MAX] = NULL,
	[SND_CLICK] = "Click",		[SND_BELL] = "Bell",
	[SND_TONE] = "Tone"
};

char **names[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = events,			[EV_KEY] = keys,
	[EV_REL] = relatives,			[EV_ABS] = absolutes,
	[EV_MSC] = misc,			[EV_LED] = leds,
	[EV_SND] = sounds,			[EV_REP] = repeats,
};

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

//#define CONFIG_DEBUG

void Read_config_file(void)
{

/*************************************READ BLOCK**********************************************/

	file=fopen("config.txt", "r");
	if (file == NULL)
	{
	return;
	}
 
	while (lineIndex < 20  && (Read = getline(&line, &len, file)) != -1)
	{
		buf[lineIndex] = line;
        	line = NULL;
        	lineIndex++;
	}


/*** GETTING FPS ******/

	fps = strtok(buf[0], "=");
	while(fps!=NULL)
	{
		Fps = atoi(fps);
		fps = strtok(NULL, "=");
	}
#ifdef CONFIG_DEBUG
	PRINT_D("DBG : FPS = %d\n",Fps);
#endif

#ifdef CONFIG_FACTORY
	PRINT_F("FAC : FPS = %d\n",Fps);
#endif


/*** Getting Video Frame Width ***/

	width = strtok(buf[1], "=");
		while(width!=NULL)
		{
			WIDTH = atoi(width);
			width = strtok(NULL, "=");
		}
	
		if((WIDTH!=320)&&(WIDTH!=1280)&&(WIDTH!=640)&&(WIDTH!=1920))
		{
			#ifdef CONFIG_DEBUG
			PRINT_D("DBG : Video Width is Invalid\n");
			#endif
			#ifdef CONFIG_FACTORY
			PRINT_F("FAC : Video Width is Invalid\n");
			#endif
		}
		else
		{
			#ifdef CONFIG_DEBUG
			PRINT_D("DBG : Width = %d\n",WIDTH);
			#endif
			#ifdef CONFIG_FACTORY
			PRINT_F("FAC : Width = %d\n",WIDTH);
			#endif
		}


/*** GETTING VIDEO FRAME HEIGHT ******/
	height = strtok(buf[2], "=");
        	while(height!=NULL)
        	{
                	HEIGHT = atoi(height);
                	height = strtok(NULL, "=");
        	}

        	if((HEIGHT!=240)&&(HEIGHT!=720)&&(HEIGHT!=480)&&(HEIGHT!=1080))
        	{
			#ifdef CONFIG_DEBUG
                	PRINT_D("DBG : Video Height is Invalid\n");
			#endif
			#ifdef CONFIG_FACTORY
                	PRINT_F("FAC : Video Height is Invalid\n");
			#endif
        	}
        	else
        	{
			#ifdef CONFIG_DEBUG
                	PRINT_D("DBG : Height = %d\n",HEIGHT);
			#endif
			#ifdef CONFIG_FACTORY
                	PRINT_F("FAC : Height = %d\n",HEIGHT);
			#endif
        	}


/*** Assign Capture mode acoording to width and height***/
	if((WIDTH==320)&&(HEIGHT==240))
	{
		CAP_MODE = 1;
		#ifdef CONFIG_DEBUG
		PRINT_D("DBG : CAP_MDE=%d\n",CAP_MODE);
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : CAP_MDE=%d\n",CAP_MODE);
		#endif

	}
	else if((WIDTH==640)&&(HEIGHT==480))
	{
		CAP_MODE=0;
		#ifdef CONFIG_DEBUG
		PRINT_D("DBG : CAP_MDE=%d\n",CAP_MODE);
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : CAP_MDE=%d\n",CAP_MODE);
		#endif
	}
	else if((WIDTH==1280)&&(HEIGHT==720))
	{
		CAP_MODE=4;
		#ifdef CONFIG_DEBUG
		PRINT_D("DBG : CAP_MDE=%d\n",CAP_MODE);
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : CAP_MDE=%d\n",CAP_MODE);
		#endif
	}
	else if((WIDTH==1920)&&(HEIGHT==1080))
	{
		CAP_MODE = 5;
		#ifdef CONFIG_DEBUG
		PRINT_D("DBG : CAP_MDE=%d\n",CAP_MODE);
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : CAP_MDE=%d\n",CAP_MODE);
		#endif
	}
	else
	{
		#ifdef CONFIG_DEBUG	
		PRINT_D("DBG : 	You have entered Invalid Combination of Width & Height for Video\n \
						Check your config.txt\n \
					Valid Combinations are Listed Below\n \
				    		VGA     =   640X480\n \
				    		WVGA    =   320X240\n \
				    		HD      =  1280X720\n \
				    		FULL HD = 1920X1080\n");
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : 	You have entered Invalid Combination of Width & Height for Video\n \
						Check your config.txt\n \
					Valid Combinations are Listed Below\n \
				    		VGA     =   640X480\n \
				    		WVGA    =   320X240\n \
				    		HD      =  1280X720\n \
				    		FULL HD = 1920X1080\n");
		#endif
	}


/*** Getting Image Width****/
	i_width = strtok(buf[3], "=");
                while(i_width!=NULL)
                {
                        I_WIDTH = atoi(i_width);
                        i_width = strtok(NULL, "=");
                }

                if((I_WIDTH!=320)&&(I_WIDTH!=1280)&&(I_WIDTH!=640)&&(I_WIDTH!=1920)&&(I_WIDTH!=2592))
                {
			#ifdef CONFIG_DEBUG
			PRINT_D("DBG : Image Width is Invalid\n");
			#endif
			#ifdef CONFIG_FACTORY
			PRINT_F("FAC : Image Width is Invalid\n");
			#endif
                }
                else
                {
			#ifdef CONFIG_DEBUG
			PRINT_D("DBG : Image Width = %d\n",I_WIDTH);
			#endif
			#ifdef CONFIG_FACTORY
			PRINT_F("FAC : Image Width = %d\n",I_WIDTH);
			#endif
                }

/*** Getting Image Height ****/
	 i_height = strtok(buf[4], "=");
                while(i_height!=NULL)
                {
                        I_HEIGHT = atoi(i_height);
                        i_height = strtok(NULL, "=");
                }

                if((I_HEIGHT!=240)&&(I_HEIGHT!=720)&&(I_HEIGHT!=480)&&(I_HEIGHT!=1080)&&(I_HEIGHT!=1944))
                {
			#ifdef CONFIG_DEBUG
                	PRINT_D("DBG : Image Height is Invalid\n");
			#endif
			#ifdef CONFIG_FACTORY
                	PRINT_F("FAC : Image Height is Invalid\n");
			#endif
                }
                else
                {
			#ifdef CONFIG_DEBUG
                	PRINT_D("DBG : Image Height = %d\n",I_HEIGHT);
			#endif
			#ifdef CONFIG_FACTORY
                	PRINT_F("FAC : Image Height = %d\n",I_HEIGHT);
			#endif
                }

/*** MATCHING THE IMAGE RESOLUTION ***/

        if(((I_WIDTH==640)&&(I_HEIGHT==480)) || ((I_WIDTH==1280)&&(I_HEIGHT==720)) || 
           ((I_WIDTH==320)&&(I_HEIGHT==240)) || ((I_WIDTH==1920)&&(I_HEIGHT==1080))|| 
           ((I_WIDTH==2592)&&(I_HEIGHT==1944)))
        {
		#ifdef CONFIG_DEBUG
                PRINT_D("DBG : Image RES Mathches the DB\n");
		#endif
		#ifdef CONFIG_FACTORY
                PRINT_F("FAC : Image RES Mathches DB\n");
		#endif
		
        }
        else
        {
		#ifdef CONFIG_DEBUG	
		PRINT_D("DBG : 	You have entered Invalid Combination of Width & Height for Image\n \
						Check your config.txt\n \
					Valid Combinations are Listed Below\n \
				    		VGA     =   640X480\n \
				    		WVGA    =   320X240\n \
				    		HD      =  1280X720\n \
				    		FULL HD = 1920X1080\n \
						5MP     = 2592X1944\n");
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : 	You have entered Invalid Combination of Width & Height for Image\n \
						Check your config.txt\n \
					Valid Combinations are Listed Below\n \
				    		VGA     =   640X480\n \
				    		WVGA    =   320X240\n \
				    		HD      =  1280X720\n \
				    		FULL HD = 1920X1080\n \
						5MP     = 2592X1944\n");
		#endif
                return;
        }

/*** END OF CHECKING RESOLUTION***/

/**** Getting Video File Name ***/

	f_name = strtok(buf[5], "=");
        while(f_name!=NULL)
        {
                F_name = f_name;
                f_name = strtok(NULL, "=");
        }
	 for(i=0;i<50;i++)
        {
                while(F_name[i]=='\n')
                {
                        memset(&F_name[i], '\0', 1);
                }
        }

	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : Video File Name = %s\n",F_name);
	#endif
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : Video File Name = %s\n",F_name);
	#endif	
 	
/*** Getting FTP Details****/

/* FTP LINK */

	ftp_link = strtok(buf[6], "=");
        while(ftp_link!=NULL)
        {
                FTP_LINK = ftp_link;
                ftp_link = strtok(NULL, "=");
        }
	
	for(i=0;i<50;i++)
	{
		while(FTP_LINK[i]=='\n')
		{
			memset(&FTP_LINK[i], '\0', 1);
		}
	}
	#ifdef ONFIG_DEBUG
        PRINT_D("DBG : FTP_LINK= %s\n",FTP_LINK);
	#endif
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : FTP_LINK= %s\n",FTP_LINK);
	#endif


/* FTP USER NAME */
	ftp_uname = strtok(buf[7], "=");
        while(ftp_uname!=NULL)
        {
                FTP_UNAME = ftp_uname;
                ftp_uname = strtok(NULL, "=");
        }

        for(i=0;i<50;i++)
        {
                while(FTP_UNAME[i]=='\n')
                {
                        memset(&FTP_UNAME[i], '\0', 1);
                }
        }
	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : FTP_UNAME= %s\n",FTP_UNAME);
	#endif
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : FTP_UNAME= %s\n",FTP_UNAME);
	#endif

/* FTP PASSWORD */
	ftp_pwd = strtok(buf[8], "=");
        while(ftp_pwd!=NULL)
        {
                FTP_PWD = ftp_pwd;
                ftp_pwd = strtok(NULL, "=");
        }

        for(i=0;i<50;i++)
        {
                while(FTP_PWD[i]=='\n')
                {
                        memset(&FTP_PWD[i], '\0', 1);
                }
        }
	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : FTP_PWD= %s\n",FTP_PWD);
	#endif
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : FTP_PWD= %s\n",FTP_PWD);
	#endif	

/***END OF FTP DETAILS******/

/** GETTING APN NAME ****/

	apn = strtok(buf[9], "=");
        while(apn!=NULL)
        {
                APN = apn;
                apn = strtok(NULL, "=");
        }

        for(i=0;i<50;i++)
        {
                while(APN[i]=='\n')
                {
                        memset(&APN[i], '\0', 1);
                }
        }
	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : APN NAME= %s\n",APN);
	#endif

	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : APN NAME= %s\n",APN);
	#endif
/*** Getting IP ****/

	ip = strtok(buf[10], "=");
	while(ip!=NULL)
	{
		IP = ip;
		ip = strtok(NULL, "=");
	}
	#ifdef CONFIG_DEBUG
	PRINT_D("DBG : HOST IP=%s\n",IP);
	#endif

	#ifdef CONFIG_FACTORY
	PRINT_F("FAC : HOST IP=%s\n",IP);
	#endif

/*** Getting Video Size****/

	v_sz = strtok(buf[11], "=");
                while(v_sz!=NULL)
                {
                        V_SIZE = atoi(v_sz);
                        v_sz = strtok(NULL, "=");
                }
	#ifdef CONFIG_DEBUG
	PRINT_D("DBG : Video Size =%dMB\n",V_SIZE);
	#endif

	#ifdef CONFIG_FACTORY
	PRINT_F("FAC : Video Size =%dMB\n",V_SIZE);
	#endif	
/*** VIDEO CAPTURE INTERVAL *****/

	v_intr = strtok(buf[12], "=");
                while(v_intr!=NULL)
                {
                        V_IVAL = atoi(v_intr);
                        v_intr = strtok(NULL, "=");
                }

	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : Video Capture Interval =%dmin\n",V_IVAL);
	#endif	
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : Video Capture Interval =%dmin\n",V_IVAL);
	#endif	

/***END OF VIDEO CAPTURE INTERVAL***/ 

/*** IMAGE CAPTURE INTERVAL *****/

        i_intr = strtok(buf[13], "=");
                while(i_intr!=NULL)
                {
                        I_IVAL = atoi(i_intr);
                        i_intr = strtok(NULL, "=");
                }

	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : Image Capture Interval =%dmin\n",I_IVAL);
	#endif	
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : Image Capture Interval =%dmin\n",I_IVAL);
	#endif	

/***END OF IMAGE CAPTURE INTERVAL***/

/**** Getting Image File Name ***/

        i_name = strtok(buf[14], "=");
        while(i_name!=NULL)
        {
                I_name = i_name;
                i_name = strtok(NULL, "=");
        }
	for(i=0;i<50;i++)
        {
                while(I_name[i]=='\n')
                {
                        memset(&I_name[i], '\0', 1);
                }
        }

	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : Image File Name = %s\n",I_name);
	#endif	
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : Image File Name = %s\n",I_name);
	#endif	


//    return 0;	

}
/*************** GSTREAMER THREAD TO START AND STOP THE VIDEO CAPTURE **********************/
/* Signal handler for ctrl+c */
void intHandler(int dummy) {
//	printf("Enter int handler----------------\n");

	/* Out of the main loop, clean up nicely */
	g_print ("Returned, stopping playback\n");
	gst_element_set_state (pipeline, GST_STATE_NULL);
	g_print ("Deleting pipeline\n");
	gst_object_unref (GST_OBJECT (pipeline));
	g_main_loop_unref (loop);

	exit(0);
}

static gboolean bus_call	(GstBus     *bus,
							 GstMessage *msg,
							 gpointer    data)
{
//	printf("Enter booloeabn call --------------------\n");
	GMainLoop *loop = (GMainLoop *) data;

	switch (GST_MESSAGE_TYPE (msg)) {

		case GST_MESSAGE_EOS:
			g_print ("End of stream\n");
			g_main_loop_quit (loop);
		break;

		case GST_MESSAGE_ERROR: {
			gchar  *debug;
			GError *error;

			gst_message_parse_error (msg, &error, &debug);
			g_free (debug);

			g_printerr ("Error: %s\n", error->message);
			g_error_free (error);

			g_main_loop_quit (loop);
		break;
		}
		
		default:
		break;
	}

	return TRUE;
}

static gboolean
stop_capture (gpointer user_data)
{
	while(1)
{
	//printf("Enter Stop Capture\n");
	g_main_loop_quit (loop);
 	g_main_loop_unref (loop);
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (GST_OBJECT (pipeline));
	printf("STOP CAPTURE\n");
	sem_post(&lock);
	snprintf(res, sizeof(res), "%s%d.avi",F_name,f_cnt);
        printf("Current file name %s-----------------------\n", res);
        f_cnt++;
	factory_make(res);
	watcher_make();
	pipeline_make();
	state_handler();
	g_main_loop_run(loop);
//	sem_post(&len);
//	printf("After i incr\n");
	g_timeout_add ((20 * 1000), (GSourceFunc) stop_capture, NULL);
	}
	//g_main_loop_run(loop);

}

/*gst-launch mfw_v4lsrc fps-n=30 capture-width=640 capture-height=480 capture-mode=0  ! queue ! mfw_vpuencoder codec-type=0 ! avimux ! filesink location=test.avi sync=false*/

int factory_make(char* location)
{
	//printf("Enter factory make ------------------\n");

	timer = g_timer_new ();

	capture_count++;
	g_timer_start (timer);

	g_timeout_add ((20 * 1000), (GSourceFunc) stop_capture, NULL);
	/* Create gstreamer elements */
	pipeline = gst_pipeline_new ("pipeline");
	source   = gst_element_factory_make ("mfw_v4lsrc",	"mfw_v4lsrc");
	filter   = gst_element_factory_make ("capsfilter",	"filter");
//	accel    = gst_element_factory_make ("dmaiaccel",		"dmaiaccel");
	encoder  = gst_element_factory_make ("mfw_vpuencoder",	"mfw_vpuencoder");
	queue    = gst_element_factory_make ("queue",			"queue");
	muxer    = gst_element_factory_make ("avimux",			"mux");
	sink     = gst_element_factory_make ("filesink",		"filesink");

	/* Video caps */
	video_caps = gst_caps_new_simple ("video/x-raw",
	"format", G_TYPE_STRING, "yuv422",
	"width", G_TYPE_INT, 640,
	"height", G_TYPE_INT, 480,
	"capture-mode", G_TYPE_INT, 0,
	"fps-n", G_TYPE_INT, 30,
	NULL);

	if (!pipeline || !source || !filter || !encoder || !queue || !muxer || !sink) {
		g_printerr ("One element could not be created. Exiting.\n");
		return -1;
	}

	/* Set up elements */
//	g_object_set (G_OBJECT (filter), "caps", video_caps, NULL);
	//gst_caps_unref (video_caps);
	//g_object_set (G_OBJECT (source), "num-buffers", 300, NULL);
	g_object_set (G_OBJECT (source), "fps-n", 30, NULL);
	g_object_set (G_OBJECT (source), "capture-width", 640, NULL);
	g_object_set (G_OBJECT (source), "capture-height", 480, NULL);
	g_object_set (G_OBJECT (source), "capture-mode", 0, NULL);
	g_object_set (G_OBJECT (encoder), "codec-type", 0, NULL);

	/* we set the input filename to the source element */
	g_object_set (G_OBJECT (sink), "location", location, NULL);

	return 0;
}

int pipeline_make()
{
	//printf("Enter pipeline make ---------------\n");
        /* we add all elements into the pipeline */
        gst_bin_add_many (GST_BIN (pipeline),
                source, queue, encoder, muxer, sink, filter, NULL);
        /* we link the elements together */
        gst_element_link_many (source, queue, encoder, muxer, sink, NULL);

        return 0;
}

int state_handler()
{
	//printf("Enter State handler ---------------\n");
        gst_element_set_state (pipeline, GST_STATE_PLAYING);
        return 0;
}

int watcher_make()
{
	//printf("Enter watcher--------------\n");
        /* we add a message handler */
        bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
        bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
        gst_object_unref (bus);
        return 0;
}

void * start_capture()
{ 
	printf("Enter Start Capture\n");
/* Initialisation */
        gst_init (NULL, NULL);
        signal(SIGINT, intHandler);

        loop = g_main_loop_new (NULL, FALSE);
#if 0
        /* Check input arguments */
        if (argc != 2) {
                g_printerr ("Usage: %s <Recorded file name eg: test.h264>\n", argv[0]);
                return -1;
        }
#endif
        /* Initialize elements */
        if(factory_make(F_name) != 0)
                return -1;

        /* Add function to watch bus */
        if(watcher_make() != 0)
                return -1;

        /* Add elements to pipeline, and link them */
        if(pipeline_make() != 0)
                return -1;

        /* Handle pipeline state */
        /* Set the pipeline to "playing" state*/
        if(state_handler() !=0)
                return -1;

        /* Iterate */
        g_print ("Running...\n");
        g_main_loop_run (loop);
	

}

/****************** END OF GSTREAMER THREAD ************************************/

/***************** FTP THREAD TO SEND THE VIDEO FILES**************************/
void * Access_ftp()
{
printf("Enter FTP-------------\n");

/*** ACCESSING FTP ***/


/* this enables printing of sent FTP commands */
        ftp_debug=1;
/* and this printing of status responses */
        ftp_verbose=1;


int error(char *msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

int process_entry(char *ent, unsigned len)
{
  printf("%s\n", ent);
  return 0;
}

/** FTP OPEN **/
//        printf("FTP_LINK = %s  %s  %s\n",FTP_LINK,FTP_UNAME, FTP_PWD);
        if (ftp_open(FTP_LINK,FTP_UNAME ,FTP_PWD))
        error("ftp_open");

/** List the Directories and file in FTP*/

      //  ftp_ls(1, "", process_entry);
/** SENDING THE FILES FROM LOCAL TO REMOTE **/
        
while(1)
{
sem_wait(&lock);
        char *local= res;
printf("Local=%s\n",local);
        char *remote= local;
        printf("remote=%s\n",remote);
        ftp_putfile(local, local, 1, 1);

/** RETRIVING FILES FROM THE FTP SERVER **/
//      ftp_getfile("ftptest.txt", "blk.txt", 1);
}

}


/********* END OF FTP THREAD *******************************/

/********** Thread Function   **************/
void start_process(void)
{
printf("Enter main Thread\n");
    //    int status;
        pthread_t tcam, tftp;

        pthread_create(&tcam,NULL,start_capture,NULL);
        pthread_create(&tftp,NULL,Access_ftp,NULL);

        pthread_join(tcam,NULL);
        pthread_join(tftp,NULL);
        return 0;

}


/**********End of Thread **************/


int main (int argc, char **argv)
{


	printf(" SHAN CHECK ARGC=%d ARGV=%s\n",argc,argv[argc-1]);
	int fd, rd, i, j, k;
	struct input_event ev[64];
	int version;
	unsigned short id[4];
	unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
	char name[256] = "Unknown";
	char command[150];
	int abs[5];

	if (argc < 2) {
		printf("Usage: evtest /dev/input/eventX\n");
		printf("Where X = input device number\n");
		return 1;
	}

	if ((fd = open(argv[argc - 1], O_RDONLY)) < 0) {
		perror("evtest");
		return 1;
	}

	if (ioctl(fd, EVIOCGVERSION, &version)) {
		perror("evtest: can't get version");
		return 1;
	}

	printf("Input driver version is %d.%d.%d\n",
		version >> 16, (version >> 8) & 0xff, version & 0xff);

	ioctl(fd, EVIOCGID, id);
	printf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
		id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);

	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("Input device name: \"%s\"\n", name);

	memset(bit, 0, sizeof(bit));
	ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);
	printf("Supported events:\n");

	for (i = 0; i < EV_MAX; i++)
		if (test_bit(i, bit[0])) {
			printf("  Event type %d (%s)\n", i, events[i] ? events[i] : "?");
			if (!i) continue;
			ioctl(fd, EVIOCGBIT(i, KEY_MAX), bit[i]);
			for (j = 0; j < KEY_MAX; j++) 
				if (test_bit(j, bit[i])) {
					printf("    Event code %d (%s)\n", j, names[i] ? (names[i][j] ? names[i][j] : "?") : "?");
					if (i == EV_ABS) {
						ioctl(fd, EVIOCGABS(j), abs);
						for (k = 0; k < 5; k++)
							if ((k < 3) || abs[k])
								printf("      %s %6d\n", absval[k], abs[k]);
					}
				}
		}
		

	printf("Testing ... (interrupt to exit)\n");

	while (1) {
		rd = read(fd, ev, sizeof(struct input_event) * 64);

		if (rd < (int) sizeof(struct input_event)) {
			printf("yyy\n");
			perror("\nevtest: error reading");
			return 1;
		}
		printf("SHAN RDVAL=%d SIZE=%d\n",rd,sizeof(struct input_event));
		for (i=1; i < rd / sizeof(struct input_event); i++)

			if (ev[i].type == KEY_VOLUMEDOWN) {
				printf("Event: time %ld.%06ld, -------------- %s ------------\n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].code ? "Config Sync" : "Report Sync" );
			} else if (ev[i].type == EV_MSC && (ev[i].code == MSC_RAW || ev[i].code == MSC_SCAN)) {
				printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %02x\n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
					events[ev[i].type] ? events[ev[i].type] : "?",
					ev[i].code,
					names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
					ev[i].value);
			} else{

				Read_config_file();
				start_process();

/** CREATING THREAD **/

//				char cmd[1024];
//				sprintf(cmd, "gst-launch mfw_v4lsrc fps-n=%d capture-width=%d capture-height=%d capture-mode=%d  ! queue ! mfw_vpuencoder codec-type=0 ! avimux ! filesink location=%s.avi sync=false", Fps,WIDTH,HEIGHT,CAP_MODE,F_name);
//				printf("%s\n", cmd);

//				Access_ftp();
		

				printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %d\n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
					events[ev[i].type] ? events[ev[i].type] : "?",
					ev[i].code,
					names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
					ev[i].value);
			//	return 0;
			}	

	}
    while(lineIndex > 0)
        {
        lineIndex--;
        if (buf[lineIndex])
            {
            free(buf[lineIndex]);
            }
        }
}


