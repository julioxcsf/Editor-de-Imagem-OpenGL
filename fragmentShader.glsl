#version 430

in vec4 fragColor;
out vec4 color;

void main() {
    color = fragColor;
    //if (int(gl_FragCoord.x / 5) % 2 == int(gl_FragCoord.y / 5) % 2) {
    //    color = vec4(1.0, 1.0, 0.0, 1.0);  // Cor ciano
    //} else {
    //    color = vec4(0.0, 0.0, 1.0, 1.0);  // Cor azul
    //}

    //color = vec4(1.0,0.0,0.0,1.0);
}