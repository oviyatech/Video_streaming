#rm log.txt

rm arm_testing
export PATH=$PATH:/home/shanmugam/YOCTO/rootfs_builder/build/tmp/sysroots/i686-linux/usr/bin/armv7a-vfp-neon-poky-linux-gnueabi:

arm-poky-linux-gnueabi-gcc button_press.c logger.c  -lpthread `pkg-config --cflags --libs glib-2.0 gstreamer-0.10` -L/home/shanmugam/YOCTO/rootfs_builder/build/tmp/sysroots/imx6qsabrelite/usr/lib/  -o arm_testing

#arm-poky-linux-gnueabi-gcc  example.o libftp.a -o arm_testing

#export PATH=$PATH:/opt/toolchains/2007q1/squashfs-root/bin:
#arm-none-linux-gnueabi-gcc  button_press.c logger.c ftplib.c ftp.c -o arm_testing
#clear

