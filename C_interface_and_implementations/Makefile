
CC?=gcc

CFLAGS=-g -MMD -O0

LIBOBJS=src/atom.o

TESTOBJS=src/test_atom.o

all:bin bin/libcinterface.so bin/test_atom

bin/test_atom:src/test_atom.o
	$(CC) -g -L./bin -lcinterface -o $@  $^

bin/libcinterface.so: $(LIBOBJS)
	$(CC) -g -fPIC -shared -o $@ $^

$(LIBOBJS):%.o:%.c
	$(CC) $(CFLAGS) -fPIC -o $@ -c $< 

$(TESTOBJS):%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $< 


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
