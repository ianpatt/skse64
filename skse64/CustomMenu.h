#pragma once

#include "skse64/GameMenus.h"
#include "skse64/ScaleformLoader.h"

class CustomMenuCreator
{
public:
	static IMenu* Create(void);
	static void SetSwfPath(const char* path);

private:
	CustomMenuCreator();

	static std::string swfPath_;
};

class CustomMenu : public IMenu
{
public:
	CustomMenu(const char* swfPath);
	virtual void Accept(CallbackProcessor * processor);
	virtual void Render(void);
};
