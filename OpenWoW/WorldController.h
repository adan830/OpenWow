#pragma once

class WorldController
{
public:
	WorldController();
	~WorldController();

	inline MapController* Map() { return m_MapController; }
	inline EnvironmentManager* EnvM() { return m_EnvironmentManager; }
	
	void EnterMap(vec3 _pos);
	void EnterMap(int32 x, int32 z);
	
private:
	MapController*       m_MapController;
	CM2_Manager*         m_MDXsManager;
	WMOsManager*         m_WMOsManager;
	EnvironmentManager*  m_EnvironmentManager;
};

extern WorldController* __World;
#define _World __World