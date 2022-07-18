.PHONY = clean

CC = gcc

FLAGS = -Wall -g

container: common.o container.o
	$(CC) $(FLAGS) $^ -o $@

common.o: src/common.c
	$(CC) $(FLAGS) -c $^

container.o: container.c
	$(CC) $(FLAGS) -c $^

clean:
	rm *.o container
