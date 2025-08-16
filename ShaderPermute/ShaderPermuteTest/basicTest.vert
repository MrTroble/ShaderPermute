#version 460

layout(location=0) in vec3 POSITION;
layout(location=0) out vec3 POSITIONOUT;

#ifdef COLOR
layout(location=1) in vec4 COLOR;
layout(location=1) out vec4 COLOROUT;
#endif

#ifdef TEXCOORD_0
layout(location=2) in vec3 TEXCOORD_0;
layout(location=2) out vec3 UV;
#endif

#ifdef NORMAL
layout(location=3) in vec3 NORMAL;
layout(location=3) out vec3 NORMALOUT;
#endif

layout (binding=0) uniform { mat4 model; } ublock_0;
layout (binding=1) uniform { mat4 vp;} ublock_1;
out gl_PerVertex { vec4 gl_Position; };
void main() {
	gl_Position = ublock_1.vp * ublock_0.model * vec4(POSITION, 1);
	POSITIONOUT = gl_Position.xyz;
	
#ifdef TEXCOORD_0
	UV = TEXCOORD_0;
#endif
	
#ifdef COLOR
	COLOROUT = COLOR;
#endif
	
#ifdef NORMAL
	vec4 _tempNorm = vec4(NORMAL, 1) * ublock_0.model;
	NORMALOUT = _tempNorm.xyz;
#endif
}