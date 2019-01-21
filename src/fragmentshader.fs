#version 330 core
out vec3 FragColor;

in vec3 theColor;

void main()
{
  FragColor = theColor;
}