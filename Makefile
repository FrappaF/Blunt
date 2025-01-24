exec = blunt.out
sources = $(shell find src -name '*.c')
objects = $(patsubst src/%.c, obj/%.o, $(sources))
flags = -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	gcc -c $(flags) $< -o $@

obj:
	mkdir -p obj

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
	-rm -r obj

install:
	make clean
	make
	mkdir -p /usr/local/bin 
	cp ./$(exec) /usr/local/bin/blunt
