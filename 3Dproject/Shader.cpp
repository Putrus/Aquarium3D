#include "Shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::cout << "Loading shader..." << std::endl;
    //zmienne potrzebne do wczytania kodów shaderów z naszych plików .glsl lub .frag/.vert
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //zapewnienie obiektom typu fstream rzucania wyj¹tków
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //otworzenie odpowiednich plików z kodem naszym shaderów
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        //wczytanie treœci naszym plików do plików stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //zamkniêcie plików
        vShaderFile.close();
        fShaderFile.close();
        //konwersja stream do stringów
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    //zapisanie kodu do zmiennych const char*, które potem nam siê przydadz¹ przy wywo³aniu glShaderSource
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    //kompilacja shaderów
    unsigned int vertex, fragment;
    //tworzymy i kompilujemy vertex shader na podstawie kodu znajduj¹cego siê w vShaderCode(tam mamy kod z naszym plików .glsl)
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    //tworzymy i kompilujemy fragment shader na podstawie kodu znajduj¹cego siê w fShaderCode(tam mamy kod z naszym plików .glsl)
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    //tworzymy shader program i do³¹czamy do niego vertex i fragment shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    //usuwamy shadery, bo ju¿ do³¹czyliœmy je do shaderProgram. Nie s¹ nam d³u¿ej potrzebne
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

