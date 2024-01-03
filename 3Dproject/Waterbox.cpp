#include "Waterbox.h"



Waterbox::Waterbox() {
	readTexture();
	setup();
}

void Waterbox::readTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    std::vector<std::string> faces{
        "right.png",
        "left.png",
        "top.png",
        "bottom.png",
        "front.png",
        "back.png"
    };

    unsigned int width, height;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string facePath = "Textures/Waterbox/" + faces[i];

        std::vector<unsigned char> image;
        unsigned error = lodepng::decode(image, width, height, facePath.c_str());
        if (image.data())
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << facePath << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


void Waterbox::setup() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Waterbox::draw(Shader& shader, glm::mat4 &V, glm::mat4 &P) {
    glDepthFunc(GL_LEQUAL);
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	//shader.setInt("textureMap0", 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glm::mat4 M = glm::mat4(1.0);
    M = glm::scale(M, glm::vec3(50.0f, 50.0f, 50.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "M"), 1, false, glm::value_ptr(M));
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}