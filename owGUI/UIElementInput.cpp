#include "stdafx.h"

// Include
#include "UIMgr.h"

// General
#include "UIElement.h"

On_Mouse_Entered(UIElement)
{
    if (m_IsHided)
    {
        return;
    }

    //Log::Info("Mouse Entered [%s]", GetName().c_str());
}

void UIElement::OnMouseMoved(cvec2 _mousePos)
{
    if (m_IsHided)
    {
        return;
    }

    // This
    //Log::Info("Mouse moved [%s]", GetName().c_str());

    // Childs
    for (auto it : m_Childs)
    {
        bool beforeMoveSelection = it->IsSelected();
        bool afterMoveSelection = it->CheckSelection(_mousePos);

        if (!beforeMoveSelection && afterMoveSelection)
        {
            it->OnMouseEntered();
        }
        else if (beforeMoveSelection && !afterMoveSelection)
        {
            it->OnMouseLeaved();
        }
        else if (beforeMoveSelection && afterMoveSelection)
        {
            it->OnMouseMoved(_mousePos - it->GetPosition());
        }
        else
        {
            // Mouse outside
        }
    }
}

On_Mouse_Leaved(UIElement)
{
    if (m_IsHided)
    {
        return;
    }

    //Log::Info("Mouse Leaved [%s]", GetName().c_str());
}

bool UIElement::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Log::Info("Mouse pressed [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->IsSelected())
        {
            if (it->OnMouseButtonPressed(_button, _mods, _mousePos - it->GetPosition()))
            {
                result = true;
            }
        }
    }

    return result;
}

bool UIElement::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Log::Info("Mouse released [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->OnMouseButtonReleased(_button, _mods, _mousePos - it->GetPosition()))
        {
            result = true;
        }
    }
    return result;
}

bool UIElement::OnMouseWheel(int _yoffset)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Log::Info("Mouse scrolled [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->IsSelected())
        {
            if (it->OnMouseWheel(_yoffset))
            {
                result = true;
            }
        }
    }

    return result;
}

bool UIElement::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Log::Info("Keyboard pressed [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->OnKeyboardPressed(_key, _scancode, _mods))
        {
            result = true;
        }
    }
    return result;
}

bool UIElement::OnKeyboardReleased(int _key, int _scancode, int _mods)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Log::Info("Keyboard released [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->OnKeyboardReleased(_key, _scancode, _mods))
        {
            result = true;
        }
    }
    return result;
}