#version 460 core
layout (location = 0) in vec3 aPos;


uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//aPos��Ϊattribute�����ԣ�����shader
//��������ĵ�
void main()
{
	vec4 position = vec4(aPos, 1.0);
	position = projectionMatrix*viewMatrix*transform * position;
	gl_Position = position;
	//uv = aUV;
}