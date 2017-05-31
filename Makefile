CC = gcc
TARGET = 20121653
OBJECTS = main.o util.o lex.yy.o cm.tab.o

all = $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ -lfl

clean:
	rm *.o $(TARGET)
 
