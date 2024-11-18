CXX = g++
CXXFLAGS = -Wall -std=c++11

SOURCES = main.cpp PieceUtil.cpp Board.cpp
HEADERS = PieceUtil.h Board.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = chess

# Main target
all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)