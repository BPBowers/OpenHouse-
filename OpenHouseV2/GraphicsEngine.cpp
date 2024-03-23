#include "GraphicsEngine.h"

/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Don Spickler Modified by Brian Bowers
\version   1.5
\date      12/11/2023

*/

/**
\brief Constructor

\param title --- Title to be placed in the titlebar of the graphics window.
\param MajorVer --- The OpenGL major version that is requested.
\param MinorVer --- The OpenGL minor version that is requested.
\param width --- The width (in pixels) of the graphics window.
\param height --- The height (in pixels) of the graphics window.

Creates rendering window, loads the shaders, and sets some initial data settings.

*/

GraphicsEngine::GraphicsEngine(std::string title, GLint MajorVer, GLint MinorVer, int width, int height) :
    sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default,
                     sf::ContextSettings(24, 8, 4, MajorVer, MinorVer, sf::ContextSettings::Core))
{
    //Define the 2D Shader
    fragShader2DPass =
        "#version 330 core\n"
        "in  vec4 color;\n"
        "out vec4 fColor;\n"
        "void main()\n"
        "{\n"
        "    fColor = color;\n"
        "}\n";

    vertShader2DPass =
        "#version 330 core\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec4 icolor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = icolor;\n"
        "    gl_Position = position;\n"
        "}\n";

    //  Load 2D pass-through shader
    program2DPass = LoadShadersFromMemory(vertShader2DPass, fragShader2DPass);
    if (!program2DPass)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    //  Setup fonts.
    text.loadFont("fonts/Game-Of-Squids.ttf");
    text.setFontSize(24);
    text.setColor(1, 1, 1, 1);
    text.setScreenSize(getSize().x, getSize().y);

    drawAxes = false;

    //  Load cubemap shaders and texture.

    CMprogram = LoadShadersFromFile("VertexShaderCubeMap.glsl", "FragmentCubeMap.glsl");
    //Testing for weird cube map
    WeirdProgram = LoadShadersFromFile("AspectRatioVert.glsl", "StrangeFrag.glsl");

    glUseProgram(WeirdProgram);

    projLoc = glGetUniformLocation(WeirdProgram, "Projection");
    timeGG = glGetUniformLocation(WeirdProgram, "time");
    statLoc = glGetUniformLocation(program, "status");
    flopLoc = glGetUniformLocation(WeirdProgram, "flopper");
    gFlopLoc = glGetUniformLocation(WeirdProgram, "gFlopper");

    setWeirdProjection();

    glUseProgram(program);

    if (!CMprogram)
    {
        std::cerr << "Could not load Cube Map Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    CMSphere.createSphereOBJ(500, 20, 20);
    CMSphere.LoadDataToGraphicsCard(0, 1, 2, 3);

    glUseProgram(CMprogram);

    //  Load in Cube Map
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(CMprogram, "cmtex"), 0);

    // Generate a new cube map texture and bind to it
    glGenTextures (1, &CubeMapTexId);
    glBindTexture (GL_TEXTURE_CUBE_MAP, CubeMapTexId);

    // Setup some parameters for texture filters and mipmapping
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
/*
    //if (!texture.loadFromFile("Models/Yokohama3.jpg"))
    if (!texture.loadFromFile("Models/Starfield002.jpg"))
        std::cerr << "Could not load texture: Starfield002.jpg" << std::endl;

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
*/
    bool texloaded;
    //texloaded = texture.loadFromFile("Models/Skybox001.jpg");
    //texloaded = texture.loadFromFile("Models/Skybox002.jpg");
    //texloaded = texture.loadFromFile("Models/Skybox003.jpg");
    //texloaded = texture.loadFromFile("Models/Skybox004.jpg");
    //texloaded = texture.loadFromFile("Models/Skybox005.jpg");
    //texloaded = texture.loadFromFile("Models/Skybox006.png");
    texloaded = texture.loadFromFile("Models/Skybox007.png");
    //texloaded = texture.loadFromFile("Models/Skybox008.png");
    //texloaded = texture.loadFromFile("Models/Skybox009.png");
    //texloaded = texture.loadFromFile("Models/SkyboxLayout.png");
    //texloaded = texture.loadFromFile("Models/Yokohama3.png");

    if (!texloaded)
        std::cerr << "Could not load Skybox texture." << std::endl;

    unsigned int s = texture.getSize().x/4;
    sf::Image img;
    img.create(s, s);

    img.copy(texture, 0, 0, sf::IntRect(0, s, s, 2*s));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

    img.copy(texture, 0, 0, sf::IntRect(2*s, s, 3*s, 2*s));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

    img.copy(texture, 0, 0, sf::IntRect(s, 0, 2*s, s));
    img.flipHorizontally();
    img.flipVertically();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

    img.copy(texture, 0, 0, sf::IntRect(s, 2*s, 2*s, 3*s));
    img.flipHorizontally();
    img.flipVertically();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

    img.copy(texture, 0, 0, sf::IntRect(3*s, s, 4*s, 2*s));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

    img.copy(texture, 0, 0, sf::IntRect(s, s, 2*s, 2*s));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    //*/

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    //Load with Assimp method
    pMesh = new BasicMesh();
    /*
    if (!pMesh->LoadMesh("Models/testSkel/testSkel.obj")){
        printf("Failed to load with assimp!");
    }
    /*
    if (!pMesh->LoadMesh("Models/untitled.obj")){
        printf("Failed to load with assimp!");
    }
    */
/*
    if (!pMesh->LoadMesh("Models/testSkel/testSkel.fbx")){
        printf("Failed to load with assimp!");
    }
*/
    //sMesh = new BasicMesh();
//    sMesh = new SkinnedMesh();
    gMesh[0] = new SkinnedMesh();
    gMesh[1] = new SkinnedMesh();
    mMesh[0] = new SkinnedMesh();
    mMesh[1] = new SkinnedMesh();
    /*
    if (!mMesh->LoadMesh("Models/animTest/multiAnim.fbx")){
        printf("Failed to load with assimp!");
    }
    //*/
    //This is the HandFighter, works very well
    for(int i = 0; i < 2; i++){
        if (!gMesh[i]->LoadMesh("Models/handFighter/handFighter2.fbx")){
            printf("Failed to load with assimp!");
        }
        //*
        if (!mMesh[i]->LoadMesh("Models/animTest/mondayMemo.fbx")){
            printf("Failed to load with assimp!");
        }
        /*
        if (!mMesh[i]->LoadMesh("Models/animTest/mondayMemoDisc.fbx")){
            printf("Failed to load with assimp!");
        }
        */
    }
    //*

    /*
    if (!gMesh->LoadMesh("Models/animTest/testAnimSkelii.fbx")){
        printf("Failed to load with assimp!");
    }
    /*
    if (!sMesh->LoadMesh("Models/animTest/boblampclean.md5mesh")){
        printf("Failed to load with assimp!");
    }
    */

    //Evil skinning tech init shit
    pSkinningTech = new SkinningTechnique();

    if (!pSkinningTech->Init())
    {
        //return false;
        printf("pSkinningTech Init has failed!\n");
    }

    pSkinningTech->Enable();

    //pSkinningTech->SetTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    //pSkinningTech->SetSpecularExponentTextureUnit(SPECULAR_EXPONENT_UNIT_INDEX);
    pSkinningTech->SetTextureUnit(0);
    pSkinningTech->SetSpecularExponentTextureUnit(6);


    //  Load Wavwfront models
    //Arena One: Night City Alley
    voltsWagon.Load("Models/LowPol/","Car_Low_Poly.obj");
    fence.Load("Models/fence5/", "uploads_files_3409235_wood_fence.obj");

    //Arena Zero: Roman Bath House?
    bathHouse.Load("Models/Bath/","bath.obj");
    tree1.Load("Models/trees2/", "smallTree.obj");
    tree2.Load("Models/trees2/", "midTree.obj");

    //MainMenu
    column.Load("Models/column/", "roman_column.obj");
    //tree1.Load("Models/Trees/06/", "bigTree.obj");

//Fighter One Load Body Part Objects

    fighterOne[0].Load("Models/FighterOne/", "chest.obj");
    fighterOne[1].Load("Models/FighterOne/", "head.obj");
    fighterOne[2].Load("Models/FighterOne/", "leftUpArm.obj");
    fighterOne[3].Load("Models/FighterOne/", "rightForeArm.obj");
    fighterOne[4].Load("Models/FighterOne/", "rightHand.obj");
    fighterOne[5].Load("Models/FighterOne/", "rightUpArm.obj");
    fighterOne[6].Load("Models/FighterOne/", "leftForeArm.obj");
    fighterOne[7].Load("Models/FighterOne/", "leftHand.obj");
    fighterOne[8].Load("Models/FighterOne/", "rightQuad.obj");
    fighterOne[9].Load("Models/FighterOne/", "rightLeg.obj");
    fighterOne[10].Load("Models/FighterOne/", "leftQuad.obj");
    fighterOne[11].Load("Models/FighterOne/", "leftLeg.obj");
    //They have different materials
    fighterOne[12].Load("Models/FighterTwo/", "chest.obj");
    fighterOne[13].Load("Models/FighterTwo/", "head.obj");
    fighterOne[14].Load("Models/FighterTwo/", "leftUpArm.obj");
    fighterOne[15].Load("Models/FighterTwo/", "rightForeArm.obj");
    fighterOne[16].Load("Models/FighterTwo/", "rightHand.obj");
    fighterOne[17].Load("Models/FighterTwo/", "rightUpArm.obj");
    fighterOne[18].Load("Models/FighterTwo/", "leftForeArm.obj");
    fighterOne[19].Load("Models/FighterTwo/", "leftHand.obj");
    fighterOne[20].Load("Models/FighterTwo/", "rightQuad.obj");
    fighterOne[21].Load("Models/FighterTwo/", "rightLeg.obj");
    fighterOne[22].Load("Models/FighterTwo/", "leftQuad.obj");
    fighterOne[23].Load("Models/FighterTwo/", "leftLeg.obj");

    float objectScale = 2;
    //INITIALIZE OBJ TEXTURE RATIO
    //Arena One
    objmodel.setTextureRatio(0.3);
    objmodel2.setTextureRatio(0.3);
    voltsWagon.setTextureRatio(0.7);
    fence.setTextureRatio(1);
    blackCar.setTextureRatio(0.3);

    //Arena Zero
    bathHouse.setTextureRatio(0.3);
    tree1.setTextureRatio(0.3);
    tree2.setTextureRatio(0.3);

    //Main Menu
    column.setTextureRatio(0.3);

    for (int i = 0; i < 24; i++)
        fighterOne[i].setTextureRatio(0.3);

    //  Load the shaders
    program = LoadShadersFromFile("VertexShaderLightingTexture.glsl", "PhongMultipleLightsAndTexture.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Turn on the shader & get location of transformation matrix.
    glUseProgram(program);
    PVMLoc = glGetUniformLocation(program, "PVM");
    ModelLoc = glGetUniformLocation(program, "Model");
    NormalLoc = glGetUniformLocation(program, "NormalMatrix");
    useTextureLoc = glGetUniformLocation(program, "useTexture");
    texTransLoc = glGetUniformLocation(program, "textrans");

    // Initialize some data.
    mode = GL_FILL;
    sscount = 1;
    CameraNumber = 2;
    restartVid = false;
    PauseVideo = false;

    // Set position of spherical camera
    sphcamera.setPosition(30, 30, 20);

    glEnable(GL_DEPTH_TEST);

    if (SetVS)
    {
        setVerticalSyncEnabled(true);
        setFramerateLimit(60);
    }
    else
    {
        setVerticalSyncEnabled(false);
        setFramerateLimit(0);
    }

    //Initialize Screen and floor obj
    screen.createTessellatedWallOBJ(6.3, 3.8, 1, 1);
    screen.LoadDataToGraphicsCard(0, 1, 2, 3);

    floor.LoadDataToGraphicsCard(0, 1, 2, 3);

    mat = Materials::bluePlastic;

    for (int i = 0; i < 3; i++)
        lt[i].setLight(true,
                       30.0, 30.0, 30.0, 1.0,
                       -1.0, -1.0, -1.0,
                       0.0, 0.0, 0.0, 1.0,
                       0.70, 0.70, 0.70, 1.0,
                       0.70, 0.70, 0.70, 1.0,
                       180.0, 10.0,
                       1.0, 0.0, 0.0
                      );

    LoadLights(lt, "Lt", 3);
    loadMaterial(mat);
    glUniform1i(glGetUniformLocation(program, "numLights"), 3);

    //INITIALLIZE OBJ GLOBAL AMBIENT
    glm::vec4 GlobalAmbient(0.02, 0.02, 0.02, 1);
    //Arena One
    objmodel.setGlobalAmbient(GlobalAmbient);
    objmodel2.setGlobalAmbient(GlobalAmbient);
    voltsWagon.setGlobalAmbient(GlobalAmbient);
    fence.setGlobalAmbient(GlobalAmbient);
    blackCar.setGlobalAmbient(GlobalAmbient);

    //Arena Zero
    bathHouse.setGlobalAmbient(GlobalAmbient);
    tree1.setGlobalAmbient(GlobalAmbient);
    tree2.setGlobalAmbient(GlobalAmbient);

    //Main Menu
    column.setGlobalAmbient(GlobalAmbient);

    //Fighters
    for (int i = 0; i < 24; i++)
        fighterOne[i].setGlobalAmbient(GlobalAmbient);

    lightobj.createSphereOBJ(0.25, 7, 7);
    lightobj.LoadDataToGraphicsCard(0, 1, 2, 3);
    lightobj.setColor(1, 1, 0);
    lightobj.setDrawBorder(GL_TRUE);
/*
    LtPos[0].setPsi(10);
    LtPos[0].setR(1);
    LtPos[1].setPsi(10);
    LtPos[1].setR(1);

    LtPos[2].setTheta(72);
    LtPos[2].setPsi(-55);
    LtPos[2].setR(1);
*/
    for (int i = 0; i < 3; i++)
    {
        lt[i].setPosition(glm::vec4(LtPos[i].getPosition(), 1.0));
        lt[i].setSpotDirection(-LtPos[i].getPosition());
    }

    //INITIALLIZE OBJ NUMB OF LIGHTS + SET LIGHTS

    //Arena One
    objmodel.setNumberOfLights(3);
    objmodel2.setNumberOfLights(3);
    voltsWagon.setNumberOfLights(3);
    fence.setNumberOfLights(3);
    blackCar.setNumberOfLights(3);

    //Arena Zero
    bathHouse.setNumberOfLights(3);
    tree1.setNumberOfLights(3);
    tree2.setNumberOfLights(3);

    //Main Menu
    column.setNumberOfLights(3);

    //Fighter
    for (int i = 0; i < 24; i++)
        fighterOne[i].setNumberOfLights(3);

    for (int i = 0; i < 3; i++)
    {
        objmodel.setLight(i, lt[i]);
        objmodel2.setLight(i, lt[i]);
        voltsWagon.setLight(i, lt[i]);
        fence.setLight(i, lt[i]);
        blackCar.setLight(i, lt[i]);

        bathHouse.setLight(i, lt[i]);
        tree1.setLight(1, lt[i]);
        tree2.setLight(1, lt[i]);

        column.setLight(1, lt[i]);

        for (int x = 0; x < 24; x++)
            fighterOne[x].setLight(i, lt[i]);
    }
    //Arena One
    objmodel.setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));
    objmodel2.setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));
    voltsWagon.setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));
    fence.setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));
    blackCar.setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));
    //Arena Zero
    bathHouse.setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));
    tree1.setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));
    tree2.setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));



    //Fighter
    for (int i = 0; i < 24; i++)
        fighterOne[i].setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(objectScale, objectScale, objectScale)));
//***********************************************************************
//Going to put all of these in a loop for October
    model = glm::mat4(1.0);

    textrans = glm::mat4(1.0);
    textrans = glm::scale(textrans, glm::vec3(5, 5, 1));

    sf::Image texture;
    //bool texloaded = texture.loadFromFile("Tetxures/Repeat-brick.jpg");
    //bool texloaded = texture.loadFromFile("Tetxures/dark.jpg");
    texloaded = texture.loadFromFile("Tetxures/dark.jpg");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }


    glGenTextures(1, &texID);

    tex1_uniform_loc = glGetUniformLocation(program, "tex1");
    glUniform1i(tex1_uniform_loc, texID);

    glActiveTexture(GL_TEXTURE0 + texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    //texloaded = texture.loadFromFile("Tetxures/asfault.jpg");
    texloaded = texture.loadFromFile("Tetxures/stonePattern.jpg");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID2);

    glUniform1i(tex1_uniform_loc, texID2);

    glActiveTexture(GL_TEXTURE0 + texID2);
    glBindTexture(GL_TEXTURE_2D, texID2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    //texloaded = texture.loadFromFile("Tetxures/street.jpg");
    texloaded = texture.loadFromFile("Tetxures/skyline-at-night.jpg");
    //texloaded = texture.loadFromFile("Tetxures/bearNbird.png");
    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID3);

    glUniform1i(tex1_uniform_loc, texID3);

//  Load the texture into texture memory.
    glActiveTexture(GL_TEXTURE0 + texID3);
    glBindTexture(GL_TEXTURE_2D, texID3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    texloaded = texture.loadFromFile("Tetxures/road1.jpg");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID4);

    glUniform1i(tex1_uniform_loc, texID4);

    glActiveTexture(GL_TEXTURE0 + texID4);
    glBindTexture(GL_TEXTURE_2D, texID4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    texloaded = texture.loadFromFile("Tetxures/start2.png");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID5);

    glUniform1i(tex1_uniform_loc, texID5);

    glActiveTexture(GL_TEXTURE0 + texID5);
    glBindTexture(GL_TEXTURE_2D, texID5);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    texloaded = texture.loadFromFile("Tetxures/VsMode.png");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID6);

    glUniform1i(tex1_uniform_loc, texID6);

    glActiveTexture(GL_TEXTURE0 + texID6);
    glBindTexture(GL_TEXTURE_2D, texID6);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    texloaded = texture.loadFromFile("Tetxures/StoryMode.png");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID7);

    glUniform1i(tex1_uniform_loc, texID7);

    glActiveTexture(GL_TEXTURE0 + texID7);
    glBindTexture(GL_TEXTURE_2D, texID7);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    texloaded = texture.loadFromFile("Tetxures/AiBattle.png");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID8);

    glUniform1i(tex1_uniform_loc, texID8);

    glActiveTexture(GL_TEXTURE0 + texID8);
    glBindTexture(GL_TEXTURE_2D, texID8);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    texloaded = texture.loadFromFile("Tetxures/TempleArena.png");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID9);

    glUniform1i(tex1_uniform_loc, texID9);

    glActiveTexture(GL_TEXTURE0 + texID9);
    glBindTexture(GL_TEXTURE_2D, texID9);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//*******************************************
    texloaded = texture.loadFromFile("Tetxures/AlleyArena.png");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texID10);

    glUniform1i(tex1_uniform_loc, texID10);

    glActiveTexture(GL_TEXTURE0 + texID10);
    glBindTexture(GL_TEXTURE_2D, texID10);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
/*
// Loading in sound effects
    if (!hitBuffer.loadFromFile("Sounds/mixkit-karate-fighter-hit-2154.wav"))
        std::cout << "Hit sound not loaded." << std::endl;
    else
        hit.setBuffer(hitBuffer);

    if (!punchBuffer.loadFromFile("Sounds/mixkit-martial-arts-fast-punch-2047.wav"))
        std::cout << "Punch sound not loaded." << std::endl;
    else
        punc.setBuffer(punchBuffer);

    if (!strikeBuffer.loadFromFile("Sounds/mixkit-quick-ninja-strike-2146.wav"))
        std::cout << "Strike sound not loaded." << std::endl;
    else
        strike.setBuffer(strikeBuffer);
*/

fighterClass[0].newFighter(0, 3); // Important to change later, right now set to hand fighter by default
fighterClass[1].newFighter(1, 1);

cameraPos = 0;
cameraX = 0;
cameraZ = 100;
//Sets default "arena" to the main menu
arenaNum = 3;

//Used for skeletal animation;
StartTimeMillis[0] = GetCurrentTimeMillis();
StartTimeMillis[1] = GetCurrentTimeMillis();

//Concluding Statements
    setActive();
    glClearColor(0, 0, 0, 1);
    resize();
}


/**
\brief Destructor

Currently empty, no allocated memory to clear.

*/

GraphicsEngine::~GraphicsEngine() {}

/**
\brief The function responsible for drawing to the OpenGL frame buffer.

This function clears the screen and calls the draw functions of the box and circle.

*/

void GraphicsEngine::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set view matrix via current camera.
    glm::mat4 view(1.0);
    if (CameraNumber == 1)
        view = sphcamera.lookAt();
    else if (CameraNumber == 2)
        view = yprcamera.lookAt();
/*
    glUseProgram(CMprogram);
    glUniformMatrix4fv(glGetUniformLocation(CMprogram, "PVM"),
                       1, GL_FALSE, glm::value_ptr(projection*view));

    CMSphere.draw();
*/
    glUseProgram(program);


    for (int i = 0; i < 3; i++)
    {
        lt[i].setPosition(glm::vec4(LtPos[i].getPosition(), 1.0));
        lt[i].setSpotDirection(-LtPos[i].getPosition());
    }
    LoadLights(lt, "Lt", 3);

    for (int i = 0; i < 3; i++)
    {
        objmodel.setLight(i, lt[i]);
        objmodel2.setLight(i, lt[i]);
        voltsWagon.setLight(i, lt[i]);
        fence.setLight(i, lt[i]);
        bathHouse.setLight(i, lt[i]);
        for (int x = 0; i < 12; i++)
            fighterOne[x].setLight(i, lt[i]);
    }
    //yprcamera.setPosition(((fighterClass[0].fighterX[0]+fighterClass[1].fighterX[1])/2), yprcamera.getPosition().y, ((fighterClass[0].fighterX[0]-fighterClass[1].fighterX[1])*(-1) + 25));
    //std::cout << "YPR::" << yprcamera.getPosition().z << " Fighter One::" << fighterX[0] << " Fighter Two" << fighterX[1] << std::endl;
    glm::vec3 eye;
    if (CameraNumber == 1)
        eye = sphcamera.getPosition();
    else if (CameraNumber == 2)
        eye = yprcamera.getPosition();

    glUniform3fv(glGetUniformLocation(program, "eye"), 1, glm::value_ptr(eye));

    long long CurrentTimeMillis = GetCurrentTimeMillis();
    AnimationTimeSec[0] = ((float)(CurrentTimeMillis - StartTimeMillis[0])) / 1000.0f;
    AnimationTimeSec[1] = ((float)(CurrentTimeMillis - StartTimeMillis[1])) / 1000.0f;
    //float AnimationTimeSec = ((float)(CurrentTimeMillis - StartTimeMillis)) / 1000.0f;

    //Call function to load the arena in
    switch(arenaNum)
    {
        case 0:
            loadArenaZero(view, eye);
            //loadFighter(0, view, eye);
            //loadHandFighter(0, view, eye);
            loadSelectedFighter(curFIndex[0], 0, view, eye);
            //loadFighter(1, view, eye);
            loadSelectedFighter(curFIndex[1], 1, view, eye);
            activateHUD();
            yprcamera.setPosition(((fighterClass[0].fighterX[0]+fighterClass[1].fighterX[1])/2), yprcamera.getPosition().y, ((fighterClass[0].fighterX[0]-fighterClass[1].fighterX[1])*(-1) + 25));
            glUseProgram(CMprogram);
            glUniformMatrix4fv(glGetUniformLocation(CMprogram, "PVM"), 1, GL_FALSE, glm::value_ptr(projection*view));
            CMSphere.draw();
            glUseProgram(program);
            break;

        case 1:
            loadSelectedFighter(curFIndex[0], 0, view, eye);
            //loadFighter(1, view, eye);
            loadSelectedFighter(curFIndex[1], 1, view, eye);
            loadArenaOne(view, eye);
            //loadFighter(0, view, eye, false);
            //loadFighter(1, view, eye, false);
            activateHUD();
            yprcamera.setPosition(((fighterClass[0].fighterX[0]+fighterClass[1].fighterX[1])/2), yprcamera.getPosition().y, ((fighterClass[0].fighterX[0]-fighterClass[1].fighterX[1])*(-1) + 25));
            //CMSphere.draw();
            //glUseProgram(program);
            break;

        case 2:
            //loadArenaTwo(view, eye);
            break;
        case 3:
            runMainMenu(view, eye);
            //yprcamera.setPosition(((fighterClass[0].fighterX[0]+fighterClass[1].fighterX[1])/2), yprcamera.getPosition().y, ((fighterClass[0].fighterX[0]-fighterClass[1].fighterX[1])*(-1) + 25));
            //DO CAMERA STUFF
            //activateHUD();
            break;
    }

    //Combat Animation
    for (int i = 0; i < 2; i ++)
    {
        if (!PauseVideo)
        {
            fighterClass[i].oppTracker(rivalNum[i], curFIndex[i], fighterClass[rivalNum[i]].fighterX[rivalNum[i]], fighterClass[rivalNum[i]].fighterY[rivalNum[i]], fighterClass[rivalNum[i]].attacking[rivalNum[i]], fighterClass[rivalNum[i]].kick[rivalNum[i]], fighterClass[rivalNum[i]].punch[rivalNum[i]], fighterClass[rivalNum[i]].punch2[rivalNum[i]], fighterClass[rivalNum[i]].tornado[rivalNum[i]], fighterClass[i].fighterStun[rivalNum[i]], fighterClass[rivalNum[i]].fighterStun[i]);
            fighterClass[i].preformActions();
            //std::cout<< PauseVideo << std::endl;
        }
    }

    turnLightsOff("Lt", 3);
    glUniform1i(useTextureLoc, false);

//Back to OG programing
    turnLightsOn("Lt", 3);

    sf::RenderWindow::display();
    //printOpenGLErrors();
}

/**
\brief Loads the matrices to the shader.

\param projection --- The projection matrix for the scene.
\param view --- The view matrix for the scene.
\param model --- The model matrix for the object.
\param PVMLocation --- Unsigned integer for the location of the PVM matrix in the shader.
\param ModelLocation --- Unsigned integer for the location of the model matrix in the shader.
\param NormalLocation --- Unsigned integer for the location of the normal matrix in the shader.

*/

void GraphicsEngine::loadMatrices(glm::mat4 projection, glm::mat4 view, glm::mat4 model,
                                  GLuint PVMLocation, GLuint ModelLocation, GLuint NormalLocation)
{
    glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(PVMLocation, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    glm::mat3 nM(model);
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

/**
\brief Changes the fill and line mode being used.

*/

void GraphicsEngine::changeMode()
{
    if (mode == GL_FILL)
        mode = GL_LINE;
    else
        mode = GL_FILL;

    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

/**
\brief Saves a screenshot of the current display to a file, ScreenShot###.png.

*/

void GraphicsEngine::screenshot()
{
    char ssfilename[100];
    sprintf(ssfilename, "ScreenShot%d.png", sscount);
    sf::Vector2u windowSize = getSize();
    sf::Texture texture;
    texture.create(windowSize.x, windowSize.y);
    texture.update(*this);
    sf::Image img = texture.copyToImage();
    img.saveToFile(ssfilename);
    sscount++;
}

/**
\brief Handles the resizing events of the window.

Sets the viewport to the entire screen and recalculates the projection matrix.
*/

void GraphicsEngine::resize()
{
    glViewport(0, 0, getSize().x, getSize().y);
    projection = glm::perspective(50.0f*degf, (float)getSize().x/getSize().y, 0.01f, 1000.0f);
}

/**
\brief Sets the size of the rendering window.

\param width --- the width in pixels of the new size.

\param height --- the height in pixels of the new size.

*/

void GraphicsEngine::setSize(unsigned int width, unsigned int height)
{
    sf::RenderWindow::setSize(sf::Vector2u(width, height));
    resize();
}

/**
\brief Returns a pointer to the spherical camera.

*/

SphericalCamera* GraphicsEngine::getSphericalCamera()
{
    return &sphcamera;
}

/**
\brief Returns a pointer to the spherical camera that represents the movable light..

*/

SphericalCamera* GraphicsEngine::getLtPos()
{
    return &LtPos[0];
}

/**
\brief Returns a pointer to the yaw-pitch-roll camera.

*/

YPRCamera* GraphicsEngine::getYPRCamera()
{
    return &yprcamera;
}

/**
\brief Prints all OpenGL errors to stderr.

*/

void GraphicsEngine::printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s\n", errString);
    }
}

/**
\brief Prints the glm matrix to the console window.  Remember that glm
matrices are column major. This is for the 3 X 3 matrices.

\param m --- the glm matrix to be displayed.

*/

void GraphicsEngine::print_GLM_Matrix(glm::mat4 m)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            printf("%7.2f", m[c][r]);

        std::cout << std::endl;
    }
    std::cout << std::endl;
}


/**
\brief Prints the glm matrix to the console window.  Remember that glm
matrices are column major. This is for the 3 X 3 matrices.

\param m --- the glm matrix to be displayed.

*/

void GraphicsEngine::print_GLM_Matrix(glm::mat3 m)
{
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
            printf("%7.2f", m[c][r]);

        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
\brief Returns true if the spherical camera is currently in use.

\return True if the spherical camera is currently in use and false otherwise.

*/

GLboolean GraphicsEngine::isSphericalCameraOn()
{
    return CameraNumber == 1;
}

/**
\brief Turns the spherical camera on.

*/

void GraphicsEngine::setSphericalCameraOn()
{
    CameraNumber = 1;
}

/**
\brief Returns true if the yaw-pitch-roll camera is currently in use.

\return True if the yaw-pitch-roll camera is currently in use and false otherwise.

*/

GLboolean GraphicsEngine::isYPRCameraOn()
{
    return CameraNumber == 2;
}

/**
\brief Turns the YPR camera on.

*/

void GraphicsEngine::setYPRCameraOn()
{
    CameraNumber = 2;
}

/**
\brief Turns the light on

*/

void GraphicsEngine::turnLightOn()
{
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "Lt.on"), true);
}

/**
\brief Turns the light off

*/

void GraphicsEngine::turnLightOff()
{
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "Lt.on"), false);
}

/**
\brief Loads the light structure to the shader light structure.

\param Lt --- Light structure to load.

*/

void GraphicsEngine::loadLight(Light Lt)
{
    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "Lt.on"), Lt.getOn());
    glUniform4fv(glGetUniformLocation(program, "Lt.position"), 1, glm::value_ptr(Lt.getPosition()));
    glUniform4fv(glGetUniformLocation(program, "Lt.ambient"), 1, glm::value_ptr(Lt.getAmbient()));
    glUniform4fv(glGetUniformLocation(program, "Lt.diffuse"), 1, glm::value_ptr(Lt.getDiffuse()));
    glUniform4fv(glGetUniformLocation(program, "Lt.specular"), 1, glm::value_ptr(Lt.getSpecular()));
    glUniform3fv(glGetUniformLocation(program, "Lt.spotDirection"), 1, glm::value_ptr(Lt.getSpotDirection()));
    glUniform3fv(glGetUniformLocation(program, "Lt.attenuation"), 1, glm::value_ptr(Lt.getAttenuation()));
    glUniform1f(glGetUniformLocation(program, "Lt.spotCutoff"), Lt.getSpotCutoff());
    glUniform1f(glGetUniformLocation(program, "Lt.spotExponent"), Lt.getSpotExponent());
}

/**
\brief Loads the material structure to the shader material structure.

\param Mat --- Material structure to load.

*/

void GraphicsEngine::loadMaterial(Material Mat)
{
    glUseProgram(program);

    glUniform4fv(glGetUniformLocation(program, "Mat.ambient"), 1, glm::value_ptr(Mat.getAmbient()));
    glUniform4fv(glGetUniformLocation(program, "Mat.diffuse"), 1, glm::value_ptr(Mat.getDiffuse()));
    glUniform4fv(glGetUniformLocation(program, "Mat.specular"), 1, glm::value_ptr(Mat.getSpecular()));
    glUniform4fv(glGetUniformLocation(program, "Mat.emission"), 1, glm::value_ptr(Mat.getEmission()));
    glUniform1f(glGetUniformLocation(program, "Mat.shininess"), Mat.getShininess());
}

/**
\brief Loads a single light into a light array in the shader.

\param Lt --- Light to load.

\param name --- The name of the array in the shader.

\param i --- The index of the light to load.

*/

void GraphicsEngine::LoadLight(Light Lt, std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), Lt.getOn());

    sprintf(locID, "%s[%d].%s", arrayname, i, "position");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getPosition()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "ambient");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getAmbient()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "diffuse");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getDiffuse()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "specular");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getSpecular()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotDirection");
    glUniform3fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getSpotDirection()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "attenuation");
    glUniform3fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getAttenuation()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotCutoff");
    glUniform1f(glGetUniformLocation(program, locID), Lt.getSpotCutoff());

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotExponent");
    glUniform1f(glGetUniformLocation(program, locID), Lt.getSpotExponent());
}

/**
\brief Loads the first num entries of a light array to the a light array in the shader.

\param Lt --- Light array to load.

\param name --- The name of the array in the shader.

\param i --- The number of lights to load.

*/

void GraphicsEngine::LoadLights(Light Lt[], std::string name, int num)
{
    for (int i = 0; i < num; i++)
        LoadLight(Lt[i], name.c_str(), i);
}

/**
\brief Turns the light at index i on.

\param name --- The name of the array in the shader.

\param i --- The index of the light to turn on.

*/

void GraphicsEngine::turnLightOn(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), true);
}

/**
\brief Turns the light at index i off.

\param name --- The name of the array in the shader.

\param i --- The index of the light to turn off.

*/

void GraphicsEngine::turnLightOff(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), false);
}

/**
\brief Turns the first num lights on.

\param name --- The name of the array in the shader.

\param num --- Number of lights to turn on.

*/

void GraphicsEngine::turnLightsOn(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnLightOn(name.c_str(), i);
}

/**
\brief Turns the first num lights off.

\param name --- The name of the array in the shader.

\param num --- Number of lights to turn off.

*/

void GraphicsEngine::turnLightsOff(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnLightOff(name.c_str(), i);
}

/**
\brief Toggles the drawing of the axes.

*/

void GraphicsEngine::toggleAxes()
{
    drawAxes = !drawAxes;
}

/**
\brief Toggles the drawing of the lights.

*/

void GraphicsEngine::toggleLights()
{
    drawLights = !drawLights;
}

/**
\brief Toggles the Video Pause

*/

void GraphicsEngine::togglePause()
{
    if (PauseVideo)
        PauseVideo = false;
    else
        PauseVideo = true;
}

/**
\brief Toggles the drawing of the axes.

*/

void GraphicsEngine::restartVideo()
{
    restartVid = true;
}

/**
\brief Loads the first arena

\param view --- Camera

\param eye --- Camera Matrix

*/

void GraphicsEngine::loadArenaOne(glm::mat4 view, glm::vec3 eye)
{
//Low Polygon Car

    voltsWagon.setEye(eye);
    voltsWagon.setProjectionMatrix(projection);
    voltsWagon.setViewMatrix(view);

    glm::mat4 voltsMod(1.0);

    static float a = 500;
    static float angle1 = 10;
    if(!(PauseVideo || gameOver))
    {
        if (a >= -500)
            a -= 8;
        else
            a = 500;

        if (a <= 120 && a >= -120)
        {
            angle1 += 5;
            LtPos[0].setTheta(angle1);
        }
        else
        {
            angle1 = 10;
            LtPos[0].setTheta(0);
        }
    }
    //if (angle >= )
    voltsMod = glm::translate(voltsMod, glm::vec3(a, 0, -40));
    voltsMod = glm::scale(voltsMod, glm::vec3(5, 5, 5));


    lt[0].setPosition(glm::vec4(LtPos[0].getPosition(), 1.0));
    lt[0].setSpotDirection(-LtPos[0].getPosition());
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*voltsMod));
    voltsWagon.setModelMatrix(voltsMod);
    voltsWagon.draw();


//Car Two (Was a different model but decided it would be better to reuse for optimization on loading)

    LtPos[0].setPsi(10);
    LtPos[0].setR(1);
    LtPos[1].setPsi(10);
    LtPos[1].setR(1);

    LtPos[2].setTheta(72);
    LtPos[2].setPsi(-55);
    LtPos[2].setR(1);

    glm::mat4 carMod(1.0);

    static float b = -800;
    static float angle2 = 156;
    if(!(PauseVideo || gameOver))
    {
        if (b <= 800)
            b += 6;
        else
            b = -800;

        if (b <= 120 && b >= -120)
        {
            angle2 -= 3;
            LtPos[1].setTheta(angle2);
        }
        else
        {
            angle2 = 156;
            LtPos[1].setTheta(0);
        }
    }
    carMod = glm::translate(carMod, glm::vec3(b, 0, -80));
    carMod = glm::rotate(carMod, 180*degf, glm::vec3(0, 1, 0));
    carMod = glm::scale(carMod, glm::vec3(5, 5, 5));

    lt[1].setPosition(glm::vec4(LtPos[1].getPosition(), 1.0));
    lt[1].setSpotDirection(-LtPos[1].getPosition());
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*carMod));
    voltsWagon.setModelMatrix(carMod);
    voltsWagon.draw();

//FENCE
    fence.setEye(eye);
    fence.setProjectionMatrix(projection);
    fence.setViewMatrix(view);

    glm::mat4 fenceMod(1.0);

    fenceMod = glm::translate(fenceMod, glm::vec3(-45, 0.2, -20));

    fenceMod = glm::scale(fenceMod, glm::vec3(18, 10, 18));

    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*fenceMod));
    fence.setModelMatrix(fenceMod);
    fence.draw();

//OUT OF OBJ MODELS
    turnLightsOff("Lt", 3);
    glUniform1i(useTextureLoc, false);
    glm::mat4 axesscale = glm::scale(glm::mat4(1.0), glm::vec3(10, 10, 10));
    loadMatrices(projection, view, axesscale, PVMLoc, ModelLoc, NormalLoc);

    loadMatrices(projection, view, model, PVMLoc, ModelLoc, NormalLoc);
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    turnLightsOn("Lt", 3);
    glUniform1i(useTextureLoc, true);
//Right Brick Wall
    mat = Materials::obsidian;
    loadMaterial(mat);
    glm::mat4 brickMod1(1.0);
    brickMod1 = glm::translate(brickMod1, glm::vec3(34, 14, 5));
    brickMod1 = glm::rotate(brickMod1, -70*degf, glm::vec3(0, 1, 0));
    brickMod1 = glm::scale(brickMod1, glm::vec3(30, 35, 12));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*brickMod1));
    glUniform1i(tex1_uniform_loc, texID);
    floor.draw();
//Left Brick Wall
    glm::mat4 brickMod2(1.0);
    brickMod2 = glm::translate(brickMod2, glm::vec3(-34, 14, 5));
    brickMod2 = glm::rotate(brickMod2, 250*degf, glm::vec3(0, 1, 0));
    brickMod2 = glm::scale(brickMod2, glm::vec3(30, 35, 12));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*brickMod2));
    floor.draw();
//Asfault Ground
    mat = Materials::bluePlastic;
    loadMaterial(mat);
    glm::mat4 asfaultMod(1.0);
    //asfaultMod = glm::translate(asfaultMod, glm::vec3(0, 2, 0));
    asfaultMod = glm::translate(asfaultMod, glm::vec3(0, 0, 19));
    asfaultMod = glm::rotate(asfaultMod, 90*degf, glm::vec3(-1, 0, 0));
    asfaultMod = glm::scale(asfaultMod, glm::vec3(20, 20, 4));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*asfaultMod));

    glUniform1i(tex1_uniform_loc, texID2);
    screen.draw();
//Road Ground
    glm::mat4 roadMod(1.0);
    roadMod = glm::translate(roadMod, glm::vec3(0, 0, -69));
    roadMod = glm::rotate(roadMod, 90*degf, glm::vec3(-1, 0, 0));
    roadMod = glm::scale(roadMod, glm::vec3(100, 50, 0));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*roadMod));

    glUniform1i(tex1_uniform_loc, texID4);
    floor.draw();
//Background of Street
    mat = Materials::polishedBronze;
    loadMaterial(mat);
    glm::mat4 backMod(1.0);
    backMod = glm::translate(backMod, glm::vec3(0, 5, -120));
    backMod = glm::scale(backMod, glm::vec3(120, 120, 0));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*backMod));
    glUniform1i(tex1_uniform_loc, texID3);
    floor.draw();
}
/**
\brief Loads the main menu

\param view --- Camera

\param eye --- Camera Matrix

*/
void GraphicsEngine::runMainMenu(glm::mat4 view, glm::vec3 eye)
{
    //Light Pos for Arena Zero
    LtPos[0].setTheta(45);
    LtPos[0].setPsi(45);
    LtPos[0].setR(15);

    LtPos[1].setTheta(100);
    LtPos[1].setPsi(-45);
    LtPos[1].setR(20);

    LtPos[2].setTheta(-100);
    LtPos[2].setPsi(60);
    LtPos[2].setR(20);

    for (int i = 0; i < 3; i++)
    {
        lt[i].setPosition(glm::vec4(LtPos[i].getPosition(), 1.0));
        lt[i].setSpotDirection(-LtPos[i].getPosition());
    }

    //Camera Movements Handler
    switch (cameraPos)
    {
        case 0:
            makeValue(0, 0, 1);
            makeValue(1, 100, 1);
            break;
        case 1:
            makeValue(0, 0, 1);
            makeValue(1, 50, 1);
            break;
        case 2:
            makeValue(0, 35, 1);
            makeValue(1, 50, 1);
            break;
        case 3:
            makeValue(0, -35, 1);
            makeValue(1, 50, 1);
            break;
        case 4:
            makeValue(0, -10, 0.5);
            makeValue(1, -10, 0.5);
            break;
        case 5:
            makeValue(0, 10, 0.5);
            makeValue(1, -10, 0.5);
            break;
        case 6:
            displayFighter(curFIndex[0], 0, view, eye);
            displayFighter(curFIndex[1], 1, view, eye);
            makeValue(0, 0, 1);
            makeValue(1, -30, 1);
            break;
        case 7:
            displayFighter(curFIndex[0], 0, view, eye);
            displayFighter(curFIndex[1], 1, view, eye);
            makeValue(0, 0, 1);
            makeValue(1, -30, 1);
            break;
    }

    yprcamera.setPosition(cameraX, yprcamera.getPosition().y, cameraZ);

    //Column 1 (0 1 0 0)
    column.setEye(eye);
    column.setProjectionMatrix(projection);
    column.setViewMatrix(view);

    glm::mat4 colMod(1.0);

    colMod = glm::scale(colMod, glm::vec3(0.5, 0.5, 0.5));
    colMod = glm::translate(colMod, glm::vec3(-35, -1, 0));
    //colMod = glm::rotate(bathMod, 90*degf, glm::vec3(0, -1, 0));

    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*colMod));
    column.setModelMatrix(colMod);
    column.draw();
    //Column 2 (1 0 0 0)
    colMod = glm::translate(colMod, glm::vec3(-70, 0, 0));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*colMod));
    column.setModelMatrix(colMod);
    column.draw();
    //Column 3 (0 0 1 0)
    colMod = glm::mat4(1.0);

    colMod = glm::scale(colMod, glm::vec3(0.5, 0.5, 0.5));
    colMod = glm::translate(colMod, glm::vec3(35, -1, 0));

    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*colMod));
    column.setModelMatrix(colMod);
    column.draw();
    //Column 4 (0 0 0 1)
    colMod = glm::translate(colMod, glm::vec3(70, 0, 0));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*colMod));
    column.setModelMatrix(colMod);
    column.draw();
    //Column 5 (0 1 0)
    colMod = glm::mat4(1.0);
    colMod = glm::translate(colMod, glm::vec3(0, 0, -45));
    colMod = glm::scale(colMod, glm::vec3(0.5, 0.5, 0.5));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*colMod));
    column.setModelMatrix(colMod);
    column.draw();

    turnLightsOn("Lt", 3);
    loadMatrices(projection, view, model, PVMLoc, ModelLoc, NormalLoc);
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    glUniform1i(useTextureLoc, true);

    //Ground
    //turnLightsOn("Lt", 3);
    loadMatrices(projection, view, model, PVMLoc, ModelLoc, NormalLoc);
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    //turnLightsOn("Lt", 3);
    glUniform1i(useTextureLoc, true);

    mat = Materials::bluePlastic;
    loadMaterial(mat);
    glm::mat4 asfaultMod(1.0);
    //asfaultMod = glm::translate(asfaultMod, glm::vec3(0, 2, 0));
    asfaultMod = glm::translate(asfaultMod, glm::vec3(0, 0, 0));
    asfaultMod = glm::rotate(asfaultMod, 90*degf, glm::vec3(-1, 0, 0));
    //asfaultMod = glm::scale(asfaultMod, glm::vec3(30, 30, 4));
    asfaultMod = glm::scale(asfaultMod, glm::vec3(30, 40, 4));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*asfaultMod));

    glUniform1i(tex1_uniform_loc, texID2);
    screen.draw();

    //Start Blade
    glm::mat4 msgMod(1.0);

    msgMod = glm::translate(msgMod, glm::vec3(0, 10, 80));
    static float angle = 0;
    angle += 1;
    //if (cameraPos == 0)
        msgMod = glm::rotate(msgMod, angle*degf, glm::vec3(0, 1, 0));
    msgMod = glm::scale(msgMod, glm::vec3(5, 5, 5));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*msgMod));

    glUniform1i(tex1_uniform_loc, texID5);
    floor.draw();

    //Vs Blade
    msgMod = glm::mat4(1.0);
    msgMod = glm::translate(msgMod, glm::vec3(0, 10, 0));
    if (cameraPos != 1)
        msgMod = glm::rotate(msgMod, angle*degf, glm::vec3(0, 1, 0));
    msgMod = glm::scale(msgMod, glm::vec3(5, 5, 5));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*msgMod));
    glUniform1i(tex1_uniform_loc, texID6);
    floor.draw();

    //Story Mode Blade
    msgMod = glm::mat4(1.0);
    msgMod = glm::translate(msgMod, glm::vec3(-35, 10, 0));
    if (cameraPos != 3)
        msgMod = glm::rotate(msgMod, angle*degf, glm::vec3(0, 1, 0));
    msgMod = glm::scale(msgMod, glm::vec3(5, 5, 5));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*msgMod));
    glUniform1i(tex1_uniform_loc, texID7);
    floor.draw();

    //Vs AI Blade
    msgMod = glm::mat4(1.0);
    msgMod = glm::translate(msgMod, glm::vec3(35, 10, 0));
    if (cameraPos != 2)
        msgMod = glm::rotate(msgMod, angle*degf, glm::vec3(0, 1, 0));
    msgMod = glm::scale(msgMod, glm::vec3(5, 5, 5));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*msgMod));
    glUniform1i(tex1_uniform_loc, texID8);
    floor.draw();

    //Temple Arena Blade
    msgMod = glm::mat4(1.0);
    msgMod = glm::translate(msgMod, glm::vec3(-10, 10, -30));
    if (cameraPos != 4)
        msgMod = glm::rotate(msgMod, angle*degf, glm::vec3(0, 1, 0));
    msgMod = glm::scale(msgMod, glm::vec3(5, 5, 5));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*msgMod));
    glUniform1i(tex1_uniform_loc, texID9);
    floor.draw();

    //Alley Arena Blade
    msgMod = glm::mat4(1.0);
    msgMod = glm::translate(msgMod, glm::vec3(10, 10, -30));
    if (cameraPos != 5)
        msgMod = glm::rotate(msgMod, angle*degf, glm::vec3(0, 1, 0));
    msgMod = glm::scale(msgMod, glm::vec3(5, 5, 5));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*msgMod));
    glUniform1i(tex1_uniform_loc, texID10);
    floor.draw();

    runWeirdProgram(view);
}


void GraphicsEngine::makeValue(float changingVar, float toValue, float rate)
{
    bool isX = true;
    if (changingVar == 0)
        changingVar = cameraX;
    else if (changingVar == 1)
    {
        changingVar = cameraZ;
        isX = 0;
    }

    if (changingVar < toValue)
        changingVar += rate;
    else if (changingVar > toValue)
        changingVar -= rate;

    if (isX)
        cameraX = changingVar;
    else
        cameraZ = changingVar;
}
void GraphicsEngine::menuRightSlide()
{
    switch (cameraPos)
    {
        case 1:
            cameraPos = 2;
            break;
        case 2:
            cameraPos = 3;
            break;
        case 3:
            cameraPos = 1;
            break;
        case 4:
            cameraPos = 5;
            break;
        case 5:
            cameraPos = 4;
            break;
        case 6:
            //Swaps displayed fighter
            if (curFIndex[0] == 3)
                curFIndex[0] = 0;
            else if (curFIndex[0] == 1)
                curFIndex[0] = 3;
            else
                curFIndex[0]++;
            break;
        case 7:
            //Swaps displayed fighter
            if (curFIndex[0] == 3)
                curFIndex[0] = 0;
            else if (curFIndex[0] == 1)
                curFIndex[0] = 3;
            else
                curFIndex[0]++;
            break;
        default:
            break;
    }
    //std::cout<<cameraPos<<std::endl;
}

void GraphicsEngine::menuLeftSlide()
{
    switch (cameraPos)
    {
        case 1:
            cameraPos = 3;
            break;
        case 2:
            cameraPos = 1;
            break;
        case 3:
            cameraPos = 2;
            break;
        case 4:
            cameraPos = 5;
            break;
        case 5:
            cameraPos = 4;
        case 6:
            //Swaps displayed fighter
            if (curFIndex[0] == 0)
                curFIndex[0] = 3;
            else if (curFIndex[0] == 3)
                curFIndex[0] = 1;
            else
                curFIndex[0]--;
            break;
        case 7:
            //Swaps displayed fighter
            if (curFIndex[0] == 3)
                curFIndex[0] = 0;
            else if (curFIndex[0] == 3)
                curFIndex[0] = 1;
            else
                curFIndex[0]--;
            break;
        default:
            break;
    }
    //std::cout<<cameraPos<<std::endl;
}

void GraphicsEngine::menuSelect()
{
    switch (cameraPos)
    {
        case 0:
            cameraPos = 1;
            break;
        case 1:
            cameraPos = 4;
            break;
        case 2:
            //Goes to Vs Battle options (When gamemode is added)
            break;
        case 3:
            //Goes to Story mode options (When gamemode is added)
            break;
        case 4:
            //arenaNum = 0;
            //restartGame();
            cameraPos = 6;//Roman Arena Selected
            break;
        case 5:
            //arenaNum = 1;
            //restartGame();
            cameraPos = 7;//Roman Arena Selected
            break;
        case 6:
            arenaNum = 0;//Loads Roman Bath House
            //Set Correct Fighter IDs
            fighterClass[0].setFighterID(curFIndex[0], curFIndex[1]);
            fighterClass[1].setFighterID(curFIndex[1], curFIndex[0]);
            restartGame();
            break;
        case 7:
            arenaNum = 1;//Loads alley way
            //Set Correct Fighter IDs
            fighterClass[0].setFighterID(curFIndex[0], curFIndex[1]);
            fighterClass[1].setFighterID(curFIndex[1], curFIndex[0]);
            restartGame();
            break;
        default:
            break;
    }
    //std::cout<<cameraPos<<std::endl;
}

void GraphicsEngine::menuBackout()
{
    switch (cameraPos)
    {
        case 1:
            cameraPos = 0;
            break;
        case 2:
            cameraPos = 0;
            break;
        case 3:
            cameraPos = 0;
            break;
        case 4:
            cameraPos = 1;
            break;
        case 5:
            cameraPos = 1;
            break;
        case 6:
            cameraPos = 4;
            break;
        case 7:
            cameraPos = 5;
        default:
            break;
    }
    //std::cout<<cameraPos<<std::endl;
}

void GraphicsEngine::returnToMenu()
{
    if (PauseVideo || gameOver)
    {
        cameraPos = 1;
        arenaNum = 3;
    }
}
//Returns Arena Num (for the UI to only allow attacks on non menu arenas && menu selectors on menu only)
int GraphicsEngine::getArenaNum()
{
    return arenaNum;
}

// GraphicsEngine::menuObjLoad()
//{

//}


void GraphicsEngine::loadArenaZero(glm::mat4 view, glm::vec3 eye)
{
    //Light Pos for Arena Zero
    LtPos[0].setTheta(45);
    LtPos[0].setPsi(45);
    LtPos[0].setR(15);

    LtPos[1].setTheta(100);
    LtPos[1].setPsi(-45);
    LtPos[1].setR(20);

    LtPos[2].setTheta(-100);
    LtPos[2].setPsi(60);
    LtPos[2].setR(20);

    for (int i = 0; i < 3; i++)
    {
        lt[i].setPosition(glm::vec4(LtPos[i].getPosition(), 1.0));
        lt[i].setSpotDirection(-LtPos[i].getPosition());
    }

    //First Object, Roman Bath House "bathHouse": Uses model matrix "bathMod"
    bathHouse.setEye(eye);
    bathHouse.setProjectionMatrix(projection);
    bathHouse.setViewMatrix(view);

    glm::mat4 bathMod(1.0);

    bathMod = glm::translate(bathMod, glm::vec3(0, -6, -35));
    bathMod = glm::rotate(bathMod, 90*degf, glm::vec3(0, -1, 0));
    bathMod = glm::scale(bathMod, glm::vec3(5, 5, 5));

    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*bathMod));
    bathHouse.setModelMatrix(bathMod);
    bathHouse.draw();

    //Small Tree
    tree1.setEye(eye);
    tree1.setProjectionMatrix(projection);
    tree1.setViewMatrix(view);

    glm::mat4 treeMod(1.0);

    //treeMod = glm::rotate(treeMod, 90*degf, glm::vec3(0, -1, 0));
    //treeMod = glm::rotate(treeMod , 90*degf, glm::vec3(-1, 0, 0));
    treeMod = glm::scale(treeMod, glm::vec3(10, 10, 10));
    treeMod = glm::translate(treeMod, glm::vec3(0, -1, -8));
    treeMod = glm::translate(treeMod, glm::vec3(-20.5, 0, 0));

    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*treeMod));
    tree1.setModelMatrix(treeMod);
    tree1.draw();


    //Small Tree
    tree2.setEye(eye);
    tree2.setProjectionMatrix(projection);
    tree2.setViewMatrix(view);

    treeMod = glm::mat4(1.0);

    //treeMod = glm::rotate(treeMod, 90*degf, glm::vec3(0, -1, 0));
    //treeMod = glm::rotate(treeMod , 90*degf, glm::vec3(-1, 0, 0));
    treeMod = glm::scale(treeMod, glm::vec3(10, 10, 10));
    treeMod = glm::translate(treeMod, glm::vec3(3, -2, -8));

    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*treeMod));
    tree2.setModelMatrix(treeMod);
    tree2.draw();
}

void GraphicsEngine::activateHUD()
{
    glUseProgram(program2DPass);
    glViewport(0, 0, getSize().x, getSize().y);

    //Initilaize text variables
    int starty = getSize().y/20;
    int startx = getSize().x/2.7;
    text.setScreenSize(getSize().x, getSize().y);
    text.setColor(1, 1, 1, 1);
    text.setFontSize((int)(0.8*starty));

    //Player One's HP bar
    hpBar1.setCenter(-1, 1.35);


    if (fighterClass[1].fighterStun[0] == true || fighterClass[0].stunCounter[0] != 15)
        hpBar1.setAllColor(0.45, 1);
    else
    {
        hpBar1.setAllColor(0, 1);
        hpBar1.setWidth((fighterClass[1].fighterHP[0]/0.75)/100);
    }
    hpBar1.draw();

    //Player Two's HP bar
    hpBar2.setCenter(1, 1.35);

    if (fighterClass[0].fighterStun[1] == true || fighterClass[1].stunCounter[1] != 15)
        hpBar2.setAllColor(0.45, 1);
    else
    {
        hpBar2.setAllColor(0, 1);
        hpBar2.setWidth((fighterClass[0].fighterHP[1]/0.75)/100);
    }
    hpBar2.draw();

    behindBar1.setCenter(-1, 1.35);
    behindBar1.setAllColor(0, 0.2);
    behindBar1.setHeight(1.03);
    behindBar2.setCenter(1, 1.35);
    behindBar2.setAllColor(0, 0.2);
    behindBar2.setHeight(1.03);
    behindBar2.draw();
    behindBar1.draw();
//Pause and Game Over Stuff
    if (fighterClass[0].fighterHP[1] <= 0 || fighterClass[1].fighterHP[0] <= 0)
    {
        gameOver = true;
        PauseVideo = true;
        text.setScreenSize(getSize().x, getSize().y);
        int ftsz = 200;
        if (arenaNum == 0)
            text.setColor(1, 0, 0, 1);
        else
            text.setColor(1, 1, 1, 1);
        text.setFontSize(ftsz);

        while (text.textWidth("-Game Over-") > getSize().x * 0.9)
        text.setFontSize(--ftsz);

        int x = (getSize().x - text.textWidth("-Game Over-"))/2;
        int y = (getSize().y)/2;

        if (x < 1)
            x = 1;
        if (y < 1)
            y = 1;

        text.draw("-Game Over-", x, y);

        text.setFontSize((int)(0.9*starty));
        if (fighterClass[0].fighterHP[0] <= 0)
        {
            text.setColor(0, 0, 1, 1);
            char playerTwoWin[100] = "-Player Two Wins-";
            text.draw(playerTwoWin,(getSize().x/2)-startx/2, getSize().y/2-startx/2);
        }
        else if (fighterClass[1].fighterHP[1] <= 0)
        {
            text.setColor(1, 0, 0, 1);
            char playerOneWin[100] = "-Player One Wins-";
            text.draw(playerOneWin, (getSize().x/2)-startx/2, getSize().y/2-startx/2);
        }

        while (text.textWidth("-Press Z for Menu-") > getSize().x * 0.9)
        text.setFontSize(--ftsz);

        x = (getSize().x - text.textWidth("-Press Z for Menu-"))/2;
        y = (getSize().y)/2;

        if (x < 1)
            x = 1;
        if (y < 1)
            y = 1;

        text.draw("-Press Z for Menu-", x, y-200);
    }
    else if (PauseVideo && !gameOver)
    {
        text.setScreenSize(getSize().x, getSize().y);
        int ftsz = 200;
        if (arenaNum == 0)
            text.setColor(1, 0, 0, 1);
        else
            text.setColor(1, 1, 1, 1);
        text.setFontSize(ftsz);

        while (text.textWidth("-Game Paused-") > getSize().x * 0.9)
        text.setFontSize(--ftsz);

        int x = (getSize().x - text.textWidth("-Game Paused-"))/2;
        int y = (getSize().y)/2;

        if (x < 1)
            x = 1;
        if (y < 1)
            y = 1;

        text.draw("-Game Paused-", x, y);

        while (text.textWidth("-Press Z for Menu-") > getSize().x * 0.9)
        text.setFontSize(--ftsz);

        x = (getSize().x - text.textWidth("-Press Z for Menu-"))/2;
        y = (getSize().y)/2;

        if (x < 1)
            x = 1;
        if (y < 1)
            y = 1;

        text.draw("-Press Z for Menu-", x, y-200);
    }
}


/**
\brief Loads Selected Fighter into by default T-Pose, but then applied rotations to this form to alter it

\param fiNum --- Number of the fighter that is being loaded in

\param view --- Camera Matrix

\param eye --- Camera Matrix

*/
void GraphicsEngine::loadFighter(int fiNum, glm::mat4 view, glm::vec3 eye, bool menuMode)
{
    for (int i = 0; i < 12; i++)
    {
        fighterOne[i+addNum[fiNum]].setEye(eye);
        fighterOne[i+addNum[fiNum]].setProjectionMatrix(projection);
        fighterOne[i+addNum[fiNum]].setViewMatrix(view);
    }

    ModelMatrix[fiNum] = glm::mat4(1.0);
    if (!menuMode){
        ModelMatrix[fiNum] = glm::translate(ModelMatrix[fiNum], glm::vec3(fighterClass[fiNum].fighterX[fiNum], fighterClass[fiNum].fighterY[fiNum]+0.7, 0));
    }
    else{
        ModelMatrix[fiNum] = glm::translate(ModelMatrix[fiNum], glm::vec3(fighterClass[fiNum].fighterDir[fiNum]*-14, fighterClass[fiNum].fighterY[fiNum]+0.7, -60));
    }
    ModelMatrix[fiNum] = glm::scale(ModelMatrix[fiNum], glm::vec3(7.5, 7.5, 7.5));
    ModelMatrix[fiNum] = glm::rotate(ModelMatrix[fiNum], 90*degf, glm::vec3(1, 0, 0));
    ModelMatrix[fiNum] = glm::rotate(ModelMatrix[fiNum], 180*degf, glm::vec3(0, 0, 1));


    leftArmMat[fiNum] = glm::mat4(1.0);
    headMat[fiNum] = glm::mat4(1.0);
    leftLegMat[fiNum] = glm::mat4(1.0);
    rightLegMat[fiNum] = glm::mat4(1.0);


    for (int i = 0; i < 12; i++)
    {
        if (i == 0)
        {
            ModelMatrix[fiNum] = glm::rotate(ModelMatrix[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][2] * degf, glm::vec3(0, 0, 1));
            ModelMatrix[fiNum] = glm::rotate(ModelMatrix[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][1] * degf, glm::vec3(0, 1, 0));
            ModelMatrix[fiNum] = glm::rotate(ModelMatrix[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][0] * degf, glm::vec3(1, 0, 0));
            ModelMatrix[fiNum] = glm::scale(ModelMatrix[fiNum], glm::vec3(0.5, 0.5, 0.5));

            glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMatrix[fiNum]));

            fighterOne[i+addNum[fiNum]].setModelMatrix(ModelMatrix[fiNum]);
            fighterOne[i+addNum[fiNum]].draw();

            ModelMatrix[fiNum] = glm::scale(ModelMatrix[fiNum], glm::vec3(1/0.5, 1/0.5, 1/0.5));
            //Set the other Matrices as children of the torso
            leftArmMat[fiNum] = ModelMatrix[fiNum];
            headMat[fiNum] = ModelMatrix[fiNum];
            leftLegMat[fiNum] = ModelMatrix[fiNum];
            rightLegMat[fiNum] = ModelMatrix[fiNum];
            ModelMatrix[fiNum] = glm::translate(ModelMatrix[fiNum], glm::vec3(-fighterClass[fiNum].scales[i], 0.025 , -0.08));
            leftArmMat[fiNum] = glm::translate(leftArmMat[fiNum], glm::vec3(fighterClass[fiNum].scales[i], 0.025 , -0.08));
            headMat[fiNum] = glm::translate(headMat[fiNum], glm::vec3(0.01, 0 , -0.32));
            rightLegMat[fiNum] = glm::translate(rightLegMat[fiNum], glm::vec3(-0.08, -0.01 , 0.55));
            //rightLegMat = glm::translate(rightLegMat, glm::vec3(-0.08 + ((rotations[i+8][0])/500), -0.01+((rotations[i+8][0])/500) , 0.55));
            leftLegMat[fiNum] = glm::translate(leftLegMat[fiNum], glm::vec3(0.13, -0.01 , 0.63));
        }
        else if (i == 1)
        {
            headMat[fiNum] = glm::rotate(headMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][2] * degf, glm::vec3(0, 0, 1));
            headMat[fiNum] = glm::rotate(headMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][1] * degf, glm::vec3(0, 1, 0));
            headMat[fiNum] = glm::rotate(headMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][0] * degf, glm::vec3(1, 0, 0));

            headMat[fiNum] = glm::scale(headMat[fiNum], glm::vec3(0.5, 0.5, 0.5));

            glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(headMat[fiNum]));

            fighterOne[i+addNum[fiNum]].setModelMatrix(headMat[fiNum]);
            fighterOne[i+addNum[fiNum]].draw();
        }
        else if (i >= 2 && i <= 4)
        {
            //Rotate Arms
            ModelMatrix[fiNum] = glm::rotate(ModelMatrix[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][2] * degf, glm::vec3(0, 0, 1));
            ModelMatrix[fiNum] = glm::rotate(ModelMatrix[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][1] * degf, glm::vec3(0, 1, 0));
            ModelMatrix[fiNum] = glm::rotate(ModelMatrix[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][0] * degf, glm::vec3(1, 0, 0));
            // Scale
            ModelMatrix[fiNum] = glm::scale(ModelMatrix[fiNum], glm::vec3(0.5, 0.5, 0.5));

            // Load transformation.
            glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMatrix[fiNum]));

            // Draw
            fighterOne[i+addNum[fiNum]].setModelMatrix(ModelMatrix[fiNum]);
            fighterOne[i+addNum[fiNum]].draw();

            // Scale back
            ModelMatrix[fiNum] = glm::scale(ModelMatrix[fiNum], glm::vec3(1/0.5, 1/0.5, 1/0.5));
            //Translate to the end of the arm to start the next arm.
            ModelMatrix[fiNum] = glm::translate(ModelMatrix[fiNum], glm::vec3(-fighterClass[fiNum].scales[i-1], 0 , 0.04));
        }
        else if (i >=5 && i <= 7)
        {
            //Left Arm
            leftArmMat[fiNum] = glm::rotate(leftArmMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][2] * degf, glm::vec3(0, 0, 1));
            leftArmMat[fiNum] = glm::rotate(leftArmMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][1] * degf, glm::vec3(0, 1, 0));
            leftArmMat[fiNum] = glm::rotate(leftArmMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][0] * degf, glm::vec3(1, 0, 0));

            leftArmMat[fiNum] = glm::scale(leftArmMat[fiNum], glm::vec3(0.5, 0.5, 0.5));

            glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(leftArmMat[fiNum]));

            fighterOne[i+addNum[fiNum]].setModelMatrix(leftArmMat[fiNum]);
            fighterOne[i+addNum[fiNum]].draw();

            leftArmMat[fiNum] = glm::scale(leftArmMat[fiNum], glm::vec3(1/0.5, 1/0.5, 1/0.5));

            leftArmMat[fiNum] = glm::translate(leftArmMat[fiNum], glm::vec3(fighterClass[fiNum].scales[i-4], 0 , 0.05));
        }
        else if (i >= 8 && i <= 9)
        {
            //Right Leg
            rightLegMat[fiNum] = glm::rotate(rightLegMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][2] * degf, glm::vec3(0, 0, 1));
            rightLegMat[fiNum] = glm::rotate(rightLegMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][1] * degf, glm::vec3(0, 1, 0));
            rightLegMat[fiNum] = glm::rotate(rightLegMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][0] * degf, glm::vec3(1, 0, 0));

            rightLegMat[fiNum] = glm::scale(rightLegMat[fiNum], glm::vec3(0.5, 0.5, 0.5));

            glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(rightLegMat[fiNum]));

            fighterOne[i+addNum[fiNum]].setModelMatrix(rightLegMat[fiNum]);
            fighterOne[i+addNum[fiNum]].draw();

            rightLegMat[fiNum] = glm::scale(rightLegMat[fiNum], glm::vec3(1/0.5, 1/0.5, 1/0.5));

            //rightLegMat = glm::translate(rightLegMat, glm::vec3(-0.09, 0.027+((rotations[i][0])/400), 0.57));
            rightLegMat[fiNum] = glm::translate(rightLegMat[fiNum], glm::vec3(-0.09, 0.027 , 0.57));
        }
        else if (i >= 10 && i <= 11)
        {
            //Left Leg
            leftLegMat[fiNum] = glm::rotate(leftLegMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][2] * degf, glm::vec3(0, 0, 1));
            leftLegMat[fiNum] = glm::rotate(leftLegMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][1] * degf, glm::vec3(0, 1, 0));
            leftLegMat[fiNum] = glm::rotate(leftLegMat[fiNum], fighterClass[fiNum].rotations[i+addNum[fiNum]][0] * degf, glm::vec3(1, 0, 0));

            leftLegMat[fiNum] = glm::scale(leftLegMat[fiNum], glm::vec3(0.5, 0.5, 0.5));

            glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(leftLegMat[fiNum]));

            fighterOne[i+addNum[fiNum]].setModelMatrix(leftLegMat[fiNum]);
            fighterOne[i+addNum[fiNum]].draw();

            leftLegMat[fiNum] = glm::scale(leftLegMat[fiNum], glm::vec3(1/0.5, 1/0.5, 1/0.5));

            leftLegMat[fiNum] = glm::translate(leftLegMat[fiNum], glm::vec3(0.08, 0.027 , 0.50));
        }
    }
}

/**
\brief Graphs to screen the hand fighter

*/
void GraphicsEngine::loadHandFighter(int playerNum, glm::mat4 view, glm::vec3 eye, bool menuMode)
{
    glm::mat4 atm(1.0);
    if(!menuMode){
        atm = glm::translate(atm, glm::vec3(fighterClass[playerNum].fighterX[playerNum], 3, 0));
    }
    else if(menuMode){
        atm = glm::translate(atm, glm::vec3(fighterClass[playerNum].fighterDir[playerNum]*-14, 3, -60));
    }

    if (playerNum == 1)
        atm = glm::scale(atm, glm::vec3(-0.009, 0.009, 0.009));
    else
        atm = glm::scale(atm, glm::vec3(0.009, 0.009, 0.009));


    //atm = glm::scale(glm::mat4(1.0), glm::vec3(0.005, 0.005, 0.005));
    //atm = glm::translate(atm, glm::vec3(fighterClass[playerNum].fighterX[playerNum], 3, -30));
    //atm = glm::rotate(atm, 90*degf, glm::vec3(1, 0, 0));
    //atm = glm::scale(glm::mat4(1.0), glm::vec3(0.03, 0.03, 0.03));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*atm));

    //pMesh->Render();

    glm::mat4 WVP = projection * view * atm;

    pSkinningTech->SetWVP(WVP);

    pSkinningTech->SetMaterial(gMesh[playerNum]->GetMaterial());

    vector<Matrix4f> Transforms;
    //printf("%f\n", AnimationTimeSec);

    //Check to reset animation clock, will need to make these into arrays of two for both fighters
    if (fighterClass[playerNum].getFighterAnimation() != curAnimIndex[playerNum]){
        //reset AnimationTimeSec to 0
        StartTimeMillis[playerNum] = GetCurrentTimeMillis();
        AnimationTimeSec[playerNum] = ((float)(GetCurrentTimeMillis() - StartTimeMillis[playerNum])) / 1000.0f;
        //setAnimationIndex
        gMesh[playerNum]->setAnimationIndex(fighterClass[playerNum].getFighterAnimation());
        //set current animation
        curAnimIndex[playerNum] = fighterClass[playerNum].getFighterAnimation();

    }
    //printf("PlayerNum=%d, curAnimIndex=%d\n", playerNum, curAnimIndex[playerNum]);
    gMesh[playerNum]->GetBoneTransforms(AnimationTimeSec[playerNum], Transforms);


    for(uint i = 0; i < Transforms.size(); i++){
        //sMesh->SetBoneTransform(i, Transforms[i]);
        pSkinningTech->SetBoneTransform(i, Transforms[i]);
    }

    gMesh[playerNum]->Render();
}

/**
\brief Graphs to screen Monday Memo fighter

*/
void GraphicsEngine::loadMondayFighter(int playerNum, glm::mat4 view, glm::vec3 eye, bool menuMode)
{
    glm::mat4 atm(1.0);
    if(!menuMode){
        atm = glm::translate(atm, glm::vec3(fighterClass[playerNum].fighterX[playerNum], fighterClass[playerNum].fighterHeight[playerNum], 0));
    }
    else if(menuMode){
        atm = glm::translate(atm, glm::vec3(fighterClass[playerNum].fighterDir[playerNum]*-14, 0, -60));
    }

    if (playerNum == 1)
        atm = glm::scale(atm, glm::vec3(-0.0008, 0.0008, 0.0008));
    else
        atm = glm::scale(atm, glm::vec3(0.0008, 0.0008, 0.0008));

    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*atm));

    glm::mat4 WVP = projection * view * atm;

    pSkinningTech->SetWVP(WVP);

    pSkinningTech->SetMaterial(mMesh[playerNum]->GetMaterial());

    vector<Matrix4f> Transforms;
    //printf("%f\n", AnimationTimeSec);

    //Check to reset animation clock, will need to make these into arrays of two for both fighters
    if (fighterClass[playerNum].getFighterAnimation() != curAnimIndex[playerNum]){
        //reset AnimationTimeSec to 0
        StartTimeMillis[playerNum] = GetCurrentTimeMillis();
        AnimationTimeSec[playerNum] = ((float)(GetCurrentTimeMillis() - StartTimeMillis[playerNum])) / 1000.0f;
        //setAnimationIndex
        mMesh[playerNum]->setAnimationIndex(fighterClass[playerNum].getFighterAnimation());
        //set current animation
        curAnimIndex[playerNum] = fighterClass[playerNum].getFighterAnimation();
    }

    mMesh[playerNum]->GetBoneTransforms(AnimationTimeSec[playerNum], Transforms);


    for(uint i = 0; i < Transforms.size(); i++){
        //sMesh->SetBoneTransform(i, Transforms[i]);
        pSkinningTech->SetBoneTransform(i, Transforms[i]);
    }

    mMesh[playerNum]->Render();
}



/**
\brief Restarts the game if and only if the game is already over

*/
void GraphicsEngine::restartGame()
{
    //if (gameOver)
    //{
    for (int i = 0; i < 2; i++)
    {
        fighterClass[i].fighterHP[rivalNum[i]] = 75;
        fighterClass[i].readyStance(i);
        fighterClass[i].fighterX[i] = -fighterClass[i].fighterDir[i]*18;
        fighterClass[i].fighterHeight[i] = 0;
    }
    gameOver = false;
    PauseVideo = false;
    //}
}

void GraphicsEngine::displayFighter(int fighterNum, int playerNum, glm::mat4 view, glm::vec3 eye)
{
    //fighterClass[playerNum].setFighterID(fighterNum, curFIndex[rivalNum[playerNum]]);
    switch (fighterNum)
    {
        case 0:
            loadHandFighter(playerNum, view, eye, true);
            break;
        case 1:
            loadFighter(playerNum, view, eye, true);
            break;
        case 2:
            break;
        case 3:
            loadMondayFighter(playerNum, view, eye, true);
            break;
        default:
            break;
    }
}

void GraphicsEngine::loadSelectedFighter(int fighterNum, int playerNum, glm::mat4 view, glm::vec3 eye)
{
    //fighterClass[playerNum].setFighterID(fighterNum);
    switch (fighterNum)
    {
        case 0:
            loadHandFighter(playerNum, view, eye, false);
            break;
        case 1:
            loadFighter(playerNum, view, eye, false);
            break;
        case 2:
            break;
        case 3:
            loadMondayFighter(playerNum, view, eye, false);
            break;
        default:
            break;
    }
}

void GraphicsEngine::setCurFIndex(int playerNum, int fighterIndex)
{
    //Don't want fighter swapping mid fight, only in arena  which is menu
    if (arenaNum == 3){
        curFIndex[playerNum] = fighterIndex;
    }
}

bool GraphicsEngine::getPause()
{
    return PauseVideo;
}

void GraphicsEngine::runWeirdProgram(glm::mat4 view)
{
    changeMode();
    glUseProgram(WeirdProgram);

    glUniform1i(statLoc, cameraPos);
    static sf::Clock shaderClock;
    static float time = 0;
    time += 0.005 * shaderClock.getElapsedTime().asMilliseconds();
    shaderClock.restart();

    glUniform1f(timeGG, time);

    //The flopper for Status Number 6
    static float flopper = 1;
    static float greenFlop = 0;
    static bool goPositive = true;
    if (flopper >= 1)
        goPositive = false;
    else if (flopper <= 0)
        goPositive = true;

    if (goPositive)
    {
        flopper += 0.005;
        greenFlop -= 0.005;
    }

    else
    {
        flopper -= 0.01;
        greenFlop += 0.01;
    }
    //std::cout<<flopper << "____" << greenFlop <<std::endl;
    glUniform1f(flopLoc, flopper);
    glUniform1f(gFlopLoc, greenFlop);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection*view));
    CMSphere.draw();



    glUseProgram(program);
    changeMode();
}

void GraphicsEngine::setWeirdProjection()
{
    glm::mat4 ProjectionMatrix = projection;
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
}
