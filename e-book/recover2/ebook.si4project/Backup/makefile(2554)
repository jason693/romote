TARGET:=test

SEC:=$(wildcard ./*.c)

INC:=-I.
LIB:=-L.

DES:=$(patsubst %.c,%.o,$(SEC))

CC:=arm-linux-gcc

%.o:%.c
	$(CC) -c $? -o $@ $(LIB) $(INC)
	
$(TARGET):$(DES)
	$(CC) $^ -o $@ $(LIB) $(INC)
	cp $(TARGET) ~/tftp_share
clean:
	rm $(TARGET) $(DES)
	
