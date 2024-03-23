#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <iostream>

#include "ProgramDefines.h"

/**
\file Box.h

\brief Header file for Box.cpp

\author    Don Spickler
\version   1.2
\date      Written: 1/11/2016  <BR> Revised: 1/12/2016

*/

/**
\class Box

\brief The Box class holds position and color data for a multicolored rectangle.  It also has
facilities for loading the data to the graphics card and invoking draw commands on the data.

*/

class Box
{
private:
    GLfloat width;        ///< Width of the box.
    GLfloat height;       ///< Height of the box.
    GLfloat centerX;      ///< X coordinate of the center of the box.
    GLfloat centerY;      ///< Y coordinate of the center of the box.
    GLfloat colors[12];   ///< Array holding the (r,g,b) color values for each of the 4 vertices, [r1, g1, b1, r2, g2, b2, ...]

    GLuint BoxVAO;        ///< Vertex Array Object ID.
    GLuint ArrayBuffer;   ///< Vertex and Color Information Array Buffer ID.
    GLuint BoxEBO;        ///< Indices Array Object ID.

    GLint vPosition;      ///< Array Buffer position index.
    GLint vColor;         ///< Array Buffer color index.
    GLfloat anchorA;      ///< Anchor for dragging the box on the Y axis
    GLfloat anchorB;      ///< Anchor for dragging the box on the X axis
    //Added for Program #2
    bool mouseIn;         ///< Boolean that determines if the mouse is hovering over the box
    bool select;          ///< Boolean value that determines if the box is selected or not
    void LoadDataToGraphicsCard();

public:
    Box(GLfloat cx = 0, GLfloat cy = 0, GLfloat w = 1, GLfloat h = 1);
    ~Box();

    void setWidth(GLfloat w);
    void setHeight(GLfloat h);
    void setCenter(GLfloat cx, GLfloat cy);
    void setVertexColor(GLint v, GLfloat r, GLfloat g,GLfloat b);
    void setAllColor(GLfloat c, GLfloat g);
    GLfloat getWidth();
    GLfloat getHeight();
    void getCenter(GLfloat* c);
    bool checkMouse(GLfloat mouseX, GLfloat mouseY);
    void setAnchorA(GLfloat curserY);
    void setAnchorB(GLfloat curserX);
    GLfloat getAnchorA();
    GLfloat getAnchorB();

    bool getSelect();
    void setSelect(bool setter);
    //void highlight();
    void draw();
};

#endif // BOX_H_INCLUDED
