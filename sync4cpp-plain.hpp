#pragma once
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
 *	- native guard
 *	- split assignment guard
 *	- (raw mutex to assignment) guard
 *
 *
 **/


/********************* Declarations **********************/
namespace sync4cpp {
namespace traits {
	template<typename Mutex>
	struct mutex_registry;
}

namespace detail {

	struct unused_type;
}

template<size_t Index, typename Modifier>
struct modifier_parameter;

template<typename Mutex, typename Modifier>
struct assignment;

template<	typename Tag,
			typename P1 = detail::unused_type,
			typename P2 = detail::unused_type,
			typename P3 = detail::unused_type,
			typename P4 = detail::unused_type,
			typename P5 = detail::unused_type>
struct mutex_modifier;


template<typename Type>
struct is_registered_mutex;

template<typename Mutex, typename Modifier>
struct has_registered_guard;

template<typename Type>
struct is_decorated;

template<typename Type>
struct get_decor_handler;

template<typename Mutex>
struct get_default_modifier;

template<typename Type, Type Value>
struct val
	: public std::integral_constant<Type, Value>
{
};

template<int Index>
struct map
	: public std::integral_constant<int, Index>
{
};

struct map_mutex
{
};

struct disable_default_modifier
{
	typedef void modifier_type;
	static void inst()
	{
		//static_assert(sizeof(Mutex) != sizeof(Mutex), "Not standard modifier for 'Mutex' set.");
	}
};

template<typename DefaultModifierFactory = disable_default_modifier>
struct decor_handler_base
{
	typedef DefaultModifierFactory default_modifier_factory;
};

template<typename Mutex>
static typename get_default_modifier<Mutex>::type instance_default_modifier();

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

		template<typename Modifier>
		struct guard
		{
			typedef void guard_type;
			typedef void mapping;
		};
	};


	template<typename Mutex>
	struct default_modifier_registry
	{
		typedef std::false_type found;
		typedef disable_default_modifier factory;
	};

	template<typename Guard>
	bool is_locked(const Guard&)
	{
		return true;
	}
}

	
template<typename Handler>
struct decor
{
};

namespace detail {

	struct unused_type {};

	struct unused_guard
	{
		unused_guard() {}
		unused_guard(unused_type&){}
		~unused_guard(){}
	};

	template<bool Value>
	struct bool_base : public std::false_type
	{};

	template<>
	struct bool_base<true> : public std::true_type
	{};



	template<typename Map>
	struct get_from_mapping_impl
	{
		static_assert(sizeof(Map) != sizeof(Map), "Unknown mapping instruction!");
	};

	template<int MapIndex>
	struct get_from_mapping_impl< sync4cpp::map<MapIndex> >
	{

		template<typename Assignment>
		static const typename std::tuple_element<MapIndex, typename Assignment::params_type>::type& get(const Assignment& as)
		{
			static_assert(!std::is_same<typename std::tuple_element<MapIndex, typename Assignment::params_type>::type, unused_type>::value, "Parameter with this Index is not used in the Modifier!");
			return std::get<MapIndex>(as.params());
		}
	};

	template<typename Type , Type Value>
	struct get_from_mapping_impl< sync4cpp::val<Type, Value> >
	{
		template<typename Assignment>
		static Type get(const Assignment& as)
		{
			return Value;
		}
	};

	template<>
	struct get_from_mapping_impl< map_mutex >
	{
		template<typename Assignment>
		static typename Assignment::mutex_type& get(const Assignment& as)
		{
			return as.mutex();
		}
	};

	template<typename Assignment, typename Guard, typename Mapping, int ParamCount = std::tuple_size<Mapping>::value>
	struct assignment_mapping_wrapper_guard
	{
		static_assert(sizeof(Assignment) != sizeof(Assignment), "Invalid param count on an assignment!");
	};

#define MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(_count, ...)						\
	template<typename Assignment, typename Guard, typename Mapping>								\
	struct assignment_mapping_wrapper_guard<Assignment, Guard, Mapping, _count> : public Guard	\
	{																							\
		typedef Guard native_guard;																\
		assignment_mapping_wrapper_guard(const Assignment& as) : native_guard(__VA_ARGS__){}	\
		native_guard& as_native_guard() { return *this; }										\
		const native_guard& as_native_guard() const { return *this; }							\
	};	
#define MAPPING_GET(_index) get_from_mapping_impl<typename std::tuple_element<_index, Mapping>::type>::get(as)
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(0, as.mutex());
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(1, MAPPING_GET(0));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(2, MAPPING_GET(0), MAPPING_GET(1));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(3, MAPPING_GET(0), MAPPING_GET(1), MAPPING_GET(2));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(4, MAPPING_GET(0), MAPPING_GET(1), MAPPING_GET(2), MAPPING_GET(3));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(5, MAPPING_GET(0), MAPPING_GET(1), MAPPING_GET(2), MAPPING_GET(3), MAPPING_GET(4));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(6, MAPPING_GET(0), MAPPING_GET(1), MAPPING_GET(2), MAPPING_GET(3), MAPPING_GET(4), MAPPING_GET(5));
#undef MAPPING_GET
#undef MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS

	template<typename Guard>
	static bool is_locked_impl(const Guard& guard)
	{
		return traits::is_locked(guard);
	}

	template<typename Assignment, typename Guard, typename Mapping, size_t Count>
	static bool is_locked_impl(const assignment_mapping_wrapper_guard<Assignment, Guard, Mapping, Count>& guard)
	{
		return is_locked_impl(guard.as_native_guard());
	}

	template<typename Type>
	struct is_decorated_impl
	{
	private:
		static Type* type_ptr();

		struct yes_type { char dummy; };
		struct no_type { yes_type dummy[4]; };

		template<typename Handler>
		static yes_type _check(const sync4cpp::decor<Handler>&);
		template<typename Handler>
		static yes_type _check(const sync4cpp::decor<Handler>* const);
		static no_type _check(...);

	public:
		static const bool value = (sizeof(_check(*type_ptr())) == sizeof(yes_type));
	};

	template<typename Type>
	struct get_decor_handler_impl
	{
	private:
		struct DummyHandler
			: public decor_handler_base<>
		{
			template<typename Mutex, typename Modifier>
			struct guard
			{
				typedef void type;
			};
		};
		static Type* type_ptr();

		template<typename Handler>
		static Handler _resolveHandler(const sync4cpp::decor<Handler>&);
		template<typename Handler>
		static Handler _resolveHandler(const sync4cpp::decor<Handler>* const);
		static DummyHandler _resolveHandler(...);
	public:
		typedef decltype(_resolveHandler(*type_ptr())) type;
	};

	template<typename Mutex>
	struct get_default_modifier_factory_impl
	{
		typedef traits::default_modifier_registry<Mutex> entry;
		typedef typename entry::found found_in_regestry;
		typedef typename std::conditional
			< // if is registered
				found_in_regestry::value
			, // then
				typename entry::factory
			, // else
				typename std::conditional
				< // is Mutex is decorated
					is_decorated<Mutex>::value
				, // then
					typename get_decor_handler_impl<Mutex>::type::default_modifier_factory
				, // else
					disable_default_modifier
				>::type
			>::type type;
	};

	template<typename Mutex>
	struct get_default_modifier_impl
	{
		typedef typename get_default_modifier_factory_impl<Mutex>::type::modifier_type type;
	};

	template<typename Type>
	struct is_registered_impl
	{
		typedef typename traits::mutex_registry<Type>::found found_mutex;
	};

	template<typename Mutex, typename Modifier>
	struct search_registry_impl
	{
		typedef typename traits::mutex_registry<Mutex>::found found_mutex;
		typedef traits::mutex_registry<Mutex> entry_type;

		typedef typename entry_type::template guard<Modifier> guard_entry;
		typedef bool_base<!std::is_void<typename guard_entry::guard_type>::value> found_guard;

		//static_assert(found_guard::value, "Guard not found!");
		typedef typename std::conditional
			< // if the guard mapping is void
				std::is_void<typename guard_entry::mapping>::value
			, // then
				typename guard_entry::guard_type
			, // else
				assignment_mapping_wrapper_guard<assignment<Mutex, Modifier>, typename guard_entry::guard_type, typename guard_entry::mapping>
			>::type guard_type;


		//static_assert(std::is_void<type>::value, "Can not determinate a guard to given Mutex!");


		//typedef typename traits::defaults<traits::Modifier, Operation>::type	modifier_type;
		//typedef typename modifier_type::template assignment<Operation>::type	assignment_type;
		//typedef typename resolve_guard_impl<assignment_type>::type				type;
	};


	template<typename Mutex>
	struct resolve_guard_impl
	{
		typedef typename get_default_modifier<Mutex>::type modifier_type;
		static_assert(!std::is_void<modifier_type>::value, "No standard modifier for 'Mutex' set.");
		typedef typename resolve_guard_impl< assignment<Mutex, modifier_type> >::guard_type base_guard;
		typedef struct modify_wrapper: public base_guard
		{
			modify_wrapper(Mutex& mutex): base_guard(instance_default_modifier<Mutex>() << mutex) {}
			~modify_wrapper() {}
		} guard_type;
	};

	template<typename Mutex, typename Modifier>
	struct resolve_guard_impl< assignment<Mutex, Modifier> >
	{
		//static_assert(is_registered_mutex<Mutex>::value, "'Mutex' is not a registered mutex!");
		typedef search_registry_impl<Mutex, Modifier> serach_entry;

		typedef typename std::conditional
			< // if a mutex is registered and a guard can be found
				is_registered_mutex<Mutex>::value && has_registered_guard<Mutex, Modifier>::value
			, // then
				typename serach_entry::guard_type
			, // else
				typename std::conditional
				< // if Mute is decorated
					is_decorated<Mutex>::value
				, // then
					typename get_decor_handler_impl<Mutex>::type::template guard<Mutex, Modifier>::type
				, // else
					void
				>::type
			>::type guard_type;
		//static_assert(!std::is_void<guard_type>::value, "No guard set for 'Mutex' and 'Modifier'!");
	};
}

template<typename Mutex, typename Modifier>
struct assignment
{
	static_assert(sizeof(Mutex) != sizeof(Mutex), "The template parameter modifier must be an instance of the template mutex_modifier<...>!");
};


template<typename Mutex, typename Tag, typename P1, typename P2, typename P3, typename P4, typename P5>
struct assignment<Mutex, mutex_modifier<Tag, P1, P2, P3, P4, P5> >
{
	typedef mutex_modifier<Tag, P1, P2, P3, P4, P5>		modifier_type;
	typedef Mutex										mutex_type;
	typedef typename modifier_type::value_type			params_type;

	assignment(mutex_type& mutex, const params_type& params)
		: mMutex(mutex)
		, mParams(params)
	{
	}

	assignment(const assignment& another)
		: mMutex(another.mutex())
		, mParams(another.params())
	{
	}

	template<typename NewMutex>
	assignment<NewMutex, modifier_type> operator[](NewMutex& m) const
	{
		return assignment<NewMutex, modifier_type>(m, params());
	}

	mutex_type& mutex() const { return mMutex; }
	const params_type& params() const { return mParams; }
private:
	mutex_type& mMutex;
	const params_type mParams;
};

template<typename Tag, typename P1, typename P2, typename P3, typename P4, typename P5>
struct mutex_modifier
{
	typedef Tag									tag_type;
	typedef std::tuple<P1, P2, P3, P4, P5>		value_type;
	typedef mutex_modifier						this_type;

	template<typename Mutex>
	struct assignment_selector { typedef assignment<Mutex, this_type> type; };

	template<typename Mutex>
	typename assignment_selector<Mutex>::type operator <<(Mutex& mutex) const
	{
		return assignment_selector<Mutex>::type(mutex, value);
	}

	template<std::size_t Index>
	const typename std::tuple_element<Index, value_type>::type& get() const
	{
		return std::get<Index>(value);
	}

	mutex_modifier(const P1& p1 = P1(), const P2& p2 = P2(), const P3& p3 = P3(), const P4& p4 = P4(), const P5& p5 = P5())
		: value(p1, p2, p3, p4, p5)
	{
	}

private:
	const value_type value;
};

typedef mutex_modifier<struct ExclusiveModifierTag>	exclusive;
typedef mutex_modifier<struct SharedModifierTag>	shared;


template<size_t Index, typename Modifier>
struct modifier_parameter
{
	typedef typename std::tuple_element<Index, typename Modifier::value_type>::type type;
};

template<typename Type>
struct is_registered_mutex
	: public detail::bool_base<detail::is_registered_impl<Type>::found_mutex::value>
{
};

template<typename Mutex, typename Modifier>
struct has_registered_guard
	: public detail::bool_base<detail::search_registry_impl<Mutex, Modifier>::found_guard::value>::type
{
	//static_assert(is_registered_mutex<Mutex>::value, "'Mutex' is not a registered mutex!");
};

template<typename Type>
struct is_decorated
	: public detail::bool_base<detail::is_decorated_impl<Type>::value>
{

};

template<typename Type>
struct get_decor_handler
{
	static_assert(is_decorated<Type>::value, "'Type' is not decorated!");
	typedef typename detail::get_decor_handler_impl<Type>::type type;
};

template<typename Type>
struct has_default_modifier
	: public detail::bool_base<!std::is_void<typename detail::get_default_modifier_impl<Type>::type>::value>
{
};

template<typename Mutex>
struct get_default_modifier
{
	static_assert(has_default_modifier<Mutex>::value, "'Mutex' has no default modifier!");
	typedef typename detail::get_default_modifier_impl<Mutex>::type type;
};

template<typename Mutex>
struct get_default_modifier_factory
{
	typedef typename detail::get_default_modifier_factory_impl<Mutex>::type type;
};


template<typename Mutex>
struct guard
{
	typedef typename detail::resolve_guard_impl<Mutex>::guard_type type;
	static_assert(!std::is_void<type>::value, "Was not able to resolve a guard for 'Mutex'!");
	//typedef detail::resolve_guard_impl<Mutex>::type type;
};


template<typename Mutex>
static typename get_default_modifier<Mutex>::type instance_default_modifier()
{
	return detail::get_default_modifier_factory_impl<Mutex>::type::inst();
}

template<typename Guard>
static bool is_locked(const Guard& guard)
{
	return detail::is_locked_impl(guard);
}


/********************************** default settings **********************************************/
namespace traits {

	/*template<typename Tag, typename Dummy>
	struct defaults
	{
	};

	template<typename Dummy>
	struct defaults<traits::Modifier, Dummy>
	{
		typedef sync4cpp::exclusive type;
	};*/

}

}

/********************************** definitions **********************************************/

#define SYNC4CPP_REGISTER_MUTEX(_mutex)			\
		namespace sync4cpp { namespace traits {	\
			template<>							\
			struct mutex_registry<_mutex>		\
			{									\
				typedef std::true_type found;	\
				template<typename Modifier>		\
				struct guard					\
				{								\
					typedef void guard_type;	\
					typedef void mapping;		\
				};								\
			};									\
		}}

#define SYNC4CPP_REGISTER_GUARD(_mutex, _modifier, _guard, ...)	\
		namespace sync4cpp { namespace traits {				\
			template<>										\
			struct mutex_registry<_mutex>::guard<_modifier>	\
			{												\
				typedef _guard guard_type;					\
				typedef std::tuple<__VA_ARGS__> mapping;	\
			};												\
		}}

#define SYNC4CPP_MAKE_MODIFIER_FACTORY(_modifier, ...)							\
		struct {																\
			typedef _modifier modifier_type;									\
			static modifier_type inst() { return modifier_type(__VA_ARGS__); }	\
		}

#define SYNC4CPP_PLACE_DEFAULT_MODIFIER_FACTORY(_factory)	\
		typedef _factory default_modifier_factory;

#define SYNC4CPP_SET_DEFAULT_GUARD(_mutex, _modifier, ...)				\
		namespace sync4cpp { namespace traits {							\
			template<> struct default_modifier_registry<_mutex>			\
			{															\
				typedef std::true_type found;							\
				typedef SYNC4CPP_MAKE_MODIFIER_FACTORY(_modifier, __VA_ARGS__) factory;\
			};															\
		}}

SYNC4CPP_REGISTER_MUTEX(sync4cpp::detail::unused_type);
SYNC4CPP_REGISTER_GUARD(sync4cpp::detail::unused_type, sync4cpp::exclusive, sync4cpp::detail::unused_guard);
SYNC4CPP_SET_DEFAULT_GUARD(sync4cpp::detail::unused_type, sync4cpp::exclusive);

//#define syncuse(_op, ...)		auto pair##_op  auto& _op
// syncuse(mLocked) syncuse(lock = mLocked)
// sync_here(sync4cpp::shared <= mutex)


#define SYNC4CPP_SYNCGUARD(_tolock) (::sync4cpp::guard<decltype(_tolock)>::type(_tolock))

#include "sync4cpp-guardarray.hpp"
#include "sync4cpp-utils.hpp"
#include "sync4cpp-synchronized.hpp"

#endif // _SYNC4CPP_PLAIN_HPP
