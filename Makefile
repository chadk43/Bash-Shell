TARGET = mysh
FLAGS = -g

$(TARGET): *.o *.h
	gcc $(FLAGS) *.o *.h -o $(TARGET)

*.o: *.c
	gcc  $(FLAGS) -c *.c

clean:
	rm -f *.o $(TARGET)

all: clean $(TARGET)
