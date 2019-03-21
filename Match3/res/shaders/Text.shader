#shader vertex
#version 330 core

layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 v_TexCoord;

uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * vec4(vertex.xy, 0.0, 1.0);
	v_TexCoord = vertex.zw;
}


#shader fragment
#version 330 core
in vec2 v_TexCoord;
out vec4 color;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Text, v_TexCoord).r);
	color = vec4(u_TextColor, 1.0) * sampled;
}