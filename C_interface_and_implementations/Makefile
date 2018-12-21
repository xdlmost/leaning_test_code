
CC?=gcc

CFLAGS=-g

all:main main1

bin/liblib.so: bin src/lib.c
	gcc -g -fPIC -shared -o bin/liblib.so -c src/lib.c

main:bin src/main.c bin/liblib.so
	gcc -g src/main.c -L ./bin -llib -o bin/main

main1:bin src/main1.c bin/liblib.so
	gcc -g src/main1.c -L ./bin -llib -o bin/main1

bin: checkbin

checkbin:
	-@mkdir -pv ./bin

.PHONY: all clean checkbin

clean:
	@find . -name "*.o" -exec rm -v {} \;
	@find . -name "*.d" -exec rm -v {} \;
	@find . -type f -perm -111 -exec rm -v {} \;
	@find ./bin -name "lib*.so*" -exec rm -v {} \;
