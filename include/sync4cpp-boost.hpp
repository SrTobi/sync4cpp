#pragma once
#ifndef SYNC4CPP_BOOST_HPP
#define SYNC4CPP_BOOST_HPP


#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>


#include "sync4cpp-plain.hpp"



SYNC4CPP_SET_DEFAULT_MUTEX(boost::mutex);

// Register guards for boost::mutex
SYNC4CPP_REGISTER_GUARD(boost::mutex, sync4cpp::exclusive, boost::lock_guard<boost::mutex>);

// Register guards for boost::shared_mutex
SYNC4CPP_REGISTER_GUARD(boost::shared_mutex, sync4cpp::exclusive, boost::lock_guard<boost::shared_mutex>)
SYNC4CPP_REGISTER_GUARD(boost::shared_mutex, sync4cpp::shared, boost::shared_lock<boost::shared_mutex>)


#endif