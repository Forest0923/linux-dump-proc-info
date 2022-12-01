KDIR		:= /lib/modules/`uname -r`/build
MODNAME		:= dump_proc_info
APP		:= tmux
PID		:= 1

obj-m		:= $(MODNAME).o
$(MODNAME)-objs	:= main.o tls.o io.o task_struct.o syscall.o current.o page_table_walk.o

all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean

test:
	sudo insmod $(MODNAME).ko
	sudo dmesg | grep "dump_proc"
	sudo rmmod $(MODNAME)

test_proc:
	sudo insmod $(MODNAME).ko pid=`pidof $(APP) | awk '{print $$1}'`
	sudo dmesg | grep "dump_proc"
	sudo rmmod $(MODNAME)

test_pid:
	sudo insmod $(MODNAME).ko pid=$(PID)
	sudo dmesg | grep "dump_proc"
	sudo rmmod $(MODNAME)
