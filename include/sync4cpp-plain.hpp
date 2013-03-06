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
	struct default_modifier;

	template<typename Mutex>
	struct mutex_registry;
}

namespace detail {

	struct unused_type;

}

template<	typename Tag,
			typename P1 = detail::unused_type,
			typename P2 = detail::unused_type,
			typename P3 = detail::unused_type,
			typename P4 = detail::unused_type,
			typename P5 = detail::unused_type,
			typename P6 = detail::unused_type>
struct mutex_modifier;

template<typename Type>
struct is_registered_mutex;

template<typename Type>
struct is_decorated;

template<typename Type>
struct get_decor_tag;

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

}

	
template<typename Tag>
struct decor
{
};


namespace detail {

	struct unused_type {};

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

		mutex_assignment(mutex_type* mutex, const params_type& params)
			: mutex(mutex)
			, params(params)
		{
		}

		mutex_type* mutex;
		const params_type params;
	};


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
			return std::get<MapIndex>(as.params);
		}
	};

	template<typename Type , Type Value>
	struct get_from_mapping_impl< sync4cpp::val<Type, Value> >
	{
		template<typename Assignment>
		static const Type& get(const Assignment& as)
		{
			return Value;
		}
	};

	template<>
	struct get_from_mapping_impl< map_mutex >
	{
		template<typename Assignment>
		static const typename Assignment::mutex_type& get(const Assignment& as)
		{
			return *as.mutex;
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
	{	assignment_mapping_wrapper_guard(const Assignment& as) : Guard(__VA_ARGS__){}	};
#define MAPPING_GET(_index) get_from_mapping_impl<typename std::tuple_element<_index, Mapping>::type>::get(as)
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(0, *as.mutex);
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(1, MAPPING_GET(0));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(2, MAPPING_GET(0), MAPPING_GET(1));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(3, MAPPING_GET(0), MAPPING_GET(1), MAPPING_GET(2));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(4, MAPPING_GET(0), MAPPING_GET(1), MAPPING_GET(2), MAPPING_GET(3));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(5, MAPPING_GET(0), MAPPING_GET(1), MAPPING_GET(2), MAPPING_GET(3), MAPPING_GET(4));
	MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS(6, MAPPING_GET(0), MAPPING_GET(1), MAPPING_GET(2), MAPPING_GET(3), MAPPING_GET(4), MAPPING_GET(5));
#undef MAPPING_GET
#undef MAKE_ASSIGNMENT_MAPPING_WRAPPER_GUARD_WITH_N_PARAMS


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

	template<typename Mutex, typename Parent>
	struct dereference_wrapper_guard
		: public Parent
	{
		dereference_wrapper_guard(Mutex* mutex)
			: Parent(*mutex)
		{
			assert(mutex);
		}

		~dereference_wrapper_guard()
		{
		}
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
		typedef typename std::conditional
			< // if 'Mutex' was registered
				found_mutex::value
			, // then
				traits::mutex_registry<Mutex>
			, // else
				void
			>::type entry_type;

		typedef typename entry_type::template guard<Modifier> guard_entry;
		typedef bool_base<!std::is_void<typename guard_entry::guard_type>::value> found_guard;

		static_assert(found_guard::value, "Guard not found!");
		typedef typename std::conditional
			< // if the guard mapping is void
				std::is_void<typename guard_entry::mapping>::value
			, // then
				typename guard_entry::guard_type
			, // else
				assignment_mapping_wrapper_guard<mutex_assignment<Mutex, Modifier>, typename guard_entry::guard_type, typename guard_entry::mapping>
			>::type guard_type;


		//static_assert(std::is_void<type>::value, "Can not determinate a guard to given Mutex!");


		//typedef typename traits::defaults<traits::Modifier, Operation>::type	modifier_type;
		//typedef typename modifier_type::template assignment<Operation>::type	assignment_type;
		//typedef typename resolve_guard_impl<assignment_type>::type				type;
	};


	template<typename Mutex>
	struct resolve_guard_impl
	{
		static_assert(is_registered_mutex<Mutex>::value, "'Mutex' is not a registered mutex!");
		typedef typename search_registry_impl<Mutex, traits::default_modifier>::guard_type guard_type;
		static_assert(!std::is_void<guard_type>::value, "No default guard set for 'Mutex'!");
	};

	template<typename Mutex, typename Modifier>
	struct resolve_guard_impl< mutex_assignment<Mutex, Modifier> >
	{
		static_assert(is_registered_mutex<Mutex>::value, "'Mutex' is not a registered mutex!");
		typedef typename search_registry_impl<Mutex, Modifier>::guard_type guard_type;
		static_assert(!std::is_void<guard_type>::value, "No guard set for 'Mutex' and 'Modifier'!");
	};
	
	/*template<typename Mutex>
	struct resolve_guard_impl<Mutex*>
	{
		static_assert(is_registered_mutex<Mutex>::value || is_registered_mutex<Mutex*>::value, "'Mutex' or 'Mutex*' is not a registered mutex!");

		typedef typename std::conditional
			< // if Mutex* is registered
				is_registered_mutex<Mutex*>::value
			, // then
				typename resolve_guard_impl<Mutex*>::guard_type
			, // else
				typename std::conditional
				< // if Mutex is registered
					is_registered_mutex<Mutex>::value
				, // then
					dereference_wrapper_guard<Mutex, typename resolve_guard_impl<Mutex>::guard_type>
				, // else
					void
				>::type
			>::type guard_type;

		static_assert(!std::is_void<guard_type>::value, "No default guard set for 'Mutex' or 'Mutex*'!");
	};
	*/
}

template<typename Tag, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct mutex_modifier
{
	typedef Tag									tag_type;
	typedef std::tuple<P1, P2, P3, P4, P5, P6>	value_type;
	typedef mutex_modifier						this_type;

	template<typename Mutex>
	struct assignment { typedef detail::mutex_assignment<Mutex, this_type> type; };
	template<typename Mutex>
	struct assignment<Mutex*> { typedef detail::mutex_assignment<Mutex*, this_type> type; };

	template<typename Mutex>
	typename assignment<Mutex>::type operator [](Mutex& mutex) const
	{
		return assignment<Mutex>::type(&mutex, value);
	}

	template<typename Mutex>
	typename assignment<Mutex*>::type operator [](Mutex* mutex) const
	{
		return typename assignment<Mutex*>::type(&mutex, value);
	}

	template<std::size_t Index>
	const typename std::tuple_element<Index, value_type>::type& get() const
	{
		return std::get<Index>(value);
	}

	mutex_modifier(const P1& p1 = P1(), const P2& p2 = P2(), const P3& p3 = P3(), const P4& p4 = P4(), const P5& p5 = P5(), const P6& p6 = P6())
		: value(p1, p2, p3, p4, p5, p6)
	{
	}

private:
	const value_type value;
};

typedef mutex_modifier<struct ExclusiveModifierTag>	exclusive;
typedef mutex_modifier<struct SharedModifierTag>	shared;


template<typename Type>
struct is_registered_mutex
	: public detail::bool_base<detail::is_registered_impl<Type>::found_mutex::value>
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

template<typename Mutex>
struct guard
{
	typedef typename detail::resolve_guard_impl<Mutex>::guard_type type;
	//typedef detail::resolve_guard_impl<Mutex>::type type;
};


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

#define SYNC4CPP_SET_DEFAULT_GUARD(_mutex, _guard)			\
		namespace sync4cpp { namespace traits {				\
			template<>										\
			struct mutex_registry<_mutex>::guard<sync4cpp::traits::default_modifier>	\
			{												\
				typedef _guard guard_type;					\
				typedef void mapping;						\
			};												\
		}}


//#define syncuse(_op, ...)		auto pair##_op  auto& _op
// syncuse(mLocked) syncuse(lock = mLocked)
// sync_here(sync4cpp::shared <= mutex)


#endif // _SYNC4CPP_PLAIN_HPP
