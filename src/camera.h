#ifndef CAMERA_H
#define CAMERA_H

#include "constants.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::vec2;
using glm::vec3;
using glm::mat4;

// Singleton class
class Camera {
public:
    Camera(const vec3& position = vec3(0.0f, 0.0f, 10.0f), const vec3& front = vec3(0.0f, 0.0f, -1.0f));

	const vec3& getCameraPosition() const;
	void setCameraPosition(const vec3& pos);
	const vec3& getCameraRight() const;
	const vec3& getCameraUp() const;
	const vec3& getCameraFront() const;
	float getFov() const;
	void setFov(float newFov);
	void orbitCamera(double xoffset, double yoffset);
	float getNearPlane() const;
	float getFarPlane() const;
	vec2 getNearPlaneDims() const;
	vec2 getScreenDims() const;
	void setScreenDims(float width, float height) { screenWidth = width; screenHeight = height; }
	const mat4& calcViewMatrix();
	const mat4& calcProjectionMatrix(const float width = Constants::SCR_WIDTH, const float height = Constants::SCR_HEIGHT);

private:
	mat4 viewMatrix;
	mat4 projectionMatrix;
	vec3 cameraPos;
	vec3 cameraRight;
	vec3 cameraUp;
	vec3 cameraFront;
	vec3 orbitTarget = vec3(0.0f, 0.0f, 0.0f);
	double pitch;
	double yaw;
	float fov;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	float screenWidth = Constants::SCR_WIDTH;
	float screenHeight = Constants::SCR_HEIGHT;
	void updateCameraVectors();

};

#endif