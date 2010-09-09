// This file is part of the ustl library, an STL implementation.
//
// Copyright (c) Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
//
// This file is free software, distributed under the MIT License.
//
// Tests for Symbian OS Extensions
//

#include "stdtest.h"
#include <E32std.h>
#include <bautils.h>
#include <stdio.h>
#include <e32cons.h>

#undef ASSERT
#define ASSERT( x ) if (!(x)) { console->Printf( _L("Failed at bvtSymbian.cpp (%d)\n"), __LINE__ ); \
	__DEBUGGER();	\
	}

void TestSymbianOSExtensions()
{
#define XString "Hello world!"
	// Ctor from descriptor
	_LIT( KString, "Hello world!" );
	string str( KString );
	ASSERT( str == XString );
	str = KNullDesC;
	ASSERT( str == "" );
	ASSERT( str.length() == 0 )
	str = XString;
	// Convert to descriptor
	TBuf<5> small;
	SymbianString temp( str.DesL() );
	BaflUtils::CopyWithTruncation(small, temp);
	str = small;
	// Check some UNICODE
	ASSERT( str.utf_length() == 5 ); // Length is in unicode chars.
	ASSERT( str.size() == 7 ); // Includes terminating zero.
	TBuf<16> result( str.DesL() );
	ASSERT( result[4] == KBaflCharTruncation ); // UNICODE 0x2026

	// Check streams and 8-bit descriptors:
	str.clear();
	ostringstream s;
	s << 1 << "+";
	ASSERT( s.str() == "1+"); 
	s << 1;
	ASSERT( s.str() == "1+1");
	s << string("=") << '2';
	ASSERT( s.str() == "1+1=2");
	
	str = s.str();
//	ASSERT( str == "1+1=2");
	TBuf8<16> buf( str.Des8L() );
	ASSERT( buf.Length() == 5 );
	ASSERT( buf == _L8("1+1=2") );
	
	/// And some tests for things to do with changes to string semantics:
	string ss = "hello";
	ASSERT( ss.length() == 5 );

}


