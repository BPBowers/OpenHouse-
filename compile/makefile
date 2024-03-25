# Compiler
CC = g++

# Compiler flags
#CFLAGS = -Wall -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -lGLU -lglut -lGLEW

# Source files
SOURCES = ../OpenHouseV2/main.cpp ../OpenHouseV2/GraphicsEngine.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = myprogram

# Rule to build executable from object files
$(EXECUTABLE): $(OBJECTS)
	$(CC) -Wall $(OBJECTS) -o $(EXECUTABLE) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -lGLU -lglut -lGLEW

# Rule to build object files from source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the directory
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)