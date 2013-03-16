#ifdef GL_ES
    precision highp float;
#endif
#if __VERSION__ >= 140
    #define varying_frag in
    #define varying_vert out
    #define attribute in
    #define texture2D texture
    out vec4 fragColor;
#else
    #define varying_frag varying
    #define varying_vert varying
    #define fragColor gl_FragColor
#endif
