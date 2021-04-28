#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "lodepng.h"
#include "Shader.h"

//wzorowany na klasie Mesh.h

class Object {
	//vertex array object oraz 3 vertex buffer object(na pozycje, wektory normalne i wspó³rzêdne teksturowania)
	unsigned int VAO, VBO[3];
	//uchwyt tekstury/ID tekstury
	unsigned int texture;
	//pozycja wierzcho³ka
	std::vector<float> vertices;
	//wektory normalne
	std::vector<float> normals;
	//wspó³rzêdne teksturowania
	std::vector<float> texCoords;
public:
	//konstruktor
	Object(const char* path, const char* texturePath);
	//destruktor
	~Object();
	std::vector<float> getVertices();
	std::vector<float> getNormals();
	std::vector<float> getTexCoords();
	unsigned int getVAO();
	unsigned int* getVBO();
	void draw(Shader &shader);
private:
	bool loadOBJ(const char* path);
	void setup();
	void readTexture(const char* path);
};




 


