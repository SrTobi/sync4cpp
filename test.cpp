#include <iostream>
#include <string>
//#include <boost/thread.hpp>
//#include "sync4cpp-boost.hpp"
#include "sync4cpp.hpp"
#include <map>


using namespace std;


struct testMutex
{

};

struct testGuard
{
	testGuard(testMutex&)
	{
		std::cout << "lock\n";
	}

	~testGuard()
	{
		std::cout << "unlock\n";
	}
};

struct testSharedGuard
{
	testSharedGuard(testMutex&)
	{
		std::cout << "shared lock\n";
	}

	~testSharedGuard()
	{
		std::cout << "shared unlock\n";
	}
};

SYNC4CPP_REGISTER_GUARD(testMutex, sync4cpp::exclusive, testGuard);
SYNC4CPP_REGISTER_GUARD(testMutex, sync4cpp::shared, testSharedGuard);
//SYNC4CPP_REGISTER_GUARD(boost::mutex, sync4cpp::default_operation, boost::lock_guard<boost::mutex>)

class testSync: public sync4cpp::syncable_mutex<testMutex>
{

};
/*

template<typename T = void>
class wrapper;

template<typename T = void>
class holder
{
public:
	holder()
	{
		i = "hallo";
	}

	
	wrapper<void> operator ->();
	wrapper<void> synced();

	string i;
};

template<typename T>
class wrapper
{
public:
	inline wrapper(holder<>& h)
		:i(h.i)
	{
		std::cout << "wrapper construct\n";
	}
	inline wrapper(holder<>&& h)
		:i(h.i)
	{
		std::cout << "wrapper construct&&\n";
	}


	inline wrapper(const wrapper& h)
		:i(h.i)
	{
		std::cout << "wrapper copy construct\n";
	}
	wrapper(const wrapper&& h)
		:i(h.i)
	{
		std::cout << "wrapper copy construct&&\n";
	}

	wrapper& operator =(const wrapper& w)
	{
		std::cout << "wrapper copy\n";
		i = w.i;
		return *this;
	}

	wrapper& operator =(const wrapper&& w)
	{
		std::cout << "wrapper copy&&\n";
		i = w.i;
		return *this;
	}

	~wrapper()
	{
		std::cout << "wrapper destroyed\n";
	}

	operator string& () const
	{
		return i;
	}

	string* operator -> () const
	{
		return &i;
	}

	string& i;
};

template<>
wrapper<void> holder<void>::operator ->()
{
	return wrapper<void>(*this);
}

template<>
wrapper<void> holder<void>::synced()
{
	return wrapper<void>(*this);
}

	wrapper<void> synced(bool x = false)
	{
		return x? *reinterpret_cast<wrapper<void>* >(0) : wrapper<void>(*this);
	}

void dos(string i)
{
	std::cout << i << endl;
}*/


	/*
int main()
{
	typedef sync4cpp::shared sh;
	sync4cpp::synchronized<int, testMutex> syn(5);


	//auto& xxx = syn->holder(); 

	synchronize( (syn), sh::with(syn))
	{
		syncbind(test, syn, sh);

		std::cout << "Hallo" << synccast(syn) + 4 << endl;
	}

	std::cin.get();

	/*{
		holder<> h;

		wrapper<void> s(h.synced());
		s= h.synced();

		dos(h.synced());

		cout << h->size() << endl;
	}*/

	/*testSync xxx;
	testSync xxx2;
	boost::mutex m;
	{
		synchronize(xxx, xxx2, &m)
		{
			std::cout << "Hallo\n";
		}
	}*/

	//std::cin.get();

	//sync4cpp::wrap_with_default_if_not_wrapped<identity<decltype(sync4cpp::shared::with(xxx))>::self>::wrapped_guard::guard_type guard(xxx);
	//sync(sync4cpp::shared::with(xxx));
//}



/*
sync4cpp::synchronized<int, boost::shared_mutex> testString(0);
boost::mutex m1, m2;

void func(int id)
{
	while(true)
	{
		syncuse(testString, sync4cpp::shared);
		boost::this_thread::sleep(boost::posix_time::millisec(100));
		std::cout << id << ":" << testString << std::endl;
	}
}


int main()
{

	boost::thread th = boost::thread(&func, 1);
	boost::thread th1 = boost::thread(&func, 2);
	boost::thread th2 = boost::thread(&func, 3);
	boost::thread th3 = boost::thread(&func, 4);
	boost::thread th4 = boost::thread(&func, 5);

	while(true)
	{
		syncuse(testString);
		testString++;
		std::cout << "-------- " << testString << " ---------";
	}

	std::cin.get();
	return 0;
}*/