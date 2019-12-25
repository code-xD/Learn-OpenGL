// Vertex Shader (Written in GLSL) This will be compiled during runtime to be implemented
/* The shader first takes in 6 values for a vertex then
   one half of the vector is used to set the position
   other half is used as a out vector used by fragment shader
   to be then define the colour.
*/

#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
uniform vec3 offsetRGB; 
out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aPos; // set ourColor to the input color we got from the vertex data
}       