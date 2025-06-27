CIRCLEHOME = ../..

OBJS = imagefileblockdevice.o

INCLUDES = -I$(CIRCLEHOME)/include -I$(CIRCLEHOME)/addon

libimagefileblockdevice.a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk

-include $(DEPS)
