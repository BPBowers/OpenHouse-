#include "UI.h"

/**
\file UI.cpp
\brief User interface processor for the program.

\author    Don Spickler Modified by Brian Bowers
\version   1.2
\date      5/22/2023

*/

/**
\brief Constructor

\param graph --- Pointer to the GraphicsEngine that this interface processor is attached.

Simply stores the pointer of the GraphicsEngine.

*/

UI::UI(GraphicsEngine* graph)
{
    ge = graph;
    mouseDown = false;
}

/**
\brief Destructor

No implementation needed at this point.

*/

UI::~UI() {}

/**
\brief The method handles the SFML event processing and calls the keyboard state processor
method.

This method processes all events in the current SFML event queue and calls the
corresponding processing method.  At the end it calls the keyboard state processor
method, outside the event loop.

*/

void UI::processEvents()
{
    // Process user events
    sf::Event event;
    while (ge->pollEvent(event))
    {
        // Close Window or Escape Key Pressed: exit
        if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            ge->close();

        // Key is pressed.
        if (event.type == sf::Event::KeyPressed)
            keyPressed(event.key);

        // Window is resized.
        if (event.type == sf::Event::Resized)
            ge->resize();

        if (event.type == sf::Event::MouseMoved)
            processMouseMoved(event.mouseMove);

        if (event.type == sf::Event::MouseButtonPressed)
            processMouseButtonPressed(event.mouseButton);

        if (event.type == sf::Event::MouseButtonReleased)
            processMouseButtonReleased(event.mouseButton);

        if (event.type == sf::Event::JoystickButtonPressed)
            processJoystickButtonPressed(event.joystickButton);

        if (event.type == sf::Event::JoystickButtonReleased)
            processJoystickButtonReleased(event.joystickButton);

        if (event.type == sf::Event::JoystickConnected)
            processJoystickConnect(event.joystickConnect);

        if (event.type == sf::Event::JoystickDisconnected)
            processJoystickDisconnect(event.joystickConnect);

        if (event.type == sf::Event::JoystickMoved)
        {
            processJoystickMoved(event.joystickMove);
        }
        else
        {
            keyboardStateProcessing();
        }

    }

    // Process the state of the keyboard outside of event firing,
}

/**
\brief The method simply outputs the joystick, axis, axis name, and position on
a JoystickMoveEvent.

\param joystickMoved --- The SFML joystick move event structure.

*/

void UI::processJoystickMoved(sf::Event::JoystickMoveEvent joystickMoved)
{
    if (!(ge->getArenaNum() == 3)){
        if (joystickMoved.axis == sf::Joystick::PovX)
        {
            if (joystickMoved.position == 100)
            {
                ge->fighterClass[joystickMoved.joystickId].fighterMoveX(joystickMoved.joystickId, 1, 0);
            }
            else if (joystickMoved.position == -100)
            {
                ge->fighterClass[joystickMoved.joystickId].fighterMoveX(joystickMoved.joystickId, 0, 1);
            }
            else if (joystickMoved.position == 0)
            {
            ge->fighterClass[joystickMoved.joystickId].fighterMoveX(joystickMoved.joystickId, 0, 0);
            }
        }
    }
    /*
    if (joystickMoved.axis == sf::Joystick::PovX && joystickMoved.joystickId == 0)
    {
        if (joystickMoved.position == 100)
        {
            ge->fighterMoveX(0, 1, 0);
        }
        else if (joystickMoved.position == -100)
        {
            ge->fighterMoveX(0, 0, 1);
        }
        else if (joystickMoved.position == 0)
        {
            ge->fighterMoveX(0, 0, 0);
        }
    }

    if (joystickMoved.axis == sf::Joystick::PovX && joystickMoved.joystickId == 1)
    {
        if (joystickMoved.position == -100)
            ge->fighterMoveX(1, 1, 0);
        else if (joystickMoved.position == 100)
            ge->fighterMoveX(1, 0, 1);
        else if (joystickMoved.position == 0)
            ge->fighterMoveX(1, 0, 0);
    }
*/
}

/**
\brief The method simply outputs a joystick connected message and the joystick
that was connected.

\param joystickConnectEvent --- The SFML joystick connected event structure.

*/

void UI::processJoystickConnect(sf::Event::JoystickConnectEvent joystickConnectEvent)
{
    std::cout << "Joystick: " << joystickConnectEvent.joystickId << "   Connected" << std::endl;
}

/**
\brief The method simply outputs a joystick disconnected message and the joystick
that was disconnected.

\param joystickConnectEvent --- The SFML joystick connected event structure.

*/

void UI::processJoystickDisconnect(sf::Event::JoystickConnectEvent joystickConnectEvent)
{
    std::cout << "Joystick: " << joystickConnectEvent.joystickId << "   Disconnected" << std::endl;
}

/**
\brief The method simply outputs the joystick and button when a button is pressed.

\param joystickButtonEvent --- The SFML joystick button event structure.

*/

void UI::processJoystickButtonPressed(sf::Event::JoystickButtonEvent joystickButtonEvent)
{
    //std::cout << "Joystick: " << joystickButtonEvent.joystickId << "   Button Pressed: " << joystickButtonEvent.button << std::endl;
    if (!(ge->getArenaNum() == 3)){
        if (joystickButtonEvent.button == 2 && joystickButtonEvent.joystickId == 0)
            ge->fighterClass[joystickButtonEvent.joystickId].punchNow(0);

        if (joystickButtonEvent.button == 2 && joystickButtonEvent.joystickId == 1)
            ge->fighterClass[joystickButtonEvent.joystickId].punchNow(1);

        if(joystickButtonEvent.button == 3 && joystickButtonEvent.joystickId == 0)
            ge->fighterClass[joystickButtonEvent.joystickId].kickNow(0);

        if(joystickButtonEvent.button == 3 && joystickButtonEvent.joystickId == 1)
            ge->fighterClass[joystickButtonEvent.joystickId].kickNow(1);

        if(joystickButtonEvent.button == 1 && joystickButtonEvent.joystickId == 0)
            ge->fighterClass[joystickButtonEvent.joystickId].tornadoNow(0);

        if(joystickButtonEvent.button == 1 && joystickButtonEvent.joystickId == 1)
            ge->fighterClass[joystickButtonEvent.joystickId].tornadoNow(1);

        if (joystickButtonEvent.button == 6)
            ge->restartGame();

        if (joystickButtonEvent.button == 7)
            ge->togglePause();
    }

}

/**
\brief The method simply outputs the joystick and button when a button is released.

\param joystickButtonEvent --- The SFML joystick button event structure.

*/

void UI::processJoystickButtonReleased(sf::Event::JoystickButtonEvent joystickButtonEvent)
{
    //std::cout << "Joystick: " << joystickButtonEvent.joystickId << "   Button Released: " << joystickButtonEvent.button << std::endl;
}


/**
\brief The method updates the theta and psi values of the spherical camera
on a click and drag.  If the control key is down the vertical movement will
alter the radius of the camera.

\param mouseMoveEvent --- The SFML mouse move event structure.

*/

void UI::processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent)
{
    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    if (ge->isSphericalCameraOn() && mouseDown)
    {
        if (ctrldown)
        {
            ge->getSphericalCamera()->addR((LastPosition.y - mouseMoveEvent.y)*0.25);
        }
        else
        {
            ge->getSphericalCamera()->addTheta((mouseMoveEvent.x - LastPosition.x)*degf*10);
            ge->getSphericalCamera()->addPsi((mouseMoveEvent.y - LastPosition.y)*degf*10);
        }

        LastPosition.x = mouseMoveEvent.x;
        LastPosition.y = mouseMoveEvent.y;
    }
}

/**
\brief On a left mouse click it will track the mouse down and tag the current position
of the mouse as the last position the mouse was at.

\param mouseButtonEvent --- The SFML mouse button event structure.

*/

void UI::processMouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        mouseDown = true;
        LastPosition.x = mouseButtonEvent.x;
        LastPosition.y = mouseButtonEvent.y;
    }
}

/**
\brief If the left mouse button is released this method will track the release and
exit any drag movement.

\param mouseButtonEvent --- The SFML mouse button event structure.

*/

void UI::processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        mouseDown = false;
    }
}

/**
\brief The function handles the keyboard input events from the user.

\param keyevent --- The SFML key code for the key pressed.

\remark

- M: Toggles between fill mode and line mode to draw the triangles.
- F10: Saves a screen shot of the graphics window to a png file.
- P: Toggles the Game Pause
- R: Restarts the Game if it is over

Fighter One Keyboard Controls

- C: Fighter One Punch and Double Punch
- S: Fighter One Kick attack
- Q: Fighter One Tornado Attack

Fighter Two Keyboard Controls

- B: Fighter One Punch and Double Punch
- K: Fighter One Kick attack
- O: Fighter One Tornado Attack

*/

void UI::keyPressed(sf::Event::KeyEvent keyevent)
{
    int key = keyevent.code;

    switch (key)
    {
    case sf::Keyboard::F10:
        ge->screenshot();
        break;

    case sf::Keyboard::P:
        if (!(ge->getArenaNum() == 3))
            ge->togglePause();
        break;

    case sf::Keyboard::M:
        ge->changeMode();
        break;

    case sf::Keyboard::R:
        if (!(ge->getArenaNum() == 3))
            ge->restartGame();
        break;
    case sf::Keyboard::B:
        if (!(ge->getArenaNum() == 3))
            ge->fighterClass[1].punchNow(1);
        break;
    case sf::Keyboard::K:
        if (!(ge->getArenaNum() == 3))
            ge->fighterClass[1].kickNow(1);
        break;
    case sf::Keyboard::O:
        if (!(ge->getArenaNum() == 3))
            ge->fighterClass[1].tornadoNow(1);
        break;
    case sf::Keyboard::C:
        if (!(ge->getArenaNum() == 3))
            ge->fighterClass[0].punchNow(0);
        break;
    case sf::Keyboard::S:
        if (!(ge->getArenaNum() == 3))
            ge->fighterClass[0].kickNow(0);
        break;
    case sf::Keyboard::Q:
        if (!(ge->getArenaNum() == 3))
            ge->fighterClass[0].tornadoNow(0);
        break;
    case sf::Keyboard::Enter:
        if (ge->getArenaNum() == 3)
            ge->menuSelect();
        break;
    case sf::Keyboard::Right:
        if (ge->getArenaNum() == 3)
            ge->menuRightSlide();
        break;
    case sf::Keyboard::Left:
         if (ge->getArenaNum() == 3)
            ge->menuLeftSlide();
        break;
    case sf::Keyboard::BackSpace:
        if (ge->getArenaNum() == 3)
           ge->menuBackout();
        break;
    case sf::Keyboard::Z:
        if (ge->getPause() == true)
            ge->returnToMenu();
        break;
    case sf::Keyboard::Numpad1:
        if (ge->getArenaNum() == 3)
            ge->setCurFIndex(1, 0);
        break;
    case sf::Keyboard::Numpad2:
        if (ge->getArenaNum() == 3)
            ge->setCurFIndex(1, 1);
        break;
    case sf::Keyboard::Numpad3:
        if (ge->getArenaNum() == 3)
            ge->setCurFIndex(1, 3);
        break;
    case sf::Keyboard::Space:
        if (!(ge->getArenaNum() == 3))
            ge->fighterClass[0].fighterJump(0);
        break;
    default:
        break;
    }
}

/**
\brief Calls the respective method for key processing depending on
which camera, spherical or yaw-pitch-roll, is currently selected.
If Alt or X is pressed the arrow control goes to the light and if
not the arrow control goes to the camera.

*/

void UI::keyboardStateProcessing()
{
    bool rightDownTwo = sf::Keyboard::isKeyPressed(sf::Keyboard::L);
    bool leftDownTwo = sf::Keyboard::isKeyPressed(sf::Keyboard::J);

    bool rightDownOne = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool leftDownOne = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    if (!(ge->getArenaNum() == 3)){
        if (rightDownTwo)
            ge->fighterClass[1].fighterMoveX(1, 0, 1);
        else if (leftDownTwo)
            ge->fighterClass[1].fighterMoveX(1, 1, 0);
        else if (!(leftDownTwo || rightDownTwo))
            ge->fighterClass[1].fighterMoveX(1, 0, 0);

        if (rightDownOne)
            ge->fighterClass[0].fighterMoveX(0, 1, 0);
        else if (leftDownOne)
            ge->fighterClass[0].fighterMoveX(0, 0, 1);
        else if (!(leftDownOne || rightDownOne))
            ge->fighterClass[0].fighterMoveX(0, 0, 0);
    }

}

/**
\brief The method processes the keyboard state if the spherical camera is the one currently
being used.

\remark

If no modifier keys are pressed:

- Left: Increases the camera's theta value.
- Right: Decreases the camera's theta value.
- Up: Increases the camera's psi value.
- Down: Decreases the camera's psi value.

If the control or Z key is down:

- Up: Decreases the camera's radius.
- Down: Increases the camera's radius.

*/

void UI::keyboardStateProcessingSphericalCamera()
{
    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    if (ctrldown || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getSphericalCamera()->addR(-0.1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getSphericalCamera()->addR(0.1);
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ge->getSphericalCamera()->addTheta(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ge->getSphericalCamera()->addTheta(-1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getSphericalCamera()->addPsi(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getSphericalCamera()->addPsi(-1);
    }
}


/**
\brief The method processes the keyboard state if the light is currently
being used.

*/

void UI::keyboardStateProcessingLight()
{}

/**
\brief The method processes the keyboard state if the yaw-pitch-roll camera is the
one currently being used.

\remark

*/

void UI::keyboardStateProcessingYPRCamera()
{}
