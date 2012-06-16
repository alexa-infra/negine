-- vertex
attribute vec3 position;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform vec4 color;

varying vec4 a_color;

void main(void) {
    a_color = color;
    gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
}

-- pixel
varying vec4 a_color;

void main() {
    gl_FragColor = vec4(1, 0, 0, 1); //a_color;
}