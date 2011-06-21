attribute vec2 tex;
attribute vec2 position;
attribute vec4 color;
varying vec2 tex0;
varying vec4 col;

void main(void) {
    tex0 = tex;
    col = color;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(position, 0, 0.5);
}
