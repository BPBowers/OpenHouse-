#include "ModelLoaderAssimp.h"
using namespace std;

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
//aiPostProcessSteps
//#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

#define COLOR_TEXTURE_UNIT GL_TEXTURE0

BasicMesh::~BasicMesh()
{
    Clear();
}

void BasicMesh::Clear()
{
    for(unsigned int i = 0; i < m_Textures.size() ; i++){
        SAFE_DELETE(m_Textures[i]);
    }

    if(m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }

    if(m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
    }
}

bool BasicMesh::LoadMesh(const string& Filename)
{
    //Release previously loaded mesh
    Clear();

    //Create VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    //Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool Ret = false;

    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), ASSIMP_LOAD_FLAGS);

    if (pScene) {
        Ret = InitFromScene(pScene, Filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), ASSIMP_LOAD_FLAGS);

    }

    glBindVertexArray(0);

    return Ret;
}

bool BasicMesh::InitFromScene(const aiScene* pScene, const string& Filename)
{
    m_Meshes.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);
    printf("Made it past meshes.resize\n");
    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    CountVerticesAndIndices(pScene, NumVertices, NumIndices);
    printf("Past countVertAndIndices\n");
    ReserveSpace(NumVertices, NumIndices);
    printf("Past ReserveSpace\n");
    InitAllMeshes(pScene);
    printf("Past InitAllMeshes\n");
    if(!InitMaterials(pScene, Filename)) {
        return false;
    }
    printf("Past if statement\n");
    PopulateBuffers();
    printf("Past Populate Buffers\n");
    return GLCheckError();
}

void BasicMesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices)
{
    for(unsigned int i = 0; i < m_Meshes.size(); i++) {
        m_Meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Meshes[i].BaseVertex = NumVertices;
        m_Meshes[i].BaseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices += m_Meshes[i].NumIndices;
    }
}

void BasicMesh::ReserveSpace(unsigned int NumVertices, unsigned int NumIndices)
{
    m_Positions.reserve(NumVertices);
    m_Normals.reserve(NumVertices);
    m_TexCoords.reserve(NumVertices);
    m_Indices.reserve(NumIndices);
}


//Traverses the assimp mesh structures and calls InitSingleMesh for each mesh
void BasicMesh::InitAllMeshes(const aiScene* pScene)
{
    for(unsigned int i = 0; i < m_Meshes.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitSingleMesh(paiMesh);
    }
}

//Populates the vectors with vertex and index data
void BasicMesh::InitSingleMesh(const aiMesh* paiMesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    //populate the vertex attribute vectors
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D& pPos = paiMesh->mVertices[i];
        const aiVector3D& pNormal = paiMesh->mNormals[i];
        const aiVector3D& pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;

        m_Positions.push_back(glm::vec3(pPos.x, pPos.y, pPos.z));
        m_Normals.push_back(glm::vec3(pNormal.x, pNormal.y, pNormal.z));
        m_TexCoords.push_back(glm::vec3(pTexCoord.x, pTexCoord.y, 0));
        //m_TexCoords.push_back(Vector3f(pTexCoord.x, pTexCoord.y));
    }

    //Populate the index buffer
    for(unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        m_Indices.push_back(Face.mIndices[0]);
        m_Indices.push_back(Face.mIndices[1]);
        m_Indices.push_back(Face.mIndices[2]);
    }
}

bool BasicMesh::InitMaterials(const aiScene* pScene, const string& Filename)
{
    //Extract the directory part from the file name
    string::size_type SlashIndex = Filename.find_last_of("/");
    string Dir;
    printf("In initAllMaterials\n");
    if (SlashIndex == string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0){
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }
    printf("Past If file directory thing\n");
    bool Ret = true;

    //Initialize the materials
    /*
    for (unsigned int i = 0; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];
        m_Textures[i] = NULL;
        printf("In for loop for material initialization %d\n", i);
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0 ) {
            aiString Path;
            printf("Past if 1\n");
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                string p(Path.data);
                printf("Past If 2\n");
                if (p.substr(0, 2) == ".\\") {
                    p = p.substr(2, p.size() -2);
                    printf("Past If 3\n");
                }

                string FullPath = Dir + "/" + p;

                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());
                printf("Past Texture init\n");
                if (!m_Textures[i]->Load()) {
                    printf("Error loading texure '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    //Remove after testing
                    printf("Loaded texture '%s'\n", FullPath.c_str());
                }
            }

        }
    }
    */
    printf("Out of loop\n");
    return Ret;
}

void BasicMesh::PopulateBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    //glVertexAttribPointer(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_TexCoords[0]) * m_TexCoords.size(), &m_TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    //glVertexAttribPointer(TEX_COORD_LOCTAION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    //glVertexAttribPointer(NORMAL_LOCTAION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);
}

void BasicMesh::Render()
{
    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_Meshes.size(); i++) {
        unsigned int MaterialIndex = m_Meshes[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());

        if (m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES, m_Meshes[i].NumIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * m_Meshes[i].BaseIndex), m_Meshes[i].BaseVertex);
    }

    //Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}

//Instead of world mats do projectionMatrix, eye, viewMatrix and for WVP do modelMatrix
void BasicMesh::Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats)
{

}

