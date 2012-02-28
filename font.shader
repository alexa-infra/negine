-- vertex
attribute vec2 tex;
attribute vec3 position;
attribute vec4 col;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

varying vec2 tex0;
varying vec4 color;

void main(void) {
    color = col;
    tex0 = tex;
    
    gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
}

-- pixel
uniform sampler2D diffuse;
varying vec2 tex0;
varying vec4 color;

void main() {
    vec4 DiffuseColor = texture2D(diffuse, tex0);
    gl_FragColor = color;
    gl_FragColor.a = DiffuseColor.a;
}
