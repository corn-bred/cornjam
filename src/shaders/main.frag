#version 330 core

in vec2 iTexCoords;
uniform sampler2D Texture;
uniform vec3 Colour;
uniform bool isSolidColour;

out vec4 FragColour;

void main() {
    if (isSolidColour) {
        FragColour = vec4(Colour, 1.0);
    } else {
        FragColour = texture(Texture, iTexCoords);
    }
}