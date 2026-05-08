#ifndef MODEL3D_H
#define MODEL3D_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glut.h>

class Model3D {
private:
    const aiScene* scene;
    Assimp::Importer importer;

    void applyMaterial(const aiMaterial* material);
    void drawMesh(const aiMesh* mesh, bool useOriginalColors);
    void drawNode(const aiNode* node, bool useOriginalColors);

public:
    Model3D(const char* filepath);
    void draw(bool useOriginalColors = true);
};

#endif // MODEL3D_H











// #ifndef MODEL3D_H
// #define MODEL3D_H

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
// #include <vector>
// #include <GL/gl.h>
// #include <iostream>

// struct Vertex {
//     GLfloat position[3];
//     GLfloat normal[3];
//     GLfloat texCoord[2];
// };

// struct Mesh {
//     std::vector<Vertex> vertices;
//     std::vector<GLuint> indices;
// };

// class Model3D {
// private:
//     std::vector<Mesh> meshes;

//     void processNode(aiNode *node, const aiScene *scene);
//     Mesh processMesh(aiMesh *mesh, const aiScene *scene);

// public:
//     Model3D(const char *path);
//     void drawMesh(Mesh mesh);
//     void draw();
// };

// #endif // MODEL3D_H
