#pragma once

#include "FontsManager.h"
#include "Image.h"

#include "UIElement.h"

class UIMgr : public IUIMgr, public IUIMgrEx, public IUpdatable, public IRenderableUI, public IInputListener
{
public:
	UIMgr();
	~UIMgr();

	// IUIMgr
	void AttachToRoot(UIElement* _element) override;
    void DetachFromRoot(UIElement* _element, bool _checkChilds = false) override;
    void AttachElementToParent(UIElement* _element, UIElement* _parent) override;

    UIElement* GetRootElement() const override { return m_RootElement; }
    void SetRootElement(UIElement* _element) override { m_RootElement = _element; }

	UIElement* GetFocus() const override { return m_FocusedElement; }
    void SetFocus(UIElement* _element) override;

	void RenderImage(vec2 _pos, Image* _image);
	void RenderImage(vec2 _pos, Image* _image, vec2 _size);

	void RenderText(vec2 _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, SharedFontPtr _font, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, SharedFontPtr _font, const Color& _color = COLOR_WHITE) const;

	FontsManager* FontsMgr() { return m_FontsMgr; }

	// IUIMgrEx
	string GetNewName();
	void SetForDetach(UIElement* _element) override;
	void SetForDelete(UIElement* _element) override;
	void DetachFromParent(UIElement* _element) override;
	void DeleteUIElement(UIElement* _element) override;

	// IUpdatable
	void Input(CInput* _input, double _time, double _dTime) override {}
	void Update(double _time, double _dTime) override;

	// IRenderableUI
	void RenderUI() override;

	// IInputListener
	void OnMouseMoved(cvec2 _mousePos) override;
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseWheel(int _yoffset) override;
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override;
	bool OnCharInput(uint32 _char) override;

protected:
	uint32              m_IDCounter;
    UIElement*          m_RootElement;
    UIElement*          m_FocusedElement;

	vector<UIElement*>  m_ObjectsToDetach;
	vector<UIElement*>  m_ObjectsToDelete;

private:
	FontsManager*		m_FontsMgr;
};
