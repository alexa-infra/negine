#ifdef VERTEX_SHADER
attribute vec2 tex;
attribute vec3 position;
attribute vec3 n;
attribute vec2 t;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat4 clip_matrix;

varying vec2 tex0;
varying vec2 tex1;

void main(void) {
    tex0 = tex;
    tex1 = t;
    gl_Position = clip_matrix * vec4(position, 1.0);
}
#endif

#ifdef PIXEL_SHADER
uniform sampler2D diffuse;
uniform sampler2D lightmap;
varying vec2 tex0;
varying vec2 tex1;

void main() {
    vec4 color_sample = texture2D(diffuse, tex0);
    vec4 lightmap_color = texture2D(lightmap, tex1);
    gl_FragColor = color_sample * lightmap_color;
}
#endif
