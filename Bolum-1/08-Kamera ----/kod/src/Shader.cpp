#include "Shader.h"

Shader::Shader()
{
    
}

Shader::~Shader()
{
}

void Shader::createProgram()
{
    m_programId = glad_glCreateProgram();

#ifdef _DEBUG
    m_programCreated = true;
#endif

}

void Shader::compile(GLenum shadertype, std::string path)
{
#ifdef _DEBUG
    if (!m_programCreated)
    {
        spdlog::error("alooo program olusturma fonksiyonunu cagirmadin");
    }
#endif

    //shader olustur
    GLuint shaderid;
    shaderid = glad_glCreateShader(shadertype);

    //dosyayi oku
    path = "shaders/" + path;
    std::string source = cmake_PROJECT_RES + path;

    std::string shaderSource = readFile(source);
    const char* sourceCStr = shaderSource.c_str();

    //derle
    glad_glShaderSource(shaderid, 1, &sourceCStr, NULL);
    glad_glCompileShader(shaderid);

    //hata varsa yazdir
    int  success;
    char infoLog[512];
    glad_glGetShaderiv(shaderid, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glad_glGetShaderInfoLog(shaderid, 512, NULL, infoLog);
        switch (shadertype)
        {
        case GL_VERTEX_SHADER:
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
            break;

        case GL_FRAGMENT_SHADER:
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
            break;
        }
    }

    glad_glAttachShader(m_programId, shaderid);

    glad_glDeleteShader(shaderid);

}

void Shader::addUniform(std::string uniformName)
{
    int uniformLocation = glad_glGetUniformLocation(m_programId, uniformName.c_str());
    if (uniformLocation != -1)
    {
        m_uniforms[uniformName] = uniformLocation;
    }
    else
    {
        std::cout << "ERROR:: uniform eklerken hata olustu isim yanlis veya uniform tanimsiz[" << uniformName << "]\n";
    }
}

void Shader::setUniform(std::string uniformName, glm::vec4 vec)
{
    glad_glUniform4f(m_uniforms[uniformName], vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniform(std::string uniformName, glm::mat4 mat)
{
    glad_glUniformMatrix4fv(m_uniforms[uniformName], 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::link()
{
#ifdef _DEBUG
    if (!m_programCreated)
    {
        spdlog::error("alooo program olusturma fonksiyonunu cagirmadin");
    }
#endif

    glad_glLinkProgram(m_programId);
    int isLinked;
    char log[512];
    glad_glGetShaderiv(m_programId, GL_LINK_STATUS, &isLinked);

    if (!isLinked)
    {
        glad_glGetProgramInfoLog(m_programId, 512, 0, log);

        std::cout << "Program Linking Error:\n" << log << "\n";
    }
}

std::string Shader::readFile(fs::path path)
{
    std::fstream file(path);

    if (!file.is_open())
    {
        std::cout << "ERROR:: couldnt open the file\n";
        return "";
    }

    std::stringstream ss;

    ss << file.rdbuf();

    return ss.str();
}
