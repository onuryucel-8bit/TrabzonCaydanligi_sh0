#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

class Shader
{
public:
    Shader();
    ~Shader();

    void createProgram();

    void compile(GLenum shadertype, std::string path);

    void addUniform(std::string uniformName);
    void setUniform(std::string uniformName, glm::vec4 vec);
    void setUniform(std::string uniformName, glm::mat4 mat);

    void link();

    GLuint m_programId;

#ifdef _DEBUG
    bool m_programCreated = false;
#endif // _DEBUG

    
private:
    std::string readFile(fs::path path);
    std::unordered_map<std::string, GLint> m_uniforms;
};
