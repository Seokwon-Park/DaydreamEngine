#version 450 core
			
layout(location = 0) out vec4 color;

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in vec2 v_TexCoord;

layout(binding = 1) uniform sampler2D u_Texture;

void main()
{
	color = vec4(v_Position * 0.5 + 0.5, 1.0);
	color = texture(u_Texture, v_TexCoord);
	color = vec4(v_TexCoord, 0.0f, 1.0f);
}