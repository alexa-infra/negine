#ifdef VERTEX_SHADER
attribute vec2 tex;
attribute vec3 position;
attribute vec4 col;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

varying_vert vec2 tex0;
varying_vert vec4 color;

void main(void) {
    color = col;
    tex0 = tex;
    
    gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
}
#endif

#ifdef PIXEL_SHADER
uniform sampler2D diffuse;
varying_frag vec2 tex0;
varying_frag vec4 color;

void main() {
    vec4 DiffuseColor = texture(diffuse, tex0);
    fragColor = DiffuseColor * color;
}
#endif
