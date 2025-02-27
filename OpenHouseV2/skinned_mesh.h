#ifndef SKINNED_MESH_H
#define SKINNED_MESH_H

#include <map>
#include <vector>
#include <GL/glew.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "ogldev_util.h"
#include "ogldev_math_3d.h"
#include "ogldev_texture.h"
#include "ogldev_world_transform.h"
#include "ogldev_material.h"
#include "Material.h"

/**
\file skinned_mesh.h
\brief Header file for skinned_mesh.cpp

\author    Etay Meiri Modified by Brian Bowers
\version   1.1
\date      12/11/2023

*/

/**
\class SkinnedMesh

\brief Skinned Mesh loads an 3D model that contains bone & animation data to allow animation
    Relies on ASSIMP (Open Asset Library) to do much of the heavy lifting but still loads all of the data
    ASSIMP loader guarners and sends it to the shader with the help of skinning_technique.cpp

    Original Copyright:

    Copyright 2021 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

class SkinnedMesh
{
public:
    SkinnedMesh() {};

    ~SkinnedMesh();

    bool LoadMesh(const std::string& Filename);

    void Render();

    uint NumBones() const
    {
        return (uint)m_BoneNameToIndexMap.size();
    }

    WorldTrans& GetWorldTransform() { return m_worldTransform; }

    const oMaterial& GetMaterial();

    //void GetBoneTransforms(vector<Matrix4f>& Transforms);//Calculates all the transformations for all the bones and returns in vector of matrices, each bone has own matrix
    void GetBoneTransforms(float TimeInSeconds, vector<Matrix4f>& Transforms);

    void SetBoneTransform(uint Index, const Matrix4f& Transform);

    //What I will use to switch between animations for the fighters
    void setAnimationIndex(uint Index);
    //Returns the index value of the current animation so that it can be compared against other values, might end up not needing this but gonna implement just in case
    int getAnimationIndex();

private:
    #define MAX_NUM_BONES_PER_VERTEX 4

    GLuint m_boneLocation[MAX_BONES];

    void Clear();

    bool InitFromScene(const aiScene* pScene, const std::string& Filename);

    void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices);

    void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);

    void InitAllMeshes(const aiScene* pScene);

    void InitSingleMesh(uint MeshIndex, const aiMesh* paiMesh);

    bool InitMaterials(const aiScene* pScene, const std::string& Filename);

    void PopulateBuffers();

    void LoadTextures(const string& Dir, const aiMaterial* pMaterial, int index);

    void LoadDiffuseTexture(const string& Dir, const aiMaterial* pMaterial, int index);

    void LoadSpecularTexture(const string& Dir, const aiMaterial* pMaterial, int index);

    void LoadColors(const aiMaterial* pMaterial, int index);

    struct VertexBoneData
    {
        uint BoneIDs[MAX_NUM_BONES_PER_VERTEX] = { 0 };
        float Weights[MAX_NUM_BONES_PER_VERTEX] = { 0.0f };

        VertexBoneData()
        {
        }

        void AddBoneData(uint BoneID, float Weight)
        {
            for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(BoneIDs) ; i++) {
                if (Weights[i] == 0.0) {
                    BoneIDs[i] = BoneID;
                    Weights[i] = Weight;
                    //printf("Adding bone %d weight %f at index %i\n", BoneID, Weight, i);
                    return;
                }
            }

            // should never get here - more bones than we have space for
            //assert(0);//If asserted, prevents me from loading larger values, need edit program to dynamically assign the value of MAX_NUM_BONES_PER_VERTEX
        }
    };

    void LoadMeshBones(uint MeshIndex, const aiMesh* paiMesh);
    void LoadSingleBone(uint MeshIndex, const aiBone* pBone);
    int GetBoneId(const aiBone* pBone);
    //void ReadNodeHeirarchy(const aiNode* pNode, const Matrix4f& ParentTransform);
    void ReadNodeHeirarchy(float AnimationTimeTicks, const aiNode* pNode, const Matrix4f& ParentTransform);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);

    //Following used for animated transformation matrix

    uint FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    uint FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    uint FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);


#define INVALID_MATERIAL 0xFFFFFFFF

    enum BUFFER_TYPE {
        INDEX_BUFFER = 0,
        POS_VB       = 1,
        TEXCOORD_VB  = 2,
        NORMAL_VB    = 3,
        BONE_VB      = 4,
        NUM_BUFFERS  = 5
    };

    WorldTrans m_worldTransform;
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

    //Assimp Stuffs, will need during runtime to prevent segfaults
    Assimp::Importer Importer;
    const aiScene* pScene = NULL;
    std::vector<BasicMeshEntry> m_Meshes;
    std::vector<oMaterial> m_Materials;//Used in original form
    Material mat;//Og Spickler format, has more features but textures not included

    // Temporary space for vertex stuff before we load them into the GPU
    vector<Vector3f> m_Positions;
    vector<Vector3f> m_Normals;
    vector<Vector2f> m_TexCoords;
    vector<unsigned int> m_Indices;
    vector<VertexBoneData> m_Bones;

    map<string,uint> m_BoneNameToIndexMap;

    //Last update from previous version that dude made (might mess around with making them all glm::mat4's)
    //Future note, was able to get them to play nice with glm::mat4's so no problem
    struct BoneInfo
    {
        Matrix4f OffsetMatrix;
        Matrix4f FinalTransformation;

        BoneInfo(const Matrix4f& Offset)
        {
            OffsetMatrix = Offset;
            FinalTransformation.SetZero();
        }

    };

    vector<BoneInfo> m_BoneInfo;
    Matrix4f m_GlobalInverseTransform;

    uint animationIndex = 0; //Current animation to be run, by default is 0
};


#endif  /* SKINNED_MESH_H */
