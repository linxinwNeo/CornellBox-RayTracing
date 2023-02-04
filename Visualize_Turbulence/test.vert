#version 330 core
layout(location=0) in vec3 aPos;
out vec4 Color;

void main()
{
    Color = vec4(0, 1, 0., 1.);
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
