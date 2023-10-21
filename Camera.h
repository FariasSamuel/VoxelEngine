#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Operations.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
	
const float YAW = -90.0f;
const float PITCH = 0.0f;

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	glm::vec3 Right;
	glm::mat4 view;
	float Yaw;
	float Pitch;
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)){
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	glm::mat4 getView() {
		return lookAt(Position, Position + Front, Up);
	}
	void processInput(Camera_Movement direction) {
		if (direction == FORWARD)
			Position += Front * 0.1f;
		if (direction == BACKWARD)
			Position -= Front * 0.1f;
		if (direction == LEFT)
			Position -= Right * 0.1f;
		if (direction == RIGHT)
			Position += Right * 0.1f;
	}
	void mouse_input(float xoffset, float yoffset) {
		xoffset *= 0.1f;
		yoffset *= 0.1f;
		Yaw += xoffset;
		Pitch += yoffset;
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
		updateCameraVectors();
	}
private:
	void updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front,WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));;
	}
};
#endif