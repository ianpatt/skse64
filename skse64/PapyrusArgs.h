#pragma once

#include "skse64_common/Utilities.h"
#include "skse64/PapyrusVM.h"

#include <vector>

class VMState;
class VMValue;
class VMClassRegistry;
struct StaticFunctionTag;
class EffectSetting;

class VMArgList
{
public:
	VMArgList();
	~VMArgList();

	MEMBER_FN_PREFIX(VMArgList);
	// 20AE20213D7A9C7AB3491192BCCB2467282AE266+44
	DEFINE_MEMBER_FN(GetOffset, UInt32, 0x01244970, VMState * state);
	// 20AE20213D7A9C7AB3491192BCCB2467282AE266+62
	DEFINE_MEMBER_FN(Get, VMValue *, 0x012449D0, VMState * state, UInt32 idx, UInt32 offset);
};

template <typename T>
class VMArray
{
public:
	VMValue::ArrayData	* arr;
	UInt32 Length() const				{ return arr ? arr->len : 0; }
	void Get(T * dst, const UInt32 idx)	{ UnpackValue(dst, arr->GetData()+idx); }
	void Set(T * src, const UInt32 idx)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		PackValue(arr->GetData()+idx, src, registry);
	}
};

template<typename T>
class VMResultArray : public std::vector<T>
{
public:
	void PackArray(VMValue::ArrayData * data, VMClassRegistry * registry)
	{
		// Copy the contents from the reference array to the VM array
		UInt32 i = 0;
		for(std::vector<T>::iterator it = begin(); it != end(); ++it, i++) {
			VMValue * value = data->GetData() + i;
			PackValue(value, (T*)&(*it), registry);
			value->type = GetTypeID<T>(registry); // Always pack the type, even if empty data
		}
	}
};

template <typename T>
void PackValue(VMValue * dst, VMResultArray<T> * src, VMClassRegistry * registry)
{
	// Clear out old contents if any
	dst->SetNone();
	if(src->size() > 0) {
		VMValue::ArrayData * data = NULL;
		// Request the VM allocate a new array
		registry->CreateArray(dst, src->size(), &data);
		if(data) {
			src->PackArray(data, registry);

			// Set the appropriate TypeID and assign the new data array
			dst->type = GetTypeID<VMResultArray<T>>(registry);
			dst->data.arr = data;
		}
	}

	// Clear the temp contents of the reference array
	src->clear();
}

template <typename T>
void UnpackValue(VMArray<T*> * dst, VMValue * src)
{
	UnpackArray(dst, src, GetTypeIDFromFormTypeID(T::kTypeID, (*g_skyrimVM)->GetClassRegistry()) | VMValue::kType_Identifier);
}

template <typename T>
void PackValue(VMValue * dst, T * src, VMClassRegistry * registry);

template <typename T>
void UnpackValue(T * dst, VMValue * src);

template <> void PackValue <void>(VMValue * dst, void * src, VMClassRegistry * registry);
template <> void PackValue <UInt32>(VMValue * dst, UInt32 * src, VMClassRegistry * registry);
template <> void PackValue <SInt32>(VMValue * dst, SInt32 * src, VMClassRegistry * registry);
template <> void PackValue <float>(VMValue * dst, float * src, VMClassRegistry * registry);
template <> void PackValue <bool>(VMValue * dst, bool * src, VMClassRegistry * registry);
template <> void PackValue <BSFixedString>(VMValue * dst, BSFixedString * src, VMClassRegistry * registry);

void PackHandle(VMValue * dst, void * src, UInt32 typeID, VMClassRegistry * registry);

template <typename T>
void PackValue(VMValue * dst, T ** src, VMClassRegistry * registry)
{
	typedef std::remove_pointer <T>::type	BaseType;
	PackHandle(dst, *src, BaseType::kTypeID, registry);
}

template <> void UnpackValue <float>(float * dst, VMValue * src);
template <> void UnpackValue <UInt32>(UInt32 * dst, VMValue * src);
template <> void UnpackValue <SInt32>(SInt32 * dst, VMValue * src);
template <> void UnpackValue <bool>(bool * dst, VMValue * src);
template <> void UnpackValue <BSFixedString>(BSFixedString * dst, VMValue * src);

template <> void UnpackValue <VMArray<float>>(VMArray<float> * dst, VMValue * src);
template <> void UnpackValue <VMArray<UInt32>>(VMArray<UInt32> * dst, VMValue * src);
template <> void UnpackValue <VMArray<SInt32>>(VMArray<SInt32> * dst, VMValue * src);
template <> void UnpackValue <VMArray<bool>>(VMArray<bool> * dst, VMValue * src);
template <> void UnpackValue <VMArray<BSFixedString>>(VMArray<BSFixedString> * dst, VMValue * src);

void * UnpackHandle(VMValue * src, UInt32 typeID);

template <typename T>
void UnpackValue(T ** dst, VMValue * src)
{
	*dst = (T *)UnpackHandle(src, T::kTypeID);
}

template <typename T>
void UnpackArray(VMArray<T> * dst, VMValue * src, const UInt64 type)
{
	VMValue::ArrayData * arrData;

	if (src->type != type || (arrData = src->data.arr, !arrData))
	{
		dst->arr = NULL;
		return;
	}

	dst->arr = arrData;
}

UInt64 GetTypeIDFromFormTypeID(UInt32 formTypeID, VMClassRegistry * registry);

template<typename T>
struct IsArrayType
{
	enum { value = 0 };
	typedef T TypedArg;
};

template<typename T>
struct IsArrayType<VMArray<T*>>
{
	enum { value = 1 };
	typedef T TypedArg;
};

template<typename T>
struct IsArrayType<VMResultArray<T*>>
{
	enum { value = 1 };
	typedef T TypedArg;
};

template <typename T>
auto GetTypeID(VMClassRegistry* registry) -> std::enable_if_t<!std::is_pointer<T>::value && !IsArrayType<T>::value, UInt64>;

template <> UInt64 GetTypeID <void>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <UInt32>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <SInt32>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <int>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <float>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <bool>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <BSFixedString>(VMClassRegistry * registry);

template <> UInt64 GetTypeID <VMArray<UInt32>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMArray<SInt32>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMArray<int>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMArray<float>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMArray<bool>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMArray<BSFixedString>>(VMClassRegistry * registry);

template <> UInt64 GetTypeID <VMResultArray<UInt32>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMResultArray<SInt32>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMResultArray<int>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMResultArray<float>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMResultArray<bool>>(VMClassRegistry * registry);
template <> UInt64 GetTypeID <VMResultArray<BSFixedString>>(VMClassRegistry * registry);

template<typename T>
auto GetTypeID(VMClassRegistry * registry)
-> std::enable_if_t<IsArrayType<T>::value, UInt64>
{
	return GetTypeIDFromFormTypeID(IsArrayType<T>::TypedArg::kTypeID, registry) | VMValue::kType_Identifier;
}

template<typename T>
auto GetTypeID(VMClassRegistry * registry)
-> std::enable_if_t<std::is_pointer<T>::value && !IsArrayType<std::remove_pointer<T>>::value, UInt64>
{
	return GetTypeIDFromFormTypeID(std::remove_pointer<T>::type::kTypeID, registry);
}

template <class T>
struct IsStaticType
{
	enum { value = 0 };
};

template <>
struct IsStaticType <StaticFunctionTag>
{
	enum { value = 1 };
};
