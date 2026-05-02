#version 330 core
out vec4 fragColor;
in vec4 vColor;

uniform vec4 uColor;

void main()
{
    fragColor = aColor * uColor;
}

