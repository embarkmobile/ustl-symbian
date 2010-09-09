// This file is part of the ustl library, an STL implementation.
//
// Copyright (c) Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
//
// This file is free software, distributed under the MIT License.
//
// Tests for Symbian OS Extensions
//

#include <e32std.h>
#include <e32cons.h>
#include "stdtest.h"
#include "printfsupport.h"

#undef ASSERT
#define ASSERT( x ) if (!(x)) { console->Printf( _L("Failed at bvtPrintf.cpp (%d)\n"), __LINE__ ); \
	__DEBUGGER();	\
	}

static int tprintf_length (const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  size_t ret = printf_length(fmt, ap);
  va_end (ap);
  return (ret);
}

static int tvsnprintf (char * str, size_t size, const char *fmt, ...)
	{
	  va_list ap;
	  va_start (ap, fmt);
	  size_t ret = vsnprintf(str, size, fmt, ap);
	  va_end (ap);
	  return (ret);
	}

void TestPrintf()
{
	ASSERT( tprintf_length( "" ) == 0 );
	ASSERT( tprintf_length( "%s", "12345") == 5 );
	ASSERT( tprintf_length( "%d", 0 ) == 1 );
	ASSERT( tprintf_length( "%1000.1000s", "" ) == 1000 );
	
	char buf[10];
	memset( buf, 'x', sizeof( buf ) );
	ASSERT( tvsnprintf( buf, 3, "" ) == 0 );
	ASSERT( buf[0] == 0 && buf[1] == 'x' );
	ASSERT( tvsnprintf( buf, 3, "%s", "12345" ) == 5 );
	ASSERT( strcmp( buf, "12" ) == 0 && buf[3] == 'x' );
	ASSERT( tvsnprintf( buf, 8, "%1000.1000s", "" ) == 1000 );
	ASSERT( strcmp( buf, "       " ) == 0 && buf[8] == 'x' );
	
	ASSERT( tvsnprintf( buf, 10, "123456789" ) == 9 );
	ASSERT( strcmp( buf, "123456789" ) == 0 );
	char buf2[100]; // Check with a buffer that must be used...
	ASSERT( tvsnprintf( buf2, 100, "%1000.1000s", "" ) == 1000 );
	ASSERT( strlen( buf2 ) == 99 );
}