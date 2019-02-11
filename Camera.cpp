#include "stdafx.h"
#include "Camera.h"

Camera::Camera() {
	cout << "Camera default constructor" << endl;
	this->camPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	this->camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->SetupFront();
	this->Update();
	this->keyMoveEnable = false;
}
void Camera::SetTarget(glm::vec3 target) {
	if (keyRotateEnable) {
		cout << "keyRotate disabled" << endl;
		system("pause");
		this->cameraKeyRotate(false);
	}
	this->camFront = target;
}
void Camera::KeyEvent(Keys key, float deltaTime) {
	if (!keyMoveEnable) return;
	float camSpeed = moveSpeed * deltaTime;
	if (key == W) this->camPosition += camSpeed * this->camFront;
	if (key == S) this->camPosition -= camSpeed * this->camFront;
	if (key == A) this->camPosition -= this->camRight * camSpeed;
	if (key == D) this->camPosition += this->camRight * camSpeed;
	if (key == TAB) this->camPosition.y += camSpeed;
	if (key == LSHIFT) this->camPosition.y -= camSpeed;
	if (!keyRotateEnable) return;
	if (key == UP) {
		this->Pitch += rotateSpeed;
		this->SetupFront();
		this->Update();
	}
	if (key == DOWN) {
		this->Pitch -= rotateSpeed;
		this->SetupFront();
		this->Update();
	}
	if (key == RIGHT) {
		this->Yaw += this->rotateSpeed;
		this->SetupFront();
		this->Update();
	}
	if (key == LEFT){
		this->Yaw -= this->rotateSpeed;
		this->SetupFront();
		this->Update();
	}
}
void Camera::Update() {
	this->camRight = glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 0.1f, 0.0f)));
	this->camUp = glm::normalize(glm::cross(camRight, camFront));
}
void Camera::SetupFront() {
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(this->Pitch)) * cos(glm::radians(this->Yaw));
	newFront.y = sin(glm::radians(this->Pitch));
	newFront.z = cos(glm::radians(this->Pitch)) * sin(glm::radians(this->Yaw));
	this->camFront = glm::normalize(newFront);
}