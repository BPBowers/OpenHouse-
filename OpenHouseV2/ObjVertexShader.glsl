#version 330 core

/**
\file VertexShaderLightingTexture.glsl

\brief Vertex shader that incorporates the transformation of vertices
by a projection*view*model matrix. Also updates position and normal
vectors by pre-projected matrices.

\param [in] vposition --- vec4 vertex position from memory.

\param [in] vcolor --- vec4 vertex color from memory.

\param [in] vnormal --- vec3 normal vector from memory.

\param [in] in_tex_coord --- vec2 texture coordinates from memory.

\param [out] color --- vec4 output color to the fragment shader.

\param [out] position --- vec4 output transformed position (before view and projection)
to the fragment shader.

\param [out] normal --- vec3 output transformed normal to the fragment shader.

\param [out] tex_coord --- vec2 pass through of the texture coordinates.

\param [uniform] PVM --- mat4 transformation matrix in the form projection*view*model.

\param [uniform] Model --- mat4 model transformation matrix.

\param [uniform] NormalMatrix --- mat3 normal transformation matrix.

*/

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vcolor;
layout(location = 2) in vec3 vnormal;
layout(location = 3) in vec2 in_tex_coord;
//Bone Stuff
layout(location = 4) in ivec4 BoneIDs;
layout(location = 5) in vec4 Weights;

uniform mat4 PVM;
uniform mat4 Model;
uniform mat3 NormalMatrix;

out vec4 color;
out vec4 position;
out vec3 normal;
out vec2 tex_coord;
//Bone Stuffs
flat out ivec4 BoneIDs0;
out vec4 Weights0;

const int MAX_BONES = 200;
uniform mat4 gBones[MAX_BONES];

//mat4 BoneTransform;

//vec4 PosL;

void main()
{
    //If no bones, weight will be 0 so multiplication has no effect (hopefully)
    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    mat4 EmptyMatrix;
    if(BoneTransform == EmptyMatrix){
        BoneTransform = mat4(1.0);
    }
    else{

        //mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];/Use this line to load animated files correctly
        BoneTransform += gBones[BoneIDs[1]] * Weights[1];
        BoneTransform += gBones[BoneIDs[2]] * Weights[2];
        BoneTransform += gBones[BoneIDs[3]] * Weights[3];
        //vec4 PosL = BoneTransform * vposition;
        //vec4 PosL = BoneTransform * (vposition * vec4(1.0));
    }

    tex_coord = in_tex_coord;
    color = vcolor;
    normal = normalize(NormalMatrix * vnormal);
    position = Model * vposition;
    //gl_Position = PVM * vposition;//Original in case of debug needs
    //Bone Stuffs
    //gl_Position = PVM * PosL;//This includes the bone transformations
    gl_Position = PVM * BoneTransform * vposition;
    BoneIDs0 = BoneIDs;
    Weights0 = Weights;
}
