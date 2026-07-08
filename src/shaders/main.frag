#version 330 core

in vec2 iTexCoords;
uniform sampler2D Texture;

uniform bool isSolidColour;
uniform vec3 Colour;

uniform bool isAnimation;
uniform int Columns;
uniform int Rows;
uniform int Frame;

out vec4 FragColour;

void main() {
    if (isSolidColour) {
        FragColour = vec4(Colour, 1.0);
    } else if (isAnimation) {
        vec2 TexSize = vec2(1.0 / Columns, 1.0 / Rows);
        vec2 TexLocalCoords = iTexCoords * TexSize;

        int Column = Frame % Columns;
        int Row = int(floor(float(Frame) / float(Columns)));

        vec2 TexCoords = vec2(TexLocalCoords.x + (TexSize.x * Column), TexLocalCoords.y + (TexSize.y * Row));
        TexCoords.y = 1.0 - TexCoords.y; //Flip
        FragColour = texture(Texture, TexCoords);
    } else {
        FragColour = texture(Texture, iTexCoords);
    }
}