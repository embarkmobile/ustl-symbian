// This file is part of the ustl library, an STL implementation.
//
// Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
//
// This file is free software, distributed under the MIT License.
//
// SymbianStringSupport.cpp
//
// Support for converting between string and Symbian OS descriptors.
//

#include "ustring.h"
#include "ualgo.h"

/**
	Copy ctor - needed to ensure we can return by value.
	Note - does a Leave on failure, which is illegal by Symbian rules,
	but this is only used by string::DesL, so won't cause problems
*/
SymbianString::SymbianString( const SymbianString& aString )
{
	CreateL( aString );
}

SymbianString::SymbianString()
{
}

/// Dtor - not inline, to avoid generating lots of code.

SymbianString::~SymbianString() 
{ Close(); }


SymbianString8::SymbianString8( const SymbianString8& aString )
{
	CreateL( aString );
}

SymbianString8::SymbianString8()
{
}

SymbianString8::~SymbianString8() 
{ Close(); }


namespace ustl {

/// Constructs the string by translating a Symbian OS descriptor to UTF8

string::string(const TDesC& aDes) 
	: memblock ()
{
	resize( 0 ); // Else append can't create the UTF8 iterator.
	append( aDes );
}

/// Appends the contents of a Symbian OS descriptor converted to UTF8

void string::append( const TDesC& aDes )
{
	if (aDes.Length() == 0)
		return;
	const_wpointer ptr = (const_wpointer)&(aDes[0]);
	const_wpointer end_ptr = ptr + aDes.Length();
	append( ptr, end_ptr );
}

/** Answers a temporary (stack-based) descriptor corresponding to 
	my contents interpreted as a UTF8 string.
*/

SymbianString string::DesL()
{
	SymbianString result;
	size_t myLength = utf_length();
	result.CreateL( myLength );
	result.SetLength( myLength );
	if (myLength > 0)
	{
		wpointer ptr = (wpointer)&result[0];
		ustl::copy (utf8_begin(), utf8_end(), ptr);
	}
	return result;
}

/** Answers a temporary (stack-based) 8-bit descriptor corresponding to 
	my contents interpreted as an 8-bit string.
*/
SymbianString8 string::Des8L()
{
	SymbianString8 result;
	result.CreateL( size() );
	result.SetLength( size() );
	if (size() > 0)
	{
		pointer ptr = (pointer)(&result[0]);
		ustl::copy (begin(), end(), ptr);
	}
	return result;
}
} // namespace ustl

