rm testing


gcc cam_thread.c logger.c ftplib.c ftp.c `pkg-config --cflags --libs glib-2.0 gstreamer-0.10` -o testing
#gcc  button_press.c logger.c ftplib.c ftp.c -o testing
#clear
