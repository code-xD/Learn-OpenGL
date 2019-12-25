#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 600;

//Vertex Shader (Written in GLSL) This will be compiled during runtime to be implemented
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

//Fragment Shader (Written in GLSL) This will be compiled during runtime to be implemented
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int CompileShader(unsigned int SHADER_TYPE,const char* ShaderSource);
unsigned int GenerateShaderProgram(unsigned int shaders[]);
void LinkVertexToShaderProgram();

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle made of 0th 1st and 3rd Element of Vec3 Vertex Array
    1, 2, 3    // second triangle same as above
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    //Creating Vertex and Fragment Shader.
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    unsigned int ShaderArray[2] = { vertexShader,fragmentShader };
    //Creating A program from compiled shaders.
    unsigned int shaderProgram = GenerateShaderProgram(ShaderArray);
   
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    LinkVertexToShaderProgram();

    //Unbind The Buffers to be bound for later use.
    //Now the VAO remembers the state of the buffer till it was last bound and can be bound again for drawing.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
       
    while (!glfwWindowShouldClose(window))
    {   
        //input
        processInput(window);

        //rendering commands here
        //Setting the colour which fills the frame while clearing the present window 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// For WireFrame Mode in GL_TRIANGLES
        // The Attributes are (DrawingMode, No of Indices , Type of Indices , Starting Index) of the Index Array Bound.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

void checkShaderError(int shaderObject) {
   
    //Check for Compilation Error of Shader
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

unsigned int CompileShader(unsigned int SHADER_TYPE,const char* ShaderSource) {
    
    //Creating a ShaderObject and refering it by a variable.
    unsigned int ShaderObject;
    ShaderObject = glCreateShader(SHADER_TYPE);

    //Attaching shader source code to ShaderObject and Compiling it.
    glShaderSource(ShaderObject, 1, &ShaderSource, NULL);
    glCompileShader(ShaderObject);

    checkShaderError(ShaderObject);
    return ShaderObject;
}

void CheckProgramError(unsigned int shaderProgram) {
    
    //Check for Errors While Linking Shader to a Program
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

unsigned int GenerateShaderProgram(unsigned int shaders[]) {
    
    //Create Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //Attach the Compiled Shaders to the Program
    for(int i = 0; i < sizeof(shaders)/sizeof(int);i++) glAttachShader(shaderProgram, shaders[i]);
    glLinkProgram(shaderProgram);

    CheckProgramError(shaderProgram);
    
    //Deleting the shaders after getting Linked.
    for (int i = 0; i < sizeof(shaders) / sizeof(int); i++) glDeleteShader(shaders[i]);

    return shaderProgram;
}

void LinkVertexToShaderProgram() {
    // This function defines how the shader should use the data stored in the buffer object
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
