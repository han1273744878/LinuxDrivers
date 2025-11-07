obj-m += param.o
KDIR:=/home/han/desktop/code/Linux驱动/linux-4.9.88
PWD?=$(shell pwd)
CROSS_COMPILE:=/opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
ARCH:=arm
all:
	make -C $(KDIR) M=$(PWD) modules MODPOST_VMLINUX= ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *.d