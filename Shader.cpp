#include "Shader.h"

void checkShaderError(int shaderObject) {
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

unsigned int CompileShader(unsigned int SHADER_TYPE, const char* ShaderSource) {
    unsigned int ShaderObject;
    ShaderObject = glCreateShader(SHADER_TYPE);
    glShaderSource(ShaderObject, 1, &ShaderSource, NULL);
    glCompileShader(ShaderObject);
    checkShaderError(ShaderObject);
    return ShaderObject;
}

void CheckProgramError(unsigned int shaderProgram) {
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

unsigned int GenerateShaderProgram(unsigned int shaders[]) {
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    for (int i = 0; i < sizeof(shaders) / sizeof(int); i++) glAttachShader(shaderProgram, shaders[i]);
    glLinkProgram(shaderProgram);
    CheckProgramError(shaderProgram);
    for (int i = 0; i < sizeof(shaders) / sizeof(int); i++) glDeleteShader(shaders[i]);
    return shaderProgram;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    unsigned int ShaderArray[2] = {
        CompileShader(GL_VERTEX_SHADER, vShaderCode),
        CompileShader(GL_FRAGMENT_SHADER, fShaderCode)
        };

    //Creating A program from compiled shaders.
    ID = GenerateShaderProgram(ShaderArray);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()),x ,y ,z );
}

