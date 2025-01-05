exec = blunt.out
sources = $(wildcard src/*.c)
objects = $(patsubst src/%.c, obj/%.o, $(sources))
flags = -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

obj/%.o: src/%.c | obj
	gcc -c $(flags) $< -o $@

obj:
	mkdir -p obj

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
	-rm obj/*.o

install:
	make clean
	make
	mkdir -p /usr/local/bin 
	cp ./$(exec) /usr/local/bin/blunt
