#pragma once

#include "DayNightCycle.h"
#include "SkyManager.h"

//

class EnvironmentManager : public IUpdatable, public CRenderable3DObject
{
public:
	EnvironmentManager();
	~EnvironmentManager();

	void InitSkies(DBC_MapRecord* _mapRecord);
	void outdoorLighting();
	void SetAmbientLights(bool on);
	void SetFog();

	// IUpdatable
	void Input(double _time, double _dTime) override {}
	void Update(double _Time, double _deltaTime) override;

	// IRenderable3D
	void PreRender3D(double t, double dt) override;
	void Render3D() override {}
	void PostRender3D() override {}

public:
	vec4 m_OutdoorAmbientColor;
	vec4 m_OutdoorDayDiffuseColor;
	vec4 m_OutdoorNightDiffuseColor;
	vec4 m_OutdoorSpecularColor;
	

public:
	float animtime;
	int globalTime;


	bool m_HasSky;
	WowTime m_GameTime;


	SmartPtr<SkyManager> skies;

	SmartPtr<DayNightCycle> dayNightCycle;
	DayNightPhase dayNightPhase;

	CGroupDistances& m_DistancesSettings;
	CGroupQuality& m_QualitySettings;
};