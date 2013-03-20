#pragma once
#ifndef _SYNC4CPP_GUARDARRAY_HPP
#define _SYNC4CPP_GUARDARRAY_HPP


#include "sync4cpp-plain.hpp"



namespace sync4cpp {
namespace detail {

	template<typename Type>
	struct make_assignment_const_ref
	{
		typedef Type type;
	};

	template<typename Mutex, typename Modifier>
	struct make_assignment_const_ref<assignment<Mutex, Modifier> >
	{
		typedef const assignment<Mutex, Modifier> type;
	};

	static unused_type& UInst() { static unused_type uused; return uused; }

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	struct GuardArray
	{
		

		GuardArray(	typename make_assignment_const_ref<T1>::type& m1, 
					typename make_assignment_const_ref<T2>::type& m2 = UInst(),
					typename make_assignment_const_ref<T3>::type& m3 = UInst(),
					typename make_assignment_const_ref<T4>::type& m4 = UInst(),
					typename make_assignment_const_ref<T5>::type& m5 = UInst(),
					typename make_assignment_const_ref<T6>::type& m6 = UInst(),
					typename make_assignment_const_ref<T7>::type& m7 = UInst(),
					typename make_assignment_const_ref<T8>::type& m8 = UInst())
			: guard1(m1)
			, guard2(m2)
			, guard3(m3)
			, guard4(m4)
			, guard5(m5)
			, guard6(m6)
			, guard7(m7)
			, guard8(m8)
		{
			using sync4cpp::is_locked;
			bool allLocked = is_locked(guard1) && is_locked(guard2) && is_locked(guard3) && is_locked(guard4)
							&& is_locked(guard5) && is_locked(guard6) && is_locked(guard7) && is_locked(guard8);
			done = !allLocked;
		}

		typename guard<T1>::type guard1;
		typename guard<T2>::type guard2;
		typename guard<T3>::type guard3;
		typename guard<T4>::type guard4;
		typename guard<T5>::type guard5;
		typename guard<T6>::type guard6;
		typename guard<T7>::type guard7;
		typename guard<T8>::type guard8;
		bool done;
	};

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename Unused>
	inline GuardArray<T1, T2, T3, T4, T5, T6, T7, T8>
		GetGuardType(const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6, const T7& p7, const T8& p8, const Unused&, ...)
	{
		static_assert(std::is_same<Unused, unused_type>::value, "You can only synchronize up to 8 mutexes with a guard array!");
		assert(!"Never call this!");
	}



}
}

#define SYNC4CPP_SYNCHERE(...)	decltype(sync4cpp::detail::GetGuardType(__VA_ARGS__, sync4cpp::detail::UInst(), sync4cpp::detail::UInst(), sync4cpp::detail::UInst(), sync4cpp::detail::UInst(), sync4cpp::detail::UInst(), sync4cpp::detail::UInst(), sync4cpp::detail::UInst(), sync4cpp::detail::UInst())) SYNC4CPP_LOCK_NAME(_lockarray_in_line_)(__VA_ARGS__);
#define SYNC4CPP_SYNCHRONIZE(...)	for(SYNC4CPP_SYNCHERE(__VA_ARGS__) !SYNC4CPP_LOCK_NAME(_lockarray_in_line_).done; SYNC4CPP_LOCK_NAME(_lockarray_in_line_).done = true)




#endif
