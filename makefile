# Compiler settings
CXX = g++
CXXFLAGS = -Wall -std=c++17

# SFML settings
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# File names
TARGET = game
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SFML_LIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJECTS)

# Phony targets
.PHONY: clean