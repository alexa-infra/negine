attribute vec2 tex;
attribute vec2 position;
attribute vec4 color;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

varying vec2 tex0;
varying vec4 col;

void main(void) {
    tex0 = tex;
    col = color;
    gl_Position = projection_matrix * modelview_matrix * vec4(position, 0, 0.5);
}
