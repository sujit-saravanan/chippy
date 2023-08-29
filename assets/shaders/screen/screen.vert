#version 460 core
layout (location = 0) in vec3 pos;

uniform vec2 size;
uniform vec2 offset;
out vec2 UVs;

const vec2 positions[6] = vec2[6](
vec2(-1.0f, -1.0f),
vec2( 1.0f, -1.0f),
vec2( 1.0f,  1.0f),
vec2(-1.0f, -1.0f),
vec2( 1.0f,  1.0f),
vec2(-1.0f, 1.0f)
);

const vec2 uvs[6] = vec2[6](
vec2(0.0f, 1.0f),
vec2(1.0f, 1.0f),
vec2(1.0f, 0.0f),
vec2(0.0f, 1.0f),
vec2(1.0f, 0.0f),
vec2(0.0f, 0.0f)
);

void main()
{
    gl_Position = vec4(positions[gl_VertexID % 6], 0.0f, 1.0f);
    UVs = uvs[gl_VertexID % 6];
}