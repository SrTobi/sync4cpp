#ifndef _SYNC4CPP_PLAIN_HPP
#define _SYNC4CPP_PLAIN_HPP



#ifndef SYNC4CPP_NO_ASSERTS

#	include <cassert>
#	define SYNC4CPP_ASSERT(_exp)	assert(_exp)

#else

#	define SYNC4CPP_ASSERT(_exp)

#endif

#define SYNC4CPP_BUILD_NAME_IMPL( x, y ) x##y
#define SYNC4CPP_BUILD_NAME( x, y ) SYNC4CPP_BUILD_NAME_IMPL( x, y )
#define SYNC4CPP_LOCK_NAME(_named) SYNC4CPP_BUILD_NAME(_named, __LINE__)

namespace sync4cpp {

	template<typename T = void>
	struct defaults
	{
	};

	template<>
	struct defaults<void>;

	template<typename ValueType,
		typename Mutex = typename defaults<>::mutex,
		bool InheritSyncable = false>
	class synchronized;

	namespace internal {
		template<typename T>
		struct Type
		{
			typedef T raw;
		};

		template<typename T>
		struct Type<T*>
		{
			typedef typename Type<T>::raw raw;
		};

		template<typename T>
		struct Type<T&>
		{
			typedef typename Type<T>::raw raw;
		};

		template<typename T>
		struct Type<const T>
		{
			typedef typename Type<T>::raw raw;
		};


		template<typename Target>
		struct TargetCast
		{
			static Target& Cast(Target& src) { return src; }
			static Target& Cast(Target* src) { SYNC4CPP_ASSERT(src); return *src; }
		};

		template<typename Target>
		struct TargetCast<Target*>
		{
			static Target* Cast(Target& src) { return &src; }
			static Target* Cast(Target* src) { SYNC4CPP_ASSERT(src); return src; }
		};


		template<typename Target>
		struct TargetCast<Target&>
		{
			static Target& Cast(Target& src) { return src; }
			static Target& Cast(Target* src) { SYNC4CPP_ASSERT(src); return *src; }
		};

		template<typename Source>
		Source& to_ref(Source& src) { return src; }

		template<typename Source>
		Source& to_ref(Source* src) { SYNC4CPP_ASSERT(src); return *src; }

		template<typename Target>
		struct RefOrPtr	{ typedef Target& Arg; };
		template<typename Target>
		struct RefOrPtr<Target*> { typedef Target* Arg; };

		template<class Type>
		struct identity
		{
			typedef Type type;

			const type& operator ()(const type& left) const
			{
				return (left);
			}
		};

		struct Nil
		{
		};

		static Nil NilInstance;


		template<typename RawMutex, typename Operation>
		struct register_guard
		{
			typedef typename RawMutex::mutex_type mutex_type;
			typedef RawMutex target_mutex;
			typedef typename register_guard<mutex_type, Operation>::guard_type internal_guard_type;

			typedef struct wrapper_guard
				: public internal_guard_type
			{
				typedef typename RawMutex::mutex_type mutex_type;

				wrapper_guard(RawMutex& m)
					:internal_guard_type(m.getSyncMutex())
				{}

			} guard_type;
		};



		template<typename Operation>
		struct register_guard<Nil, Operation>
		{
			typedef Nil guard_type;
			typedef Nil target_mutex;
		};

		template<typename Guard, typename TargetMutex>
		class mutex_wrapper
		{
		public:
			typedef Guard		guard_type;
			typedef TargetMutex target_mutex;

			typedef TargetCast<target_mutex> cast;


			template<typename Mutex>
			mutex_wrapper(Mutex& mutex)
				: mMutex(cast::Cast(mutex))
			{
			}

			operator target_mutex& ()
			{
				return mMutex;
			}

		private:
			target_mutex& mMutex;
		};



		template<typename ValueType, typename Mutex, typename Operation>
		class syncguard
		{
		public:
			typedef typename Operation::template select_guard<Mutex>::guard guard_type;
			typedef ValueType value_type;

			template<bool InheritSyncable>
			syncguard(synchronized<ValueType, Mutex, InheritSyncable>& toSync)
				: mGuard(toSync.getSyncMutex()), mValueRef(toSync.mValue)
			{}

			template<bool InheritSyncable>
			syncguard(synchronized<ValueType, Mutex, InheritSyncable>* toSync)
				: mGuard(toSync->getSyncMutex()), mValueRef(toSync->mValue)
			{}

			value_type& value() const
			{
				return mValueRef;
			}

		private:
			guard_type mGuard;
			value_type& mValueRef;
		};

		template<typename ValueType, typename Mutex, bool InheritSyncable, typename Operation>
		struct register_guard<synchronized<ValueType, Mutex, InheritSyncable>, Operation>
		{
			typedef syncguard<ValueType, Mutex, Operation> guard_type;
			typedef synchronized<ValueType, Mutex, InheritSyncable> target_mutex;
		};

	}


	





	template<typename Mutex>
	class syncable
	{
	public:
		typedef Mutex mutex_type;

		virtual mutex_type& getSyncMutex() const = 0;
	};

	template<typename Mutex = typename defaults<>::mutex, bool InheritSyncable = true>
	class syncable_mutex
		: public syncable<Mutex>
	{
	public:
		typedef typename syncable<Mutex>::mutex_type mutex_type;

		syncable_mutex()
		{}

		syncable_mutex(const syncable_mutex& m)
			: _mSync4cppMutex()
		{}

		syncable_mutex(const syncable_mutex&& m)
			: _mSync4cppMutex()
		{}

		syncable_mutex& operator =(const syncable_mutex& mutex)
		{
		}

		inline virtual mutex_type& getSyncMutex() const
		{
			return _mSync4cppMutex;
		}
	private:

		mutable mutex_type _mSync4cppMutex;
	};

	template<typename Mutex>
	class syncable_mutex<Mutex, false>
	{
	public:
		typedef typename syncable<Mutex>::mutex_type mutex_type;

		syncable_mutex()
		{}

		syncable_mutex(const syncable_mutex& m)
			: _mSync4cppMutex()
		{}

		syncable_mutex(const syncable_mutex&& m)
			: _mSync4cppMutex()
		{}

		syncable_mutex& operator =(const syncable_mutex& mutex)
		{
		}

		inline mutex_type& getSyncMutex() const
		{
			return _mSync4cppMutex;
		}
	private:

		mutable mutex_type _mSync4cppMutex;
	};



	template<typename Tag>
	struct mutex_operation
	{
		typedef mutex_operation<Tag> operation_type;

		template<typename Mutex>
		struct select_guard
		{
			typedef typename internal::Type<Mutex>::raw raw_mutex;
			typedef typename internal::register_guard<raw_mutex, operation_type>::guard_type	guard;
			typedef typename internal::register_guard<raw_mutex, operation_type>::target_mutex	target_mutex;
		};

		template<typename Mutex>
		inline static internal::mutex_wrapper<typename select_guard<Mutex>::guard, typename select_guard<Mutex>::target_mutex> with(Mutex& mutex)
		{
			return internal::mutex_wrapper<typename select_guard<Mutex>::guard, typename select_guard<Mutex>::target_mutex>(mutex);
		}

		template<typename Guard, typename TargetMutex>
		inline static void with(const internal::mutex_wrapper<Guard, TargetMutex>&)
		{
			// Can not wrap an already wrapped mutex!
			static_assert(sizeof(Guard) != sizeof(Guard), "You can not wrap an already wrapped mutex!");
		}
	};

	typedef mutex_operation<struct ExclusiveTag> exclusive;
	typedef mutex_operation<struct SharedTag> shared;
	typedef exclusive default_operation;



	template<typename ValueType, typename Mutex, bool InheritSyncable>
	class synchronized
		: public syncable_mutex<Mutex, InheritSyncable>
	{
		template<typename ValueType_, typename Mutex_, typename Operation>
		friend class internal::syncguard;

		inline synchronized(const synchronized& p)
		{

		}
	public:
		typedef syncable_mutex<Mutex, InheritSyncable>	base_type;
		typedef typename base_type::mutex_type			mutex_type;
		typedef ValueType								value_type;

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

	namespace internal {


		template<typename Operation = void>
		struct void_operation_selector
		{
			typedef Operation operation;
		};

		template<typename Operation>
		struct void_to_default_operation
		{
			typedef Operation operation;
		};

		template<>
		struct void_to_default_operation<void>
		{
			typedef default_operation operation;
		};

		template<typename Mutex>
		struct wrap_with_default_if_not_wrapped
		{
		private:
			typedef default_operation::select_guard<Mutex> selector;
		public:
			typedef mutex_wrapper<typename selector::guard, typename selector::target_mutex> wrapped_mutex;
		};

		template<typename Guard, typename TargetMutex>
		struct wrap_with_default_if_not_wrapped<mutex_wrapper<Guard, TargetMutex> >
		{
			typedef mutex_wrapper<Guard, TargetMutex> wrapped_mutex;
		};

		template<typename Mutex, typename Operation>
		struct wrap_and_select_mutex
		{
		private:
			typedef typename void_to_default_operation<Operation>::operation operation;
			typedef typename operation::template select_guard<Mutex> selector;
		public:
			typedef mutex_wrapper<typename selector::guard, typename selector::target_mutex> target_mutex;
			typedef typename target_mutex::guard_type guard_type;
		};

		template<typename Guard, typename TargetMutex>
		struct wrap_and_select_mutex< mutex_wrapper<Guard, TargetMutex>, void>
		{
			typedef mutex_wrapper<Guard, TargetMutex> target_mutex;
			typedef typename target_mutex::guard_type guard_type;
		};

		template<typename Guard, typename TargetMutex, typename Operation>
		struct wrap_and_select_mutex< mutex_wrapper<Guard, TargetMutex>, Operation>
		{
			static_assert(sizeof(Guard) != sizeof(Guard), "You can not wrap an already wrapped mutex!");
			typedef mutex_wrapper<Guard, TargetMutex> target_mutex;
			typedef typename target_mutex::guard_type guard_type;
		};


		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		struct GuardArray
		{
			typedef typename wrap_with_default_if_not_wrapped<T1>::wrapped_mutex mutex_type1;
			typedef typename wrap_with_default_if_not_wrapped<T2>::wrapped_mutex mutex_type2;
			typedef typename wrap_with_default_if_not_wrapped<T3>::wrapped_mutex mutex_type3;
			typedef typename wrap_with_default_if_not_wrapped<T4>::wrapped_mutex mutex_type4;
			typedef typename wrap_with_default_if_not_wrapped<T5>::wrapped_mutex mutex_type5;
			typedef typename wrap_with_default_if_not_wrapped<T6>::wrapped_mutex mutex_type6;
			typedef typename wrap_with_default_if_not_wrapped<T7>::wrapped_mutex mutex_type7;
			typedef typename wrap_with_default_if_not_wrapped<T8>::wrapped_mutex mutex_type8;

			inline GuardArray(	typename RefOrPtr<T1>::Arg g1 = NilInstance,
								typename RefOrPtr<T2>::Arg g2 = NilInstance,
								typename RefOrPtr<T3>::Arg g3 = NilInstance,
								typename RefOrPtr<T4>::Arg g4 = NilInstance,
								typename RefOrPtr<T5>::Arg g5 = NilInstance,
								typename RefOrPtr<T6>::Arg g6 = NilInstance,
								typename RefOrPtr<T7>::Arg g7 = NilInstance,
								typename RefOrPtr<T8>::Arg g8 = NilInstance,
								...)
				: done(false)
				, guard1(mutex_type1::cast::Cast(g1))
				, guard2(mutex_type2::cast::Cast(g2))
				, guard3(mutex_type3::cast::Cast(g3))
				, guard4(mutex_type4::cast::Cast(g4))
				, guard5(mutex_type5::cast::Cast(g5))
				, guard6(mutex_type6::cast::Cast(g6))
				, guard7(mutex_type7::cast::Cast(g7))
				, guard8(mutex_type8::cast::Cast(g8))
			{}

			bool done;
			typename mutex_type1::guard_type guard1;
			typename mutex_type2::guard_type guard2;
			typename mutex_type3::guard_type guard3;
			typename mutex_type4::guard_type guard4;
			typename mutex_type5::guard_type guard5;
			typename mutex_type6::guard_type guard6;
			typename mutex_type7::guard_type guard7;
			typename mutex_type8::guard_type guard8;
		};

		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		inline GuardArray<T1, T2, T3, T4, T5, T6, T7, T8>
			GetGuardType(const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6, const T7& p7, const T8& p8, ...)
		{
			throw "Never call this!";
		}

	}
}
#define SYNC4CPP_SET_DEFAULT_MUTEX(_mutex)		namespace sync4cpp {template<> struct defaults<void> { typedef _mutex mutex; }; }
#define SYNC4CPP_REGISTER_GUARD(_mutex, _operation, _guard)	namespace sync4cpp { namespace internal {template<> struct register_guard<typename Type<_mutex>::raw, _operation> {typedef _guard guard_type; typedef _mutex target_mutex;};} }


#define SYNC4CPP_SYNCHERE(...)		decltype(sync4cpp::internal::GetGuardType(__VA_ARGS__, sync4cpp::internal::Nil(), sync4cpp::internal::Nil(), sync4cpp::internal::Nil(), sync4cpp::internal::Nil(), sync4cpp::internal::Nil(), sync4cpp::internal::Nil(), sync4cpp::internal::Nil()))  SYNC4CPP_LOCK_NAME(_lockarray_in_line_) (__VA_ARGS__);
#define SYNC4CPP_SYNCHRONIZE(...)	for(SYNC4CPP_SYNCHERE(__VA_ARGS__) !SYNC4CPP_LOCK_NAME(_lockarray_in_line_).done; SYNC4CPP_LOCK_NAME(_lockarray_in_line_).done = true)
#define SYNC4CPP_DETERMINATE_MUTEX(_src, ...)	sync4cpp::internal::wrap_and_select_mutex<decltype(_src), sync4cpp::internal::void_operation_selector<__VA_ARGS__>::operation>::target_mutex
#define SYNC4CPP_SYNCUSE(_name, ...)			SYNC4CPP_SYNCBIND(_name, _name, __VA_ARGS__)
#define SYNC4CPP_SYNCBIND(_name, _src, ...)		SYNC4CPP_DETERMINATE_MUTEX(_src, __VA_ARGS__)::guard_type SYNC4CPP_BUILD_NAME(SYNC4CPP_LOCK_NAME(_lock_in_line_),_name)(SYNC4CPP_DETERMINATE_MUTEX(_src, __VA_ARGS__)::cast::Cast(_src)); auto& _name = SYNC4CPP_BUILD_NAME(SYNC4CPP_LOCK_NAME(_lock_in_line_),_name).value();
#define SYNC4CPP_SYNCCAST(_src, ...)			(SYNC4CPP_DETERMINATE_MUTEX(_src, __VA_ARGS__)::guard_type(SYNC4CPP_DETERMINATE_MUTEX(_src, __VA_ARGS__)::cast::Cast(_src))).value()


//#define syncuse(_op, ...)		auto pair##_op  auto& _op
// syncuse(mLocked) syncuse(lock = mLocked)
// sync_here(sync4cpp::shared <= mutex)


#endif
