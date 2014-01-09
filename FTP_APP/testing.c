#include <stdlib.h>     /* exit, malloc, realloc, free */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ftplib.h"
//#include "char.h"
int main()
{
/********* VARIABLE DECLARATION *************/

/* Variables for FIle read*/
	FILE *file;
	char *line = NULL;
	size_t len = 0;
	char read;
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
	char *f_name, *F_name;

/* Variables for taking FTP deatils*/

	char *ftp_link;
	char *FTP_LINK=NULL;
	char *ftp_uname;
	char *FTP_UNAME=NULL;
	char *ftp_pwd;
	char *FTP_PWD=NULL;
/*Variables for taking APN NAME*/
	char *apn; 
	char *APN=NULL;

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

/*************************************READ BLOCK**********************************************/

	file=fopen("config.txt", "r");
	if (file == NULL)
	{
	return 1;
	}
 
	while (lineIndex < 20  && (read = getline(&line, &len, file)) != -1)
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

	printf("FPS = %d\n",Fps);


/*** Getting Video Frame Width ***/

	width = strtok(buf[1], "=");
		while(width!=NULL)
		{
			WIDTH = atoi(width);
			width = strtok(NULL, "=");
		}
	
		if((WIDTH!=320)&&(WIDTH!=1280)&&(WIDTH!=640)&&(WIDTH!=1920))
		{
			printf("Video Width is Invalid\n");
		}
		else
		{
			printf("Width = %d\n",WIDTH);
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
                	printf("Video Height is Invalid\n");
        	}
        	else
        	{
                	printf("Height = %d\n",HEIGHT);
        	}


/*** Assign Capture mode acoording to width and height***/
	if((WIDTH==320)&&(HEIGHT==240))
	{
		CAP_MODE = 1;
		printf("CAP_MDE=%d\n",CAP_MODE);
	}
	else if((WIDTH==640)&&(HEIGHT==480))
	{
		CAP_MODE=0;
		printf("CAP_MDE=%d\n",CAP_MODE);
	}
	else if((WIDTH==1280)&&(HEIGHT==720))
	{
		CAP_MODE=4;
		printf("CAP_MDE=%d\n",CAP_MODE);
	}
	else if((WIDTH==1920)&&(HEIGHT==1080))
	{
		CAP_MODE = 5;
		printf("CAP_MDE=%d\n",CAP_MODE);
	}
	else
	{
		printf("Width and Height Does not match\n");
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
                        printf("Image Width is Invalid\n");
                }
                else
                {
                        printf("Image Width = %d\n",I_WIDTH);
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
                        printf("Image Height is Invalid\n");
                }
                else
                {
                        printf("Image Height = %d\n",I_HEIGHT);
                }

/*** MATCHING THE IMAGE RESOLUTION ***/

        if(((I_WIDTH==640)&&(I_HEIGHT==480)) || ((I_WIDTH==1280)&&(I_HEIGHT==720)) || 
           ((I_WIDTH==320)&&(I_HEIGHT==240)) || ((I_WIDTH==1920)&&(I_HEIGHT==1080))|| 
           ((I_WIDTH==2592)&&(I_HEIGHT==1944)))
        {
                printf("Image RES Mathches\n");
        }
        else
        {
                printf(" IMAGE RES NOT MATCHES\n");
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

        printf("File Name = %s\n",F_name);
 	
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
        printf("FTP_LINK= %s\n",FTP_LINK);


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
        printf("FTP_UNAME= %s\n",FTP_UNAME);

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
        printf("FTP_PWD= %s\n",FTP_PWD);

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
        printf("APN NAME= %s\n",APN);
/*** Getting IP ****/

	ip = strtok(buf[10], "=");
	while(ip!=NULL)
	{
		IP = ip;
		ip = strtok(NULL, "=");
	}
	printf("HOST IP=%s\n",IP);


/*** Getting Video Size****/

	v_sz = strtok(buf[11], "=");
                while(v_sz!=NULL)
                {
                        V_SIZE = atoi(v_sz);
                        v_sz = strtok(NULL, "=");
                }

	printf("Video Size =%dMB\n",V_SIZE);
/*** VIDEO CAPTURE INTERVAL *****/

	v_intr = strtok(buf[12], "=");
                while(v_intr!=NULL)
                {
                        V_IVAL = atoi(v_intr);
                        v_intr = strtok(NULL, "=");
                }

        printf("Video Capture Interval =%dmin\n",V_IVAL);

/***END OF VIDEO CAPTURE INTERVAL***/ 

/*** IMAGE CAPTURE INTERVAL *****/

        i_intr = strtok(buf[13], "=");
                while(i_intr!=NULL)
                {
                        I_IVAL = atoi(i_intr);
                        i_intr = strtok(NULL, "=");
                }

        printf("Image Capture Interval =%dmin\n",I_IVAL);

/***END OF IMAGE CAPTURE INTERVAL***/

/**** Getting Image File Name ***/

        i_name = strtok(buf[14], "=");
        while(i_name!=NULL)
        {
                I_name = i_name;
                i_name = strtok(NULL, "=");
        }

        printf("Image File Name = %s\n",I_name);

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
	if (ftp_open(FTP_LINK,FTP_UNAME ,FTP_PWD)) 
	error("ftp_open");
	
/** List the Directories and file in FTP*/

	ftp_ls(1, "", process_entry);
/** SENDING THE FILES FROM LOCAL TO REMOTE **/
	char *local= "sample.txt";
	char *remote= "log.txt";
	ftp_putfile(local, remote, 1, 1);

/** RETRIVING FILES FROM THE FTP SERVER **/
	ftp_getfile("ftptest.txt", "blk.txt", 1);

    while(lineIndex > 0)
        {
        lineIndex--;
        if (buf[lineIndex])
            {
            free(buf[lineIndex]);
            }
        }
    return 0;
    }
