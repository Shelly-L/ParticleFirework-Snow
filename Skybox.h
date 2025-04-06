#pragma once
#include"core.h"
#include"shader.h"
#include<vector>

class Skybox
{
private:
	Shader* shader = nullptr;
	static float skyboxVertices[108];
	unsigned int skyboxTexture;
	unsigned int skyboxVAO, skyboxVBO;

	Skybox();
public:
	Skybox(const std::vector<std::string>& texturePaths);

	int Draw(glm::mat4& projection, glm::mat4& view);
	Shader* GetShaderPtr();
	const unsigned int GetSkyboxTexture();
};