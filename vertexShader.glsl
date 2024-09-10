#version 430
       
layout(location = 0) in vec2 pixelPos;
layout(location = 1) in vec4 pixelColor;

out vec4 fragColor;

void main(void) {
    gl_Position = vec4(pixelPos, 0.0, 1.0);
    fragColor = pixelColor;
}