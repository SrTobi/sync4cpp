#pragma once
#ifndef _SYNC4CPP_SYNCHRONIZED_HPP
#define _SYNC4CPP_SYNCHRONIZED_HPP

#include "sync4cpp-plain.hpp"
#include "sync4cpp-utils.hpp"

namespace sync4cpp {

template<typename ValueType, typename Mutex>
class synchronized;


namespace detail {

	template<typename InternalMutex, typename ValueType>
	struct synchronized_decor
		: public decor<synchronized_decor<InternalMutex, ValueType> >
	{
		typedef synchronized<ValueType, InternalMutex> sync_type;

	private:
		static ValueType& ExtractValue(sync_type& mutex)
		{
			return mutex.mValue;
		}
	public:
		SYNC4CPP_PLACE_DEFAULT_MODIFIER_FACTORY(typename get_default_modifier_factory<InternalMutex>::type);
		template<typename Mutex, typename Modifier>
		struct guard
		{
			typedef typename sync4cpp::guard<assignment<syncable<InternalMutex>, Modifier>>::type base_type;
			typedef struct Wrapper
				: public base_type
			{
				Wrapper(const assignment<Mutex, Modifier>& as)
					: base_type(as[(syncable<InternalMutex>&)as.mutex()])
					, value(ExtractValue(as.mutex()))
				{}

				ValueType& value;
			} type;
		};
	};
}


template<typename ValueType, typename Mutex>
class synchronized
	: public detail::synchronized_decor<Mutex, ValueType>
	, public syncable<Mutex>
{
	template<typename InternalMutex, typename ValueType>
	friend struct detail::synchronized_decor;

	inline synchronized(const synchronized& p)
	{

	}
public:
	typedef syncable<Mutex>						base_type;
	typedef typename base_type::mutex_type		mutex_type;
	typedef ValueType							value_type;

	synchronized()
	{}

	template<typename T1>
	inline synchronized(const T1& p1)
		: mValue(p1)
	{}

	template<typename T1, typename T2>
	inline synchronized(const T1& p1, const T2& p2)
		: mValue(p1, p2)
	{}

	template<typename T1, typename T2, typename T3>
	inline synchronized(const T1& p1, const T2& p2, const T3& p3)
		: mValue(p1, p2, p3)
	{}

	template<typename T1, typename T2, typename T3, typename T4>
	inline synchronized(const T1& p1, const T2& p2, const T3& p3, const T4& p4)
		: mValue(p1, p2, p3, p4)
	{}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	inline synchronized(const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5)
		: mValue(p1, p2, p3, p4, p5)
	{}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	inline synchronized(const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6)
		: mValue(p1, p2, p3, p4, p5, p6)
	{}


private:
	value_type mValue;
};


}

#endif