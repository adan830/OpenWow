#pragma once

#include "UIElement.h"

class UIButton : public UIElement
{
    typedef UIElement base;
public:
    UIButton(IUIMgr* _uimgr);
    virtual ~UIButton();

    void Init(cvec2 _position, Image* _image);

    void SetAction(Function* _onPress);
    void CallAction(int _button, int _mods, cvec2 _mousePos);

    virtual void Show();
    virtual void Hide();

    On_Mouse_Entered_V;
    On_Mouse_Leaved_V;

	// IInputrListener
    bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
    bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;

protected:
    Function* onPressFunction;

    enum ButtonStatus
    {
        BUTTON_NORMAL = 0,
        BUTTON_HOVER,
        BUTTON_PRESSED,
        BUTTON_DISABLED
    };
    ButtonStatus buttonStatus;
};

// Set action
/*#define SETBUTTONACTION(_uielement, _funcName)\
{\
    if(_uielement != nullptr){\
        UIButton* elemAsBtn = dynamic_cast<UIButton*>(_uielement);\
        if (elemAsBtn != nullptr) { \
            elemAsBtn->SetAction(FUNCTION_CLASS_SA_Builder(_funcName));\
        }\
    }\
}*/

#define SETBUTTONACTION_ARG(_uielement, _className, _instance, _funcName, argType, arg)\
{\
	if(_uielement != nullptr) \
	{\
		UIButton* elemAsBtn = dynamic_cast<UIButton*>(_uielement);\
		if (elemAsBtn != nullptr) \
		{ \
			elemAsBtn->SetAction(FUNCTION_CLASS_SA_Builder(_className, _instance, _funcName, argType, arg));\
		}\
	}\
}
