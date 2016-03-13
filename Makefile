CC          = g++
CFLAGS      = -Wall -ansi -pedantic -ggdb --std=c++11
OBJS        = player.o board.o table.o timer.o common.o
OBJECTS = $(addprefix obj/,$(OBJS))
PLAYERNAME  = ANDREadnought

all: $(PLAYERNAME) testgame

$(PLAYERNAME): $(OBJECTS) obj/wrapper.o
	$(CC) -o $@ $^

testgame: obj/testgame.o
	$(CC) -o $@ $^

testminimax: $(OBJECTS) obj/testminimax.o
	$(CC) -o $@ $^

obj/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) -x c++ $< -o $@
	
java:
	make -C java/

cleanjava:
	make -C java/ clean

clean:
	rm -f obj/*.o $(PLAYERNAME) testgame testminimax
	
.PHONY: java testminimax
