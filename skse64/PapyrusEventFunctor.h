#pragma once

#include <tuple>

#include "skse64/PapyrusVM.h"
#include "skse64/PapyrusArgs.h"

template <typename T_Base, typename... T_values>
class EventQueueFunctor : public IFunctionArguments
{
	static const UInt32 size = sizeof...(T_values);
public:
	EventQueueFunctor(BSFixedString & a_eventName, T_values... args)
		: m_eventName(a_eventName.data), m_args(args...) {}

	template <typename T_Base>
	void SendEvent(T_Base* base)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy* policy = registry->GetHandlePolicy();
		UInt64 handle = policy->Create(T_Base::kTypeID, base);

		registry->QueueEvent(handle, &m_eventName, this);
	}

	virtual bool	Copy(Output * dst)
	{
		dst->Resize(size);
		for_each_in_tuple<size, T_values...>(dst, m_args);
		return true;
	}

private:
	template<size_t I = 0, class ...P>
	typename std::enable_if<I == sizeof...(P)>::type
	for_each_in_tuple(Output * dst, std::tuple<P...> & parts)
	{

	}

	template<size_t I = 0, class ...P>
	typename std::enable_if<I < sizeof...(P)>::type
	for_each_in_tuple(Output * dst, std::tuple<P...> & parts)
	{
		PackValue(dst->Get(I), &std::get<I>(parts), (*g_skyrimVM)->GetClassRegistry());
		for_each_in_tuple<I + 1>(dst, parts);
	}

	BSFixedString			m_eventName;
	std::tuple<T_values...>	m_args;
};
