#pragma once
#ifndef _SYNC4CPP_UTILS_HPP
#define _SYNC4CPP_UTILS_HPP

#include <memory>
#include "sync4cpp.hpp"
namespace sync4cpp {

template<typename Mutex>
class syncable;

template<typename Mutex>
class shared_mutex;


namespace detail {

	template<typename InternalMutex>
	struct shared_mutex_decor
		: public decor<shared_mutex_decor<InternalMutex> >
	{
		SYNC4CPP_PLACE_DEFAULT_MODIFIER_FACTORY(typename get_default_modifier_factory<InternalMutex>::type);
		template<typename Mutex, typename Modifier>
		struct guard
		{
			typedef typename sync4cpp::guard<assignment<InternalMutex, Modifier>>::type base_type;
			typedef struct Wrapper
				: public base_type
			{
				Wrapper(const assignment<Mutex, Modifier>& as)
					: base_type(as[as.mutex().get()])
				{}
			} type;
		};
	};



	template<typename InternalMutex>
	struct syncable_decor
		: public decor<syncable_decor<InternalMutex> >
	{
	private:
		static InternalMutex& ExtractMutex(const syncable<InternalMutex>& mutex)
		{
			return mutex._mMutex;
		}
	public:
		SYNC4CPP_PLACE_DEFAULT_MODIFIER_FACTORY(typename get_default_modifier_factory<InternalMutex>::type);
		template<typename Mutex, typename Modifier>
		struct guard
		{
			typedef typename sync4cpp::guard<assignment<InternalMutex, Modifier>>::type base_type;
			typedef struct Wrapper
				: public base_type
			{
				Wrapper(const assignment<Mutex, Modifier>& as)
					: base_type(as[ExtractMutex(as.mutex())])
				{}
			} type;
		};
	};

}


template<typename Mutex>
class shared_mutex
	: public detail::shared_mutex_decor<Mutex>
{
public:
	typedef Mutex						mutex_type;
	typedef shared_mutex<mutex_type>	this_type;

	shared_mutex()
		: _mMutex(new mutex_type())
	{
	}

	shared_mutex(const this_type& other)
	{
		_mMutex = other._mMutex;
	}

	~shared_mutex()
	{
	}

	void set(const mutex_type& mutex)
	{
		_mMutex = mutex._mMutex;
	}

	mutex_type& get() const
	{
		return *_mMutex;
	}

private:
	std::shared_ptr<mutex_type> _mMutex;
};

template<typename Mutex>
class syncable
	: public detail::syncable_decor<Mutex>
{
	template<typename InternalMutex>
	friend struct detail::syncable_decor;
public:
	typedef Mutex mutex_type;

	syncable()
	{
	}

	syncable(const mutex_type& mutex)
		: _mMutex(mutex)
	{

	}


private:
	mutable mutex_type _mMutex;
};

}

#endif