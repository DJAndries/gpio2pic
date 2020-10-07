SRC = $(wildcard src/*.c)
SRC_OBJ = $(SRC:src/%.c=obj/%.o)

all: obj bin bin/gpio2pic

firmware: firmware/debug.hex firmware/debug_bootstrap.hex

bin/gpio2pic: $(SRC_OBJ)
	gcc $(ADD_FLAGS) -lgpiod -o $@ $(SRC_OBJ) 

obj/%.o: src/%.c
	gcc $(ADD_FLAGS) -Wall -O -c $< -o $@

obj:
	mkdir obj

bin:
	mkdir bin

firmware/debug.hex: firmware/debug.asm
	gpasm ./firmware/debug.asm -o ./firmware/debug.hex

firmware/debug_bootstrap.hex: firmware/debug_bootstrap.asm
	gpasm ./firmware/debug_bootstrap.asm -o ./firmware/debug_bootstrap.hex

clean:
	rm -f obj/* bin/*

install: bin/gpio2pic
	install -m 755 ./bin/gpio2pic /usr/local/bin/
	(ls /etc/gpio2pic.conf 2> /dev/null) || install -m 644 ./etc/gpio2pic.conf /etc/
