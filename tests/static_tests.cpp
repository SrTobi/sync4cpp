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

SYNC4CPP_REGISTER_MUTEX(ValueMutex);
SYNC4CPP_REGISTER_GUARD(ValueMutex, exclusive, ExclusiveValueGuard);
SYNC4CPP_REGISTER_GUARD(ValueMutex, shared, SharedValueGuard);
SYNC4CPP_SET_DEFAULT_GUARD(ValueMutex, exclusive);




BOOST_AUTO_TEST_CASE(determination_test_for_value_mutex)
{
	ValueMutex mutex;

	(void) mutex;


	// ************* Check if guard is right ************* //
	// With reference
	BOOST_STATIC_ASSERT((std::is_convertible<
							typename sync4cpp::guard<ValueMutex>::type,
							ExclusiveValueGuard
						>::value));

	BOOST_STATIC_ASSERT((std::is_convertible<
							typename sync4cpp::guard<decltype(exclusive() << mutex)>::type,
							ExclusiveValueGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_convertible<
							typename sync4cpp::guard<decltype(shared() << mutex)>::type,
							SharedValueGuard
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

SYNC4CPP_REGISTER_MUTEX(PointerMutex*);
SYNC4CPP_REGISTER_GUARD(PointerMutex*, exclusive, ExclusivePointerGuard);
SYNC4CPP_REGISTER_GUARD(PointerMutex*, shared, SharedPointerGuard);
SYNC4CPP_SET_DEFAULT_GUARD(PointerMutex*, exclusive);


BOOST_AUTO_TEST_CASE(determination_test_for_pointer_mutex)
{
	PointerMutex* mutex;

	// Suppress warnings
	(void) mutex;
	
	// ************* Check if guard is right ************* //
	// With reference
	BOOST_STATIC_ASSERT((std::is_convertible<
							typename sync4cpp::guard<PointerMutex*>::type,
							ExclusivePointerGuard
						>::value));

	BOOST_STATIC_ASSERT((std::is_convertible<
							typename sync4cpp::guard<decltype(exclusive() << mutex)>::type,
							ExclusivePointerGuard
						>::value));
	
	BOOST_STATIC_ASSERT((std::is_convertible<
							typename sync4cpp::guard<decltype(shared() << mutex)>::type,
							SharedPointerGuard
						>::value));
}




