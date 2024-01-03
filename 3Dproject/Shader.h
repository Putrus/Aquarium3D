#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//klasa shader utworzona na podstawie klasy shader ze strony learnOpenGL.com
class Shader
{
public:
    //ID naszego programu cieniuj¹cego(shaderProgram)
    GLuint ID;
    //konstruktor
    Shader(const char* vertexPath, const char* fragmentPath);
    
    //aktywacja shadera
    void use();

    //ustawienie zmiennej jednorodnej typu bool
    void setBool(const std::string& name, bool value) const;

    //ustawienie zmiennej jednorodnej typu int
    void setInt(const std::string& name, int value) const;

    //ustawienie zmiennej jednorodnej typu float
    void setFloat(const std::string& name, float value) const;

private:
    //funkcja sprawdzaj¹ca blêdy kompilacji lub wi¹zania shaderów z programShaderem
    void checkCompileErrors(unsigned int shader, std::string type);
};
#endif