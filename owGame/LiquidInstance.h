#pragma once

#include "Liquid.h"

class Liquid_Instance : public SceneNode
{
public:
	Liquid_Instance(SceneNode* _parent, Liquid* _liquidObject, vec3 _position);
	virtual ~Liquid_Instance() {};

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

private:
	Liquid*	m_Object;

	//--
	CGroupQuality&		m_QualitySettings;
};