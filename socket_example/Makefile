EXEC = test
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.c=.o)
FLAG = -Wall 

all: $(EXEC)

$(EXEC): $(OBJECTS) $(HEADERS)
	$(CC) $(FLAG) -L/usr/local/Cellar/boost/1.54.0/lib -lpthread -lboost_system -lboost_filesystem-mt -lboost_thread-mt $(OBJECTS) -o $(EXEC)
 
%.o: %.c $(HEADERS) 
	$(CC) $(FLAG) -I/usr/local/Cellar/boost/1.54.0/include -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(EXEC) $(OBJECTS) *.o
