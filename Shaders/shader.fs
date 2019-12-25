// Fragment Shader (Written in GLSL) This will be compiled during runtime to be implemented
// Out Vector is responsible for the output for the shader.

#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
  
void main()
{
    FragColor = vec4(ourColor, 1.0);
}