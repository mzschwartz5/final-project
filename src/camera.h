#ifndef CAMERA_H
#define CAMERA_H

#include "constants.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::vec3;
using glm::mat4;

// Singleton class
class Camera {
public:
    static Camera& getInstance(const vec3& position = vec3(0.0f, 0.0f, 10.0f), const vec3& front = vec3(0.0f, 0.0f, -1.0f)) {
        static Camera instance(position, front);
        return instance;
    }

	const vec3& getCameraPosition();
	void setCameraPosition(const vec3& pos);
	const vec3& getCameraRight();
	const vec3& getCameraUp();
	const vec3& getCameraFront();
	float getFov();
	void setFov(float newFov);
	void setCameraFront(double xoffset, double yoffset);
	const mat4& calcViewMatrix();
	const mat4& calcProjectionMatrix(const float width = Constants::SCR_WIDTH, const float height = Constants::SCR_HEIGHT);

private:
    Camera(const vec3& position, const vec3& front);
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	vec3 cameraPos;
	vec3 cameraRight;
	vec3 cameraUp;
	vec3 cameraFront;
	double pitch;
	double yaw;
	float fov;
	void updateCameraVectors();

};

#endif