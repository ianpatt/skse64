#pragma once

#include "skse64/ScaleformTypes.h"

class GFxValue;
class GFxFunctionHandler;

class GFxMovieView : public GRefCountBase
{
public:
	GFxMovieView();
	virtual ~GFxMovieView();

	enum ScaleModeType
	{
		kNoScale,	// The size of SWF content is fixed
		kShowAll,	// Makes the entire SWF content visible in the viewport without distortion while maintaining the original aspect ratio
		kExactFit,	// Makes the entire SWF content visible in the viewport without trying to preserve the original aspect ratio
		kNoBorder	// Scales the SWF content to fill whole viewport area, without distortion but possibly with some cropping while maintaining original aspect ratio
	};

	virtual UInt32	Unk_01(void);
	virtual UInt32	Unk_02(void);
	virtual UInt32	Unk_03(void);
	virtual void	GotoFrame(UInt32 idx);
	virtual bool	GotoLabeledFrame(const char * label, UInt32 unk);
	virtual void	SetPause(UInt32 pause);
	virtual UInt32	GetPause(void);
	virtual void	Unk_08(UInt32 unk);
	virtual UInt32	Unk_09(void);
	virtual bool	Unk_0A(void);
	virtual void	CreateString(GFxValue * value, const char * str);
	virtual void	CreateWideString(GFxValue * value, const wchar_t * str);
	virtual void	CreateObject(GFxValue * value, const char * className = NULL, GFxValue * args = NULL, UInt32 numArgs = 0);
	virtual void	CreateArray(GFxValue * value);
	virtual void	CreateFunction(GFxValue * value, GFxFunctionHandler * callback, void * refcon = NULL);
	virtual void	SetVariable(const char * name, GFxValue * value, UInt32 flags);
	virtual bool	GetVariable(GFxValue * value, const char * name);
	virtual bool	SetArray(UInt32 type, const char * name, UInt32 offset, void * buf, UInt32 len, UInt32 flags);
	virtual bool	ResizeArray(const char * name, UInt32 len, UInt32 flags);
	virtual UInt32	GetArrayLen(const char * name);	// 050
	virtual bool	GetArray(UInt32 type, const char * name, UInt32 offset, void * buf, UInt32 len);
	virtual bool	Invoke(const char * name, GFxValue * result, GFxValue * args, UInt32 numArgs);
	virtual bool	Invoke(const char * name, GFxValue * result, const char * fmt, ...);
	virtual bool	Invoke_v(const char * name, GFxValue * result, const char * fmt, va_list args);	// 060
	virtual void	SetViewport(const GViewport & viewDesc);
	virtual void	GetViewport(GViewport *pviewDesc) const;
	virtual void	Unk_1B(UInt32 unk);  // SE: 0D8 (Skyrim32: 06C)
	virtual void	Unk_1C(void);	// 070
	virtual void	Unk_1D(void);
	virtual void	Unk_1E(void);
	virtual GRectF	GetVisibleFrameRect(void);
	virtual void	Unk_20(void);	// 080
	virtual void	Unk_21(void);
	virtual void	Unk_22(void);
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);	// 090
	virtual void	Unk_25(void);
	virtual void	Render(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);	// 0A0
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(void);	// 0B0
	virtual UInt32	HandleEvent(void* data);
	virtual void	Unk_2E(void);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(void);
	virtual void	Unk_3C(void);
	virtual void	Unk_3D(void);
	virtual void	Unk_3E(void);
	virtual void	Unk_3F(void);
	virtual void	Unk_40(void);
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual void	Unk_44(void);
	virtual void	ForceCollectGarbage(void);
	// more
};
