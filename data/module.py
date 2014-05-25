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
prog1.setShaderSource(ShaderType.VERTEX, vertexShader)
prog1.setShaderSource(ShaderType.PIXEL, pixelShader)
complete = prog1.complete()
if complete:
	print("program 1 has been compiled!")

prog2 = app.context.createProgram("prog2")
prog2.setAttribute("position", VertexAttrs.tagPosition)
prog2.setAttribute("uv", VertexAttrs.tagTexture)
prog2.setShaderSource(ShaderType.VERTEX, vertexShader1)
prog2.setShaderSource(ShaderType.PIXEL, pixelShader1)
complete = prog2.complete()
if complete:
    print("program 2 has been compiled!")

checker = app.context.createTexture("checkers", "checker.png")

fbo = app.context.createFramebuffer("fbo")
fbo.addTargetTexture(checker)
fbo.addTarget(InternalTypes.D32F)
fbo.resize(640, 480)

rp = RenderPass()
rp.target = "fbo"
rp.mode = "normal"
rp.generator = "scene"
rp.viewport = vec4f(0, 0, 640, 480)
rp.clear = True
rp.depthTest = True
rp.depthWrite = True
rp.cullBackFace = False
rp.blend = False
rp.clearColor = vec4f(1.0, 0.0, 0.0, 1.0)
#pipeline_.push_back(rp);

rp2 = RenderPass()
rp2.target = "default_fbo"
rp2.mode = "prog2"
rp2.generator = "fullscreen"
rp2.viewport = vec4f(0, 0, 640, 480)
rp2.clear = True
rp2.depthTest = False
rp2.depthWrite = False
rp2.cullBackFace = False
rp2.blend = False
rp2.clearColor = vec4f(1.0, 0.0, 0.0, 1.0)
#pipeline_.push_back(rp2);

pipeline = RenderPipeline()
pipeline.append(rp)
pipeline.append(rp2)