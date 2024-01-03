#include "Object.h"


Object::Object(const char* path, const char *texturePath) {
	loadOBJ(path);
	readTexture(texturePath);
	setup();
	std::cout << "Object created!" << std::endl;
}


Object::~Object() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBO);
	glDeleteTextures(1, &texture);

}

//wziête z internetu tutorial czêœæ 7 ze strony...
bool Object::loadOBJ(const char* path) {
	std::cout << "Loading " << path <<  "..." << std::endl;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(path, "r");
	if (file == NULL) {
		std::cout << "Impossible to open the file " << path << std::endl;
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);
		vertices.push_back(1.0f);
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
		normals.push_back(0.0f);
		texCoords.push_back(uv.x);
		texCoords.push_back(1-uv.y);
	}
	std::cout << "Load " << path << " success!" << std::endl;
	return true;
}

//zapisywanie danych do karty graficznej
void Object::setup() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glBufferData(GL_ARRAY_BUFFER, getVertices().size() * sizeof(float), getVertices().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);


	glBufferData(GL_ARRAY_BUFFER, getNormals().size() * sizeof(float), getNormals().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);


	glBufferData(GL_ARRAY_BUFFER, getTexCoords().size() * sizeof(float), getTexCoords().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


std::vector<float> Object::getVertices() {
	return vertices;
}
std::vector<float> Object::getNormals() {
	return normals;
}
std::vector<float> Object::getTexCoords() {
	return texCoords;
}

unsigned int Object::getVAO() {
	return VAO;
}

unsigned int* Object::getVBO() {
	return VBO;
}


void Object::readTexture(const char* path) {
	std::cout << "Loading " << path << texture << std::endl;
	glActiveTexture(GL_TEXTURE0);
	unsigned width, height, nrChannels;
	std::vector<unsigned char> image;

	unsigned error = lodepng::decode(image, width, height, path);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	std::cout << "Loading texture success!" << std::endl;
}





void Object::draw(Shader &shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.setInt("textureMap0", 0);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, getVertices().size());
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}



