#include "Shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::cout << "Loading shader..." << std::endl;
    //zmienne potrzebne do wczytania kod�w shader�w z naszych plik�w .glsl lub .frag/.vert
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //zapewnienie obiektom typu fstream rzucania wyj�tk�w
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //otworzenie odpowiednich plik�w z kodem naszym shader�w
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        //wczytanie tre�ci naszym plik�w do plik�w stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //zamkni�cie plik�w
        vShaderFile.close();
        fShaderFile.close();
        //konwersja stream do string�w
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    //zapisanie kodu do zmiennych const char*, kt�re potem nam si� przydadz� przy wywo�aniu glShaderSource
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    //kompilacja shader�w
    unsigned int vertex, fragment;
    //tworzymy i kompilujemy vertex shader na podstawie kodu znajduj�cego si� w vShaderCode(tam mamy kod z naszym plik�w .glsl)
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    //tworzymy i kompilujemy fragment shader na podstawie kodu znajduj�cego si� w fShaderCode(tam mamy kod z naszym plik�w .glsl)
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    //tworzymy shader program i do��czamy do niego vertex i fragment shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    //usuwamy shadery, bo ju� do��czyli�my je do shaderProgram. Nie s� nam d�u�ej potrzebne
    glDeleteShader(vertex);
    glDeleteShader(fragment);
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

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

