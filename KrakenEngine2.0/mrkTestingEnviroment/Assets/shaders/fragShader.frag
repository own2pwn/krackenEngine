#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler[11];

layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = vec4(0,0,0,0);

  for(int i = 0; i < 11; ++i)
  {
    outColor += texture(texSampler[i], fragTexCoord);
  }
}
