#include <iostream>
#include <string>
//#include <boost/thread.hpp>
//#include "sync4cpp-boost.hpp"
#include "sync4cpp.hpp"
#include <map>


using namespace std;

typedef std::true_type dd;



struct xxx
{
	xxx(int i)
		: i(i)
	{}



	int i;
};


int main()
{
	auto x = xxx(5);

	cout << x.i;

	std::cin.get();
	return 0;
}