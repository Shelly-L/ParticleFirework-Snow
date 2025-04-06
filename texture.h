#pragma once
#include"core.h"
#include<string>
#include<vector>

class Texture {
public:
	Texture(const std::string&path,unsigned int unit);
	~Texture();
	void bind();
	void bind1();
	Texture(std::vector<std::string> faces, unsigned int unit);
	friend class Skybox;
private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
    int mHeight{ 0 };
	unsigned int mUnit{ 0 };
};