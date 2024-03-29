#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"

#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>

class Mesh : public AGLDrawable {
public:
    glm::vec3 pos;
    glm::vec3 scale;

    std::string meshPath;
    std::string texturePath;
    std::string vsPath;
    std::string fsPath;

    Mesh(const std::string& _path);
    void loadFragmentShader(const std::string& _path);
    //~Mesh();
    void setBuffers();
    void setShader(const std::string& _path);
    void draw(glm::mat4& MVP, const glm::vec3& light);
private:
    GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
    bool loadOBJ(
        const char* path,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals
    );
    GLuint loadBMP(const char* imagepath);

    std::vector<glm::vec3> vertex_buffer_data;
    std::vector<glm::vec2> uvs_buffer_data;
    std::vector<glm::vec3> normal_buffer_data;

    GLuint texture;
    GLuint shader;
};

#endif