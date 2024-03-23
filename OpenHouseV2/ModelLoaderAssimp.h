#ifndef MODELLOADERASSIMP_H_INCLUDED
#define MODELLOADERASSIMP_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SFML/Graphics.hpp>  // Used only for image loading.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <assimp/Importer.hpp> //C++ importer interface for assimp
//#include <assimp/cimport.h>
#include <assimp/scene.h>      //Output data structure
#include <assimp/postprocess.h>//Post processing flags

//#include <assimp/code/Importer.h>

#include "LoadShaders.h"
#include "ProgramDefines.h"
#include "Material.h"
#include "Light.h"
#include "Shape.h"

//Crap
#include "ogldev_texture.h"
#include "ogldev_util.h"

/**
\file ModelLoaderAssimp.h
\brief Header file for ModelLoaderAssimp.cpp

\author    Brian Bowers
\version   1.0
\date      10/29/2023

*/

class BasicMesh
{
public:
    BasicMesh() {};

    ~BasicMesh();

    bool LoadMesh(const string& Filename);

    void Render();

    void Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);

    //WorldTrans& GetWorldTransformation() {return m_worldTransformation}

private:
    void Clear();

    bool InitFromScene(const aiScene* pScene, const std::string& Filename);

    void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVerticies, unsigned int& NumIndices);

    void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);

    void InitAllMeshes(const aiScene* pScene);

    void InitSingleMesh(const aiMesh* paiMesh);

    bool InitMaterials(const aiScene* pScene, const std::string& Filename);

    void PopulateBuffers();

#define INVALID_MATERIAL 0xFFFFFFFF

    enum BUFFER_TYPE {
        INDEX_BUFFER = 0,
        POS_VB = 1,
        TEXCOORD_VB = 2,
        NORMAL_VB = 3,
        WVP_MAT_VB = 4,
        WORL_MAT_VB = 5,
        NUM_BUFFERS = 6,
    };

    //WorldTrans m_worlTransformation;
    GLuint m_VAO = 0;
    GLuint m_Buffers[NUM_BUFFERS] = { 0 };

    struct BasicMeshEntry {
        BasicMeshEntry()
        {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            MaterialIndex = INVALID_MATERIAL;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    std::vector<BasicMeshEntry> m_Meshes;
    std::vector<Texture*> m_Textures;

    //Temporary space for vertex stuff before we load them into the GPU
    //if error try glm::vec3f
    /*
    std::vector<Vector3f> m_Positions;
    std::vector<Vector3f> m_Normals;
    std::vector<Vector3f> m_TexCoords;
    */
    std::vector<glm::vec3> m_Positions;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::vec3> m_TexCoords;
    std::vector<unsigned int> m_Indices;
};

#endif // MODELLOADERASSIMP_H_INCLUDED
