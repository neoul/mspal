#ifndef _COM_ERROR_H_
#define _COM_ERROR_H_
#include <Poco/Exception.h>

typedef Poco::Exception _com_error;
typedef Poco::OutOfMemoryException _out_of_memory_error;
typedef Poco::IllegalStateException _illegal_state_error;
#endif
