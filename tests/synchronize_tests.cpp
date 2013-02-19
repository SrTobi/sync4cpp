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

SYNC4CPP_REGISTER_GUARD(ValueMockMutex, exclusive, ExclusiveValueMockGuard);
SYNC4CPP_REGISTER_GUARD(ValueMockMutex, shared, SharedValueMockGuard);

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

		SYNC4CPP_SYNCHRONIZE(exclusive::with(mutex))
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

		SYNC4CPP_SYNCHRONIZE(shared::with(mutex))
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

		SYNC4CPP_SYNCHRONIZE(shared::with(mutex))
		{
			mock.expect(LockedAccess);

			SYNC4CPP_SYNCHRONIZE(shared::with(mutex))
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

		SYNC4CPP_SYNCHRONIZE(mutex, shared::with(mutex))
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
			SYNC4CPP_SYNCHERE(mutex, shared::with(mutex));
			mock.expect(LockedAccess);
		}
	}

}

BOOST_AUTO_TEST_CASE(mutex_as_pointer_test)
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
		ValueMockMutex* pmutex = &mutex;

		SYNC4CPP_SYNCHRONIZE(&mutex, shared::with(pmutex))
		{
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

SYNC4CPP_REGISTER_GUARD(PointerMockMutex*, exclusive, ExclusivePointerMockGuard);
SYNC4CPP_REGISTER_GUARD(PointerMockMutex*, shared, SharedPointerMockGuard);

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