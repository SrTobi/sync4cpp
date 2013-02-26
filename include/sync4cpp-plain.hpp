#ifndef _SYNC4CPP_PLAIN_HPP
#define _SYNC4CPP_PLAIN_HPP


#include <type_traits>
#include <tuple>


#ifndef SYNC4CPP_NO_ASSERTS

#	include <cassert>
#	define SYNC4CPP_ASSERT(_exp)	assert(_exp)

#else

#	define SYNC4CPP_ASSERT(_exp)

#endif

#define SYNC4CPP_BUILD_NAME_IMPL( x, y ) x##y
#define SYNC4CPP_BUILD_NAME( x, y ) SYNC4CPP_BUILD_NAME_IMPL( x, y )
#define SYNC4CPP_LOCK_NAME(_named) SYNC4CPP_BUILD_NAME(_named, __LINE__)





/**
 * 
 *	
 *	synchronize(sync4cpp::shared[mutex])
 *	{
 *
 *	}
 *
 *
 *
 **/


namespace sync4cpp {
namespace traits {

	struct Modifier;
	template<typename Tag, typename Dummy>
	struct defaults;
}

namespace detail {

	struct Nil
	{};

	typedef Nil unused_type;


	template<typename Mutex, typename Params>
	struct mutex_assignment
	{
		typedef Mutex mutex_type;
		typedef Params params_type;

		mutex_assignment(mutex_type* const mutex, const params_type& params)
			: mutex(mutex)
			, params(params)
		{
		}

		mutex_type* const mutex;
		params_type params;
	};

	template<	typename Tag,
				typename P1 = unused_type,
				typename P2 = unused_type,
				typename P3 = unused_type,
				typename P4 = unused_type,
				typename P5 = unused_type,
				typename P6 = unused_type>
	struct mutex_modifier
	{
		typedef Tag									tag_type;
		typedef std::tuple<P1, P2, P3, P4, P5, P6>	value_type;

		template<typename Mutex>
		struct assignment { typedef mutex_assignment<Mutex, value_type> type; };
		template<typename Mutex>
		struct assignment<Mutex* const> { typedef mutex_assignment<Mutex* const, value_type> type; };

		template<typename Mutex>
		typename assignment<Mutex>::type operator [](Mutex& mutex) const
		{
			return assignment<Mutex>::type(&mutex, value);
		}

		template<typename Mutex>
		typename assignment<Mutex>::type operator [](Mutex* const mutex) const
		{
			return assignment<Mutex>::type(&mutex, value);
		}

		template<std::size_t Index>
		const typename std::tuple_element<Index, value_type>::type& get() const
		{
			return std::get<Index>(value);
		}
	protected:
		mutex_modifier(const P1& p1 = P1(), const P2& p2 = P2(), const P3& p3 = P3(), const P4& p4 = P4(), const P5& p5 = P5(), const P6& p6 = P6())
			: value(p1, p2, p3, p4, p5, p6)
		{
		}

	private:
		const value_type value;
	};

	template<typename Operation>
	struct resolve_guard_impl
	{
		typedef typename traits::defaults<traits::Modifier, Operation>::type	modifier_type;
		typedef typename modifier_type::template assignment<Operation>::type	assignment_type;
		typedef typename resolve_guard_impl<assignment_type>::type				type;
	};

	template<typename Mutex, typename Params>
	struct resolve_guard_impl< mutex_assignment<Mutex, Params> >
	{
		typedef void type;
	};
}

// No parameter modifier
template<typename Tag>
struct mutex_modifier
	: public detail::mutex_modifier<Tag>
{

};

typedef mutex_modifier<struct ExclusiveModifierTag>	exclusive;
typedef mutex_modifier<struct SharedModifierTag>	shared;


template<typename Operation>
struct resolve_guard
{
	typedef typename detail::resolve_guard_impl<Operation>::type type;
};


/********************************** default settings **********************************************/
namespace traits {

	template<typename Tag, typename Dummy>
	struct defaults
	{
	};

	template<typename Dummy>
	struct defaults<traits::Modifier, Dummy>
	{
		typedef sync4cpp::exclusive type;
	};

}

}

//#define syncuse(_op, ...)		auto pair##_op  auto& _op
// syncuse(mLocked) syncuse(lock = mLocked)
// sync_here(sync4cpp::shared <= mutex)


#endif
