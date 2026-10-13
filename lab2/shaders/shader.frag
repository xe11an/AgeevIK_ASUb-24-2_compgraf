#version 410 core
out vec4 frag_colour;

uniform vec4 ourColor;   // цвет, меняющийся во времени

void main() {
    frag_colour = ourColor;
}
