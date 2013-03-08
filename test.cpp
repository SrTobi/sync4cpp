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
	test_guard(test*, int i){cout << "in\n" << i;}
	~test_guard() {cout << "out\n";}
};



SYNC4CPP_REGISTER_MUTEX(test*);
SYNC4CPP_REGISTER_GUARD(test*, testm, test_guard, sync4cpp::map_mutex, sync4cpp::map<0>);
SYNC4CPP_SET_DEFAULT_GUARD(test*, testm, 1);


int main()
{
	//static_assert(std::is_same<typename sync4cpp::traits::mutex_registry<test*>::template guard<sync4cpp::exclusive>::guard_type, test_guard>::value, "!!!!!!!!!!!!!!!!!!!!!!!");
	test* mutex  =0;
	{
		sync4cpp::guard<decltype(mutex)>::type guard(mutex);
	}

	std::cin.get();
	return 0;
}