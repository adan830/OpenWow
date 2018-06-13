#include "stdafx.h"

// General
#include "WorldRender.h"



WorldRender::WorldRender(WorldController * _WorldContoller)	: 
	m_WorldContoller(_WorldContoller),
	groupVideo(GetSettingsGroup<CGroupVideo>()),
	groupQuality(GetSettingsGroup<CGroupQuality>())
{
	m_TestRenderBuffer = _Render->r.createRenderBuffer(groupVideo.windowSizeX, groupVideo.windowSizeY, R_TextureFormats::RGBA16F, true, 4, 0);

	m_TestCamera = new Camera;
	m_TestCamera->setupViewParams(Math::Pi / 4.0f, groupVideo.aspectRatio, 2.0f, 15000.0f);

	_Bindings->RegisterRenderable3DObject(this, 6);
	setVisible(true);
}

WorldRender::~WorldRender()
{
	_Bindings->UnregisterRenderable3DObject(this);
}

void WorldRender::PreRender3D()
{
	ADT_WMO_Instance::reset();
	ADT_MDX_Instance::reset();

	_PipelineGlobal->SetCamera(_Render->mainCamera);
	_PipelineGlobal->SetCameraFrustum(_Render->mainCamera);

	_Render->TechniquesMgr()->PreRender3D();
}

void WorldRender::Render3D()
{
	_Render->BindRBs();
}

void WorldRender::PostRender3D()
{
	_Render->UnbindRBs();

	// Postprocess
	RenderPostprocess();

	// Result pass
	/*_Render->rb->resetRenderBuffer(); // HACK!!!! MOVE RESET TO RenderDevice
	_Render->r.setTexture(0, _Render->rbFinal->getRenderBufferTex(0), 0, R_TextureUsage::Texture);
	_Render->r.clear(CLR_COLOR_RT0 | CLR_DEPTH);
	DSResultQuad();*/

	//
	// SECONDS PASS
	//

	/*ADT_WMO_Instance::reset();
	ADT_MDX_Instance::reset();

	// Conf test camera
	m_TestCamera->Position = _Render->mainCamera->Position + vec3(0, 1, 0) * 1000.0f;
	m_TestCamera->Roll = _Render->mainCamera->Roll;
	m_TestCamera->Pitch = -90.0f;
	m_TestCamera->Update();

	// Test frame
	_PipelineGlobal->SetCamera(m_TestCamera);

	// Geometry pass
	m_TestRenderBuffer->setRenderBuffer();
	_Render->r.clear();
	RenderGeom();
	//_PipelineGlobal->RenderCamera(_CameraFrustum);

	m_TestRenderBuffer->resetRenderBuffer();*/
}

void WorldRender::RenderPostprocess()
{
	_Render->PostprocessSimple();

	/*DirectionalLight light;
	light.Direction = vec3(_World->EnvM()->dayNightPhase.dayDir);
	light.ambient = _World->EnvM()->m_SkyManager->GetColor(LightColors::LIGHT_COLOR_GLOBAL_AMBIENT);
	light.diffuse = _World->EnvM()->m_SkyManager->GetColor(LightColors::LIGHT_COLOR_GLOBAL_DIFFUSE);
	light.specular = vec3(1.0f, 1.0f, 1.0f);
	DSDirectionalLightPass(light);*/

	if (groupQuality.drawfog)
	{
		DSFogRenderPass();
	}
}

void WorldRender::DSDirectionalLightPass(DirectionalLight& _light)
{
	_Render->TechniquesMgr()->Postprocess_Light_Direction->Bind();
	_Render->TechniquesMgr()->Postprocess_Light_Direction->SetCameraPos(_Camera->Position);

	_Render->TechniquesMgr()->Postprocess_Light_Direction->SetDirectionalLight(_light);

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuad();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->TechniquesMgr()->Postprocess_Light_Direction->Unbind();
}

void WorldRender::DSFogRenderPass()
{
	_Render->TechniquesMgr()->Postprocess_Fog->Bind();
	_Render->TechniquesMgr()->Postprocess_Fog->SetCameraPos(_Camera->Position);

	_Render->TechniquesMgr()->Postprocess_Fog->SetFogDistance(m_WorldContoller->EnvM()->m_SkyManager->GetFog(LIGHT_FOG_DISTANCE));
	_Render->TechniquesMgr()->Postprocess_Fog->SetFogModifier(m_WorldContoller->EnvM()->m_SkyManager->GetFog(LIGHT_FOG_MULTIPLIER));
	_Render->TechniquesMgr()->Postprocess_Fog->SetFogColor(m_WorldContoller->EnvM()->m_SkyManager->GetColor(LIGHT_COLOR_FOG));

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuad();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->TechniquesMgr()->Postprocess_Fog->Unbind();
}

void WorldRender::DSResultQuad()
{
	_Render->TechniquesMgr()->UI_Texture->Bind();

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuadVT();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->TechniquesMgr()->UI_Texture->Unbind();
}