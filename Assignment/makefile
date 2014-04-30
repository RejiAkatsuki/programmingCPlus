CC = g++
FLAGS = -Iinclude -L. -ltcod -ltcodxx -Wl,-rpath=. -g
OBJS = src/*.cpp
 
all: $(OBJS)
	$(CC) $(OBJS) -o ass1 $(FLAGS) 
clean:
	rm -f locknchase
run: all
	./locknchase
