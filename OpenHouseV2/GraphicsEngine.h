#ifndef GRAPHICSENGINE_H_INCLUDED
#define GRAPHICSENGINE_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LoadShaders.h"
#include "ProgramDefines.h"
#include "SphericalCamera.h"
#include "YPRCamera.h"
#include "Axes.h"
#include "teapot.h"
#include "Material.h"
#include "Light.h"
#include "MaterialPresets.h"
#include "Models.h"
#include "ObjModel.h"
#include "Plane.h"
#include "TextRendererTTF.h"
#include "Fighter.h"
#include "Box.h"
#include "Cube.h"
#include "skinned_mesh.h"
#include "skinning_technique.h"

#include "ModelLoaderAssimp.h"

/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Don Spickler Modified by Brian Bowers
\version   1.5
\date      10/29/2023

*/

/**
\class GraphicsEngine

\brief The GraphicsEngine class is an extension of sf::RenderWindow which
handles all of the graphics rendering in the program.

*/

class GraphicsEngine : public sf::RenderWindow
{
private:
    GLenum mode;      ///< Mode, either point, line or fill.
    int sscount;      ///< Screenshot count to be appended to the screenshot filename.
    bool drawAxes;    ///< Boolean for drawing the axes.
    bool drawLights;  ///< Boolean for drawing the lights.

    GLuint ModelLoc;       ///< Location ID of the Model matrix in the shader.
    GLuint NormalLoc;      ///< Location ID of the Normal matrix in the shader.
    GLuint PVMLoc;         ///< Location ID of the PVM matrix in the shader.
    GLuint program;        ///< ID of the shader program.
    GLuint CMprogram;      ///< ID of the cube map shader program.
    GLuint useTextureLoc;
    GLuint texTransLoc;    ///< Location ID of the texture transformation matrix in the shader.
    GLuint program2DPass;  ///< ID for the 2D hud stuff

    //Weird Shader Cube Map trial
    GLuint projLoc;
    GLuint WeirdProgram;
    GLuint timeGG;
    GLuint statLoc;
    GLuint flopLoc;
    GLuint gFlopLoc;

    void setWeirdProjection();


    GLuint gBonesLoc;   ///< ID for skeletal data

    std::string fragShader2DPass;   ///< Fragment Shader for 2D HUD
    std::string vertShader2DPass;   ///< Vertex Shader for 2D HUD

    TextRendererTTF text;   ///< Text object for the HUD

    GLuint tex1_uniform_loc;
    GLuint texID;   ///< Brick Texture
    GLuint texID2;  ///< Arena Ground Texture
    GLuint texID3;  ///< Background Texture
    GLuint texID4;  ///< Road Texture
    GLuint texID5;
    GLuint texID6;
    GLuint texID7;
    GLuint texID8;
    GLuint texID9;
    GLuint texID10;


    SphericalCamera sphcamera;   ///< Spherical Camera
    YPRCamera yprcamera;         ///< Yaw-Pitch-Roll Camera
    int CameraNumber;            ///< Camera number 1 = spherical, 2 = yaw-pitch-roll.
    SphericalCamera LtPos[10];   ///< Spherical "Camera" to control position of the light.

    Material mat;         ///< Material of the current object.
    Light lt[10];         ///< Light object.

    Axes coords;          ///< Axes Object
    Models lightobj;      ///< Used for sphere at light source.

    //Arena One
    ObjModel objmodel;    ///< Object wrapper for Wavefront obj file data.
    ObjModel objmodel2;   ///< Object wrapper for Wavefront obj file data 2
    ObjModel voltsWagon;  ///< Object wrapper for Wavefront obj file data (redcar)
    ObjModel blackCar;    ///< Object wrapper for Wavefront obj file data (blackCar)
    ObjModel fence;       ///< Object wrapper for Wavefront obj file data (chainFence)
    Models screen;        ///< Tessellated Wall Object for TV Screen
    Plane floor;         ///< Plane Object for Ground

    //Assimp model loading
    BasicMesh* pMesh = NULL;

    //Skinned Mesh for bones
    SkinnedMesh* sMesh = NULL;

    //Skinned Mesh for Hand
    //SkinnedMesh* gMesh = NULL;
    SkinnedMesh* gMesh[2] = {NULL, NULL};

    //Skinned Mesh for Monday Memo
    SkinnedMesh* mMesh[2] = {NULL, NULL};

    SkinningTechnique* pSkinningTech = NULL;
    long long StartTimeMillis[2] = {0, 0};
    float AnimationTimeSec[2];
    //BasicMesh* sMesh = NULL;

    //Arena Zero
    ObjModel bathHouse;
    ObjModel tree1;
    ObjModel tree2;

    //Menu
    ObjModel column;
    int cameraPos;
    float cameraX;
    float cameraZ;
    Cube cube;

    ObjModel fighterOne[24];    ///< Array of Object wrappers for Wavefront obj file data sets (FighterOne and FighterTwo)

    //ObjModel fighterOne[12];    ///< Array of Object wrappers for Wavefront obj file data sets (FighterOne)

    GLuint CubeMapTexId;  ///< Cube Map Texture ID.
    Models CMSphere;      ///< Sphere Object for Cube Map


    //GLboolean PauseVideo; ///< Boolean for the video pausing
    bool PauseVideo;      ///< Boolean for the gamePausing
    bool GameStart;       /// < Boolean value that if false, in main menu, if true in gameplay
    int arenaNum;         ///< Integer ID value that is equivalent with a specific arena
    int playerOneValue;   ///< Integer ID value that is equivalent with a specific fighter for playerOne
    int playerTwoValue;   ///< Integer ID value that is equivalent with a specific fighter for playerTwo
    GLboolean restartVid; ///< Boolean for video restart

    void loadHandFighter(int playerNum, glm::mat4 view, glm::vec3 eye, bool menuMode); ///< Function that loads the hand fighter character to the game
    void loadMondayFighter(int playerNum, glm::mat4 view, glm::vec3 eye, bool menuMode);   ///<Function that loads the Monday Memo fighter Character to the game

    void loadSelectedFighter(int fighterNum, int playerNum, glm::mat4 view, glm::vec3 eye);
    void displayFighter(int fighterNum, int playerNum, glm::mat4 view, glm::vec3 eye);

    int curAnimIndex[2] = {0, 0};

    glm::mat4 projection;  ///< Projection Matrix
    glm::mat4 model;       ///< Model Matrix
    glm::mat4 textrans;    ///< Texture transformation matrix.

    int addNum[2] = {0, 12};
    bool gameOver = false;
    int rivalNum[2] = {1, 0};

    glm::mat4 ModelMatrix[2];
    glm::mat4 leftArmMat[2];
    glm::mat4 headMat[2];
    glm::mat4 leftLegMat[2];
    glm::mat4 rightLegMat[2];

    sf::Sound hit; //hit.play
    sf::SoundBuffer hitBuffer;
    sf::Sound punc;
    sf::SoundBuffer punchBuffer;
    sf::Sound strike;
    sf::SoundBuffer strikeBuffer;

    //Health Bar HUD stuff
    Box hpBar1;    ///< Simple Box object used for the Player One's Health Bar
    Box hpBar2;    ///< Simple Box object used for the Player Two's Health Bar
    Box behindBar1;
    Box behindBar2;

    int curFIndex[2] = {3, 1};

    sf::Image texture;     ///< Cube map texture.
    void printOpenGLErrors();
    void print_GLM_Matrix(glm::mat4 m);
    void print_GLM_Matrix(glm::mat3 m);

    void runWeirdProgram(glm::mat4 view);

public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();

    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);
    GLfloat* getScreenBounds();

    GLboolean isSphericalCameraOn();
    void setSphericalCameraOn();
    GLboolean isYPRCameraOn();
    void setYPRCameraOn();
    void toggleAxes();
    void toggleLights();

    void loadLight(Light Lt);
    void loadMaterial(Material Mat);
    void loadMatrices(glm::mat4 projection, glm::mat4 view, glm::mat4 model,
                      GLuint PVMLocation, GLuint ModelLocation, GLuint NormalLocation);

    void turnLightOn();
    void turnLightOff();
    void turnLightOn(std::string name, int i);
    void turnLightOff(std::string name, int i);
    void turnLightsOn(std::string name, int num);
    void turnLightsOff(std::string name, int num);

    void LoadLights(Light Lt[], std::string name, int num);
    void LoadLight(Light Lt, std::string name, int i);

    //Remove during cleanup (check UI)
    void togglePause();
    void restartVideo();

    void menuObjLoad();

    void menuRightSlide();
    void menuLeftSlide();
    void menuSelect();
    void menuBackout();
    void makeValue(float changingVar, float toValue, float rate);
    void returnToMenu();

    int getArenaNum();

    void activateHUD();
    void runMainMenu(glm::mat4 view, glm::vec3 eye);
    void loadArenaZero(glm::mat4 view, glm::vec3 eye);
    void loadArenaOne(glm::mat4 view, glm::vec3 eye);
    void loadFighter(int fighterNum, glm::mat4 view, glm::vec3 eye, bool menuMode);

    void setCurFIndex(int playerNum, int fighterIndex);

    bool getPause();


    Fighter fighterClass[2];

    //void hudDispay();

    void restartGame();

    SphericalCamera* getLtPos();
    SphericalCamera* getSphericalCamera();
    YPRCamera* getYPRCamera();
};

#endif // GRAPHICSENGINE_H_INCLUDED
