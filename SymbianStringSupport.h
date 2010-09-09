// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Charles Weir <charles@penrillian.com>
// This file is free software, distributed under the MIT License.
//
// SymbianStringSupport.h
//

#ifndef SYMBIANSTRINGSUPPORT_H
#define SYMBIANSTRINGSUPPORT_H

#include <e32std.h>

/**
	Enhancement of descriptors only possible now Symbian 9 has proper exceptions.
	This class works correctly with stack-based destruction.

	Implemented only for use with string::DesL - no guarantees for any other use.
*/

class SymbianString : public RBuf16
{
public:
	SymbianString();
	SymbianString( const SymbianString& aString );
	~SymbianString();
};

/**
	Enhancement of 8-bit descriptors only possible now Symbian 9 has proper exceptions.
	This class works correctly with stack-based destruction.

  	Implemented only for use with string::Des8L - no guarantees for any other use.

*/
class SymbianString8 : public RBuf8
{
public:
	SymbianString8();
	SymbianString8( const SymbianString8& aString );
	~SymbianString8();
};

#endif

