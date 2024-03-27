Will make readme look better soon
Needed Libraries!!!!!
---SFML---
sudo apt install libsfml-dev 
---OpenGL---
sudo apt install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev
---glew(openGL extention)---
sudo apt-get install libglew-dev
---GLM---
sudo apt install libglm-dev
---Freetype---//I got rid of freetype, had bad compilation errors
sudo apt-get install libfreetype-dev
//Also need to copy the contents of the include folder freetype2 and move them into general include
---Assimp---
sudo apt-get install -y assimp-utils
&&
sudo apt-get install libassimp-dev
//First one didn't work, second one did though
Also need to do the following outside of OpenHosue folder
git clone https://github.com/assimp/assimp.git
cmake CMakeLists.txt
cd assimp
cmake --build ./path/to/includes
//I also made my Assimp lib into a static library instead of shared while setting it up, but you can do shared instead which cmake does by default



