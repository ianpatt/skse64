#pragma once

#include "skse64/ScaleformAPI.h"
#include "skse64/ScaleformTypes.h"
#include "skse64/ScaleformValue.h"
#include "skse64_common/Utilities.h"
#include <typeinfo>
#include <map>

// see RakNet/DependentExtensions/GFx3/FxGameDelegate.h

class FxDelegateHandler;
class GFxMovieView;
class GFxMovieRoot;

// the double in GFxValue is causing very weird alignment issues
//#pragma pack (push, 4)


//STATIC_ASSERT(sizeof(GFxValue) == 0x18);

//#pragma pack (pop)

class GFxFunctionHandler : public GRefCountBase
{
public:
	GFxFunctionHandler();
	virtual ~GFxFunctionHandler();

	// 1C
	class Args
	{
	public:
		GFxValue		* result;	// 00
		GFxMovieView	* movie;	// 04
		GFxValue		* thisObj;	// 08
		GFxValue		* unk0C;	// 0C
		GFxValue		* args;		// 10
		UInt32			numArgs;	// 14
		void			* refCon;	// 18
	};

	virtual void	Invoke(Args * args) = 0;

	MEMBER_FN_PREFIX(GFxFunctionHandler);
};

typedef std::map <const std::type_info *, GFxFunctionHandler *>	FunctionHandlerCache;
extern FunctionHandlerCache g_functionHandlerCache;

template <typename T>
void RegisterFunction(GFxValue * dst, GFxMovieView * movie, const char * name)
{
	// either allocate the object or retrieve an existing instance from the cache
	GFxFunctionHandler	* fn = NULL;

	// check the cache
	FunctionHandlerCache::iterator iter = g_functionHandlerCache.find(&typeid(T));
	if(iter != g_functionHandlerCache.end())
		fn = iter->second;

	if(!fn)
	{
		// not found, allocate a new one
		fn = new T;

		// add it to the cache
		// cache now owns the object as far as refcounting goes
		g_functionHandlerCache[&typeid(T)] = fn;
	}

	// create the function object
	GFxValue	fnValue;
	movie->CreateFunction(&fnValue, fn);

	// register it
	dst->SetMember(name, &fnValue);
}

// 08
class FxResponseArgsBase
{
public:
	FxResponseArgsBase() { };
	virtual ~FxResponseArgsBase() { };

	virtual UInt32	GetValues(GFxValue ** params) = 0;
//	void	** _vtbl;	// 00
};

// 08 + T_numArgs * 0x10
template <UInt32 T_numArgs>
class FxResponseArgs : public FxResponseArgsBase
{
public:
	FxResponseArgs() : curArg(T_numArgs + 1) { };
	virtual ~FxResponseArgs() { };

	virtual UInt32	GetValues(GFxValue ** params)
	{
		*params = &args[0];
		return curArg;
	}

	GFxValue	args[T_numArgs + 1];	// 08 - first entry must be empty for some reason
	UInt32		curArg;					// 20 - offsets assume one argument
};

// ?
class FxResponseArgsList : public FxResponseArgsBase
{
public:
	FxResponseArgsList();
	virtual ~FxResponseArgsList();

	void Clear();

	virtual UInt32	GetValues(GFxValue ** params)
	{
		*params = args.values;
		return args.size;
	}

	GArray <GFxValue>	args;
};

// 20
class FxDelegateArgs
{
public:
	FxDelegateArgs();
	~FxDelegateArgs();

	GFxValue			responseID;
	FxDelegateHandler	* menu;		// 10
	GFxMovieView		* movie;	// 14
	GFxValue			* args;		// 18
	UInt32				numArgs;	// 1C
};

// 08
class FxDelegateHandler : public GRefCountBase
{
public:
	//FxDelegateHandler();
	//~FxDelegateHandler();

	typedef void (* Callback)(const FxDelegateArgs & params);

	class CallbackProcessor
	{
	public:
		CallbackProcessor();
		virtual ~CallbackProcessor();

		virtual void	Process(const GString & name, Callback method);
	};

	class AddCallbackVisitor : public CallbackProcessor
	{
	public:
		AddCallbackVisitor();
		virtual ~AddCallbackVisitor();

		FxDelegateHandler	* parent;
		void				* callbackHashTable;
	};

	virtual void	Accept(CallbackProcessor * processor) = 0;
};

extern RelocAddr<FxDelegateHandler::Callback> PlaySoundCallback;
