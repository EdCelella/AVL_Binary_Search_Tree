INCLUDEDIR = ../headers/
CXXFLAGS   = -std=c++11 -I $(INCLUDEDIR) -Wall -Wfatal-errors
EXECUTABLES = avlbinarytree
SRC = main.cpp
HEADERS = avl.h

vpath %.h $(INCLUDEDIR)

all: $(EXECUTABLES)

$(EXECUTABLES): $(SRC) $(HEADERS)
	g++ $(CXXFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLES)