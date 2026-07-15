#version 430 core

in vec2 iTexCoords;
uniform sampler2D FontTexture;

out vec4 FragColour;

void main() {
    vec4 TexColour = texture(FontTexture, iTexCoords);
    FragColour = TexColour;
}