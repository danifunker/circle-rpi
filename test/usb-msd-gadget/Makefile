#
# Makefile
#


CPPFLAGS += -DUSB_GADGET_VENDOR_ID=0x04da
CPPFLAGS += -DUSB_GADGET_DEVICE_ID_CD=0x0d01

CIRCLEHOME = ../..

OBJS	= main.o kernel.o

LIBS	= $(CIRCLEHOME)/lib/usb/libusb.a \
	  $(CIRCLEHOME)/lib/usb/gadget/libusbgadget.a \
	  $(CIRCLEHOME)/addon/filelogdaemon/libfilelogdaemon.a \
	  $(CIRCLEHOME)/addon/fatfs/libfatfs.a \
	  $(CIRCLEHOME)/lib/input/libinput.a \
	  $(CIRCLEHOME)/lib/fs/libfs.a \
	  $(CIRCLEHOME)/lib/sched/libsched.a \
	  $(CIRCLEHOME)/addon/linux/liblinuxemu.a \
	  $(CIRCLEHOME)/lib/libcircle.a \
	  $(CIRCLEHOME)/addon/Properties/libproperties.a \
	  $(CIRCLEHOME)/addon/SDCard/libsdcard.a

include $(CIRCLEHOME)/Rules.mk

-include $(DEPS)
