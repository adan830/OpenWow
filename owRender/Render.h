#pragma once

class Module;
class GLFWBackend;

class RenderGL : public Module {
	friend GLFWBackend;
public:
	DEF_MODULE(RenderGL, OW_RENDER_DLL_API);

	OW_RENDER_DLL_API void Set3D();
	OW_RENDER_DLL_API void Set2D();

	OW_RENDER_DLL_API void RenderImage(const vec2& _pos, Image* _image);
	OW_RENDER_DLL_API void RenderImage(const vec2& _pos, Image* _image, const vec2& _size);
	OW_RENDER_DLL_API void RenderTexture(const vec2& _pos, Texture* _texture, const vec2& _size, const Rect<double>& _coords);
	OW_RENDER_DLL_API void RenderRectangle(const vec2& _pos, const vec2& _size, bool _filled, const Color& _color = COLOR_GREEN);

	OW_RENDER_DLL_API void RenderText(const vec2& _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	OW_RENDER_DLL_API void RenderText(const vec2& _pos, cstring _string, Font* _font, const Color& _color = COLOR_WHITE) const;
	OW_RENDER_DLL_API void RenderText(const vec2& _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	OW_RENDER_DLL_API void RenderText(const vec2& _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color = COLOR_WHITE) const;

	GLuint GetVertexBuffer() { return VertexBufferObject; }
	GLuint GetArrayBuffer() { return VertexArrayObject; }

public:
	inline vec2 GetWindowSize() const { return windowSize; }
	inline float GetAspectRatio() const { return aspectRatio; }

private:
	void OnWindowResized(uint32_t _width, uint32_t _height);

private:
	GLuint VertexArrayObject, VertexBufferObject;

	vec2 windowSize;
	float aspectRatio, aspectFactor;
};

#define _Render RenderGL::instance()

#define INVALID_OGL_VALUE 0xFFFFFFFF