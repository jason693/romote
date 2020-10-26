TARGET := v4l2_test
CC := arm-linux-gcc


CSRCS := $(wildcard *.c)
CSRCS+=$(wildcard ./lcd/*.c)
CSRCS+=$(wildcard ./yuyv/*.c)
CSRCS+=$(wildcard ./queue/*.c)
CSRCS+=$(wildcard ./tcp_ser/*.c)

INCS := -I/home/china/arm-libs/include 
INCS += -I./lcd
INCS += -I./queue
INCS += -I./yuyv
INCS += -I./tcp_ser

LIBS := -L/home/china/arm-libs/lib -ljpeg
LIBS += -L.
LIBS += -L./lcd
LIBS += -L./queue
LIBS += -L./yuyv




OBJS := $(patsubst %.c, %.o, $(CSRCS))

$(TARGET): $(OBJS) 
	$(CC) $+ $(INCS) $(LIBS) -o $@ -lpthread
	cp $(TARGET) ~/tftp_share

%.o:%.c
	$(CC) -c $< $(INCS) -o $@ -lpthread


clean:
	rm -rf $(OBJS) $(TARGET)
