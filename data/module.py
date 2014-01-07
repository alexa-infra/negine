print("hello")
from negine_core import *
from negine_runtime import *

vertexShader = """
#version 150
in vec3 position;
in vec3 normal;
uniform mat4 mvp;
out vec3 n;
void main()
{
    n = 0.5*normalize(normal) + 0.5;
    vec4 pos = vec4(position, 1);
    gl_Position = mvp * pos;
}
"""
pixelShader = """
#version 150
in vec3 n;
out vec4 fragColor;
void main()
{
    fragColor = vec4(n, 1);
}
"""
vertexShader1 = """ 
#version 150
in vec3 position;
in vec2 uv;
out vec2 tex;
void main()
{
    tex = uv;
    vec4 pos = vec4(position, 1);
    gl_Position = pos;
}
"""
pixelShader1 = """ 
#version 150
uniform sampler2D atexture;
in vec2 tex;
out vec4 fragColor;
void main()
{
    fragColor = texture(atexture, tex);
}
"""


app = globalApp()
print(app.context)

prog1 = app.context.createProgram("prog1")
prog1.setAttribute("position", VertexAttrs.tagPosition)
prog1.setAttribute("normal", VertexAttrs.tagNormal)
prog1.setShaderSource(ShaderTypes.VERTEX, vertexShader)
prog1.setShaderSource(ShaderTypes.PIXEL, pixelShader)
if prog1.complete():
	print("program 1 has been compiled!")

prog2 = app.context.createProgram("prog2")
prog2.setAttribute("position", VertexAttrs.tagPosition)
prog2.setAttribute("uv", VertexAttrs.tagTexture)
prog2.setShaderSource(ShaderTypes.VERTEX, vertexShader1)
prog2.setShaderSource(ShaderTypes.PIXEL, pixelShader1)
if prog2.complete():
    print("program 2 has been compiled!")

checker = app.context.createTexture("checkers", "checker.png")

fbo = app.context.createFramebuffer("fbo")
fbo.addTargetTexture(checker)
fbo.addTarget(InternalTypes.D32F)
fbo.resize(640, 480)