#include <stdlib.h>     /* exit, malloc, realloc, free */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "char.h"
int main()
{
/********* VARIABLE DECLARATION *************/

/* Variables for FIle read*/
	FILE *file;
	char *line = NULL;
	size_t len = 0;
	char read;
	char *buf1, *buf2, *buf3, *buf4, *buf5, *buf6, *buf7, *buf8, *buf9, *buf10, *buf11, *buf12;
	int lineIndex = 0; 
	char** lines[20];
/* Variables for Taking Video Width & Height*/
	char *WVGA, *VGA, *HD, *FULL_HD;
	int wvga, vga, hd,full_hd;
	static int WIDTH,HEIGHT;


/* Variables for Taking Image Width & Height */
	char *I_WVGA, *I_VGA, *I_HD, *I_FULL_HD, *I_5MP;
        int i_wvga, i_vga, i_hd, i_full_hd,i_5mp;
        static int I_WIDTH,I_HEIGHT;


/* Variables for Taking FPS Value*/
	unsigned char fps[2],data[4];
	int i, Fps;


/*Variables for Taking Capture mode*/
	char MODE[1],MODE3[5];
	int CAP_MODE;

/*Variables for Getting IP*/
	char *ip;
	int IP;
/*************************************READ BLOCK**********************************************/
	file=fopen("test.txt", "r");
	if (file == NULL)
	{
	return 1;
	}
 
	while (lineIndex < 20  && (read = getline(&line, &len, file)) != -1)
	{
	lines[lineIndex] = line;
		if(lineIndex == 0)
		{ 
			memset(buf1, '\0', sizeof(buf1)); 
        		buf1 = line;
//        		printf("buf1= %s Index= %d\n", buf1, lineIndex);
 		}
		else if(lineIndex == 1)
		{
       			buf2 = line;
  //      		printf("buf2= %s Index= %d\n", buf2, lineIndex);
		}
		else if(lineIndex == 2)
		{
			buf3 = line;
    //    		printf("buf3= %s Index= %d\n", buf3, lineIndex);
		}
		else if(lineIndex == 3)
		{
			buf4 = line;
      //  		printf("buf4= %s Index= %d\n", buf4, lineIndex);
		}	
		else if(lineIndex == 4)
		{
			buf5 = line;
        //		printf("buf5= %s Index= %d\n", buf5, lineIndex);
		}	
		else if(lineIndex == 5)
		{
			buf6 = line;
        //		printf("buf6= %s Index= %d\n", buf6, lineIndex);
		}	
		else if(lineIndex == 6)
		{
			buf7 = line;
        //		printf("buf7= %s Index= %d\n", buf7, lineIndex);
		}	
		else if(lineIndex == 7)
		{
			buf8 = line;
        //		printf("buf8= %s Index= %d\n", buf8, lineIndex);
		}	
		else if(lineIndex == 8)
		{
			buf9 = line;
        	//	printf("buf9= %s Index= %d\n", buf9, lineIndex);
		}	
		else if(lineIndex == 9)
		{
			buf10 = line;
        	//	printf("buf10= %s Index= %d\n", buf10, lineIndex);
		}	
		else if(lineIndex == 10)
		{
			buf11 = line;
        	//	printf("buf11= %s Index= %d\n", buf11, lineIndex);
		}	
		else if(lineIndex == 11)
		{
			buf12 = line;
        	//	printf("buf12= %s Index= %d\n", buf12, lineIndex);
		}	
        line = NULL;
        lineIndex++;
	}

/*** Getting FPS ****/
	memset(fps, '0', sizeof(fps)); 
	for(i=0; i<8; i++)
	{
		if((buf1[i] == '3') || (buf1[i]=='2') || (buf1[i]=='1'))
		{
			memset(fps, '\0', 2); 
			memcpy(fps, &buf1[i], 2);
			Fps = atoi(fps);
			printf("VIDEO FPS=%d\n", Fps);
		}
	}
/*** ENF OF FPS ***/

/*--------------------------------------------------------------------------------------------------------------------------------------------------*/

/** Getting Frame Width****/
	WVGA = strchr(buf2, '3');
	VGA = strchr(buf2, '6');
	HD = strchr(buf2, '1');
	if(WVGA != NULL)
        {
                wvga = atoi(WVGA);
		WIDTH = wvga;
        }
        
        if(VGA != NULL)
        {
                vga = atoi(VGA);
		WIDTH = vga;
        }
	
	if(HD != NULL )
        {
		hd = atoi(HD);
		WIDTH = hd;
        }
/*Checking if Width Matches the Database*/

	if((WIDTH !=640) && (WIDTH!=1280) && (WIDTH!=320) && (WIDTH!=1920))
	{
			printf("ENter width Correctly \n");
			return ;
	}
	else
	{
		printf("Video Width=%d\n", WIDTH);
	}
		
/*******END OF FRAME HEIGHT ********/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------*/

/******* GETTING FRAME HEIGHT ********/


/* THIS IS FOR WVGA 320X240  */
	
	WVGA = strchr(buf3, '2');
	if(WVGA != NULL)
        {
                wvga = atoi(WVGA);
		HEIGHT= wvga;
        }

/* THIS IS FOR VGA 640X480  */

	for(i=0;i<13;i++)
        {
                if((buf3[i] == '4') && (buf3[i+1] == '8') && (buf3[i+2] == '0'))
                {
                        memcpy(VGA, &buf3[i], 3);
                        vga = atoi(VGA);
                        HEIGHT = vga;
                }
        }

/* THIS IS FOR HD 1280X720   */

	HD = strchr(buf3, '7');
        if(HD != NULL )
        {
		hd = atoi(HD);
		HEIGHT = hd;
        }

/* THIS IS FOR FULL HD 1920X1080   */

	FULL_HD = strchr(buf3, '1');
        if(FULL_HD != NULL )
        {
		full_hd = atoi(FULL_HD);
		HEIGHT = full_hd;
        }
/* CHECKING THE HEIGHT WITH DB */

	if(((HEIGHT!=480) && (HEIGHT!=720) &&(HEIGHT!=240) && (HEIGHT!=1080)))
	{
		printf("ENter height Correctly \n");
		return ;
	}
	else
	{
		printf("Video HEIGHT=%d\n", HEIGHT);
	}
		
/***** END OF FRAME HEIGHT*******/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**** GETTING CAPTURE MODE ****/
	for(i=0;i<10;i++)
        {
		
		if((buf4[i] == '0'))
		{
                        memcpy(MODE, &buf4[i], 1);
			CAP_MODE= atoi(MODE);
		}
		if((buf4[i]  == '1'))
		{
                        memcpy(MODE, &buf4[i], 1);
			CAP_MODE= atoi(MODE);
		}
		if((buf4[i]  == '4'))
		{
                        memcpy(MODE, &buf4[i], 1);
			CAP_MODE= atoi(MODE);
		}
		if((buf4[i]  == '5'))
		{
                        memcpy(MODE, &buf4[i], 1);
			CAP_MODE= atoi(MODE);
		}
/*If Capture Mode Does not Matches the Valid Number */
		else if((buf4[i]=='2') || (buf4[i]=='3')||(buf4[i]=='6') ||(buf4[i]=='7') ||(buf4[i]=='8') ||(buf4[i]=='9'))
		{
			CAP_MODE = -1;
			printf("You have Entered Invalid Capture mode\n \
				Valid Capture Modes are\n \
				 V_mode = 0 for VGA 640x480\n    \
				 V_mode = 1 for WVGA 320x240\n   \
				 V_mode = 4 for HD 1280x720\n     \
				 V_mode = 5 for FULL HD 1920x1080\n \
				Plz check the Config.txt file\n ");
			return;

		}


}

/*CHECKING THE CAP MODE, WIDTH & HEIGHT WITH DB*/

//	printf("CAP_MODE=%d\n",CAP_MODE);
	if(CAP_MODE == 0&&((WIDTH!=640 && HEIGHT==480)||
	  		   (WIDTH==640 && HEIGHT!=480)|| 
	  		   (WIDTH!=640 && HEIGHT!=480)))
	{
		printf(" ERROR mismatching1\n");
	}
	else if(CAP_MODE == 1&&((WIDTH!=320 && HEIGHT==240)||
				(WIDTH==320 && HEIGHT!=240)||
				(WIDTH!=320 && HEIGHT!=240)))
	{
		printf(" ERROR mismatching2\n");
	}
	else if(CAP_MODE == 4&&((WIDTH!=1280 && HEIGHT==720)||
				(WIDTH==1280 && HEIGHT!=720)||
				(WIDTH!=1280 && HEIGHT!=720)))
	{
		printf(" ERROR mismatching3\n");
	}
	else if(CAP_MODE == 5 &&((WIDTH!=1920 && HEIGHT!=1080)||
				(WIDTH==1920 && HEIGHT!=1080)||
				(WIDTH!=1920 && HEIGHT!=1080)))
	{
		printf(" ERROR mismatching4\n");
	}
	else
	{
		printf("Video CAPMODE =%d\n", CAP_MODE);
	}
/**** END OF CAPTURE MODE***/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**** GETTING HOST IP ADDRESS *****/

	ip = strchr(buf5, '1');
	printf("Server IP =%s\n", ip);
	

/**** END OF IP ADDRESS****/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*** GETTING IMAGE WIDTH ****/

        I_WVGA = strchr(buf6, '3');
        I_VGA = strchr(buf6, '6');
        I_HD = strchr(buf6, '1');
	I_5MP = strchr(buf6, '2');
        if(I_WVGA != NULL)
        {
                i_wvga = atoi(I_WVGA);
                I_WIDTH = i_wvga;
        }

        if(I_VGA != NULL)
        {
                i_vga = atoi(I_VGA);
                I_WIDTH = i_vga;
        }
	
	if(I_HD != NULL )
        {
                i_hd = atoi(I_HD);
                I_WIDTH = i_hd;
        }

	for(i=0;i<13;i++)
        {
                if((buf6[i] == '2') && (buf6[i+1] == '5') && (buf6[i+2] == '9') && (buf6[i+3] == '2'))
                {
                        memcpy(I_5MP, &buf6[i], 4);
                        i_5mp = atoi(I_5MP);
                        I_WIDTH = i_5mp;
                       // printf(" I_5MP=%d\n",i_5mp);
                }
        }

        if((I_WIDTH !=640) && (I_WIDTH!=1280) && (I_WIDTH!=320) && (I_WIDTH!=1920) && (I_WIDTH!=2592))
        {
                        printf("ENter Image width Correctly \n");
                        return ;
        }
	else
	{
			printf("Image Width=%d\n", I_WIDTH);
	}



/*** END OF IMAGE WIDTH ***/

/*** GETTING IMAGE HEIGHT ***/

        I_WVGA = strchr(buf7, '2');
        I_HD = strchr(buf7, '7');
        I_FULL_HD = strchr(buf7, '1');
        
	if(I_WVGA != NULL)
        {
                i_wvga = atoi(I_WVGA);
                I_HEIGHT= i_wvga;
        }

        for(i=0;i<13;i++)
        {
                if((buf7[i] == '4') && (buf7[i+1] == '8') && (buf7[i+2] == '0'))
                {
                        memcpy(I_VGA, &buf7[i], 3);
                        i_vga = atoi(I_VGA);
                        I_HEIGHT = i_vga;
                }
        }


        if(I_HD != NULL )
        {
                i_hd = atoi(I_HD);
                I_HEIGHT = i_hd;
        }
	
	 if(I_FULL_HD != NULL )
        {
                i_full_hd = atoi(I_FULL_HD);
                I_HEIGHT = i_full_hd;
        }

        for(i=0;i<13;i++)
        {
                if((buf7[i] == '1') && (buf7[i+1] == '9') && (buf7[i+2] == '4') && (buf7[i+3] == '4'))
                {
                        memcpy(I_5MP, &buf7[i], 4);
                        i_5mp = atoi(I_5MP);
                        I_HEIGHT = i_5mp;
                        //printf(" I_5MP=%d\n",i_5mp);
                }
        }

       
	if(((I_HEIGHT!=480) && (I_HEIGHT!=720) &&(I_HEIGHT!=240) && (I_HEIGHT!=1080) && (I_HEIGHT!=1944)))
	{
	        printf("ENter Image height Correctly \n");
        	return ;
        }
	else
	{
		printf("Image HEIGHT=%d\n", I_HEIGHT);
	}


/*** END OF IMAGE HEIGHT ***/

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

/*** GETTING IMAGE FILE NAME ***/
	memset(f_name, '\0', sizeof(f_name));

        for(i=0;i<20;i++)
        {
		while(buf8[i] == '=')
                {
			i+=1;
			
			while(buf8[i]!='\n')
                        {
                                //printf("I=%d j=%d\n",i, j);
                                f_name[j] = buf8[i];
                                i++;
                                j++;
                        }
                }
        }
		strcat(f_name, ".jpg");
                printf("File name=%s\n",f_name);
/*** END OF GETTING FILE NAME***/

/*** GETTING FTP DETAILS ****/
	memset(Ftp_link, '\0', sizeof(Ftp_link));
        for(i=0;i<20;i++)
        {
                while(buf9[i] == '=')
                {
                        i+=1;

                        while(buf9[i]!='\n')
                        {
                                Ftp_link[cnt] = buf9[i];
                                i++;
                                cnt++;
                        }
                }
        }
                printf("Ftp link=%s\n",Ftp_link);

/**Getting FTP User Name **/

	memset(Ftp_uname, '\0', sizeof(Ftp_uname));
	for(i=0;i<20;i++)
        {
                while(buf10[i] == '=')
                {
                        i+=1;

                        while(buf10[i]!='\n')
                        {
                                Ftp_uname[cnt1] = buf10[i];
                                i++;
                                cnt1++;
                        }
                }
        }
		err = strcmp(Ftp_uname, "shanmugam@oviya.co");
		if(err == 0)
		{
			printf("User name is ok\n");
                	printf("Ftp U.Name=%s\n",Ftp_uname);
		}
		else
		{
			printf("Invalid User Name\n");
		}

/** Getting FTP Password **/
	memset(Ftp_pwd, '\0', sizeof(Ftp_pwd));
	for(i=0;i<20;i++)
        {
                while(buf11[i] == '=')
                {
                        i+=1;

                        while(buf11[i]!='\n')
                        {
                                Ftp_pwd[cnt2] = buf11[i];
                                i++;
                                cnt2++;
                        }
                }
        }
		ret = strcmp(Ftp_pwd, "oviya123#");
                if(ret == 0)
                {
                        printf("FTP Pwd is ok\n");
                        printf("Ftp PWD=%s\n",Ftp_pwd);
                }
                else
                {
                        printf("Invalid Password\n");
                }



/*** END OF FTP *************/

/*** GETTING APN NAME *********/

	memset(Apn, '\0', sizeof(Apn));
        for(i=0;i<20;i++)
        {
                while(buf12[i] == '=')
                {
                        i+=1;

                        while(buf12[i]!='\n')
                        {
                                Apn[cnt3] = buf12[i];
                                i++;
                                cnt3++;
                        }
                }
        }

                        printf("APN NAME=%s\n",Apn);

/****END OF APN NAME**********/

    while(lineIndex > 0)
        {
        lineIndex--;
        if (lines[lineIndex])
            {
            free(lines[lineIndex]);
            }
        }
    return 0;
    }
