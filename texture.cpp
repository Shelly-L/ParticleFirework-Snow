#include"texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"//��glframework����������
#include<iostream>
#include<vector>
Texture::Texture(const std::string& path, unsigned int unit)
{
	//1 stbImage ��ȡͼƬ
	int width, height, channels;
	mUnit = unit;

	//--��תy��
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	mWidth = width;
	mHeight = height;

	if (data)
	{

		//2 ���������Ҽ��Ԫ��

		glGenTextures(1, &mTexture);


		//--��������Ԫ--
		glActiveTexture(GL_TEXTURE0 + mUnit);


		//--���������--
		glBindTexture(GL_TEXTURE_2D, mTexture);



		//3 ������������,�����Դ�
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//�ͷ�����
		stbi_image_free(data);

		//4 ��������Ĺ��˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//5 ��������İ�����ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
	}
	else
	{
		std::cout << "texture load error" << std::endl;
	}
}


void Texture::bind() {
	//���л�����Ԫ�����л��������
	glActiveTexture(GL_TEXTURE + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::bind1() {
	//���л�����Ԫ�����л��������
	glActiveTexture(GL_TEXTURE + mUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
}

Texture::~Texture() {
	if (mTexture != 0)
		glDeleteTextures(1, &mTexture);
}

Texture::Texture(std::vector<std::string> faces, unsigned int unit)
{
	mUnit = unit;
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		mWidth = width;
		mHeight = height;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			GLenum error;
			while ((error = glGetError()) != GL_NO_ERROR) {
				std::cout << "OpenGL error: " << error << std::endl;
			}
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
}

