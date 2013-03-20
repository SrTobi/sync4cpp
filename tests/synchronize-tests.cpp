#include "tests.hpp"

#include "sync4cpp.hpp"


using sync4cpp::shared;
using sync4cpp::exclusive;



enum MockEvents
{
	MutexCreated,

	ExclusiveLock,
	ExclusiveUnLock,

	SharedLock,
	SharedUnLock,

	CustomLock,
	CustomUnLock,

	MutexDestroyed,

	LockedAccess
};


class ValueMockMutex
	: boost::noncopyable
{
public:
	ValueMockMutex(MockObserver<MockEvents> obs)
		: observer(obs)
	{
		observer.expect(MutexCreated);
	}

	~ValueMockMutex()
	{
		observer.expect(MutexDestroyed);
	}

	MockObserver<MockEvents> observer;
};

class ExclusiveValueMockGuard
	: boost::noncopyable
{
public:
	ExclusiveValueMockGuard(ValueMockMutex& m)
		: observer(m.observer)
	{
		observer.expect(ExclusiveLock);
	}

	~ExclusiveValueMockGuard()
	{
		observer.expect(ExclusiveUnLock);
	}

private:
	MockObserver<MockEvents> observer;
};


class SharedValueMockGuard
	: boost::noncopyable
{
public:
	SharedValueMockGuard(ValueMockMutex& m)
		: observer(m.observer)
	{
		observer.expect(SharedLock);
	}

	~SharedValueMockGuard()
	{
		observer.expect(SharedUnLock);
	}

private:
	MockObserver<MockEvents> observer;
};


//////////////////////////////////////////////////////////////////////////

SYNC4CPP_REGISTER_MUTEX(ValueMockMutex)
SYNC4CPP_REGISTER_GUARD(ValueMockMutex, exclusive, ExclusiveValueMockGuard);
SYNC4CPP_REGISTER_GUARD(ValueMockMutex, shared, SharedValueMockGuard);
SYNC4CPP_SET_DEFAULT_GUARD(ValueMockMutex, exclusive);

//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(simple_exclusive_lock_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
				<< ExclusiveLock
				<< LockedAccess
				<< ExclusiveUnLock
				<< MutexDestroyed;

	{
		ValueMockMutex mutex(mock);

		SYNC4CPP_SYNCHRONIZE(mutex)
		{
			mock.expect(LockedAccess);
		}
	}

}

BOOST_AUTO_TEST_CASE(simple_explicit_exclusive_lock_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
		<< ExclusiveLock
		<< LockedAccess
		<< ExclusiveUnLock
		<< MutexDestroyed;

	{
		ValueMockMutex mutex(mock);

		SYNC4CPP_SYNCHRONIZE(exclusive() << mutex)
		{
			mock.expect(LockedAccess);
		}
	}

}

BOOST_AUTO_TEST_CASE(simple_shared_lock_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
				<< SharedLock
				<< LockedAccess
				<< SharedUnLock
				<< MutexDestroyed;

	{
		ValueMockMutex mutex(mock);

		SYNC4CPP_SYNCHRONIZE(shared() << mutex)
		{
			mock.expect(LockedAccess);
		}
	}

}


BOOST_AUTO_TEST_CASE(nested_shared_lock_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
				<< SharedLock
				<< LockedAccess
				<< SharedLock
				<< LockedAccess
				<< SharedUnLock
				<< SharedUnLock
				<< MutexDestroyed;

	{
		ValueMockMutex mutex(mock);

		SYNC4CPP_SYNCHRONIZE(shared() << mutex)
		{
			mock.expect(LockedAccess);

			SYNC4CPP_SYNCHRONIZE(shared() << mutex)
			{
				mock.expect(LockedAccess);
			}
		}
	}

}

BOOST_AUTO_TEST_CASE(lock_array_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
				<< ExclusiveLock
				<< SharedLock
				<< LockedAccess
				<< SharedUnLock
				<< ExclusiveUnLock
				<< MutexDestroyed;

	{
		ValueMockMutex mutex(mock);

		SYNC4CPP_SYNCHRONIZE(mutex, shared() << mutex)
		{
			mock.expect(LockedAccess);
		}
	}

}

BOOST_AUTO_TEST_CASE(lock_array_here_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
				<< ExclusiveLock
				<< SharedLock
				<< LockedAccess
				<< SharedUnLock
				<< ExclusiveUnLock
				<< MutexDestroyed;

	{
		ValueMockMutex mutex(mock);

		{
			SYNC4CPP_SYNCHERE(mutex, shared() << mutex);
			mock.expect(LockedAccess);
		}
	}

}



/************************************** Now with pointer type *******************************************/

class PointerMockMutex
	: boost::noncopyable
{
public:
	PointerMockMutex(MockObserver<MockEvents> obs)
		: observer(obs)
	{
		observer.expect(MutexCreated);
	}

	~PointerMockMutex()
	{
		observer.expect(MutexDestroyed);
	}

	MockObserver<MockEvents> observer;
};

class ExclusivePointerMockGuard
	: boost::noncopyable
{
public:
	ExclusivePointerMockGuard(PointerMockMutex* m)
		: observer(m->observer)
	{
		observer.expect(ExclusiveLock);
	}

	~ExclusivePointerMockGuard()
	{
		observer.expect(ExclusiveUnLock);
	}

private:
	MockObserver<MockEvents> observer;
};


class SharedPointerMockGuard
	: boost::noncopyable
{
public:
	SharedPointerMockGuard(PointerMockMutex* m)
		: observer(m->observer)
	{
		observer.expect(SharedLock);
	}

	~SharedPointerMockGuard()
	{
		observer.expect(SharedUnLock);
	}

private:
	MockObserver<MockEvents> observer;
};


//////////////////////////////////////////////////////////////////////////

SYNC4CPP_REGISTER_MUTEX(PointerMockMutex*);
SYNC4CPP_REGISTER_GUARD(PointerMockMutex*, exclusive, ExclusivePointerMockGuard);
SYNC4CPP_REGISTER_GUARD(PointerMockMutex*, shared, SharedPointerMockGuard);
SYNC4CPP_SET_DEFAULT_GUARD(PointerMockMutex*, exclusive);

//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(simple_pointer_lock_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
				<< ExclusiveLock
				<< LockedAccess
				<< ExclusiveUnLock
				<< MutexDestroyed;

	{
		PointerMockMutex mutex(mock);
		PointerMockMutex* pmutex = &mutex;

		SYNC4CPP_SYNCHRONIZE(pmutex)
		{
			mock.expect(LockedAccess);
		}
	}

}


/************************************** check is_locked *******************************************/

class IsLockMockMutex
	: boost::noncopyable
{
public:
	IsLockMockMutex(MockObserver<MockEvents> obs)
		: observer(obs)
	{
		observer.expect(MutexCreated);
	}

	~IsLockMockMutex()
	{
		observer.expect(MutexDestroyed);
	}

	MockObserver<MockEvents> observer;
};


class IsLockMockGuard
{
public:
	IsLockMockGuard(IsLockMockMutex& mutex, bool lock)
		: observer(mutex.observer)
		, locked(lock)
	{
		observer.expect(CustomLock);
	}

	~IsLockMockGuard()
	{
		observer.expect(CustomUnLock);
	}

	MockObserver<MockEvents> observer;
	bool locked;
};

namespace sync4cpp {namespace traits {

	static bool is_locked(const IsLockMockGuard& guard)
	{
		return guard.locked;
	}

}}

typedef sync4cpp::mutex_modifier<struct IsLockModifierTag, bool> IsLockModifier;

//////////////////////////////////////////////////////////////////////////

SYNC4CPP_REGISTER_MUTEX(IsLockMockMutex);
SYNC4CPP_REGISTER_GUARD(IsLockMockMutex, IsLockModifier, IsLockMockGuard,
						sync4cpp::map_mutex, sync4cpp::map<0>);

//////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(is_lock_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
				<< CustomLock
				<< LockedAccess
				<< CustomUnLock
				<< MutexDestroyed
				<< MutexCreated
				<< CustomLock
				<< LockedAccess
				<< CustomUnLock
				<< MutexDestroyed;

	{
		IsLockMockMutex mutex(mock);

		{
			SYNC4CPP_SYNCGUARD(guard, IsLockModifier(true) << mutex);
			BOOST_CHECK(sync4cpp::is_locked(guard) == true);
			mock.expect(LockedAccess);
		}
	}


	{
		IsLockMockMutex mutex(mock);

		{
			SYNC4CPP_SYNCGUARD(guard, IsLockModifier(false) << mutex);
			BOOST_CHECK(sync4cpp::is_locked(guard) == false);
			mock.expect(LockedAccess);
		}
	}

}
