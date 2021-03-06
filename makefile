TARGET = danzeff
OBJS = danzeff.o main.o

#To build for custom firmware:
BUILD_PRX = 1
PSP_FW_VERSION=371

CFLAGS = -O2 -g -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =

MYLIBS=
STDLIBS= -losl -lpng -lz \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan -lm
LIBS=$(STDLIBS) $(MYLIBS)

LDFLAGS =
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Danzeff for OSLib
#PSP_EBOOT_ICON = ICON0.PNG
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
