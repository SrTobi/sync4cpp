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


/********************* Declarations **********************/
namespace sync4cpp {
namespace traits {
	struct Modifier;
	template<typename Tag, typename Dummy = void>
	struct defaults;

	template<typename Mutex>
	struct mutex_registry;
}

namespace detail {
	template<	typename Tag,
				typename P1 = unused_type,
				typename P2 = unused_type,
				typename P3 = unused_type,
				typename P4 = unused_type,
				typename P5 = unused_type,
				typename P6 = unused_type>
	struct mutex_modifier;


}

template<typename Type>
struct is_registered_mutex;

template<typename Type>
struct is_decorated;

template<typename Type>
struct get_decor_tag;


/********************* Implementation ***********************/
namespace traits {

	/**
	 * @brief This class is used to register mutexes and their guards
	 *
	 * Use template specification to register a mutex.
	 * typedef std::true_type to "found" to indicate, that your specification
	 * holds information about guards for the mutex you want to register.
	 *
	 *
	 **/
	template<typename Mutex>
	struct mutex_registry
	{
		typedef std::false_type found;
		typedef void			default_operation;

		template<typename Operation>
		struct guard
		{
			typedef std::false_type has;
		};
	};

}

	
template<typename Tag>
struct decor
{
};


namespace detail {

	struct Nil
	{};

	typedef Nil unused_type;

	template<bool Value>
	struct bool_base : public std::false_type
	{};

	template<>
	struct bool_base<true> : public std::true_type
	{};


	template<typename Mutex, typename Modifier>
	struct mutex_assignment
	{
		static_assert(sizeof(Mutex) != sizeof(Mutex), "The template parameter modifier must be an instance of the template mutex_modifier<...>!");
	};


	template<typename Mutex, typename Tag, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	struct mutex_assignment<Mutex, mutex_modifier<Tag, P1, P2, P3, P4, P5, P6> >
	{
		typedef mutex_modifier<Tag, P1, P2, P3, P4, P5, P6> modifier_type;
		typedef Mutex										mutex_type;
		typedef typename modifier_type::value_type			params_type;

		mutex_assignment(mutex_type* const mutex, const params_type& params)
			: mutex(mutex)
			, params(params)
		{
		}

		mutex_type* const mutex;
		params_type params;
	};

	template<typename Tag, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	struct mutex_modifier
	{
		typedef Tag									tag_type;
		typedef std::tuple<P1, P2, P3, P4, P5, P6>	value_type;
		typedef mutex_modifier						this_type;

		template<typename Mutex>
		struct assignment { typedef mutex_assignment<Mutex, this_type> type; };
		template<typename Mutex>
		struct assignment<Mutex* const> { typedef mutex_assignment<Mutex* const, this_type> type; };

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


	template<typename Type>
	struct is_decorated_impl
	{
	private:
		static Type* type_ptr();

		struct yes_type { char dummy; };
		struct no_type { yes_type dummy[4]; };

		template<typename Tag>
		static yes_type _check(const sync4cpp::decor<Tag>&);
		template<typename Tag>
		static yes_type _check(const sync4cpp::decor<Tag>* const);
		static no_type _check(...);

	public:
		static const bool value = (sizeof(_check(*type_ptr())) == sizeof(yes_type));
	};

	template<typename Type>
	struct get_decor_tag_impl
	{
	private:
		static_assert(is_decorated<Type>::value, "'Type' is not decorated!");
		static Type* type_ptr();

		template<typename Tag>
		static Tag _resolveTag(const sync4cpp::decor<Tag>&);
		template<typename Tag>
		static Tag _resolveTag(const sync4cpp::decor<Tag>* const);
	public:
		typedef decltype(_resolveTag(*type_ptr())) type;
	};

	template<typename Type>
	struct is_registered_mutex_impl
	{
		typedef typename traits::mutex_registry<Type>::found found_type;
		typedef typename found_type::value value;
	};

	template<typename Mutex, typename Modifier = traits::mutex_registry<Mutex>::default_operation>
	struct has_guard_impl
	{
		static_assert(is_registered_mutex<Mutex>::value, "'Mutex' is not a registered mutex!");
		typedef typename traits::mutex_registry<Mutex>::template guard<Modifier>::has::value value;
	};

	template<typename Mutex, typename Modifier = traits::mutex_registry<Mutex>::default_operation>
	struct get_guard_impl
	{
		static_assert(is_registered_mutex<Mutex>::value, "'Mutex' is not a registered mutex!");
		typedef typename traits::mutex_registry<Mutex>::template guard<Modifier>::type type;
	};

	template<typename Mutex>
	struct resolve_guard_impl
	{

		typedef typename std::conditional
			< // if 'Mutex' is registered
				is_registered_mutex<Mutex>::value
			, // then
				typename get_guard_impl<Mutex, typename traits::defaults<traits::Modifier, Mutex>::type>::type
			, // else
				typename std::conditional
				< // if decorated
					is_decorated<Mutex>::value
				, // then
					void
				, // else
					void
				>::type
			>::type type;

		static_assert(std::is_void<type>::value, "Can not determinate a guard to given Mutex!");


		//typedef typename traits::defaults<traits::Modifier, Operation>::type	modifier_type;
		//typedef typename modifier_type::template assignment<Operation>::type	assignment_type;
		//typedef typename resolve_guard_impl<assignment_type>::type				type;
	};

	template<typename Mutex, typename Params>
	struct resolve_guard_impl< mutex_assignment<Mutex, Params> >
	{
		typedef void type;
	};

	template<typename Tag>
	struct resolve_guard_impl< sync4cpp::decor<Tag> >
	{
		static_assert(sizeof(Tag) != sizeof(Tag), "You can not resolve a decor directly!");
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

template<typename Type>
struct is_registered_mutex
	: public detail::bool_base<detail::is_registered_mutex_impl<Type>::value>
{
};


template<typename Type>
struct is_decorated
	: public detail::bool_base<detail::is_decorated_impl<Type>::value>
{

};

template<typename Type>
struct get_decor_tag
{
	typedef typename detail::get_decor_tag_impl<Type>::type type;
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


#endif // _SYNC4CPP_PLAIN_HPP