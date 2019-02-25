
CC?=gcc

CFLAGS=-g -MMD -O0

LIBOBJS=src/atom.o

all:bin bin/libcinterface.so

bin/libcinterface.so: $(LIBOBJS)
	$(CC) -g -fPIC -shared -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -fPIC  -o $@ -c $^ 

bin: checkbin

checkbin:
	-@mkdir -pv ./bin

-include $(LIBOBJS:.o=.d)

.PHONY: all clean checkbin

clean:
	@find . -name "*.o" -exec rm -v {} \;
	@find . -name "*.d" -exec rm -v {} \;
	@find . -type f -perm -111 -exec rm -v {} \;
	@find ./bin -name "lib*.so*" -exec rm -v {} \;
