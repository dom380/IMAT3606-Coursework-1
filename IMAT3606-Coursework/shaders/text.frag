#version 430

uniform sampler2D tex;
in vec2 texCoord;

out vec4 FragColour;

uniform vec3 textColor;

void main() {
   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r);
   FragColour = vec4(textColor, 1.0) * sampled;
}