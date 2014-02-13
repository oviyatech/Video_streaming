/* example toy application for ftplib 

   args are: hostname, login, passwd, cddir, lsdir (both possibly as empty
   strings ""), long?

   ./example myftpserver mylogin mypassd "" "" 0
   ./example myftpserver mylogin mypasswd "mydir" "subdir" 1

*/

#include <stdio.h>
#include <stdlib.h>
#include "ftplib.h"
#include <string.h>
#include <dirent.h>
/******** variable declaration - praba *********/
	FILE *file, *fp1, *fp2;
	char *line = NULL;
	size_t len = 0;
	char Read;
	char **buf[20];
	int lineIndex = 0;

	char *f_name;
	static char *F_name;

	char res[1024], folder[1024];
	char *local;
	static int n_cnt = 1,f_cnt;

	int ret; 
	DIR *d1;
	struct dirent *dir;

/* Variables for taking FTP deatils*/

        char *ftp_link;
        static char *FTP_LINK;
        char *ftp_uname;
        static char *FTP_UNAME;
        char *ftp_pwd;
        static char *FTP_PWD;


/********* Variable declaration ends - Praba *******/

#define CLEAR(x) memset (&(x), 0, sizeof (x))


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

/* ***** Added by Praba ******** */
void Read_config_file(void)
{
int i;
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
/** Getting Video File nme ***/
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
		strcat(F_name, ".avi");
		printf("FILE NAME = %s\n", F_name);
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

/***END OF FTP DETAILS******/


}

/*** Checking Directory for a file ****/
static int k = 0;
int isDirectoryEmpty(char *dirname) {
       // printf("Ente Dirctory %s\n", dirname);

        struct dirent *d;
        DIR *dir = opendir(dirname);

        if (dir == NULL) //Not a directory or doesn't exist
                return 1;

        while ((d = readdir(dir)) != NULL) 
        {
                if(++k > 2)
                break;
        }


printf("RETURN K====%d\n",k);
return k;

}


/***** Moving A file to different location ********/

void mv_files(char *source, char *target)
{
	int ch;
	char src[1024], tgt[1024];
/* get the source file name from the user */
	snprintf(src, sizeof(src), "/oviya/%s",source);
        printf("***Enter move SRC = %s \n", src);

/* Appending the folder Location for the target file namee*/
        snprintf(tgt, sizeof(tgt), "/tmp/%s",target);
        printf("TGT=%s\n",tgt);

/* open the source file in read mode */
        printf("**********************SRC = %s \n", src);
        fp1 = fopen(src, "r");
/* open the destination file in write mode */
        fp2 = fopen(tgt, "w");

/* error handling */
        if (!fp1) {
                printf("Unable to open source file %s to read!!\n", src);
                fclose(fp1);
                return ;
        }

        if (!fp2) {
                printf("Unable to open target file to write\n");
		fclose(fp2);
                return ;
        }

/* copying contents of source file to target file */
        while ((ch = fgetc(fp1)) != EOF) {
                fputc(ch, fp2);
        }

/* closing the opened files */
        fclose(fp1);
        fclose(fp2);

/* removing the source file */
        remove(src);
//       return 0;
}

/**** End of moving a file **********/


/* ***** Praba Code Ends ****** */

void clear_memory(void)
{
	CLEAR(ftp_link);
        CLEAR(FTP_LINK);
        CLEAR(ftp_uname);
        CLEAR(FTP_UNAME);
        CLEAR(ftp_pwd);
        CLEAR(FTP_PWD);
	CLEAR(res);
	CLEAR(folder);
}


int main()
{
/*****************************************************************************
		**************************
		* START OF DAEMON PROCESS*
		**************************
****************************************************************************/

/*** DAEMONIZE THE FTP PROCESS **/
	pid_t process_id = 0;
	pid_t sid = 0;
// Create child process
	process_id = fork();
// Indication of fork() failure
	if (process_id < 0)
	{
		printf("fork failed!\n");
// Return failure in exit status
		exit(1);
	}
// PARENT PROCESS. Need to kill it.
	if (process_id > 0)
	{
		printf("process_id of child process %d \n", process_id);
// return success in exit status
		exit(0);
	}
//unmask the file mode
	umask(0);
//set new session
	sid = setsid();
	if(sid < 0)
	{
// Return failure
		exit(1);
	}

/*****************************************************************************
		*************************
		* END OF DAEMON PROCESS	*
		*************************
****************************************************************************/

/* this enables printing of sent FTP commands */
	ftp_debug=1;
/* and this printing of status responses */
	ftp_verbose=1;


  /* open the FTP connection */

	Read_config_file();
	//if (ftp_open("storagemadeeasy.com", "dineshauece", "Shanmugam@143")) error("ftp_open");
	if (ftp_open(FTP_LINK, FTP_UNAME, FTP_PWD)) error("ftp_open");

	while(1)
	{
	isDirectoryEmpty("/oviya");
		if(k > 2)
		{
			if(n_cnt == 1)
			{
				snprintf(folder, sizeof(folder), "/oviya/%s",F_name);
				printf("Current FIle Name in IF = %s\n", F_name);
				ftp_putfile(folder, F_name, 1, 1);
				mv_files(F_name, F_name);
				n_cnt--;
			}
			else
			{
           	       		snprintf(res, sizeof(res), "%d_%s",f_cnt,F_name);
				printf("Current FIle Name in else = %s\n",res); 
				snprintf(folder, sizeof(folder), "/oviya/%s",res);
				ftp_putfile(folder, res, 1, 1);
				mv_files(res, res);
       				f_cnt++;
			}
		}else
		{
			k =0;
		}
			
	}	

/** CLEARING BUFFER **/
while(lineIndex > 0)
        {
        lineIndex--;
        if (buf[lineIndex])
            {
            free(buf[lineIndex]);
            }
        }

ftp_close();
clear_memory();
return 0;

}