#include "tests.hpp"

#include "sync4cpp.hpp"





using sync4cpp::shared;
using sync4cpp::exclusive;

class ValueMutex
{

};

class ExclusiveValueGuard
{
	ExclusiveValueGuard(ValueMutex&)
	{

	}
};


class SharedValueGuard
{
	SharedValueGuard(ValueMutex&)
	{

	}
};


SYNC4CPP_REGISTER_GUARD(ValueMutex, exclusive, ExclusiveValueGuard);
SYNC4CPP_REGISTER_GUARD(ValueMutex, shared, SharedValueGuard);





BOOST_AUTO_TEST_CASE(determination_test_for_value_mutex)
{
	ValueMutex mutex;

	// Suppress warnings
	(void) mutex;

	// ************* Check if target mutex is right ************* //
	// With reference
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex)::target_mutex,
							ValueMutex
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex, shared)::target_mutex,
							ValueMutex
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex, exclusive)::target_mutex,
							ValueMutex
						>::value));
	
	// ************* Check if target mutex is right ************* //
	// With pointer
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex)::target_mutex,
							ValueMutex
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex, shared)::target_mutex,
							ValueMutex
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex, exclusive)::target_mutex,
							ValueMutex
						>::value));


	// ************* Check if guard is right ************* //
	// With reference
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex)::guard_type,
							ExclusiveValueGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex, shared)::guard_type,
							SharedValueGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex, exclusive)::guard_type,
							ExclusiveValueGuard
						>::value));

	
	// ************* Check if guard is right ************* //
	// With pointer
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex)::guard_type,
							ExclusiveValueGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex, shared)::guard_type,
							SharedValueGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex, exclusive)::guard_type,
							ExclusiveValueGuard
						>::value));
}






class PointerMutex
{

};

class ExclusivePointerGuard
{
	ExclusivePointerGuard(PointerMutex*)
	{

	}
};


class SharedPointerGuard
{
	SharedPointerGuard(PointerMutex*)
	{

	}
};

SYNC4CPP_REGISTER_GUARD(PointerMutex*, exclusive, ExclusivePointerGuard);
SYNC4CPP_REGISTER_GUARD(PointerMutex*, shared, SharedPointerGuard);


BOOST_AUTO_TEST_CASE(determination_test_for_pointer_mutex)
{
	PointerMutex mutex;

	// Suppress warnings
	(void) mutex;

	// ************* Check if target mutex is right ************* //
	// With reference
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex)::target_mutex,
							PointerMutex*
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex, shared)::target_mutex,
							PointerMutex*
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex, exclusive)::target_mutex,
							PointerMutex*
						>::value));
	
	// ************* Check if target mutex is right ************* //
	// With pointer
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex)::target_mutex,
							PointerMutex*
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex, shared)::target_mutex,
							PointerMutex*
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex, exclusive)::target_mutex,
							PointerMutex*
						>::value));


	// ************* Check if guard is right ************* //
	// With reference
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex)::guard_type,
							ExclusivePointerGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex, shared)::guard_type,
							SharedPointerGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(mutex, exclusive)::guard_type,
							ExclusivePointerGuard
						>::value));

	
	// ************* Check if guard is right ************* //
	// With pointer
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex)::guard_type,
							ExclusivePointerGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex, shared)::guard_type,
							SharedPointerGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_same<
							SYNC4CPP_DETERMINATE_MUTEX(&mutex, exclusive)::guard_type,
							ExclusivePointerGuard
						>::value));
}




