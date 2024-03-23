#version 330 core

/**
\file StrangeFrag.glsl

\brief Strange fragment shader that colors fragments by the gl_FragCoord.

The example also shows the use of functions inside a glsl shader.

*/

in vec4 pos;
out vec4 col;

uniform float time;
uniform int status;
uniform float flopper;
uniform float gFlopper;

vec4 setColor(vec4 p)
{
    /*
    switch (status)
    {
        case 0:
            //Status ZERO: Blank vec4
            return vec4(0, 0, 0, 1);
            break;
        case 1:
            //Status ONE: Red Ripples
            return vec4(0.8*cos(sqrt((p.x*p.x)*100 + (p.y*p.y)*100)) - 0.8*sin(sqrt((p.x*p.x)*100 + (p.y*p.y)*100)), 0, 0, 1);
            break;
        case 2:
            //Status TWO: Red and Green Ripples
            return vec4(0.8*cos(sqrt((p.x*p.x)*100 + (p.y*p.y)*100)) - 0.8*sin(sqrt((p.x*p.x)*100 + (p.y*p.y)*100)), sin(sqrt((p.x*p.x)*100 + (p.y*p.y)*100)) - 0.4+cos(sqrt((p.x*p.x)*100 + (p.y*p.y)*100)), 0, 1);
            break;
        case 3:
            //Status THREE: Blue horizontal lines, Red Vertical lines
            return vec4(sin(p.x*40), 0, sin(p.y*40), 1);
            break;
        case 4:
            //Status FOUR: Red Checker Board w/ bottom left square red: Important! Adapted from https://www.geeks3d.com/hacklab/20190225/demo-checkerboard-in-glsl/
            float gg = mod(floor(1.65*p.y)+floor(-1.65*p.x), 2);
            return vec4(sign(gg), 0, 0, 1);
            break;
        case 5:
            //Status FIVE: "Animated" Blue Ripple
            return vec4(0, 0, cos(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)) - sin(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)), 1);
            break;
        case 6:
        */
            //Status SIX: Crazy Red Checkerboard swapping with Green inverse Checkerboard w/ "Animated" Blue Ripple
    float gg2 = mod(floor(1.65*p.y)+floor(-1.65*p.x), 2);
    //float gg3 = mod(floor(1.65*p.y)+floor(1.65*p.x), 2);
    //return vec4(flopper*sign(gg2), gFlopper*sign(gg3), (0.3*cos(time-sqrt((p.x*p.x)*100 + (p.y*p.y)*100))) - (0.3*sin(time-sqrt((p.x*p.x)*100 + (p.y*p.y)*100))), 1);
            //break;
        //case 7:
            //Status 7??? The CHALLENGE
            //float c = p.x+p.y(sqrt(-1);
            //float q = q + c
            //return vec4(0, 0, 0, 1);
    //}

    //Blue Ripple
    //return vec4(0, 0, cos(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)) - sin(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)), 1);
    //Red Ripple
    //return vec4(cos(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)) - sin(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)), 0, 0, 1);
    //Orange Ripple
    return vec4(cos(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)) - sin(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)), (0.3)*(cos(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10)) - sin(time-sqrt((p.x*p.x)*10 + (p.y*p.y)*10))), flopper*sign(gg2), 1);

}

void main()
{
    col = setColor(3*pos);
}
