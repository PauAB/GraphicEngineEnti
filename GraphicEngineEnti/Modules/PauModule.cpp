#include "precompiledHeader.h"
#include "PauModule.h"

extern Mesh cube;
extern Mesh sphere;

PauModule::PauModule()
{

}

PauModule::~PauModule()
{

}

void PauModule::start()
{
	cam = new Camera();

	setCameraParams(cam,
		(float)glm::radians(45.0f),
		glm::vec3(0, 0, 10)
		);

	window = Engine::get().getRender().getCtxWindow()->getWindowGL();
	Engine::get().SetCamera(cam);

	cam->getTransform().setPosition(0, 0, 10);

	pos = cam->getPosition();
	front = glm::vec3(0, 0, 1);

	cubetransform.setPosition(glm::vec3(0, 0, 0));
	quad1.setPosition(glm::vec3(-5, 0, 0));
	quad2.setPosition(glm::vec3(5, 0, 0));
	quad3.setPosition(glm::vec3(0, 5, 0));
	//spheretransform.setPosition(glm::vec3(0, -5, 0));

	initCameras();
}

void PauModule::stop()
{
	for (int i = 0; i < cameras.size(); i++)
		delete cameras[i];
}

void PauModule::update(float elapsed)
{
	glm::vec3 euler = quad3.getEulerAngles();
	float num = euler.y + glfwGetTime() * 10.0f;

	quad1.setEulerAngles(euler.x, euler.y, euler.z + glfwGetTime());

	quad2.setEulerAngles(euler.x, glm::radians(num), euler.z);
	
	quad3.setEulerAngles(glm::radians(euler.x + glfwGetTime()), euler.y, euler.z);
	
	cameraController();
	cam->lookAt(pos, target);
}

void PauModule::initCameras()
{
	cameras.push_back(new Camera());
	cameras.push_back(new Camera());
	cameras.push_back(new Camera());
	cameras.push_back(new Camera());
	cameras.push_back(cam);

	setCameraParams(cameras[0],
		(float)glm::radians(15.0f),
		glm::vec3(14.5f, 0, 1.42883f)
		);
	setCameraParams(cameras[1],
		(float)glm::radians(45.0f),
		glm::vec3(0, 0, 20)
		);
	setCameraParams(cameras[2],
		(float)glm::radians(20.0f),
		glm::vec3(-20, 30, 20)
		);
	setCameraParams(cameras[3],
		(float)glm::radians(20.0f),
		glm::vec3(20, 30, 20)
		);

	Engine::get().SetCamera(cameras[0]);
}

void PauModule::cameraController()
{
	front = glm::vec3(0, 0, 1);
	glm::vec3 euler = cam->getTransform().getEulerAngles();

	// Left ------------->
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		newAngle += glm::radians(5.0f);
	// Right ------------>
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		newAngle -= glm::radians(5.0f);
	// Forward ---------->
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pos -= 0.1f * cam->getFront();
	// Backward --------->
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos += 0.1f * cam->getFront();
	// Up --------------->
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		newPitch += glm::radians(5.0f);
	// Down ------------->
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		newPitch += glm::radians(-5.0f);

	if (canPress && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		currentCamera++;
		currentCamera %= cameras.size();
		Engine::get().SetCamera(cameras[currentCamera]);
		canPress = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
		canPress = true;

	target = pos + glm::vec3(sinf(newAngle) * cosf(newPitch),
		sinf(newPitch), cosf(newAngle) * cosf(newPitch));
	glm::vec3 front = cam->getFront();

	cam->getTransform().setPosition(pos);
}

void PauModule::renderDebug()
{
	Engine::get().setModelObjectConstants(quad1.asMatrix(), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	quad.activateAndRender();

	Engine::get().setModelObjectConstants(quad2.asMatrix(), glm::vec4(1, 0, 0, 1));
	quad.render();

	Engine::get().setModelObjectConstants(quad3.asMatrix(), glm::vec4(0, 1, 0, 1));
	quad.render();

	Engine::get().setModelObjectConstants(cubetransform.asMatrix(), glm::vec4(0, 0, 0, 1));
	cube.activateAndRender();

	//Engine::get().setModelObjectConstants(spheretransform.asMatrix(), glm::vec4(1, 1, 1, 1));
	//sphere.activateAndRender();
}

void PauModule::setCameraParams(Camera* cam, float perspective, glm::vec3 lookAt)
{
	glm::vec2 viewport = Engine::get().getRender().getViewport();

	cam->setProjectionParams(perspective,
		viewport.x / viewport.y,
		0.1f,
		500.0f
		);

	cam->lookAt(lookAt, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}