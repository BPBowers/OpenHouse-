#ifndef FIGHTER_H_INCLUDED
#define FIGHTER_H_INCLUDED

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OPenGL/glu.h>
#else
    #include <GL/glew.h>
#endif // __APPLE__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

#include "ProgramDefines.h"

/***
\file Fighter.h

\brief Header file for Fighter.h

\author     Brian Bowers
\version    1.0
\date       8/11/2023

*/

/**
\class Fighter

\brief The fighter class is the base class for every fighter,
initial stance and connecting it to its attack and obj functions
in order to clean up the graphics engine

*/

class Fighter
{
private:
    int fighterNum; ///< aka playerNum, will probably change for clarity sake
    int fighterID[2];  ///< aka the ID of what fighter are you using (hand fighter, og fighter, ect.)
    int rivalNum;   ///< aka playerNum of opponent, used for easy array access

    //**********animArr Cheat Sheet*********
    //Pos0 = Stance, Pos1 = Walk, Pos2 = Stun, Pos3 = Punch, Pos4 = Kick, Pos5 = Special, Pos6 = Block, Pos7 = Jump
    //FighterID 0 = Hand Fighter
    //FighterID 1 = Left Player Character
    //FighterID 2 = Righter Player Character
    //FighterID 3 = Monday Memo
    //**************************************
    //*
    int animArr[4][10] = {3, 5, 1, 2, 0, 4, NULL, NULL, NULL, NULL,
                          0, 1, 2, 3, 4, 5, NULL, NULL, NULL, NULL,
                          NULL, NULL, 7, 4, 3, 6, NULL, NULL, NULL, NULL,
                          5, 8, 7, 4, 3, 6, NULL, NULL, NULL, NULL};
    /*/
    /*
    int animArr[4][10] = {3, 5, 1, 2, 0, 4, NULL, NULL, NULL, NULL,
                          0, 1, 2, 3, 4, 5, NULL, NULL, NULL, NULL,
                          NULL, NULL, 7, 4, 3, 6, NULL, NULL, NULL, NULL,
                          2, 8, 4, 5, 6, 9, NULL, NULL, NULL, NULL};
    */
    //Array of count values, [FighterID][CounterID]
    //PunchCounter, Tornado Counter, kick counter AKA g:
    int fightCounts[4][3] = {200, 0, -50,
                            50, -30, 22,
                            50, 30, 22,
                            225, 0, 0};
    float fightCloseFactor[4] = {12, 5.5, 5.5, 6};
    float centerDist = -1; //picks largest fightCloseFactor loaded

    //Distance applied to fighters to check if hit enemy
    //Punch, Kick, Special (aka tornado), bonus
    float fightRange[4][3] = {6, 6, 6,
                            8, 7, 8,
                            8, 7, 8,
                            3, 7, 4};
    //Same order as fightRange
    float fightDmg[4][4] = {2.0, 2.5, 1.0, 0.5,//Hand
                            1.0, 2.5, 0.5, 0.5,//Og fighter
                            1.0, 2.5, 0.5, 0.5,//Og Fighter
                            5.0, 2.5, 3.5, 1.5};//Monday Memo

    int fighterAnimation = animArr[0][0];

    int addNum[2] = {0, 12};

    glm::mat4 ModelMatrix[2];
    glm::mat4 leftArmMat[2];
    glm::mat4 headMat[2];
    glm::mat4 leftLegMat[2];
    glm::mat4 rightLegMat[2];

    bool punchStop[2] = {false, false};
    bool punch2Stop[2] = {false,false};
    //bool kick[2] = {false, false};
    bool kickStop[2]= {false, false};
    //bool tornado[2] = {false, false};
    bool tornadoStop[2] = {false, false};
    bool step[2] = {false, false};
    bool moveForwards[2] = {false, false};
    bool moveBackwards[2] = {false, false};
    bool jump[2] = {false, false};
    bool jumpStop[2] = {false, false};
    bool knee = false;



    //float stunCounter[2] = {15, 15};

    //Probably going to make a separate audio class
    sf::Sound hit; //hit.play
    sf::SoundBuffer hitBuffer;
    sf::Sound punc;
    sf::SoundBuffer punchBuffer;
    sf::Sound strike;
    sf::SoundBuffer strikeBuffer;

protected:

public:
    Fighter();
    ~Fighter();

    //Public vars (needs access in GE)
    float fighterX[2] = {-18, 18};
    float fighterY[2] = {9.5, 9.5};//Legacy from original stupid non animated fighter, never got the jump function to work though
    float fighterHeight[2] = {0, 0};//Hopefully will help keep track of the animated fighter's hight for jumping animations
    float fighterHP[2] = {75.0, 75.0};
    int fighterDir[2] = {1, -1};
    float rotations[24][3]; ///< Value of the rations made by a fighter's limbs
    float scales[12];    ///< Not intending on keeping
    bool attacking[2] = {false, false};
    bool punch[2] = {false, false};
    bool punch2[2] = {false, false};
    bool kick[2] = {false, false};
    bool tornado[2] = {false, false};
    bool fighterStun[2] = {false,false};
    bool fighterDown[2] = {false,false};
    float stunCounter[2] = {15, 15};

    //Fighter Functions
    void newFighter(int fiNum, int fiId);   ///< Sets what player is using this fighter, and what fighter they are using
    void oppTracker(int oppNum, int oppID, float oppX, float oppY, bool oppAttacking, bool oppKick, bool oppPunch, bool oppPunch2, bool oppTor, bool yourStun, bool oppStun);
    void fighterMoveX(int fighterNum, bool forw, bool backw);
    void fighterJump(int fighterNum);
    void checkIfHit(int fighterNum, int rivalX);
    void punchNow(int fighterNum);
    void kickNow(int fighterNum);
    void tornadoNow(int fighterNum);
    void readyStance(int fighterNum);
    void updateHP(int fighterNum, float value);
    void preformActions();
    void tposeStance(int fighterNum);
    //Animation Modifiers
    void addRotations(int limb, int axis, float val);
    void setRotations(int limb, int axis, float val);
    void addScales(int, float);

    //Swaps the animation for a specific mesh, will update more
    void swapAnimation(int index);
    int getFighterAnimation();
    void setFighterID(int fiID, int rivID);
};
#endif // FIGHTER_H_INCLUDED
