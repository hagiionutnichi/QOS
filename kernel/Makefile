
OSNAME = QOS

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = kernel.ld
CC = gcc
ASMC = nasm 
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -g -O0
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib
ASMFLAGS =

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
FONTSDIR = fonts
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)        
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm) 
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)


NBN := $(shell head -n 1 NBN)

kernel: $(OBJS) link

$(OBJDIR)/Interrupts/Interrupts.o: $(SRCDIR)/Interrupts/Interrupts.cpp
	@ echo !==== Compiling $^ ====!
	@ mkdir -p $(@D)
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo !==== Compiling $^ ====!
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@ echo !==== Compiling $^ ====!
	@ mkdir -p $(@D)
	$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@

link:
	@ echo !==== Linking ====!
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

buildimg:
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).img -f 2880 ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(FONTSDIR)/zap-ext-vga16.psf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img albie320.bmp ::

run:
	qemu-system-x86_64 -machine q35 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none

download:
	until $$(scp -i ~/awskey.pem ec2-user@34.251.121.206:/var/lib/jenkins/jobs/QOS/branches/master/builds/$(NBN)/archive/kernel/bin/QOS.img .) ; do \
		printf '.' ; \
		sleep 5 ; \
	done
	mv -f QOS.img bin/QOS.img
	echo $(shell expr $(NBN) + 1) > NBN

dr: download run

vbox:
	mv $(BUILDDIR)/$(OSNAME).vdi $(BUILDDIR)/$(OSNAME).vdi.old
	VBoxManage convertfromraw --format VDI $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/$(OSNAME).vdi