//Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;			
layout(location = 1) in vec2 a_TexCoord;			

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform int u_Flat;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;			
			
in vec2 v_TexCoord;
		
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform sampler2D u_Mask;
uniform int u_WithMask;
uniform float u_TilingFactor;

void main()
{
	if(u_WithMask == 1)
	{
		vec4 maskColor;
		maskColor = texture(u_Mask, v_TexCoord);
		color = texture(u_Texture, v_TexCoord);
		if(maskColor.x == 0 && maskColor.y == 0 && maskColor.z == 0)
			discard;
		else
		{
			if(maskColor.x != 1 && maskColor.y != 1 && maskColor.z != 1)
			{
				color.x = 0.56;
				color.y = 0.751;
				color.z = 0.9;
				
			}
		}
	}
	else
	{
		color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
		if(color.a < 0.1)
			discard;
		if(color.x + color.y + color.z == 0.0)
			discard;
	}
}
