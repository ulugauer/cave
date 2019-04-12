#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(binding = 0) uniform sampler2D texSampler;

void main() {
    outColor = texture(texSampler, texCoord);
    //outColor = vec4(texCoord, 0.0, 1.0);
}