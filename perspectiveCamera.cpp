#include"perspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float fovy,float aspect, float near, float far) {
	mFovy = fovy;
	mAspect = aspect;
	mNear = near;
	mFar = far;

}

PerspectiveCamera::~PerspectiveCamera() {

}
glm::mat4 PerspectiveCamera::getWorldToViewMatrix() {
	return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
}

void PerspectiveCamera::scale(float deltaScale) {
	auto front = glm::cross(UP, mRight);
	position += (front * deltaScale);
}
