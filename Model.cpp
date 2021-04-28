#include "Model.h"


Model::Model(int ID, glm::mat4 M) {
	this->ID = ID;
	srand(time(NULL));
	position = glm::vec3(0.0);
	angle = 0.0f;
	s = 1.0f;
	speed = glm::vec3(0.0);
	direction = 0;
	if (ID != 0) {
		float x = (float)(rand() % (10+ID) + 0);
		float y = (float)(rand() % (30+ID) + 5);
		float z = (float)(rand() % (20+ID) + 0);
		position = glm::vec3(x/10.0f, y/10.0f, z/10.0f);
		
		direction = (rand() % (30+ID) + 0) / 10;
		distance = (float)(rand() % (50+ID) + 10) / 10.0f;
		float s = (float)(rand() % (100+ID) + 10) / 100.0f;
		if (direction == 0) {
			angle = 0.0f;
			speed = glm::vec3(0, 0, -s);
		}
		if (direction == 1) {
			angle = 3.14f;
			speed = glm::vec3(0, 0, s);
		}
		if (direction == 2) {
			angle = -0.5 * 3.14;
			speed = glm::vec3(s, 0, 0);
		}
		if (direction == 3) {
			angle = 0.5 * 3.14;
			speed = glm::vec3(-s, 0, 0);
		}
		std::cout << "-------------------------------" << std::endl;
		std::cout << "ID: " << ID << std::endl;
		std::cout << x << " " << y << " " << z << std::endl;
		std::cout << "dir: " << direction << " distance: " << distance << " speed: " << s << std::endl;
	}
	
}


void Model::move(float deltaTime) {
	if ((direction == 0 || direction == 1) && (position.z <= -distance || position.z >= distance)) {
		if (angle == 0.0f) {
			angle = 3.14f;
		}
		else {
			angle = 0.0f;
		}
		speed.z = -speed.z;
		position += speed * deltaTime * 2.0f;
	}
	if ((direction == 2 || direction == 3) && (position.x <= -distance || position.x >= distance)) {
		angle = -angle;
		speed.x = -speed.x;
		position += speed * deltaTime * 2.0f;
	}
	position += speed * deltaTime;
}




void Model::scale(float s) {
	this->s = s;
}

void Model::draw(std::vector<Object*> &objects, Shader& shader) {
	glm::mat4 M = glm::mat4(1.0);
	M = glm::translate(M, position);
	M = glm::rotate(M, angle, glm::vec3(0, 1, 0));
	M = glm::scale(M, glm::vec3(s));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "M"), 1, false, glm::value_ptr(M));
	objects[ID]->draw(shader);
}