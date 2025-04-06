#pragma once
#include"glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include"glm/glm.hpp"


class Shader
{
public:
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMatrix4x4(const std::string& name, glm::mat4 value)const;

    unsigned int id;
};