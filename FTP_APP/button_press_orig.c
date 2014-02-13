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
	int i;
/* Variables for Taking Video Width & Height*/
typedef struct
{
	char *width, *height, *f_name,*v_sz, *v_intr;
	int WIDTH,HEIGHT;
	char *F_name;
	unsigned char *fps;
	int Fps, V_SIZE, V_IVAL,CAP_MODE;
}video_param;
/* Variables for Taking Image Width & Height */
struct image_param
{
	char *i_width,*i_height,*i_intr,*i_name, *I_name;
        int I_WIDTH,I_HEIGHT;
	int I_IVAL;
}image;
/* Variables for Taking FPS Value*/
	//unsigned char *fps;
	//int i, Fps;
/*Varialbles for Taking FIle name*/
	//char *f_name;
//	static char *F_name;
/* Variables for taking FTP deatils*/
struct ftp_param
{
	char *ftp_link;
	char *FTP_LINK;
	char *ftp_uname;
	char *FTP_UNAME;
	char *ftp_pwd;
	char *FTP_PWD;
}ftp;
/*Variables for taking APN NAME*/
struct apn_param
{
	char *apn; 
	char *APN;
}Apn;
/* Variables for taking Video size and capture interval*/
//	char *v_sz, *v_intr;
//	int V_SIZE, V_IVAL;

/* Variables for taking Image Capture Interval */
//	char *i_intr;
//	int I_IVAL;

/* Variables for taking Image File Name */
//	char *i_name, *I_name;

/*Variables for Taking Capture mode*/
//	int CAP_MODE;

/*Variables for Getting IP*/
struct host_ip
{
	char *ip, *IP;
}hostip;
/* ADDED BY SHAN */
	video_param video;
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

	video.fps = strtok(buf[0], "=");
	while(video.fps!=NULL)
	{
		video.Fps = atoi(video.fps);
		video.fps = strtok(NULL, "=");
	}
#ifdef CONFIG_DEBUG
	PRINT_D("DBG : FPS = %d\n",video.Fps);
#endif

#ifdef CONFIG_FACTORY
	PRINT_F("FAC : FPS = %d\n",video.Fps);
#endif


/*** Getting Video Frame Width ***/

	video.width = strtok(buf[1], "=");
		while(video.width!=NULL)
		{
			video.WIDTH = atoi(video.width);
			video.width = strtok(NULL, "=");
		}
	
		if((video.WIDTH!=320)&&(video.WIDTH!=1280)&&(video.WIDTH!=640)&&(video.WIDTH!=1920))
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
			PRINT_D("DBG : Width = %d\n",video.WIDTH);
			#endif
			#ifdef CONFIG_FACTORY
			PRINT_F("FAC : Width = %d\n",video.WIDTH);
			#endif
		}


/*** GETTING VIDEO FRAME HEIGHT ******/
	video.height = strtok(buf[2], "=");
        	while(video.height!=NULL)
        	{
                	video.HEIGHT = atoi(video.height);
                	video.height = strtok(NULL, "=");
        	}

        	if((video.HEIGHT!=240)&&(video.HEIGHT!=720)&&(video.HEIGHT!=480)&&(video.HEIGHT!=1080))
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
                	PRINT_D("DBG : Height = %d\n",video.HEIGHT);
			#endif
			#ifdef CONFIG_FACTORY
                	PRINT_F("FAC : Height = %d\n",video.HEIGHT);
			#endif
        	}


/*** Assign Capture mode acoording to width and height***/
	if((video.WIDTH==320)&&(video.HEIGHT==240))
	{
		video.CAP_MODE = 1;
		#ifdef CONFIG_DEBUG
		PRINT_D("DBG : CAP_MDE=%d\n",video.CAP_MODE);
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : CAP_MDE=%d\n",video.CAP_MODE);
		#endif

	}
	else if((video.WIDTH==640)&&(video.HEIGHT==480))
	{
		video.CAP_MODE=0;
		#ifdef CONFIG_DEBUG
		PRINT_D("DBG : CAP_MDE=%d\n",video.CAP_MODE);
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : CAP_MDE=%d\n",video.CAP_MODE);
		#endif
	}
	else if((video.WIDTH==1280)&&(video.HEIGHT==720))
	{
		video.CAP_MODE=4;
		#ifdef CONFIG_DEBUG
		PRINT_D("DBG : CAP_MDE=%d\n",video.CAP_MODE);
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : CAP_MDE=%d\n",video.CAP_MODE);
		#endif
	}
	else if((video.WIDTH==1920)&&(video.HEIGHT==1080))
	{
		video.CAP_MODE = 5;
		#ifdef CONFIG_DEBUG
		PRINT_D("DBG : CAP_MDE=%d\n",video.CAP_MODE);
		#endif
		#ifdef CONFIG_FACTORY
		PRINT_F("FAC : CAP_MDE=%d\n",video.CAP_MODE);
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
	image.i_width = strtok(buf[3], "=");
                while(image.i_width!=NULL)
                {
                        image.I_WIDTH = atoi(image.i_width);
                        image.i_width = strtok(NULL, "=");
                }

                if((image.I_WIDTH!=320)&&(image.I_WIDTH!=1280)&&(image.I_WIDTH!=640)&&(image.I_WIDTH!=1920)&&(image.I_WIDTH!=2592))
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
			PRINT_D("DBG : Image Width = %d\n",image.I_WIDTH);
			#endif
			#ifdef CONFIG_FACTORY
			PRINT_F("FAC : Image Width = %d\n",image.I_WIDTH);
			#endif
                }

/*** Getting Image Height ****/
	 image.i_height = strtok(buf[4], "=");
                while(image.i_height!=NULL)
                {
                        image.I_HEIGHT = atoi(image.i_height);
                        image.i_height = strtok(NULL, "=");
                }

                if((image.I_HEIGHT!=240)&&(image.I_HEIGHT!=720)&&(image.I_HEIGHT!=480)&&(image.I_HEIGHT!=1080)&&(image.I_HEIGHT!=1944))
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
                	PRINT_D("DBG : Image Height = %d\n",image.I_HEIGHT);
			#endif
			#ifdef CONFIG_FACTORY
                	PRINT_F("FAC : Image Height = %d\n",image.I_HEIGHT);
			#endif
                }

/*** MATCHING THE IMAGE RESOLUTION ***/

        if(((image.I_WIDTH==640)&&(image.I_HEIGHT==480)) || ((image.I_WIDTH==1280)&&(image.I_HEIGHT==720)) || 
           ((image.I_WIDTH==320)&&(image.I_HEIGHT==240)) || ((image.I_WIDTH==1920)&&(image.I_HEIGHT==1080))|| 
           ((image.I_WIDTH==2592)&&(image.I_HEIGHT==1944)))
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

	video.f_name = strtok(buf[5], "=");
        while(video.f_name!=NULL)
        {
                video.F_name = video.f_name;
                video.f_name = strtok(NULL, "=");
        }

	for(i=0;i<50;i++)
        {
                while(video.F_name[i]=='\n')
                {
                        memset(&video.F_name[i], '\0', 1);
                }
        }

	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : Video File Name = %s\n",video.F_name);
	#endif
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : Video File Name = %s\n",video.F_name);
	#endif	
 	
/*** Getting FTP Details****/

/* FTP LINK */

	ftp.ftp_link = strtok(buf[6], "=");
        while(ftp.ftp_link!=NULL)
        {
                ftp.FTP_LINK = ftp.ftp_link;
                ftp.ftp_link = strtok(NULL, "=");
        }
	
	for(i=0;i<50;i++)
	{
		while(ftp.FTP_LINK[i]=='\n')
		{
			memset(&ftp.FTP_LINK[i], '\0', 1);
		}
	}
	#ifdef ONFIG_DEBUG
        PRINT_D("DBG : FTP_LINK= %s\n",ftp.FTP_LINK);
	#endif
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : FTP_LINK= %s\n",ftp.FTP_LINK);
	#endif


/* FTP USER NAME */
	ftp.ftp_uname = strtok(buf[7], "=");
        while(ftp.ftp_uname!=NULL)
        {
                ftp.FTP_UNAME = ftp.ftp_uname;
                ftp.ftp_uname = strtok(NULL, "=");
        }

        for(i=0;i<50;i++)
        {
                while(ftp.FTP_UNAME[i]=='\n')
                {
                        memset(&ftp.FTP_UNAME[i], '\0', 1);
                }
        }
	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : FTP_UNAME= %s\n",ftp.FTP_UNAME);
	#endif
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : FTP_UNAME= %s\n",ftp.FTP_UNAME);
	#endif

/* FTP PASSWORD */
	ftp.ftp_pwd = strtok(buf[8], "=");
        while(ftp.ftp_pwd!=NULL)
        {
                ftp.FTP_PWD = ftp.ftp_pwd;
                ftp.ftp_pwd = strtok(NULL, "=");
        }

        for(i=0;i<50;i++)
        {
                while(ftp.FTP_PWD[i]=='\n')
                {
                        memset(&ftp.FTP_PWD[i], '\0', 1);
                }
        }
	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : FTP_PWD= %s\n",ftp.FTP_PWD);
	#endif
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : FTP_PWD= %s\n",ftp.FTP_PWD);
	#endif	

/***END OF FTP DETAILS******/

/** GETTING APN NAME ****/

	Apn.apn = strtok(buf[9], "=");
        while(Apn.apn!=NULL)
        {
                Apn.APN = Apn.apn;
                Apn.apn = strtok(NULL, "=");
        }

        for(i=0;i<50;i++)
        {
                while(Apn.APN[i]=='\n')
                {
                        memset(&Apn.APN[i], '\0', 1);
                }
        }
	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : APN NAME= %s\n",Apn.APN);
	#endif

	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : APN NAME= %s\n",Apn.APN);
	#endif
/*** Getting IP ****/

	hostip.ip = strtok(buf[10], "=");
	while(hostip.ip!=NULL)
	{
		hostip.IP = hostip.ip;
		hostip.ip = strtok(NULL, "=");
	}
	#ifdef CONFIG_DEBUG
	PRINT_D("DBG : HOST IP=%s\n",hostip.IP);
	#endif

	#ifdef CONFIG_FACTORY
	PRINT_F("FAC : HOST IP=%s\n",hostip.IP);
	#endif

/*** Getting Video Size****/

	video.v_sz = strtok(buf[11], "=");
                while(video.v_sz!=NULL)
                {
                        video.V_SIZE = atoi(video.v_sz);
                        video.v_sz = strtok(NULL, "=");
                }
	#ifdef CONFIG_DEBUG
	PRINT_D("DBG : Video Size =%dMB\n",video.V_SIZE);
	#endif

	#ifdef CONFIG_FACTORY
	PRINT_F("FAC : Video Size =%dMB\n",video.V_SIZE);
	#endif	
/*** VIDEO CAPTURE INTERVAL *****/

	video.v_intr = strtok(buf[12], "=");
                while(video.v_intr!=NULL)
                {
                        video.V_IVAL = atoi(video.v_intr);
                        video.v_intr = strtok(NULL, "=");
                }

	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : Video Capture Interval =%dmin\n",video.V_IVAL);
	#endif	
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : Video Capture Interval =%dmin\n",video.V_IVAL);
	#endif	

/***END OF VIDEO CAPTURE INTERVAL***/ 

/*** IMAGE CAPTURE INTERVAL *****/

        image.i_intr = strtok(buf[13], "=");
                while(image.i_intr!=NULL)
                {
                        image.I_IVAL = atoi(image.i_intr);
                        image.i_intr = strtok(NULL, "=");
                }

	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : Image Capture Interval =%dmin\n",image.I_IVAL);
	#endif	
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : Image Capture Interval =%dmin\n",image.I_IVAL);
	#endif	

/***END OF IMAGE CAPTURE INTERVAL***/

/**** Getting Image File Name ***/

        image.i_name = strtok(buf[14], "=");
        while(image.i_name!=NULL)
        {
                image.I_name = image.i_name;
                image.i_name = strtok(NULL, "=");
        }
	for(i=0;i<50;i++)
        {
                while(image.I_name[i]=='\n')
                {
                        memset(&image.I_name[i], '\0', 1);
                }
        }

	#ifdef CONFIG_DEBUG
        PRINT_D("DBG : Image File Name = %s\n",image.I_name);
	#endif	
	#ifdef CONFIG_FACTORY
        PRINT_F("FAC : Image File Name = %s\n",image.I_name);
	#endif	

#if 0
    while(lineIndex > 0)
        {
        lineIndex--;
        if (buf[lineIndex])
            {
            free(buf[lineIndex]);
            }
        }
//    return 0;	
#endif
}
#if 1
void Access_ftp(void)
{

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
	printf("FTP_LINK = %s  %s  %s\n",ftp.FTP_LINK,ftp.FTP_UNAME, ftp.FTP_PWD);
	if (ftp_open(ftp.FTP_LINK,ftp.FTP_UNAME ,ftp.FTP_PWD)) 
	error("ftp_open");
	
/** List the Directories and file in FTP*/

	ftp_ls(1, "", process_entry);
/** SENDING THE FILES FROM LOCAL TO REMOTE **/
	strcat(video.F_name, ".avi");
	char *local= video.F_name;
	printf("Local=%s\n",local);
	char *remote= video.F_name;
	printf("remote=%s\n",remote);
	ftp_putfile(local, remote, 1, 1);

/** RETRIVING FILES FROM THE FTP SERVER **/
//	ftp_getfile("ftptest.txt", "blk.txt", 1);

}
#endif
/**** Adding Timer   *****/
volatile int mark = 0;

void trigger(int sig)
{
        mark = 1;
}



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
				char cmd[1024];
				sprintf(cmd, "gst-launch mfw_v4lsrc fps-n=%d capture-width=%d capture-height=%d capture-mode=%d  ! queue ! mfw_vpuencoder codec-type=0 ! avimux ! filesink location=%s.avi sync=false", video.Fps,video.WIDTH,video.HEIGHT,video.CAP_MODE,video.F_name);
				printf("%s\n", cmd);
/*** Start Timer***/
					signal(SIGALRM, trigger);
					alarm(30);

						while (!mark)
						{
   							system(cmd);
						//	printf("capturing\n");
						}
				printf("closing after 30 sec...\n");

				Access_ftp();
		


				
#if 0 
				strcpy( command, "gst-launch-0.10 mfw_v4lsrc num-buffers=1 ! video/x-raw-yuv,width=640,height=480,framerate=8/1 ! ffmpegcolorspace ! textoverlay halign=left valign= bottom xpad=\"30\" ypad=\"70\" font-desc=\"arial\" text=\"testid: 0001\" ! textoverlay halign=left valign=bottom xpad=\"30\" ypad=\"50\" font-desc=\"arial\" text=\"bacval: 0.003\" ! timeoverlay halign=left valign=bottom xpad=\"30\" ypad=\"30\" font-desc=\"arial\" text=\"time:\" ! textoverlay halign=left valign=bottom xpad=\"10\" ypad=\"10\" font-desc=\"arial\" text=\"date: 05/06/2013\" ! jpegenc ! filesink location=test9.jpg  " );
#endif

				 //capture_init ();
				printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %d\n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
					events[ev[i].type] ? events[ev[i].type] : "?",
					ev[i].code,
					names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
					ev[i].value);
    while(lineIndex > 0)
        {
        lineIndex--;
        if (buf[lineIndex])
            {
            free(buf[lineIndex]);
            }
        }
			//	return 0;
			}	

	}
}


