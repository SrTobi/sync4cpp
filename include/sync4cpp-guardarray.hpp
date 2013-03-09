#ifndef _SYNC4CPP_GUARDARRAY_HPP
#define _SYNC4CPP_GUARDARRAY_HPP


#include "sync4cpp-plain.hpp"



namespace sync4cpp {
namespace detail {

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	struct GuardArray
	{
		template<typename T>
		struct def
		{
			inline operator T() const { throw "You forgot to give an argument";}
		};
		template<>
		struct def<unused_type>
		{
			unused_type unused;
			inline operator unused_type&() { return unused; }
		};

		inline GuardArray(	T1& m1, T2& m2 = def<T2>(), T3& m3 = def<T3>(), T4& m4 = def<T4>(),
							T5& m5 = def<T5>(), T6& m6 = def<T6>(), T7& m7 = def<T7>(), T8& m8 = def<T8>(), ...)
			: done(false)
			, guard1(m1)
			, guard2(m2)
			, guard3(m3)
			, guard4(m4)
			, guard5(m5)
			, guard6(m6)
			, guard7(m7)
			, guard8(m8)
		{}

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
		GetGuardType(T1& p1, T2& p2, T3& p3, T4& p4, T5& p5, T6& p6, T7& p7, T8& p8, Unused&, ...)
	{
		static_assert(std::is_same<Unused, unused_type>::value, "You can only synchronize up to 8 mutexes with a guard array!");
		assert(!"Never call this!");
	}



}
}

#define SYNC4CPP_GUARDARRAY(...)	decltype(sync4cpp::detail::GetGuardType(__VA_ARGS__, sync4cpp::detail::unused_type(), sync4cpp::detail::unused_type(), sync4cpp::detail::unused_type(), sync4cpp::detail::unused_type(), sync4cpp::detail::unused_type(), sync4cpp::detail::unused_type(), sync4cpp::detail::unused_type(), sync4cpp::detail::unused_type()))(__VA_ARGS__)
#define SYNC4CPP_SYNCHERE(...)		auto SYNC4CPP_LOCK_NAME(_lockarray_in_line_) = SYNC4CPP_GUARDARRAY(__VA_ARGS__);
#define SYNC4CPP_SYNCHRONIZE(...)	for(SYNC4CPP_SYNCHERE(__VA_ARGS__) !SYNC4CPP_LOCK_NAME(_lockarray_in_line_).done; SYNC4CPP_LOCK_NAME(_lockarray_in_line_).done = true)




#endif