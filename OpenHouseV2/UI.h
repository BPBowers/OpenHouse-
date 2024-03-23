#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "GraphicsEngine.h"
#include <SFML/Audio.hpp>

/**
\file UI.h

\brief Header file for UI.cpp

\author    Don Spickler Modified by Brian Bowers
\version   1.2
\date      5/22/2023

*/

/**
\class UI

\brief The UI class handles the user interface event processing for the program.  It
contains a pointer to the graphics engine as the communication link to the graphics
and scene objects.

*/

class UI
{
private:
    GraphicsEngine* ge;  ///< Pointer to the GraphicsEngine that this processor is attached.
    bool mouseDown;      ///< Boolean tracking of the left mouse button.

    sf::Vector2i LastPosition;  ///< Last position of the mouse on the screen.

    void keyPressed(sf::Event::KeyEvent keyevent);
    void keyboardStateProcessing();
    void keyboardStateProcessingSphericalCamera();
    void keyboardStateProcessingYPRCamera();
    void keyboardStateProcessingLight();

    void processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent);
    void processMouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent);
    void processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent);

    //Joystick Functionality
    void processJoystickButtonPressed(sf::Event::JoystickButtonEvent joystickButtonEvent);
    void processJoystickButtonReleased(sf::Event::JoystickButtonEvent joystickButtonEvent);
    void processJoystickMoved(sf::Event::JoystickMoveEvent joystickMoved);

    void processJoystickConnect(sf::Event::JoystickConnectEvent joystickConnectEvent);
    void processJoystickDisconnect(sf::Event::JoystickConnectEvent joystickConnectEvent);
public:
    UI(GraphicsEngine* graph);
    ~UI();

    void processEvents();
};

#endif // UI_H_INCLUDED
