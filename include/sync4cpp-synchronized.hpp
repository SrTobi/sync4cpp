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
	template<typename InternalMutex_, typename ValueType_>
	friend struct detail::synchronized_decor;

	/*inline synchronized(const synchronized& p)
	{

	}*/
public:
	typedef syncable<Mutex>		base_type;
	typedef Mutex				mutex_type;
	typedef ValueType			value_type;

	synchronized()
	{}

	inline synchronized(const value_type& value)
		: mValue(value)
	{}

	inline synchronized(const mutex_type&& mutex)
		: base_type(mutex)
	{}

	inline synchronized(const value_type& value, const mutex_type&& mutex)
		: base_type(std::move(mutex))
		, mValue(value)
	{}



private:
	value_type mValue;
};


}


#define SYNC4CPP_SYNCBIND(_var, _mutex) auto SYNC4CPP_LOCK_NAME(_sync_guard_named_##_var##_in_) = SYNC4CPP_SYNCGUARD(_mutex); auto& _var = SYNC4CPP_LOCK_NAME(_sync_guard_named_##_var##_in_).value;
#define SYNC4CPP_SYNCUSE(_mutex)		SYNC4CPP_SYNCBIND(_mutex, _mutex);
#endif
