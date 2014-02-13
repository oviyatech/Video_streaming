#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/*** ADDED FOR SERIAL PORT READ AND WRITE***/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#define BAUDRATE B2400   
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

//#define CONFIG_SERIAL

#define CONFIG_FILE
struct termios oldtp, newtp;

FILE *fp ;
int fd=0;
static int SESSION_TRACKER; //Keeps track of session

void close_port()
{
	
	close(fd);
	printf("Closing device\n");
}

void open_port(void)
{
         fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY |O_NDELAY );
         if (fd <0)
         {
		printf(" Cannot ope device %s.\n \
			No such file or Directory.\n", MODEMDEVICE);
   		close_port();
		return;
				
         }
                                                                                
         fcntl(fd,F_SETFL,0);
         tcgetattr(fd,&oldtp); /* save current serial port settings */
         bzero(&newtp, sizeof(newtp));
                                                                                
         newtp.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
                                                                                
         newtp.c_iflag = IGNPAR | ICRNL;
                                                                                
         newtp.c_oflag = 0;
                                                                                
         newtp.c_lflag = ICANON;
                                                                                
         newtp.c_cc[VINTR]    = 0;     /* Ctrl-c */
         newtp.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
         newtp.c_cc[VERASE]   = 0;     /* del */
         newtp.c_cc[VKILL]    = 0;     /* @ */
         newtp.c_cc[VEOF]     = 4;     /* Ctrl-d */
         newtp.c_cc[VEOF]     = 0;     /* Ctrl-d */
         newtp.c_cc[VTIME]    = 0;     /* inter-character timer unused */
         newtp.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */
         newtp.c_cc[VSWTC]    = 0;     /* '\0' */
         newtp.c_cc[VSTART]   = 0;     /* Ctrl-q */
         newtp.c_cc[VSTOP]    = 0;     /* Ctrl-s */
         newtp.c_cc[VSUSP]    = 0;     /* Ctrl-z */
         newtp.c_cc[VEOL]     = 0;     /* '\0' */
         newtp.c_cc[VREPRINT] = 0;     /* Ctrl-r */
         newtp.c_cc[VDISCARD] = 0;     /* Ctrl-u */
         newtp.c_cc[VWERASE]  = 0;     /* Ctrl-w */
         newtp.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
         newtp.c_cc[VEOL2]    = 0;     /* '\0' */
                                                                                
                                                                                
        tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtp);
}





char* print_time()
{
    time_t t;
    char *buf;
    
    time(&t);
    buf = (char*)malloc(strlen(ctime(&t))+ 1);
    
    snprintf(buf,strlen(ctime(&t)),"%s ", ctime(&t));
   
    return buf;
}

void log_write(void)
{
#ifdef CONFIG_FILE
		if(SESSION_TRACKER > 0)
		{
			fp = fopen ("log.txt","a+");
		}
		else
		{
			fp = fopen ("log.txt","w");
		}
#endif

#ifdef CONFIG_SERIAL
			open_port();
#endif
}
void log_print(char* filename, int line, char *fmt,...)
{
    va_list         list;
    char            *p, *r;
    int             e;

	log_write();
//    if(SESSION_TRACKER > 0)
//    fp = fopen ("log.txt","a+");
//    else
//      fp = fopen ("log.txt","w");
#ifdef CONFIG_FILE    
    fprintf(fp,"%s ",print_time());
#endif
    va_start( list, fmt );

    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )//If simple string
        {
#ifdef CONFIG_FILE
            fputc( *p,fp );
#endif
#ifdef CONFIG_SERIAL
		write(fd, &p, sizeof(p));
#endif
        }
        else
        {
            switch ( *++p )
            {
                /* string */
            case 's':
            {
                r = va_arg( list, char * );
#ifdef CONFIG_FILE
	fprintf(fp,"%s", r);
#endif

#ifdef CONFIG_SERIAL
	write(fd, &r, sizeof(r));
#endif
                continue;
            }

            /* integer */
            case 'd':
            {
                e = va_arg( list, int );

#ifdef CONFIG_FILE
                fprintf(fp,"%d", e);
#endif
#ifdef CONFIG_SERIAL
	write(fd, &e, sizeof(e));
#endif
                continue;
            }

            default:
#ifdef CONFIG_FILE
                fputc( *p, fp );
#endif
#ifdef CONFIG_SERIAL
		write(fd, &p, sizeof(p));

#endif
            }
        }
    }
    va_end( list );
#ifdef CONFIG_FILE
    fprintf(fp," [%s][line: %d] ",filename,line);
    fputc( '\n', fp );
#endif
    SESSION_TRACKER++;
#ifdef CONFIG_FILE
    fclose(fp);
#endif
#ifdef CONFIG_SERIAL
	close_port(fd);
#endif
}

