CC = gcc
TARGET = project4_8
OBJECTS = main.o util.o symtab.o analyze.o lex.yy.o cm.tab.o cgen.o

all = $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ -lfl

clean:
	rm *.o $(TARGET)
 
