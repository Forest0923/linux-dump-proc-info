KDIR		:= /lib/modules/`uname -r`/build
MODNAME		:= dump_task_struct

obj-m		:= $(MODNAME).o
$(MODNAME)-objs	:= main.o dump_tls.o

all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean
