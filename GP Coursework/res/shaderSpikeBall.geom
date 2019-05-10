#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 9) out;

uniform mat4 view;
uniform mat4 projection;

in vec2 TexCoord[];
in vec3 Normal[];

out vec2 tex;

void main()
{
    vec3 centerPos = vec3((gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0f);
	vec2 centerTexCoord = (TexCoord[0] + TexCoord[1] + TexCoord[2]) / 3.0f;
    
    vec3 a = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 centerNormal = normalize(cross(a, b));
    
    vec4 spikeyCenter = vec4(centerPos + centerNormal / 8.0, 1.0);

	gl_Position = projection * view * gl_in[0].gl_Position;
    tex = TexCoord[0];
	EmitVertex();
	gl_Position = projection * view * gl_in[1].gl_Position;
    tex = TexCoord[1];
	EmitVertex();
	gl_Position = projection * view * spikeyCenter;
    tex = centerTexCoord;
	EmitVertex();
    EndPrimitive();
    
	gl_Position = projection * view * gl_in[1].gl_Position;
    tex = TexCoord[1];
	EmitVertex();
	gl_Position = projection * view * gl_in[2].gl_Position;
    tex = TexCoord[2];
	EmitVertex();
	gl_Position = projection * view * spikeyCenter;
    tex = centerTexCoord;
	EmitVertex();
    EndPrimitive();

	gl_Position = projection * view * gl_in[2].gl_Position;
    tex = TexCoord[2];
	EmitVertex();
	gl_Position = projection * view * gl_in[0].gl_Position;
    tex = TexCoord[0];
	EmitVertex();
	gl_Position = projection * view * spikeyCenter;
    tex = centerTexCoord;
	EmitVertex();
    EndPrimitive();
}
