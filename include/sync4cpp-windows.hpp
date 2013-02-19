#pragma once
#ifndef SYNC4CPP_WINDOWS_HPP
#define SYNC4CPP_WINDOWS_HPP

#include <windows.h>

#include "sync4cpp-plain.hpp"


namespace sync4cpp { namespace windows {

class critical_section_guard
{
public:
	inline critical_section_guard(CRITICAL_SECTION& section)
		: mSection(&section)
	{
		EnterCriticalSection(mSection);
	}

	inline ~critical_section_guard()
	{
		LeaveCriticalSection(mSection);
	}

private:
	CRITICAL_SECTION* mSection;
};


}}


//SYNC4CPP_SET_DEFAULT_MUTEX(boost::mutex);

// Register guards for boost::mutex
SYNC4CPP_REGISTER_GUARD(CRITICAL_SECTION, sync4cpp::exclusive, sync4cpp::windows::critical_section_guard);

// Register guards for boost::shared_mutex
// SYNC4CPP_REGISTER_GUARD(boost::shared_mutex, sync4cpp::exclusive, boost::lock_guard<boost::shared_mutex>)
// SYNC4CPP_REGISTER_GUARD(boost::shared_mutex, sync4cpp::shared, boost::shared_lock<boost::shared_mutex>)


#endif