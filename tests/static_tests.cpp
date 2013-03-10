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
	


	// ************* Check some meta functions ************* //
	BOOST_STATIC_ASSERT((sync4cpp::is_registered_mutex<ValueMutex>::value));
	BOOST_STATIC_ASSERT((sync4cpp::has_registered_guard<ValueMutex, exclusive>::value));
	BOOST_STATIC_ASSERT((sync4cpp::has_registered_guard<ValueMutex, shared>::value));
	BOOST_STATIC_ASSERT((!sync4cpp::is_decorated<ValueMutex>::value));
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
	BOOST_STATIC_ASSERT((sync4cpp::is_registered_mutex<PointerMutex*>::value));
	BOOST_STATIC_ASSERT((sync4cpp::has_registered_guard<PointerMutex*, exclusive>::value));
	BOOST_STATIC_ASSERT((sync4cpp::has_registered_guard<PointerMutex*, shared>::value));
	BOOST_STATIC_ASSERT((!sync4cpp::is_decorated<PointerMutex*>::value));
}




