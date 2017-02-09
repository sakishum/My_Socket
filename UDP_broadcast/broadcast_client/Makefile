EXEC = test
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.cpp=.o)
CXXFLAG := -std=c++11 -lpthread -lboost_system -lboost_filesystem-mt -lboost_thread-mt
 
all: $(EXEC)
 
$(EXEC): $(OBJECTS)
	$(CXX) -L/usr/local/Cellar/boost/1.54.0/lib $(CXXFLAG) $(OBJECTS) -o $(EXEC)
 
%.o: %.cpp $(HEADERS)
	$(CXX) -std=c++11 -I/usr/local/Cellar/boost/1.54.0/include -c $< -o $@
 
.PHONY: clean
clean:
	$(RM) $(EXEC) $(OBJECTS) *.o
