#version 400

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
    TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) * aNormal;  
    
	gl_Position = model * vec4(aPos, 1.0);
}
