TARGET := cli
CC := arm-linux-gcc


CSRCS := $(wildcard *.c)
CSRCS += $(wildcard ./lcd/*.c)
CSRCS += $(wildcard ./yuyv/*.c)

INCS := -I/home/china/arm-libs/include

INCS+= -I./lcd
INCS+= -I./yuyv

LIBS := -L/home/china/arm-libs/lib -ljpeg

LIBS += -L./lcd
LIBS += -L./yuyv





OBJS := $(patsubst %.c, %.o, $(CSRCS))

$(TARGET): $(OBJS) 
	$(CC) $+ $(INCS) $(LIBS) -o $@ -pthread
	cp $(TARGET) ~/tftp_share

%.o:%.c
	$(CC) -c $< $(INCS) -o $@ -pthread


clean:
	rm -rf $(OBJS) $(TARGET)

