#include "stdafx.h"

// Includes

// General
#include "Wmo_Light.h"

WMOLight::WMOLight(File& f)
{
	f.ReadBytes(&lightDef, WMOLightDef::__size);

	lightDef.pos = vec3(lightDef.pos.x, lightDef.pos.z, -lightDef.pos.y);

	fcolor = fromARGB(lightDef.color);
	fcolor *= lightDef.intensity;
	fcolor.w = 1.0f;

	//Debug::Warn("Atten = %f, %f", lightDef.attenStart, lightDef.attenEnd);
}

void WMOLight::setup(GLint light)
{
	GLfloat LightAmbient[] = {0, 0, 0, 1.0f};
	GLfloat LightPosition[] = {lightDef.pos.x, lightDef.pos.y, lightDef.pos.z, 0.0f};

	glLightfv(light, GL_AMBIENT, LightAmbient);
	glLightfv(light, GL_DIFFUSE, glm::value_ptr(fcolor));
	glLightfv(light, GL_POSITION, LightPosition);

	glEnable(light);
}

void WMOLight::setupOnce(GLint light, vec3 dir, vec4 lcol)
{
	vec4 position(dir, 0);

	vec4 ambient = lcol * 0.3f;
	vec4 diffuse = lcol;

	glLightfv(light, GL_AMBIENT, glm::value_ptr(ambient));
	glLightfv(light, GL_DIFFUSE, glm::value_ptr(diffuse));
	glLightfv(light, GL_POSITION, glm::value_ptr(position));

	glEnable(light);
}