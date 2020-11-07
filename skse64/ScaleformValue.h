#pragma once

#include "skse64/ScaleformTypes.h"
#include "skse64_common/Utilities.h"

class GFxMovieRoot;

// 10
class GFxValue
{
public:
	GFxValue()
		:objectInterface(NULL), type(kType_Undefined) { }
	~GFxValue();

	enum
	{
		kType_Undefined = 0,
		kType_Null,
		kType_Bool,
		kType_Number,
		kType_String,
		kType_WideString,
		kType_Object,
		kType_Array,
		kType_DisplayObject,

		kTypeFlag_Managed = 1 << 6,

		kMask_Type = 0x8F,	// not sure why it checks the top bit
	};

	union Data
	{
		double			number;
		bool			boolean;
		const char		* string;
		const char		** managedString;
		const wchar_t	* wideString;
		const wchar_t	** managedWideString;
		void			* obj;
	};

	//#pragma pack (push, 8)
	class DisplayInfo
	{
	public:
		DisplayInfo() : _varsSet(0) {}
		enum
		{
			kChange_x = (1 << 0),
			kChange_y = (1 << 1),
			kChange_rotation = (1 << 2),
			kChange_xscale = (1 << 3),
			kChange_yscale = (1 << 4),
			kChange_alpha = (1 << 5),
			kChange_visible = (1 << 6),
			kChange_z = (1 << 7),
			kChange_xrotation = (1 << 8),
			kChange_yrotation = (1 << 9),
			kChange_zscale = (1 << 10),
			kChange_FOV = (1 << 11),
			kChange_projMatrix3D = (1 << 12),
			kChange_viewMatrix3D = (1 << 13)
		};

		double		_x;
		double		_y;
		double		_rotation;
		double		_xScale;
		double		_yScale;
		double		_alpha;
		bool		_visible;
		double		_z;
		double		_xRotation;
		double		_yRotation;
		double		_zScale;
		double		_perspFOV;
		GMatrix3D	_viewMatrix3D;
		GMatrix3D	_perspectiveMatrix3D;
		UInt16		_varsSet;

		void SetX(double x) { SetFlags(kChange_x); _x = x; }
		void SetY(double y) { SetFlags(kChange_y); _y = y; }
		void SetRotation(double degrees) { SetFlags(kChange_rotation); _rotation = degrees; }
		void SetXScale(double xscale) { SetFlags(kChange_xscale); _xScale = xscale; }
		void SetYScale(double yscale) { SetFlags(kChange_yscale); _yScale = yscale; }
		void SetAlpha(double alpha) { SetFlags(kChange_alpha); _alpha = alpha; }
		void SetVisible(bool visible) { SetFlags(kChange_visible); _visible = visible; }
		void SetZ(double z) { SetFlags(kChange_z); _z = z; }
		void SetXRotation(double degrees) { SetFlags(kChange_xrotation); _xRotation = degrees; }
		void SetYRotation(double degrees) { SetFlags(kChange_yrotation); _yRotation = degrees; }
		void SetZScale(double zscale) { SetFlags(kChange_zscale); _zScale = zscale; }
		void SetFOV(double fov) { SetFlags(kChange_FOV); _perspFOV = fov; }
		void SetProjectionMatrix3D(const GMatrix3D *pmat)
		{
			if (pmat) {
				SetFlags(kChange_projMatrix3D);
				_perspectiveMatrix3D = *pmat;
			}
			else
				ClearFlags(kChange_projMatrix3D);
		}
		void SetViewMatrix3D(const GMatrix3D *pmat)
		{
			if (pmat) {
				SetFlags(kChange_viewMatrix3D);
				_viewMatrix3D = *pmat;
			}
			else
				ClearFlags(kChange_viewMatrix3D);
		}

		// Convenience functions
		void SetPosition(double x, double y) { SetFlags(kChange_x | kChange_y); _x = x; _y = y; }
		void SetScale(double xscale, double yscale) { SetFlags(kChange_xscale | kChange_yscale); _xScale = xscale; _yScale = yscale; }

		void SetFlags(UInt32 flags) { _varsSet |= flags; }
		void ClearFlags(UInt32 flags) { _varsSet &= ~flags; }
	};
	//#pragma pack (pop)

	// 4
	class ObjectInterface
	{
	public:
		GFxMovieRoot	* root;

		DEFINE_MEMBER_FN_3(HasMember, bool, 0x00ECA620, void * obj, const char * name, bool isDisplayObj);
		DEFINE_MEMBER_FN_4(SetMember, bool, 0x00ECCBA0, void * obj, const char * name, GFxValue * value, bool isDisplayObj);
		DEFINE_MEMBER_FN_3(DeleteMember, bool, 0x00EC9490, void * obj, const char * name, bool isDisplayObj);
		DEFINE_MEMBER_FN_4(GetMember, bool, 0x00ECA150, void * obj, const char * name, GFxValue * value, bool isDisplayObj);
		DEFINE_MEMBER_FN_6(Invoke, bool, 0x00ECA860, void * obj, GFxValue * result, const char * name, GFxValue * args, UInt32 numArgs, bool isDisplayObj);
		DEFINE_MEMBER_FN_6(AttachMovie, bool, 0x00EC83A0, void * obj, GFxValue * value, const char * symbolName, const char * instanceName, SInt32 depth, void * initArgs);
		DEFINE_MEMBER_FN_2(PushBack, bool, 0x00ECB300, void * obj, GFxValue * value);
		DEFINE_MEMBER_FN_3(SetText, bool, 0x00ECCCF0, void * obj, const char * text, bool html);
		//DEFINE_MEMBER_FN(PopBack, bool, 0x00000000, void * obj, GFxValue * value);
		DEFINE_MEMBER_FN_1(GetArraySize, UInt32, 0x00EC9BD0, void * obj);
		//DEFINE_MEMBER_FN(SetArraySize, bool, 0x00000000, void * obj, UInt32 size);
		DEFINE_MEMBER_FN_3(GetElement, bool, 0x00EC9F30, void * obj, UInt32 index, GFxValue * value);
		//DEFINE_MEMBER_FN(SetElement, bool, 0x00000000, void * obj, UInt32 index, GFxValue * value);
		DEFINE_MEMBER_FN_3(GotoLabeledFrame, bool, 0x00ECA570, void * obj, const char * frameLabel, bool stop);
		//DEFINE_MEMBER_FN(GotoFrame, bool, 0x00000000, void * obj, UInt32 frameNumber, bool stop);
		DEFINE_MEMBER_FN_2(GetDisplayInfo, bool, 0x00EC9C20, void * obj, DisplayInfo * displayInfo);
		DEFINE_MEMBER_FN_2(SetDisplayInfo, bool, 0x00ECBDB0, void * obj, DisplayInfo * displayInfo);

		DEFINE_MEMBER_FN_2(AddManaged_Internal, void, 0x00ECB080, GFxValue * value, void * obj);
		DEFINE_MEMBER_FN_2(ReleaseManaged_Internal, void, 0x00ECB0E0, GFxValue * value, void * obj);
	};

	ObjectInterface	* objectInterface;	// 00
	UInt32			type;				// 08
	Data			data;				// 10

	UInt32	GetType(void) const { return type & kMask_Type; }
	bool	IsManaged(void) const { return (type & kTypeFlag_Managed) != 0; }
	void	CleanManaged(void);
	void	AddManaged(void);

	bool	IsObject(void) const { return GetType() == kType_Object; }
	bool	IsDisplayObject(void) const { return GetType() == kType_DisplayObject; }

	bool			GetBool(void) const;
	const char *	GetString(void) const;
	const wchar_t *	GetWideString(void) const;
	double			GetNumber(void) const;

	void	SetUndefined(void);
	void	SetNull(void);
	void	SetBool(bool value);
	void	SetNumber(double value);
	void	SetString(const char * value);
	void	SetWideString(const wchar_t * value);

	UInt32	GetArraySize();
	bool	GetElement(UInt32 index, GFxValue * value);
	bool	HasMember(const char * name);
	bool	SetMember(const char * name, GFxValue * value);
	bool	GetMember(const char * name, GFxValue * value);
	bool	DeleteMember(const char * name);
	bool	Invoke(const char * name, GFxValue * result, GFxValue * args, UInt32 numArgs);
	bool	PushBack(GFxValue * value);
	bool	GetDisplayInfo(DisplayInfo * displayInfo);
	bool	SetDisplayInfo(DisplayInfo * displayInfo);
	bool	SetText(const char * text, bool html);
};
