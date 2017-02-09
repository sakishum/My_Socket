SRC += $(wildcard *.cpp)\
	$(wildcard base/*.cpp) 
EXC := test
OBJ := $(SRC:.cpp=.o)
CXX := g++-4.7
FLAG := -std=c++11 -Wall -g

all: $(EXC)

$(EXC): $(OBJ)
	$(CXX) $(FLAG) $? -o $@

%.o:%.cpp 
	$(CXX) $(FLAG) -c $< -o $@

.PHONY: clean
clean: $(EXC) $(OBJ)
	$(RM) $?
