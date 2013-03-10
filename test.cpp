#include <iostream>
#include <string>
//#include <boost/thread.hpp>
//#include "sync4cpp-boost.hpp"
#include "sync4cpp.hpp"
#include <map>


using namespace std;


typedef sync4cpp::mutex_modifier<struct testmTag, int> testm;

struct test
{

};

struct test_guard
{
	test_guard(test*, int i){cout << "in\n" << i << "\n";}
	~test_guard() {cout << "out\n";}
};

SYNC4CPP_REGISTER_MUTEX(test*);
SYNC4CPP_REGISTER_GUARD(test*, testm, test_guard, sync4cpp::map_mutex, sync4cpp::map<0>);
SYNC4CPP_SET_DEFAULT_GUARD(test*, testm, 1);

using sync4cpp::assignment;

template<typename InternalMutex>
struct test_decor
	: public sync4cpp::decor<test_decor<InternalMutex>>
{
	SYNC4CPP_PLACE_DEFAULT_MODIFIER_FACTORY(typename sync4cpp::get_default_modifier_factory<InternalMutex>::type);
	template<typename Mutex, typename Modifier>
	struct guard
	{
		typedef typename sync4cpp::guard<assignment<InternalMutex, Modifier>>::type base_type;
		typedef struct Wrapper
			: public base_type
		{
			Wrapper(assignment<Mutex, Modifier>& as)
				: base_type(as[as.mutex().getMutex()])
			{}
		} type;
	};
};

struct decorated
	: public test_decor<test*>
{
	test*& getMutex()
	{
		return m;
	}

	test* m;
};


struct hallo
	: public sync4cpp::syncable<test*>
{

};

int main()
{
	static_assert(sync4cpp::is_decorated<decorated>::value, "");
	//static_assert(std::is_same<typename sync4cpp::traits::mutex_registry<test*>::template guard<sync4cpp::exclusive>::guard_type, test_guard>::value, "!!!!!!!!!!!!!!!!!!!!!!!");
	hallo mutex;
	{
		SYNC4CPP_SYNCHERE(mutex);
		//auto guard = SYNC4CPP_SYNCGUARD(testm(5) << mutex);
	}


	std::cin.get();
	return 0;
}