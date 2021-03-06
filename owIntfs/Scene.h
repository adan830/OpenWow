#pragma once

// FORWARD BEGIN
class Camera;
class SceneNode;
// FORWARD END

__interface 
	__declspec(uuid("71DC4B4D-45AB-4030-95FD-6C7C3B061526"))
	ISceneManager : public IManager
{
	SceneNode* getIntersectedNode();

	void setCamera(Camera* _camera);
	Camera* getCamera();
	void setFrustrumCamera(Camera* _camera);
	Camera* getFrustrumCamera();
};