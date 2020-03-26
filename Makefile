# Make sure to install the linux kernel source code in /lib/modules/<version>/

obj-m += disable_wsna.o enable_arm_pmu.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
