-- vertex
attribute vec2 tex;
attribute vec3 position;
attribute vec4 col;
attribute vec3 n;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

varying vec2 tex0;
varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec4 color;

void main(void) {
    color = col;
    tex0 = tex;
    normal = normalize( vec3(modelview_matrix * vec4(n, 0.0)) );
    
    vec4 vertex_in_modelview_space = modelview_matrix * vec4(position, 1.0);
    vertex_to_light_vector = normalize(vec3(0.0, 0.0, 0.0) - vec3(vertex_in_modelview_space));

    gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
}

-- pixel
uniform sampler2D diffuse;
varying vec2 tex0;
varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec4 color;

void main() {
    vec4 DiffuseColor = texture2D(diffuse, tex0);// * color;
    float DiffuseTerm = clamp(dot(normal, vertex_to_light_vector), 0.0, 1.0);
    gl_FragColor = DiffuseColor;// * color; // * DiffuseTerm;
}
