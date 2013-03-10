#pragma once
#ifndef _SYNC4CPP_HPP
#define _SYNC4CPP_HPP


#include "sync4cpp-plain.hpp"




#define synchere(...)						SYNC4CPP_SYNCHERE(__VA_ARGS__)
#define synchronize(...)					SYNC4CPP_SYNCHRONIZE(__VA_ARGS__)

#define syncuse(_name, ...)					SYNC4CPP_SYNCUSE(_name, __VA_ARGS__)
#define syncbind(_name, _src, ...)			SYNC4CPP_SYNCBIND(_name, _src, __VA_ARGS__)
#define synccast(_src, ...)					SYNC4CPP_SYNCCAST(_src, __VA_ARGS__)



#endif