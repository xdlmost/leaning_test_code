SRC=./src/
BIN=./bin/

all:$(BIN)env\
        $(BIN)curses_hw\
        $(BIN)trySecret\
        $(BIN)tryef\
        $(BIN)tryef_ef\
        $(BIN)check_stack\
        $(BIN)filelock\
        $(BIN)signal_ouch\
        $(BIN)thread_helloworld\
        $(BIN)pipe1\
        $(BIN)netinfo\
        $(BIN)gtktest

%.o: %.cc
	gcc -o $@  -c $^ -MMD -g

$(BIN)gtktest:$(SRC)gtktest.cc
	gcc $^ -o $@ `pkg-config --cflags --libs gtk+-2.0`



$(BIN)netinfo:$(SRC)netinfo.o
	gcc -o $@ $^ 

$(BIN)pipe1:$(SRC)pipe1.o
	gcc -o $@ $^ 

$(SRC)thread_helloworld.o:$(SRC)thread_helloworld.cc
	gcc -o $@  -c $^ -MMD -D_REENTRANT

$(BIN)thread_helloworld:$(SRC)thread_helloworld.o
	gcc -o $@  $^ -lpthread

$(BIN)signal_ouch:$(SRC)signal_ouch.o
	gcc -o $@  $^

$(BIN)filelock:$(SRC)filelock.o
	gcc -o $@  $^

$(BIN)env:$(SRC)env.o
	gcc -o $@  $^

$(BIN)curses_hw:$(SRC)curses_helloworld.o
	gcc -o $@ $^ -lcurses 

$(BIN)trySecret:$(SRC)trySecret.o
	gcc -g -o $@ $^

$(BIN)tryef:$(SRC)tryef.o
	gcc -g -o $@ $^

$(BIN)tryef_ef:$(SRC)tryef.o
	gcc -g -o $@ $^ -lefence 

$(BIN)check_stack:$(SRC)echo_size_of_stack.o
	gcc -g -o $@ $^ 

-include $(OBJ:.o=.d)

.PHONY:all clean

clean:
	@find -name "*.o" -exec rm -v {} \;
	@find -name "*.d" -exec rm -v {} \;
	@find -type f -perm -111 -exec rm -v {} \;
