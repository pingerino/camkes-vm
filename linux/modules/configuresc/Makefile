obj-m += configuresc.o
CFLAGS_configuresc.o = -I../../include -I../../../common/shared_include

all:
	make -C $(KHEAD) M=$(PWD) modules

clean:
	make -C $(KHEAD) M=$(PWD) clean
