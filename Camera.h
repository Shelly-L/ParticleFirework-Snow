#pragma once

#include"glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include"glm/gtx/transform.hpp"
#include"GLFW/glfw3.h"
#include <iostream>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position);

	virtual glm::mat4 getWorldToViewMatrix();
	glm::vec3 getPosition() const;
	glm::vec3 getDirection() const;

	void moveUp(float axis);
	void moveForward(float axis);
	void moveLeft(float axis);
	void rotate(float yaw, float pitch);

	float getSpeed() const;
	void setSpeed(float newSpeed);
	static float getDeltaTime();
	static void updateDeltaTime();

	virtual void scale(float deltaScale);

protected:
	glm::vec3 position;
	glm::vec3 direction;
	const glm::vec3 UP;
	float speed;
	glm::vec3 mRight{ 1.0f,0.0f,0.0f };

	static float currentFrame;
	static float deltaTime;
	static float lastFrame;
};
