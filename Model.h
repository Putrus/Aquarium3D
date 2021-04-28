#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <random>
#include <chrono>
#include <algorithm>
#include "Object.h"



class Model {
private:
	int ID;
	glm::vec3 position;
	glm::vec3 speed;
	int direction;
	float distance;
	float angle;
	float s;
public:
	Model(int ID, glm::mat4 M = glm::mat4(1.0));
	void scale(float s);
	void move(float deltaTime);
	void draw(std::vector<Object*> &objects, Shader &shader);
};

