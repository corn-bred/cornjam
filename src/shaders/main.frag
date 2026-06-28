#version 330 core

in vec2 iTexCoords;
uniform sampler2D Texture;

out vec4 FragColour;

void main() {
    FragColour = texture(Texture, iTexCoords);
}