# Compiler
CC = g++

# Source files
SOURCES = ../OpenHouseV2/main.cpp ../OpenHouseV2/GraphicsEngine.cpp ../OpenHouseV2/Fighter.cpp ../OpenHouseV2/LoadShaders.cpp ../OpenHouseV2/Axes.cpp ../OpenHouseV2/assimp_sandbox.cpp ../OpenHouseV2/Box.cpp ../OpenHouseV2/Cube.cpp ../OpenHouseV2/Light.cpp ../OpenHouseV2/Material.cpp ../OpenHouseV2/math_3d.cpp ../OpenHouseV2/ModelLoaderAssimp.cpp ../OpenHouseV2/Models.cpp ../OpenHouseV2/ObjModel.cpp ../OpenHouseV2/ogldev_texture.cpp ../OpenHouseV2/ogldev_util.cpp ../OpenHouseV2/ogldev_world_transform.cpp ../OpenHouseV2/Plane.cpp ../OpenHouseV2/Shape.cpp ../OpenHouseV2/skinned_mesh.cpp ../OpenHouseV2/skinning_technique.cpp ../OpenHouseV2/SphericalCamera.cpp ../OpenHouseV2/stb.cpp ../OpenHouseV2/technique.cpp  ../OpenHouseV2/UI.cpp ../OpenHouseV2/YPRCamera.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = myprogram

# Rule to build executable from object files
$(EXECUTABLE): $(OBJECTS)
	$(CC) -Wall $(OBJECTS) -o $(EXECUTABLE) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -lGLU -lglut -lGLEW -I/usr/../include/include

# Rule to build object files from source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the directory
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)