EXEC = test
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.cpp=.o)
FLAG = -std=c++11 -Wall 

all: $(EXEC)

$(EXEC): $(OBJECTS) $(HEADERS)
	$(CXX) $(FLAG) -L/usr/local/Cellar/boost/1.54.0/lib -lpthread -lboost_system -lboost_filesystem-mt -lboost_thread-mt $(OBJECTS) -o $(EXEC)
 
%.o: %.cpp $(HEADERS) 
	$(CXX) $(FLAG) -I/usr/local/Cellar/boost/1.54.0/include -c $< -o $@

server: server.o
	$(CXX)  $? -o server

.PHONY: clean
clean:
	$(RM) $(EXEC) $(OBJECTS) *.o
