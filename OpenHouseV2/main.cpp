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

#include "GraphicsEngine.h"
#include "UI.h"

/**
\mainpage KnightFight

\tableofcontents

\section intro Introduction

Built Upon Dr. Spickler's Complete Wavefront obj file Loader.
Two fighters "duke it out" until only one has HitPoints remaining

\subsection options User Options

- Escape:  Ends the program.
- M: Toggles between fill mode and line mode to draw the triangles.
- F10: Saves a screen shot of the graphics window to a png file.
- P: Toggles the Game Pause
- R: Restarts the Game if it is over

Fighter One Keyboard Controls

- D: Move Fighter One Right
- A: Move Fighter One Left
- C: Fighter One Punch and Double Punch
- S: Fighter One Kick attack
- Q: Fighter One Tornado Attack

Fighter Two Keyboard Controls

- L: Move Fighter One Right
- J: Move Fighter One Left
- B: Fighter One Punch and Double Punch
- K: Fighter One Kick attack
- O: Fighter One Tornado Attack

Fighter Gamepad Controls (Gamepad 0 controls fighter 1, gamepad 1 controls fighter 2)

-DPAD Right: Move Fighter Right
-DPAD Left: Move Fighter Left
-Button 1 (aka "B"): Tornado Attack
-Button 2 (aka "X"): Punch and Double Punch
-Button 3 (aka "Y"): Kick attack
-Button 6 (aka "Left Select"): Resets Game if Gameover
-Button 7 (aka "Right Select"): Toggles Game Pause

\note Note that if only one "Gamepad" is connected, it controls fighter one
You need Two connected to fight as fighter two on gamepad. I only had
one gamepad with me while making the game so we will test during Presentation if two
work correctly


---

\subsection copyright Copyright

\author    Don Spickler
\version   1.2
\date      2/3/2018
\copyright 2018


---

\subsection license License

GNU Public License

This software is provided as-is, without warranty of ANY KIND, either expressed or implied,
including but not limited to the implied warranties of merchant ability and/or fitness for a
particular purpose. The authors shall NOT be held liable for ANY damage to you, your computer,
or to anyone or anything else, that may result from its use, or misuse.
All trademarks and other registered names contained in this package are the property
of their respective owners.  USE OF THIS SOFTWARE INDICATES THAT YOU AGREE TO THE ABOVE CONDITIONS.

*/

/**
\file main.cpp
\brief Main driver for the program.

This is the main program driver that sets up the graphics engine and links it to
the user interface processor.

\author    Don Spickler modified by Brian Bowers
\version   1.3
\date      5/22/2023

*/

/**
\brief The Main function, program entry point.

\return Standard EXIT_SUCCESS return on successful run.

The main function, responsible for initializing OpenGL and setting up
the SFML interface for OpenGL.

*/

int main()
{
    //  Program setup variables.
    std::string programTitle = "KnightFight:482 Final Project";
    GLint MinMajor = 3;
    GLint MinMinor = 3;
    GLint WindowWidth = 1500;
    GLint WindowHeight = 1000;
    bool DisplayInfo = true;

    //  Other variables
    GLint major;
    GLint minor;
    sf::Clock clock;
    sf::Time time = clock.restart();
    long framecount = 0;

    // Try core context of 10.10 (too advanced) and see what card will resort to.
    // For setting up OpenGL, GLEW, and check versions.
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "OpenGL Setup", sf::Style::Default,
                            sf::ContextSettings(24, 8, 4, 10, 10, sf::ContextSettings::Core));

    window.setVisible(false);

#ifndef __APPLE__
    // Turn on GLEW for Windows and Linux.
    glewExperimental = true;
    if (glewInit())
    {
        std::cerr << "\nUnable to initialize GLEW ... exiting. \n";
        exit(EXIT_FAILURE);
    }
#endif // __APPLE__

    //  Get major and minor OpenGL version from graphics card.
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    //  Check card version to minimum required version.
    bool versionOK = true;
    if (major < MinMajor)
        versionOK = false;
    else if (major == MinMajor && minor < MinMinor)
        versionOK = false;

    if (!versionOK)
    {
        std::cerr << "\nGraphics card OpenGL version is " << major << "." << minor << std::endl;
        std::cerr << "Program required OpenGL version is " << MinMajor << "." << MinMinor << std::endl;
        std::cerr << "Exiting\n";
        exit(EXIT_FAILURE);
    }

    //  Output OpenGL version to the console window.
    if (DisplayInfo)
    {
        std::cout << "\n";
        std::cout << "Version  = " << glGetString(GL_VERSION) << "\n";
        std::cout << "Major    = " << major << "\n";
        std::cout << "Minor    = " << minor << "\n";
        std::cout << "Vendor   = " << glGetString(GL_VENDOR) << "\n";
        std::cout << "Renderer = " << glGetString(GL_RENDERER) << "\n";
        std::cout << "\n";
    }
    //  Close setup window and context.
    window.close();

    //  Create graphics engine.
    GraphicsEngine ge(programTitle, major, minor, WindowWidth, WindowHeight);
    UI ui(&ge);
/*
    sf::Music music;

    // Open it from an audio file, 02_Going-To-Hell_The_Pretty_Reckless.wav
    if (!music.openFromFile("Sounds/02_Going-To-Hell_The_Pretty_Reckless.wav"))
        std::cout << "Background music not loaded." << std::endl;

    // Make it loop when it hits the end.
    music.setLoop(true);

    // Start the play.
    music.play();
//*/

    // Start the Game/GUI loop
    while (ge.isOpen())
    {
        // Call the display function to do the OpenGL rendering.
        ge.display();

        // Process any events.
        ui.processEvents();

        //  Increment frame counts
        framecount++;

        //  Get Elapsed Time
        float timesec = clock.getElapsedTime().asSeconds();
        char titlebar[1000];

        //  If another second has elapsed, display the FPS and total FPS.
        if (timesec > 1.0)
        {
            float fps = framecount / timesec;
            sprintf(titlebar, "%s     FPS: %.2f", programTitle.c_str(), fps);
            ge.setTitle(titlebar);
            time = clock.restart();
            framecount = 0;
        }
    }

    return EXIT_SUCCESS;
}
