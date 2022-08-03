KDIR		:= /lib/modules/`uname -r`/build
MODNAME		:= dump_proc_info
APP		:= tmux

obj-m		:= $(MODNAME).o
$(MODNAME)-objs	:= main.o tls.o io.o task_struct.o

all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean

test:
	sudo insmod $(MODNAME).ko pid=`pidof $(APP) | awk '{print $$1}'`
	sudo dmesg | grep "dump_proc"
	sudo rmmod $(MODNAME)
