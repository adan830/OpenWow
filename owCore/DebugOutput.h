#pragma once

class CDebugOutput : public IDebugOutput
{
public:
	 void PushMessage(cstring _message, IDebugOutput::DebugMessageType _type) override;
	 void PushMessage(IDebugOutput::DebugMessageType _type, const char* _message, va_list& _vaList) override;

protected:
	virtual void Print(string _messageFmt, CDebugOutput::DebugMessageType _type) = 0;
};
