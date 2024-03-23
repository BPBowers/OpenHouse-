#include "Fighter.h"

/**
\file Fighter.cpp
\brief File that does the stuff defined in "Fighter.h"

\author     Brian Bowers
\version    1.0
\date       8/11/2023

*/

/**
\brief Constructor

*/

Fighter::Fighter()
{

}

/**
\brief Pretty much does normal constructor stuff but sets fighterNum to fiNum also

*/
void Fighter::newFighter(int fiNum, int fiId)
{
    fighterNum = fiNum;
    fighterID[fighterNum] = fiId;
    scales[0] = 0.4;
    scales[1] = 0.235;
    scales[2] = 0.21;
    scales[3] = 0.265;
    scales[4] = 0.232;
    scales[5] = 0.1;

    for (int i = 0; i < 24; i++)
    {
        for (int x = 0; x < 3; x++)
            rotations[i][x] = 0;
    }
    //Need to rewrite ready stance
    readyStance(0);
    readyStance(1);
}
/**
\brief De-constructor

*/
Fighter::~Fighter()
{

}

void Fighter::oppTracker(int oppNum, int oppId, float oppX, float oppY, bool oppAttacking, bool oppKick, bool oppPunch, bool oppPunch2, bool oppTor, bool yourStun, bool oppStun)
{
    fighterX[oppNum] = oppX;
    fighterY[oppNum] = oppY;
    fighterID[oppNum] = oppId;
    if (centerDist == -1) {
        if (fightCloseFactor[fighterID[rivalNum]] >= fightCloseFactor[fighterID[fighterNum]])
            centerDist = fightCloseFactor[fighterID[rivalNum]];
        else
            centerDist = fightCloseFactor[fighterID[fighterNum]];
    }
    attacking[oppNum] = oppAttacking;
    kick[oppNum] = oppKick;
    punch[oppNum] = oppPunch;
    punch2[oppNum] = oppPunch2;
    tornado[oppNum] = oppTor;
    fighterStun[fighterNum] = oppStun;
    if (fighterStun[oppNum])
        fighterStun[oppNum] = false;
    //fighterStun[oppNum] = yourStun;
    //std::cout<<fighterStun[fighterNum]<<std::endl;
    rivalNum = oppNum;
}
/**
\brief This function adds the given amount to the rotation in the given index. From Dr. Spickler

*/
void Fighter::addRotations(int limb, int axis, float val) {
    rotations[limb][axis] += val;
}
/**
\brief This function sets the given amount to the rotation in the given index.

*/
void Fighter::setRotations(int limb, int axis, float val)
{
    rotations[limb][axis] = val;
}
/**
\brief This function adds the given amount to the scales in the given index. From Dr. Spickler

*/
void Fighter::addScales(int i, float val) {
    scales[i] += val;
}
/**
\brief This function moves a fighter left and right

\param fighterNum --- Integer id value of the current fighter being moved on the x axis (0 or 1)

\param forw --- Setting forward to true or false

\param eye --- Setting backwards to true or False
*/
void Fighter::fighterMoveX(int fighterNum, bool forw, bool backw) {
    //std::cout << "function is called" << fighterNum << forw << backw << std::endl;
    if (forw)
    {
       moveForwards[fighterNum] = true;
       moveBackwards[fighterNum] = false;
    }
    else if (backw)
    {
        moveForwards[fighterNum] = false;
        moveBackwards[fighterNum] = true;
    }
    else if (!forw || !backw)
    {
        moveForwards[fighterNum] = false;
        moveBackwards[fighterNum] = false;
    }
}
/**
\brief This function makes the fighter jump but is currently not implemented due to dissatisifcation on the animation

\param fighterNum --- Integer Id value of the current fighter being altered
*/
void Fighter::fighterJump(int fighterNum) {
    //fighterY[fighterNum] = fighterY[fighterNum];
    //Under construction
    if (!(jump[fighterNum] || jumpStop[fighterNum])){
        jump[fighterNum] = true;
        //std::cout<<"Jumped???"<<std::endl;
    }
}
/**
\brief check if a specific fighter has landed a hit

\param fighterNum --- Integer id value of the fighter who is striking

\param rivalNum --- Integer id value of the fighter who is being checked if hit

*/
//bool GraphicsEngine::checkIfHit(int fighterNum, bool fighterDirection, float attackingLimbX, float rivalX)
void Fighter::checkIfHit(int fighterNum, int rivalNum)
{
    int xDist = abs(fighterX[fighterNum] - fighterX[rivalNum]);

    if (xDist <= centerDist+4 && knee && fighterID[fighterNum] == 3){
        if (!fighterStun[rivalNum])
            fighterStun[rivalNum] = true;
        fighterHP[rivalNum] -= fightDmg[fighterID[fighterNum]][3];
        hit.play();
    }
    else if (xDist <= centerDist+fightRange[fighterID[fighterNum]][1] && kick[fighterNum] && !knee)//originally 7
    {
        if (!fighterStun[rivalNum])
            fighterStun[rivalNum] = true;
        fighterHP[rivalNum] -= fightDmg[fighterID[fighterNum]][1];//2.5;
        hit.play();
        //std::cout << "Fighter" << fighterNum << " Kicked Fighter" << rivalNum << " at distance " << xDist << std::endl;
    }
    else if (xDist <= centerDist+fightRange[fighterID[fighterNum]][0] && punch[fighterNum])//Originally 8.1
    {
        if (!fighterStun[rivalNum])
            fighterStun[rivalNum] = true;
        fighterHP[rivalNum] -= fightDmg[fighterID[fighterNum]][0];//1.0;
        strike.play();
        //std::cout << "Fighter" << fighterNum  << " Punched Fighter" << rivalNum << " at distance " << xDist << std::endl;
    }
    else if (xDist <= centerDist+2 && punch2[fighterNum])//originally 8.1
    {
        if (!fighterStun[rivalNum])
            fighterStun[rivalNum] = true;
        fighterHP[rivalNum] -= fightDmg[fighterID[fighterNum]][3];//0.5;
        strike.play();
        //std::cout << "Fighter" << fighterNum  << " Double Punched Fighter" << rivalNum << " at distance " << xDist << std::endl;
    }
    else if (xDist <= centerDist+fightRange[fighterID[fighterNum]][2] && tornado[fighterNum])//Originally 8.1
    {
        if (!fighterStun[rivalNum])
            fighterStun[rivalNum] = true;
        fighterHP[rivalNum] -= fightDmg[fighterID[fighterNum]][2];//0.5;
        strike.play();
        //std::cout << "Fighter" << fighterNum  << " Tornado-Twisted Fighter" << rivalNum << " at distance " << xDist << std::endl;
    }
}
/**
\brief Updates a provided fighter's health (A.K.A. Hit Points)

\param fighterNum --- Integer id value of the fighter whose HP is being Updated

\param value --- Float value that is being applied to the existed HP for the fighter
*/
void Fighter::updateHP(int fighterNum, float value)
{
    fighterHP[fighterNum] += value;
}
/**
\brief Sets the "punch" and "double punch" animation to begin after checking if the fighter is stunned or already attacking

\param fighterNum --- Integer Id value of the fighter attempting to throw the punch
*/
void Fighter::punchNow(int fighterNum)
{
    if (!(attacking[fighterNum] || punch[fighterNum] || punchStop[fighterNum] || fighterStun[fighterNum] || stunCounter[fighterNum] < 15 || fighterDown[fighterNum]))
    {
        //Do animation thing
        punch[fighterNum] = true;
        attacking[fighterNum] = true;
    }
    else if (attacking[fighterNum] && (punch[fighterNum] || punchStop[fighterNum]) && !(fighterStun[fighterNum] || stunCounter[fighterNum] < 15 ||fighterDown[fighterNum] || punch2[fighterNum]) && (fighterID[fighterNum] == 1 || fighterID[fighterNum] == 2))
    {
        punch2[fighterNum] = true;
    }
}
/**
\brief Sets the "kick" animation to begin after checking if the fighter is stunned or already attacking

\param fighterNum --- Integer Id value of the fighter attempting to throw the punch
*/
void Fighter::kickNow(int fighterNum)
{
    if (!(attacking[fighterNum] || kick[fighterNum] || kickStop[fighterNum] || fighterStun[fighterNum] ||  stunCounter[fighterNum] < 15 || fighterDown[fighterNum]))
    {
        kick[fighterNum] = true;
        attacking[fighterNum] = true;
    }

    //Do the check hit in separate function?
}
/**
\brief Sets the "tornado attcack!" animation to begin after checking if the fighter is stunned or already attacking

\param fighterNum --- Integer Id value of the fighter attempting to throw the punch
*/
void Fighter::tornadoNow(int fighterNum)
{
    if (!(attacking[fighterNum] || tornado[fighterNum] || tornadoStop[fighterNum] || fighterStun[fighterNum] || stunCounter[fighterNum] < 15 || fighterDown[fighterNum]))
    {
        tornado[fighterNum] = true;
        attacking[fighterNum] = true;
        tposeStance(fighterNum);
    }
}
/**
\brief This function is where the meat of the "animations" occurs. It applies rotations to the fighter's limbs

\param fighterNum --- Integer Id value of the fighter attempting to throw the punch
*/
void Fighter::preformActions()
{

    static float jumpFactor = 10;
    //static float g[2] = {22, 22};
    static float g[2] = {fightCounts[fighterID[fighterNum]][2], fightCounts[fighterID[rivalNum]][2]};
    //static float punchCounter[2] = {50, 50};
    static float punchCounter[2] = {fightCounts[fighterID[fighterNum]][0], fightCounts[fighterID[rivalNum]][0]};
    //static float stunCounter[2] = {15, 15};
    //static float tornadoCounter[2] = {30, -30};
    static float tornadoCounter[2] = {fightCounts[fighterID[fighterNum]][1], fightCounts[fighterID[rivalNum]][1]};
    //If not fighter "0" then added 12 to rotations to access correct data
    int numAdder = 0;
    int rivalNum = 1;
    if (fighterNum == 1)
    {
        numAdder = 12;
        rivalNum = 0;
    }

    //printf("FighterID= %d\n", fighterID[fighterNum]);
//Stun
    if (fighterStun[fighterNum])
    {
        //Cancel any ongoing animation
        readyStance(fighterNum);
        fighterAnimation = animArr[fighterID[fighterNum]][2];//Loads Stun animation if bone animated
        g[fighterNum] = 22;
        punchCounter[fighterNum] = 50;
        tornadoCounter[fighterNum] = fighterDir[fighterNum]*30;
        stunCounter[fighterNum]--;
        fighterStun[fighterNum] = false;
    }
    else if (stunCounter[fighterNum] < 15)
    {
        if ((!(fighterX[fighterNum] < -28) && fighterNum == 0) || (!(fighterX[fighterNum] > 28) && fighterNum == 1))
            fighterX[fighterNum] += -fighterDir[fighterNum]*0.1;
        addRotations(0+numAdder, 0, -fighterDir[fighterNum]*0.3*stunCounter[fighterNum]);
        stunCounter[fighterNum]--;

        if (stunCounter[fighterNum] <= 0)
        {
            fighterStun[fighterNum] = false;
            stunCounter[fighterNum] = 15;
            setRotations(0+numAdder, 0, 0);
            fighterAnimation = animArr[fighterID[fighterNum]][0];
        }
    }
        //std::cout << moveForwards[0] << " " << moveBackwards[0] << std::endl;
//Walking
    if (moveForwards[fighterNum] && !kick[fighterNum] && !kickStop[fighterNum] && !punch2Stop[fighterNum])
    {
        //Chooses the greater of the distances for spacing
        //float centerDist = 0;
        if (fightCloseFactor[fighterID[rivalNum]] >= fightCloseFactor[fighterID[fighterNum]])
            centerDist = fightCloseFactor[fighterID[rivalNum]];
        else
            centerDist = fightCloseFactor[fighterID[fighterNum]];

        //printf("FighterNum: %d, C.D. %f, rival%f, curr%f \n", fighterNum, centerDist, fightCloseFactor[fighterID[rivalNum]], fightCloseFactor[fighterID[fighterNum]]);

        if ((!(fighterX[fighterNum] > fighterX[rivalNum]-centerDist) && fighterNum == 0) || (!(fighterX[fighterNum] < fighterX[rivalNum]+centerDist) && fighterNum == 1))
        {
            fighterX[fighterNum] += fighterDir[fighterNum]*0.3;
            fighterAnimation = animArr[fighterID[fighterNum]][1];//Sets animation to walking animation
        }
    }
    else if (moveBackwards[fighterNum] && !kick[fighterNum] && !kickStop[fighterNum] && !punch2Stop[fighterNum])
    {
        if ((!(fighterX[fighterNum] < -28) && fighterNum == 0) || (!(fighterX[fighterNum] > 28) && fighterNum == 1))
        {
            fighterX[fighterNum] += -fighterDir[fighterNum]*0.3;
            fighterAnimation = animArr[fighterID[fighterNum]][1];//Sets animation to walking animation
        }
        //Add Animation
    }
    else if (!(moveBackwards[fighterNum] || moveForwards[fighterNum]) && !(attacking[fighterNum] || stunCounter[fighterNum] < 15))
    {
        //Return to rest rotations
        fighterAnimation = animArr[fighterID[fighterNum]][0];
        //readyStance(fighterNum);
    }

//Jump

    if (jump[fighterNum])
    {
        //jumpFactor = 0.5*(jumpFactor);
        fighterHeight[fighterNum] += (0.5*(jumpFactor));
        if (fighterHeight[fighterNum] >= 14)
        {
            jump[fighterNum] = false;
            jumpStop[fighterNum] = true;
        }
    }

    if (jumpStop[fighterNum])
    {
        //jumpFactor *= 2;
        fighterHeight[fighterNum] -= (0.5*(jumpFactor));
        if (fighterHeight[fighterNum] <= 0)
        {
            fighterHeight[fighterNum] = 0;
            jumpStop[fighterNum] = false;
            jumpFactor = 5;
        }
    }


//Kick
    if (kick[fighterNum])
    {
        knee = false;
        if (fighterID[fighterNum] == 1 || fighterID[fighterNum] == 2){
            //For Original Fighters
            g[fighterNum] += 12;
        }
        else if (fighterID[fighterNum] == 0){
            //For hand fighter
            g[fighterNum] += 2.5;
        }
        else {
            //For Monday Memo
            g[fighterNum] += 1;
        }
        //printf("%f\n", g[fighterNum]);
        fighterAnimation = animArr[fighterID[fighterNum]][4];

        //Monday Memo has double Kick
        if (fighterID[fighterNum] == 3)
        {
            if (g[fighterNum] == 30){
                knee = true;
                checkIfHit(fighterNum, rivalNum);
            }
            else if (g[fighterNum] == 50)
                checkIfHit(fighterNum, rivalNum);
        }

        //End animation
        if (g[fighterNum] >= 100)
        {
            if (fighterID[fighterNum] != 3)
                checkIfHit(fighterNum, rivalNum);
            kick[fighterNum] = false;
            kickStop[fighterNum] = true;
        }
        if (fighterNum == 0)
            setRotations(8 + numAdder, 0, g[fighterNum]);
        else if (fighterNum == 1)
            setRotations(10+numAdder, 0, g[fighterNum]);
    }
    else if (kickStop[fighterNum])
    {
        g[fighterNum] -= 4;
        fighterAnimation = animArr[fighterID[fighterNum]][4];

        if (fighterNum == 0)
            setRotations(8 + numAdder, 0, g[fighterNum]);
        else if (fighterNum == 1)
            setRotations(10+numAdder, 0, g[fighterNum]);

        if (g[fighterNum] <= fightCounts[fighterID[fighterNum]][2])
        {
            kickStop[fighterNum] = false;
            attacking[fighterNum] = false;
            //g[fighterNum] = fighterDir[fighterNum]*22;
            g[fighterNum] = fightCounts[fighterID[fighterNum]][2];
            readyStance(fighterNum);
            fighterAnimation = animArr[fighterID[fighterNum]][0];
        }
    }

//Punch And Double Punch
    if (punch[fighterNum])
    {
        punchCounter[fighterNum] -= 5;
        fighterAnimation = animArr[fighterID[fighterNum]][3];

        if (punchCounter[fighterNum] <= 0)
            {
                checkIfHit(fighterNum, rivalNum);
                punch[fighterNum] = false;
                punchStop[fighterNum] = true;
                fighterAnimation = animArr[fighterID[fighterNum]][0];
            }

        if (fighterNum == 0)
        {
            setRotations(2 + numAdder, 2, 33); //Constant 33
            setRotations(2 + numAdder, 1, punchCounter[fighterNum]); //0 to 90
            setRotations(3 + numAdder, 1, -2*punchCounter[fighterNum]); //0 to 90
            setRotations(3 + numAdder, 2, 0);
        }
        else if (fighterNum == 1)
        {
            setRotations(5 + numAdder, 2, -33); //Constant 33
            setRotations(5 + numAdder, 1, -punchCounter[fighterNum]); //0 to 90
            setRotations(6 + numAdder, 1, 2*punchCounter[fighterNum]); //0 to 90
            setRotations(6 + numAdder, 2, 0);
        }

    }
    else if (punchStop[fighterNum])
    {
        punchCounter[fighterNum] += 5;
        if (fighterNum == 0)
        {
            setRotations(2 + numAdder, 1, punchCounter[fighterNum]); //0 to 90
            setRotations(3 + numAdder, 1, -2*punchCounter[fighterNum]); //0 to 90
        }
        else if (fighterNum == 1)
        {
            setRotations(5 + numAdder, 1, -punchCounter[fighterNum]); //0 to 90
            setRotations(6 + numAdder, 1, 2*punchCounter[fighterNum]); //0 to 90
        }

        if (punchCounter[fighterNum] >= fightCounts[fighterID[fighterNum]][0]) //Originally 50
        {
            punchStop[fighterNum] = false;
            if (!punch2[fighterNum])
                attacking[fighterNum] = false;
            //punchCounter[fighterNum] = 50;
            punchCounter[fighterNum] = fightCounts[fighterID[fighterNum]][0];
            if (fighterNum == 0)
            {
                setRotations(2 + numAdder, 1, 60);
                setRotations(3 + numAdder, 1, 0);
                setRotations(2 + numAdder, 2, 0);
                setRotations(3 + numAdder, 2, 120);
            }
            else if (fighterNum == 1)
            {
                setRotations(5 + numAdder, 1, -60);
                setRotations(6 + numAdder, 1, 0);
                setRotations(5 + numAdder, 2, 0);
                setRotations(6 + numAdder, 2, -120);
            }
        }
    }
    else if (punch2[fighterNum] && !(punch[fighterNum] || punchStop[fighterNum]))
    {
        //Subtract the counter (change to multiply by direction constant)
        punchCounter[fighterNum] -= 5;
        if (fighterNum == 0)
        {
            //Head turn forward
            setRotations(1 + numAdder, 2, -punchCounter[fighterNum]);// -50 to 0
            //Body Turn to Rival
            addRotations(0 + numAdder, 2, -10); //-33 to over -90
            //Left arm second punch
            setRotations(5 + numAdder, 2, 33); //0 to 90
            setRotations(5 + numAdder, 1, punchCounter[fighterNum]); //0 to 90
            setRotations(6 + numAdder, 1, -2*punchCounter[fighterNum]); //0 to 90
            setRotations(6 + numAdder, 2, 0);
        }
        else if (fighterNum == 1)
        {
            //Head turn forward
            setRotations(1 + numAdder, 2, punchCounter[fighterNum]);// -50 to 0
            //Body Turn to Rival
            addRotations(0 + numAdder, 2, 10); //-33 to over -90
            //Left arm second punch
            setRotations(2 + numAdder, 2, 33); //0 to 90
            setRotations(2 + numAdder, 1, -punchCounter[fighterNum]); //0 to 90
            setRotations(3 + numAdder, 1, 2*punchCounter[fighterNum]); //0 to 90
            setRotations(3 + numAdder, 2, 0);
        }


        if (punchCounter[fighterNum] <= 0)
        {
            checkIfHit(fighterNum, rivalNum);
            punch2[fighterNum] = false;
            punch2Stop[fighterNum] = true;
            punchCounter[fighterNum] = 0;
        }
    }
    else if (punch2Stop[fighterNum] && !(punch[fighterNum] || punchStop[fighterNum]))
    {
        //Add the counter (Change to multiply by direction constant later)
        punchCounter[fighterNum] += 5;

        if (fighterNum == 0)
        {
            //Head turn forward
            setRotations(1 + numAdder, 2, -punchCounter[fighterNum]);// -50 to 0
            //Body Turn to Rival
            addRotations(0 + numAdder, 2, 10); //-33 to over -90
            //Left arm second punch
            setRotations(5 + numAdder, 2, 33); //0 to 90
            setRotations(5 + numAdder, 1, punchCounter[fighterNum]); //0 to 90
            setRotations(6 + numAdder, 1, -2*punchCounter[fighterNum]); //0 to 90
            setRotations(6 + numAdder, 2, 0);
        }
        else if (fighterNum == 1)
        {
            //Head turn forward
            setRotations(1 + numAdder, 2, punchCounter[fighterNum]);// -50 to 0
            //Body Turn to Rival
            addRotations(0 + numAdder, 2, -10); //-33 to over -90
            //Left arm second punch
            setRotations(2 + numAdder, 2, 33); //0 to 90
            setRotations(2 + numAdder, 1, -punchCounter[fighterNum]); //0 to 90
            setRotations(3 + numAdder, 1, 2*punchCounter[fighterNum]); //0 to 90
            setRotations(3 + numAdder, 2, 0);
        }

        if (punchCounter[fighterNum] >= fightCounts[fighterID[fighterNum]][0]) //Originally 50
        {
            punch2Stop[fighterNum] = false;
            attacking[fighterNum] = false;
            //punchCounter[fighterNum] = 50;
            punchCounter[fighterNum] = fightCounts[fighterID[fighterNum]][0];
            if (fighterNum == 0)
                setRotations(6 + numAdder, 1, 0);
            else if (fighterNum == 1)
                setRotations(3 + numAdder, 1, 0);
            readyStance(fighterNum);
        }
    }

    if (tornado[fighterNum])
    {
        if (fighterID[fighterNum] == 1 || fighterID[fighterNum] == 2)
        {
            tornadoCounter[fighterNum] += fighterDir[fighterNum]*15;

                    //Spin
            setRotations(0 + numAdder, 2, tornadoCounter[fighterNum]);
            if (tornadoCounter[fighterNum] == fighterDir[fighterNum]*180)
            {
                checkIfHit(fighterNum, rivalNum);
            }
            else if (tornadoCounter[fighterNum] == fighterDir[fighterNum]*360)
            {
                checkIfHit(fighterNum, rivalNum);
            }
            else if (tornadoCounter[fighterNum] == fighterDir[fighterNum]*540)
            {
                checkIfHit(fighterNum, rivalNum);
                tornado[fighterNum] = false;
                tornadoStop[fighterNum] = true;
            }
        }
        else if (fighterID[fighterNum] == 0)
        {
            tornadoCounter[fighterNum] += 1;
            //printf("%f\n", tornadoCounter[fighterNum]);
            fighterAnimation = animArr[fighterID[fighterNum]][5];
            if (tornadoCounter[fighterNum] == 65)
            {
                checkIfHit(fighterNum, rivalNum);
            }
            else if (tornadoCounter[fighterNum] == 85)
            {
                checkIfHit(fighterNum, rivalNum);
                tornado[fighterNum] = false;
                tornadoStop[fighterNum] = true;
            }
        }
        else {//Monday Memo
            tornadoCounter[fighterNum] += 1;
            //printf("%f\n", tornadoCounter[fighterNum]);
            fighterAnimation = animArr[fighterID[fighterNum]][5];
            if (tornadoCounter[fighterNum] == 75)
            {
                checkIfHit(fighterNum, rivalNum);
                tornado[fighterNum] = false;
                tornadoStop[fighterNum] = true;
            }
        }


    }
    else if (tornadoStop[fighterNum])
    {
        if (fighterID[fighterNum] == 1 || fighterID[fighterNum] == 2)
        {
            tornadoCounter[fighterNum] -= fighterDir[fighterNum]*5;
            setRotations(0 + numAdder, 2, tornadoCounter[fighterNum]);
            if (tornadoCounter[fighterNum] == fighterDir[fighterNum]*390)
            {
                tornadoCounter[fighterNum] = fighterDir[fighterNum]*30;
                tornadoStop[fighterNum] = false;
                readyStance(fighterNum);
            }
        }
        else //if (fighterID[fighterNum] == 0)
        {
            tornadoCounter[fighterNum] -= 10;
            if (tornadoCounter[fighterNum] <= fightCounts[fighterID[fighterNum]][1]){
                tornadoCounter[fighterNum] = fightCounts[fighterID[fighterNum]][1];
                tornadoStop[fighterNum] = false;
                readyStance(fighterNum);
                fighterAnimation = animArr[fighterID[fighterNum]][0];
            }
        }
    }
}

void Fighter::readyStance(int fighterNum)
{
    //Cancels any current animations
    punch[fighterNum] = false;
    punchStop[fighterNum] = false;
    punch2[fighterNum] = false;
    punch2Stop[fighterNum] = false;
    kick[fighterNum] = false;
    kickStop[fighterNum] = false;
    tornado[fighterNum] = false;
    tornadoStop[fighterNum] = false;
    moveForwards[fighterNum] = false;
    moveBackwards[fighterNum] = false;
    attacking[fighterNum] = false;
    //Return to "Ready Stance" Rotations
    setRotations(0 + addNum[fighterNum], 2 , -fighterDir[fighterNum]*33);
    setRotations(1 + addNum[fighterNum], 2, -fighterDir[fighterNum]*50);


    if (fighterNum == 0)
    {
        //Right Arm (Screen Perspective)
        setRotations(2 + addNum[fighterNum], 1, fighterDir[fighterNum]*60);
        setRotations(3 + addNum[fighterNum], 2, fighterDir[fighterNum]*120);
        //Left Arm (Screen Perspective)
        setRotations(5 + addNum[fighterNum], 1, -fighterDir[fighterNum]*60);
        setRotations(6 + addNum[fighterNum], 2, -fighterDir[fighterNum]*120);
        //Right Leg
        setRotations(8 + addNum[fighterNum], 2, -fighterDir[fighterNum]*64);
        setRotations(8 + addNum[fighterNum], 0, fighterDir[fighterNum]*22);
        setRotations(9 + addNum[fighterNum], 0, -fighterDir[fighterNum]*22);
        //Left Leg
        setRotations(10 + addNum[fighterNum], 0, 0);
        setRotations(11 + addNum[fighterNum], 0, 0);
    }
    else if (fighterNum == 1)
    {
        //Right Arm (Screen Perspective)
        setRotations(2 + addNum[fighterNum], 1, 60);
        setRotations(3 + addNum[fighterNum], 2, 120);
        //Left Arm (Screen Perspective)
        setRotations(5 + addNum[fighterNum], 1, -60);
        setRotations(6 + addNum[fighterNum], 2, -120);
        //Right Leg (Screen Perspective)
        //Far Leg (Away from other fighter)
        setRotations(8 + addNum[fighterNum], 0, 0);
        setRotations(9 + addNum[fighterNum], 0, 0);

        setRotations(10 + addNum[fighterNum], 2, -fighterDir[fighterNum]*64);
        setRotations(10 + addNum[fighterNum], 0, -fighterDir[fighterNum]*22);
        setRotations(11 + addNum[fighterNum], 0, fighterDir[fighterNum]*22);
    }

}

/**
\brief Sets the provided fighter into its constructed form by setting all rotations to 0. Used for "tornado attack!"

\param fighterNum --- Integer Id value of the fighter attempting to throw the punch
*/
void Fighter::tposeStance(int fighterNum)
{
    int num = 0;
    if (fighterNum == 1)
    {
        num = 12;
    }
    for(int i = num; i < 12 + num; i++)
    {
        for (int x = 0; x < 3; x++)
            setRotations(i, x, 0);
    }
}

void Fighter::swapAnimation(int index)
{
    fighterAnimation = index;
    //gMesh->setAnimationIndex(index);
    //printf("Animation Index swapped to %d\n", gMesh->getAnimationIndex());
}

int Fighter::getFighterAnimation()
{
    return fighterAnimation;
}

void Fighter::setFighterID(int fiID, int rivID)
{
    fighterID[fighterNum] = fiID;
    fighterID[rivalNum] = rivID;
}
