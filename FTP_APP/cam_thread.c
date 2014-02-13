#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "ftplib.h"
#include <signal.h>

char n[1024];
sem_t len;

	
#include "video_capture.h"

static gint capture_time = 20;
static gint capture_count = 0;
GTimer *timer = NULL;

char res[1024];

static int i = 0;
int factory_make(char* location);



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
	sem_post(&len);
	
	snprintf(res, sizeof(res), "test%d.avi",i);
        printf("Current file name %s-----------------------\n", res);
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
        if(factory_make("test.avi") != 0)
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
        if (ftp_open("ftp.drivehq.com","balaji.cosanostra" ,"balajioviya"))
        error("ftp_open");

/** List the Directories and file in FTP*/

      //  ftp_ls(1, "", process_entry);
/** SENDING THE FILES FROM LOCAL TO REMOTE **/
        
while(1)
{
sem_wait(&len);
        char *local= res;
printf("Local=%s\n",local);
        char *remote= local;
        printf("remote=%s\n",remote);
        ftp_putfile(local, local, 1, 1);
        i++;

/** RETRIVING FILES FROM THE FTP SERVER **/
//      ftp_getfile("ftptest.txt", "blk.txt", 1);
}

}


int main()
{

	printf("Enter main\n");
    //    int status;
        pthread_t tcam, tftp;

        pthread_create(&tcam,NULL,start_capture,NULL);
        pthread_create(&tftp,NULL,Access_ftp,NULL);

        pthread_join(tcam,NULL);
        pthread_join(tftp,NULL);
        return 0;
}

