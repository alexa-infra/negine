from negine_core import *
from negine_runtime import *

a = vec4f(1.0, 0.0, 0.0, 1.0)
print a.x, a.y, a.z, a.w
print a

vertexShader = """
#version 150
attribute vec3 position;
attribute vec2 uv;
uniform mat4 mvp;
varying vec2 texCoord;
void main()
{
    texCoord = uv;
    vec4 pos = vec4(position, 1);
    gl_Position = mvp * pos;
}
"""
pixelShader = """
uniform float time;
uniform vec4 viewport;
varying vec2 texCoord;
void main() {
    vec4 newColor = vec4(1.0, 0, 0, 1.0);
    float x = texCoord.s / viewport.x;
    float y = texCoord.t / viewport.y;
    newColor.g = sin(x * cos(time/15.0) * 120.0) +
                cos(y * sin(time/10.0) * 120.0) +
                sin(sqrt(y*y + x*x) * 40.0);
    gl_FragColor = newColor;
}
"""

app = globalApp()
print app.context
prog = app.context.createProgram()
prog.setAttribute("position", VertexAttrs.tagPosition)
prog.setAttribute("uv", VertexAttrs.tagTexture)
prog.setShaderSource(ShaderTypes.VERTEX, vertexShader)
prog.setShaderSource(ShaderTypes.PIXEL, pixelShader)
if prog.complete():
	print "program has been compiled!"
del(prog)
#prog.destroy()