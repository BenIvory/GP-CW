#version 400
layout( location = 0 ) out vec4 fragcolor;

in vec2 TexCoords;

uniform sampler2D water;

void main()
{
	fragcolor = vec4(texture2D(water, TexCoords).rgb, 1.0);
}
