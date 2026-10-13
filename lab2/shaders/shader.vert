#version 410 core
layout (location = 0) in vec3 vp;

uniform vec2 offset;   // смещение фигуры (движение)

void main() {
    gl_Position = vec4(vp + vec3(offset, 0.0), 1.0);
}
