#include "camera.h"
using glm::mat4;
using glm::vec3;

Camera::Camera(const vec3& position, const vec3& front)
{

	cameraPos = position;
	yaw = -90.0; // default camera to point down -z-axis
	pitch = 0.0;
	fov = 45.0;

	updateCameraVectors();
	calcViewMatrix();
}

void Camera::setCameraPosition(const vec3& position) {
	cameraPos = position;
}

const vec3& Camera::getCameraPosition() {
	return cameraPos;
}

const vec3& Camera::getCameraFront() {
	return cameraFront;
}

void Camera::setCameraFront(double xoffset, double yoffset) {
	const double sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(newFront);
	cameraRight = glm::normalize(glm::cross(worldUp, cameraFront));
	cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));

}

const vec3& Camera::getCameraRight() {
	return cameraRight;
}

const vec3& Camera::getCameraUp() {
	return cameraUp;
}

float Camera::getFov() {
	return fov;
}

void Camera::setFov(float newFov) {
	// Clamp range
	if (newFov < 1.0f)
		newFov = 1.0f;
	if (newFov > 45.0f)
		newFov = 45.0f;

	fov = newFov;
}

const mat4& Camera::calcViewMatrix() {
	vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::lookAt(cameraPos,
							cameraPos + cameraFront,
							worldUp);
	return viewMatrix; // const reference to member variable
}

const mat4& Camera::calcProjectionMatrix(const float width, const float height) {
	projectionMatrix = glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f); // TODO - no magic numbers for near and far clip planes
	return projectionMatrix;
}