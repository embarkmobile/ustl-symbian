// This file is part of the ustl library, an STL implementation.
//
// Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
//
// This file is free software, distributed under the MIT License.
//
// printfsupport.cpp
//
// Support for Symbian's missing vnprintf function.
//


#include "printfsupport.h"
#include <reent.h>
#include <string.h>
#include <assert.h>

static   int dummyWrite(void * , const char *, int _n)
	{
		return _n;
	}

/** Answers the number of characters that would be generated 
 *  by a printf with format string fmt and arguments ap 
 */
size_t printf_length(char const *fmt, va_list ap)
{
	FILE f;
	unsigned char c[2];
	f._flags = __SWR ;  // Allowed to write.
	f._bf._base = f._p = c; // Set up file buffer.
	f._bf._size = f._w = 1;  // ... of size 1
	f._write = dummyWrite;   // But all real writes are dummy.
	f._data = _REENT;		 // Symbian reentrancy stuff.
	
	size_t ret = vfprintf (&f, fmt, ap);

	return ret;
}

/**
 * Called by the OS vfprintf.  Note: _n can be negative.
 */ 
static int throwingWrite(void * cookie, const char * buf, int _n)
{
	assert( _n > 0 );
	char* ourBuf = (char*)cookie;
	if (buf != ourBuf) // Ouch! - they're not using our buffer.
		memcpy( ourBuf, buf, _n );
	throw _n;
}

/**
 * Symbian implementation of ANSI standard vsnprintf...
 * Does a printf of max size <size> (including terminating zero), taking varargs parameter ap.
 */
size_t vsnprintf(char * str, size_t size, 
		const char * format, va_list ap)
{
	size_t ret = 0;

	FILE f;
	f._flags = __SWR ;  // Allowed to write.
	f._bf._base = f._p = (unsigned char*)str; // Set up file buffer.
	f._bf._size = f._w = size-1;  // ... of size <size-1>
	f._write = throwingWrite;   // But any overflow causes a throw.
	f._data = _REENT;		 // Symbian reentrancy stuff.
	f._cookie = str;	// So we can copy.
	if (size <= 1) // small buffer worries vfprintf
	{
		ret = printf_length( format, ap ); // Standard says it returns the number of chars anyway.
		if (size == 1)
			str[0] = '\0';
	}
	else try 
	{
		ret = vfprintf (&f, format, ap);
		assert( ret < size );
		str[ret] = '\0';
	}
	catch (int n)
	{
		assert( n < size );
		str[n] = '\0';
		ret = printf_length( format, ap ); // Standard says it returns the number of chars anyway.
	}
	return ret;
}
