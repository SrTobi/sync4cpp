#include "tests.hpp"

#include "sync4cpp.hpp"





enum MockEvents
{
	MutexCreated,

	ExclusiveLock,
	ExclusiveUnLock,

	MutexDestroyed,

	LockedAccess
};


class SynchronizedMockMutex
//	: boost::noncopyable
{
public:
	SynchronizedMockMutex(MockObserver<MockEvents> obs)
		: observer(obs)
	{
		observer.expect(MutexCreated);
	}

	SynchronizedMockMutex(const SynchronizedMockMutex&& other)
	{
		observer = other.observer;
	}

	~SynchronizedMockMutex()
	{
		observer.expect(MutexDestroyed);
	}

	MockObserver<MockEvents> observer;
};

class ExclusiveSynchronizedMockGuard
	: boost::noncopyable
{
public:
	ExclusiveSynchronizedMockGuard(SynchronizedMockMutex& m)
		: observer(m.observer)
	{
		observer.expect(ExclusiveLock);
	}


	~ExclusiveSynchronizedMockGuard()
	{
		observer.expect(ExclusiveUnLock);
	}

private:
	MockObserver<MockEvents> observer;
};


//////////////////////////////////////////////////////////////////////////

SYNC4CPP_REGISTER_MUTEX(SynchronizedMockMutex)
SYNC4CPP_REGISTER_GUARD(SynchronizedMockMutex, exclusive, ExclusiveSynchronizedMockGuard);
SYNC4CPP_SET_DEFAULT_GUARD(SynchronizedMockMutex, exclusive);

//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(synchronized_lock_test)
{
	MockObserver<MockEvents> mock;
	mock.set()	<< MutexCreated
				<< MutexDestroyed
				<< ExclusiveLock
				<< LockedAccess
				<< ExclusiveUnLock
				<< MutexDestroyed;

	{

		sync4cpp::synchronized<int, SynchronizedMockMutex> synced(100, SynchronizedMockMutex(mock));

		{
			SYNC4CPP_SYNCUSE(synced);
			BOOST_CHECK_EQUAL(synced, 100);
			mock.expect(LockedAccess);
		}
	}

}