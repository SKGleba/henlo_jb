OBJS     = main.o graphics.o font.o Archives.o libc2paf.o promote.o sha1.o

LIBS = -lgcc -lSceLibKernel_stub -lSceCtrl_stub -lSceKernelThreadMgr_stub -lSceVshBridge_stub -lSceAppMgr_stub -lScePower_stub -lSceDisplay_stub -lSceHttp_stub -lSceNet_stub -lSceNetCtl_stub -lSceSysmodule_stub -lz -lSceIofilemgr_stub -lScePaf_stub_weak -lScePromoterUtil_stub_weak -lSceSysmem_stub

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -Wno-multichar -O3 -nostdlib
ASFLAGS = $(CFLAGS)

all: bootstrap.self

bootstrap.self: main.velf
	vita-make-fself -c $< $@

main.velf: main.elf
	vita-elf-create $< $@

main.elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@