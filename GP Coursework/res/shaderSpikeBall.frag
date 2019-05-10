#version 400

in vec2 tex;

uniform sampler2D bricks;

out vec4 fragcolor;

void main()
{
	fragcolor = vec4(texture2D(bricks, tex).rgb, 1.0);
}
