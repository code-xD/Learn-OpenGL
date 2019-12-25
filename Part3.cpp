#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void LinkVertexToShaderProgram();

float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); Enable for MACOSX

    GLFWwindow* window = glfwCreateWindow(WINDOW_HEIGHT, WINDOW_WIDTH, "First Tutorial", NULL, NULL);


    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //A Viewport is the active region on the window drawn where the OPENGL renders.
    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);
    //Calls the defined function whenever the window resizes.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //Make A Buffer Object in the GPU and assign it to a variable(VBO).
    //Make An Array Object to save the current states.
    unsigned int VBO, VAO;
    unsigned int EBO;// An Element Buffer Object
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // ..:: Initialization code :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    LinkVertexToShaderProgram();

    //Unbind The Buffers to be bound for later use.
    //Now the VAO remembers the state of the buffer till it was last bound and can be bound again for drawing.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering commands here
        //Setting the colour which fills the frame while clearing the present window 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float offset = (sin(2*timeValue) / 2.0f);
        ourShader.setFloat("offset", offset);
        
        // draw our first Rectangle
        ourShader.use();
        //glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// For WireFrame Mode in GL_TRIANGLES
        // The Attributes are (DrawingMode, No of Indices , Type of Indices , Starting Index) of the Index Array Bound.
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);//When Element Buffer is not used.

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        //Checks for any event call like mouse click,keyboard and 
        //calls the corresponding callback function like defined above.
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //std::cout << "Window resized.";
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



void LinkVertexToShaderProgram() {
    // This function defines how the shader should use the data stored in the buffer object
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
