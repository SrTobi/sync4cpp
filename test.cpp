#include <iostream>
#include <string>
//#include <boost/thread.hpp>
//#include "sync4cpp-boost.hpp"
#include "sync4cpp.hpp"
#include <map>


using namespace std;



struct test
{

};

struct test_guard
{
	test_guard(test&){}
	~test_guard() {}
};



SYNC4CPP_REGISTER_MUTEX(test);
SYNC4CPP_REGISTER_GUARD(test, sync4cpp::exclusive, test_guard);
SYNC4CPP_SET_DEFAULT_GUARD(test, test_guard);


int main()
{
	test mutex;
	sync4cpp::guard<test>::type guard(mutex);

	std::cin.get();
	return 0;
}