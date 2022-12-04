KDIR		:= /lib/modules/`uname -r`/build
MODNAME		:= dump_proc_info
APP		:= tmux
PID		:= 1

obj-m		:= $(MODNAME).o
$(MODNAME)-objs	:= main.o

all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean

test:
	sudo insmod $(MODNAME).ko pid=`pidof -s bash` uptime=$(UPTIME)
	sudo rmmod $(MODNAME)
	sudo dmesg -t -L=always | grep $(UPTIME)

dump_objects:
	sudo insmod $(MODNAME).ko pid=$(PID) uptime=$(UPTIME) obj_mask=$(OBJ_MASK)
	sudo rmmod $(MODNAME)
	sudo dmesg -t -L=always | grep $(UPTIME)

va2pa:
	sudo insmod $(MODNAME).ko pid=$(PID) uptime=$(UPTIME) addr=$(ADDR)
	sudo rmmod $(MODNAME)
	sudo dmesg -t -L=always | grep $(UPTIME)
