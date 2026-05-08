#include "Model3D.h"
#include <cstdio>

// Aplica materiais do modelo ao OpenGL
void Model3D::applyMaterial(const aiMaterial* material) {
    aiColor4D diffuse, specular, ambient, emissive;
    float shininess = 1.0;

    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float*)&diffuse);
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular))
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float*)&specular);
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient))
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (float*)&ambient);
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive))
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (float*)&emissive);
    if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess))
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

// Desenha os vértices de um mesh
void Model3D::drawMesh(const aiMesh* mesh, bool useOriginalColors) {
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            int index = face.mIndices[j];

            if (useOriginalColors && mesh->HasVertexColors(0)) {
                glColor4fv((GLfloat*)&mesh->mColors[0][index]);
            }

            if (mesh->HasNormals()) {
                glNormal3fv(&mesh->mNormals[index].x);
            }

            glVertex3fv(&mesh->mVertices[index].x);
        }
    }
    glEnd();
}

// Desenha um nó da hierarquia do modelo
void Model3D::drawNode(const aiNode* node, bool useOriginalColors) {
    aiMatrix4x4 transform = node->mTransformation;
    transform.Transpose(); // OpenGL usa matriz coluna-maior
    glPushMatrix();
    glMultMatrixf((float*)&transform);

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if (useOriginalColors) { applyMaterial(material); }
        drawMesh(mesh, useOriginalColors);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        drawNode(node->mChildren[i], useOriginalColors);
    }

    glPopMatrix();
}

// Construtor
Model3D::Model3D(const char* filepath) {
    scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);
    if (!scene) {
        // const char* errorString = importer.GetErrorString();
        char errorString[150]; strcpy(errorString, importer.GetErrorString());
        char newfilepath[100] = "../";
        strcat(newfilepath,filepath);
        scene = importer.ReadFile(newfilepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);

        if (!scene) {
            printf("Erro ao carregar o modelo: %s\n", errorString);
            printf("Erro ao carregar o modelo: %s\n", importer.GetErrorString());
        }
    }
}

// Método para desenhar o modelo
void Model3D::draw(bool useOriginalColors) {
    if (scene) {
        drawNode(scene->mRootNode, useOriginalColors);
    }
}








// #include "Model3D.h"
// //---------------------------------------------------------------------------
// void Model3D::processNode(aiNode *node, const aiScene *scene) {
//     for (unsigned int i = 0; i < node->mNumMeshes; i++) {
//         aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
//         meshes.push_back(processMesh(mesh, scene));
//     }

//     for (unsigned int i = 0; i < node->mNumChildren; i++) {
//         processNode(node->mChildren[i], scene);
//     }
// }
// //---------------------------------------------------------------------------
// Mesh Model3D::processMesh(aiMesh *mesh, const aiScene *scene) {
//     Mesh m;
//     for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
//         Vertex vertex;

//         vertex.position[0] = mesh->mVertices[i].x;
//         vertex.position[1] = mesh->mVertices[i].y;
//         vertex.position[2] = mesh->mVertices[i].z;

//         vertex.normal[0] = mesh->mNormals[i].x;
//         vertex.normal[1] = mesh->mNormals[i].y;
//         vertex.normal[2] = mesh->mNormals[i].z;

//         if (mesh->mTextureCoords[0]) {
//             vertex.texCoord[0] = mesh->mTextureCoords[0][i].x;
//             vertex.texCoord[1] = mesh->mTextureCoords[0][i].y;
//         } else {
//             vertex.texCoord[0] = 0.0f;
//             vertex.texCoord[1] = 0.0f;
//         }

//         m.vertices.push_back(vertex);
//     }

//     for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
//         aiFace face = mesh->mFaces[i];
//         for (unsigned int j = 0; j < face.mNumIndices; j++) {
//             m.indices.push_back(face.mIndices[j]);
//         }
//     }

//     return m;
// }
// //---------------------------------------------------------------------------
// //Construtor
// Model3D::Model3D(const char *path)
// {
//     Assimp::Importer importer;
//     const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

//     if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//         std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
//         return;
//     }

//     processNode(scene->mRootNode, scene);
// }
// //---------------------------------------------------------------------------
// void Model3D::drawMesh(Mesh mesh) {
//     glEnableClientState(GL_VERTEX_ARRAY);
//     glEnableClientState(GL_NORMAL_ARRAY);
//     glEnableClientState(GL_TEXTURE_COORD_ARRAY);

//     glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &mesh.vertices[0].position);
//     glNormalPointer(GL_FLOAT, sizeof(Vertex), &mesh.vertices[0].normal);
//     glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mesh.vertices[0].texCoord);

//     glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

//     glDisableClientState(GL_VERTEX_ARRAY);
//     glDisableClientState(GL_NORMAL_ARRAY);
//     glDisableClientState(GL_TEXTURE_COORD_ARRAY);
// }
// //---------------------------------------------------------------------------
// void Model3D::draw() {
//     for (auto &mesh : meshes) {
//         drawMesh(mesh);
//     }
// }
// //---------------------------------------------------------------------------
