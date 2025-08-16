#version 450

layout(location=0) in vec3 POSITION;
layout(location=0) out vec3 POSITIONOUT;

#ifdef REQ_COLOR
layout(location=1) in vec4 COLOR;
layout(location=1) out vec4 COLOROUT;
#endif

#ifdef REQ_TEXCOORD_0
layout(location=2) in vec3 TEXCOORD_0;
layout(location=2) out vec3 UV;
#endif

#ifdef REQ_NORMAL
layout(location=3) in vec3 NORMAL;
layout(location=3) out vec3 NORMALOUT;
#endif

layout(binding=0) uniform UBLOCK1 { mat4 model; } ublock_0;
layout(binding=1) uniform UBLOCK2 { mat4 vp;} ublock_1;

out gl_PerVertex { vec4 gl_Position; };

void main() {
	gl_Position = ublock_1.vp * ublock_0.model * vec4(POSITION, 1);
	POSITIONOUT = gl_Position.xyz;
	
#ifdef REQ_TEXCOORD_0
	UV = TEXCOORD_0;
#endif
	
#ifdef REQ_COLOR
	COLOROUT = COLOR;
#endif
	
#ifdef REQ_NORMAL
	vec4 _tempNorm = vec4(NORMAL, 1) * ublock_0.model;
	NORMALOUT = _tempNorm.xyz;
#endif
}