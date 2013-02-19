#pragma once
#ifndef _SYNC4CPP_HPP
#define _SYNC4CPP_HPP


#include "sync4cpp-plain.hpp"




#define synchere(...)						HELP_SYNCHERE(__VA_ARGS__)
#define synchronize(...)					HELP_SYNCHRONIZE(__VA_ARGS__)

#define syncuse(_name, ...)					HELP_SYNCUSE(_name, __VA_ARGS__)
#define syncbind(_name, _src, ...)			HELP_SYNCBIND(_name, _src, __VA_ARGS__)
#define synccast(_src, ...)					HELP_SYNCCAST(_src, __VA_ARGS__)



#endif