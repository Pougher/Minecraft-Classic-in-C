#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float faceshade;

uniform sampler2D ourTexture;

void main() 
{
    FragColor = texture(ourTexture, TexCoord) * faceshade;
}