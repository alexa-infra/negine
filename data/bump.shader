#ifdef VERTEX_SHADER
attribute vec2 tex;
attribute vec3 position;
attribute vec4 col;
attribute vec3 n;
attribute vec3 t;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform vec3 light_pos;
uniform vec3 camera_pos;

varying vec2 tex0;
varying vec3 eye;
varying vec3 light;
varying float distance;
varying vec3 halfVec;

void main(void) {
    vec3 bitan = cross(n, t);
    vec3 tmp = light_pos - position;
    distance = length(tmp);
    light.x = dot(tmp, t);
    light.y = dot(tmp, bitan);
    light.z = dot(tmp, n);
    light = normalize(light);

    tmp = camera_pos - position;
    halfVec.x = dot(tmp, t);
    halfVec.y = dot(tmp, bitan);
    halfVec.z = dot(tmp, n);
    halfVec = normalize(halfVec);
    halfVec = (halfVec + light) / 2.0;
    halfVec = normalize(halfVec);

    eye = camera_pos;
    tex0 = tex;

    gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
}
#endif

#ifdef PIXEL_SHADER
uniform sampler2D diffuse;
uniform sampler2D bump;

varying vec2 tex0;
varying vec3 eye;
varying vec3 light;
varying float distance;
varying vec3 halfVec;

void main() {
    vec4 color_sample = texture2D(diffuse, tex0);
    vec3 color = color_sample.rgb;
    float alpha = color_sample.a;
    vec3 bump_color = texture2D(bump, tex0).rgb * 2.0 - 1.0;
    float lamberFactor  = max(dot(light, bump_color), 0.0);
    float specFactor = max(pow(dot(halfVec, bump_color), 28.0), 0.0);

    gl_FragColor = vec4(color * lamberFactor + vec3(1.0) * specFactor, alpha);
}
#endif
