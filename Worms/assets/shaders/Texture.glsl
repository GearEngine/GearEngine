//Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;			
layout(location = 1) in vec2 a_TexCoord;			

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

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
uniform float u_TilingFactor;
uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	color = texture(u_Texture0, vec2(v_TexCoord.x, v_TexCoord.y) ) * u_Color;
	if(color.a == 0)
		discard;

}