-- vertex
attribute vec2 tex;
attribute vec3 position;
attribute vec3 n;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

varying vec2 tex0;

void main(void) {
    tex0 = tex;
    gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
}

-- pixel
uniform sampler2D diffuse;
varying vec2 tex0;

void main() {
    vec4 color_sample = texture2D(diffuse, tex0);
    gl_FragColor = color_sample;
}
