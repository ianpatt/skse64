#pragma once

// Old interface for compatibility with plugins
class UIDelegate_v1
{
public:
	virtual void	Run(void)		= 0;
	virtual void	Dispose(void)	= 0;
};

void Hooks_UI_Commit(void);
