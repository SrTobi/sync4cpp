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

using sync4cpp::detail::mutex_assignment;

template<typename InternalMutex>
struct test_decor
	: public sync4cpp::decor<test_decor<InternalMutex>>
{
	template<typename Mutex, typename Modifier>
	struct guard
	{
		typedef typename sync4cpp::guard<mutex_assignment<InternalMutex, Modifier>>::type base_type;
		typedef struct Wrapper
			: public base_type
		{
			Wrapper(mutex_assignment<Mutex, Modifier>& m)
				: base_type(mutex_assignment<InternalMutex, Modifier>(&m.mutex->getMutex(), m.params))
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



int main()
{
	static_assert(sync4cpp::is_decorated<decorated>::value, "");
	//static_assert(std::is_same<typename sync4cpp::traits::mutex_registry<test*>::template guard<sync4cpp::exclusive>::guard_type, test_guard>::value, "!!!!!!!!!!!!!!!!!!!!!!!");
	decorated mutex;
	{
		sync4cpp::guard<decltype(testm(5) << mutex)>::type guard(testm(9) << mutex);
	}

	std::cin.get();
	return 0;
}