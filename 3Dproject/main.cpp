
//modele z free3d.com
//klasa camera.h wzorowana na klasie camera z learnOpenGL.com
//Waterbox wzorowane na klasie zwi¹zanej z Cube Mapping
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Waterbox.h"
#include "Camera.h"


#define PI 3.14f

//na learnOpenGL u¿yte stbi_image.h tu u¿yta pierwsza lepsza biblioteka z neta
#include "lodepng.h"

//czas jednej klatki
float deltaTime = 0.0f;
//wielkoœæ okna
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
//klasa kamera
Camera * camera = new Camera(glm::vec3(0.0, 2.0, -1.0));
//wzorowane na OpenGL gdzie zaczynamy nasz œrodek ekranu
float lastX = SCR_WIDTH/2;
float lastY = SCR_HEIGHT/2;

//do rasd
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  
    float xoffset = (xpos - lastX);
    float yoffset = (lastY - ypos);
    lastX = xpos;
    lastY = ypos;
    
    camera->ProcessMouseMovement(xoffset, yoffset);
}


int main()
{
    //inicjalizacja i konfiguracja glfwinit learnOpengl
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //stworzenie okna aplikacji
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    //zaladowanie glad czyli wszystkich funkcji openGL
    //oprócz glad istnieje jeszcze glew.h
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //ustawiamy wielkoœæ obszaru który widzimy
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    //w³¹czenie testu g³êbokoœci
    glEnable(GL_DEPTH_TEST);
    
    //shader do ryb
    Shader shader("v_shader.glsl", "f_shader.glsl");
    //shader do t³a
    Shader waterShader("v_waterbox.glsl", "f_waterbox.glsl");
    //shader do piasku
    Shader sandShader("v_lamberttextured.glsl", "f_lamberttextured.glsl");
    //tworzymy t³o
    Waterbox waterbox;
    std::vector<Object*> objects;
    for (int i = 0; i <= 10; i++) {
        std::string modelPath = "Models/" + std::to_string(i) + ".obj";
        std::string texturePath = "Textures/" + std::to_string(i) + ".png";
        Object * object = new Object(modelPath.c_str(), texturePath.c_str());
        objects.push_back(object);
    }
    

    //chcia³aœ zrobiæ wiêcej takich samych ryb, ale funkcja odpowiadaj¹ca za liczby losowe nie dziala³a poprawnie
    //tworzenie modelów ryb
    std::vector<Model*> models;
    for (int i = 1; i <= 10; i++) {
        Model* model = new Model(i);
        models.push_back(model);
    }

    //tworzymy model piasku
    Model sand(0);
    sand.scale(10.0f);


    glfwSetTime(0); //Wyzeruj licznik czasu
    while (!glfwWindowShouldClose(window))
    {
        deltaTime = glfwGetTime();
        processInput(window);
        glfwSetTime(0); //Wyzeruj licznik czasu
        //czyszczenie buforu kolorów i buforu g³êbokoœci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 V = camera->getViewMatrix();
        glm::mat4 P = glm::perspective(50.0f * 3.14f / 180.0f, 1.0f, 0.1f, 1000.0f);
        shader.use();
        //przekazanie macierzy do shadera
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "V"), 1, false, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "P"), 1, false, glm::value_ptr(P));

        //rysujemy ryby i ruszamy nimi
        for (int i = 0; i < 10; i++) {
            models[i]->draw(objects, shader);
            models[i]->move(deltaTime);
        }


        sandShader.use();
        glUniformMatrix4fv(glGetUniformLocation(sandShader.ID, "V"), 1, false, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(sandShader.ID, "P"), 1, false, glm::value_ptr(P));
        sand.draw(objects, sandShader);

        waterbox.draw(waterShader, V, P);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader.ID);
    glDeleteProgram(sandShader.ID);
    glDeleteProgram(waterShader.ID);
    //wy³¹czenie glfw
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera->ProcessKeyboard(FORWARD, deltaTime * 2);
        else
            camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

