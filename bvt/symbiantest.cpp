// \file SymbianTest.cpp
// \brief Symbian test harness.
//
// This file is part of the ustl library, an STL implementation.
//
// Copyright (c) Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
// This file is free software, distributed under the MIT License.
//

/// Define this to do full-scale memory checking rather than outputting to files.
/// It simulates all possible memory allocation failures, and checks for memory leaks.

/// Note (17/11/07): Normal runs see a memory leak on exit; this is a feature of the 
/// Symbian OS libraries, and isn't to worry about.
///
/// Use MEMORY_CHECKING to check that individual tests don't leak memory.

//#define MEMORY_CHECKING
#undef MEMORY_CHECKING

#define _GNU_SOURCE 1
//#include "stdtest.h"
#include "symbiantest.h"
#include <stdlib.h>
#include <stdio.h>
#include <ofstream.h>
#include <e32cons.h>

string* TestFileName;

#ifndef MEMORY_CHECKING

ofstream FTestCout;
ostringstream& TestCout( FTestCout );

static void InitOutputFile( const char* testFileName )
	{
		*TestFileName = testFileName;
		FTestCout.open( testFileName );
	}

	static void WriteOutput()
	{
		FTestCout.close();
	}

_LIT( KSayTest, "." );
#define SAY_TEST( num, function ) console->Printf( _L("%S\n"), &_L(#num) )
//#define SAY_TEST( num, function ) console->Printf( KSayTest)

#if defined __WINS__
#define FILEPART1 "c:\\bvt"
#elif defined  UIQ3
#define FILEPART1 "d:\\bvt"
#elif defined  SERIES60_3RD
#define FILEPART1 "e:\\bvt"
#endif
#define FILEPART3 ".std"
#define TEST( aTestNum, aFunction ) try { SAY_TEST( aTestNum, aFunction ); \
	InitOutputFile( FILEPART1 #aTestNum FILEPART3 ); \
	aFunction(); WriteOutput(); } \
	catch (ustl::exception& e) { console->Printf( _L("Exception thrown: %S\n"), &(string(e.what()).DesL()) ); }

#else // MEMORY_CHECKING
ostringstream STestCout;
ostringstream& TestCout( STestCout );
string OutputString;

void Reset()
{
	OutputString.clear();
	STestCout.str(OutputString); 
}

#define P(x) console->Printf( _L(x) )
#define P2(x, y) console->Printf( _L(x), y )
#define TEST( aTestNum, aFunction ) { \
	P(#aFunction); \
	{ \
		__UHEAP_MARK; \
		TRAPD( err, aFunction() ); \
		Reset(); \
		__UHEAP_MARKEND; \
	} \
	P( "\n" ); }
#endif

int main( int , char** )
{
	string testFile;
	TestFileName = &testFile;

#ifdef MEMORY_CHECKING
	// Various things to ensure that all Symbian memory is pre-allocated:
	TRAPD( dummy, User::LeaveNoMemory() ); // User::LeaveNoMemory allocates memory!!
	Reset();
	TestCML(); // Initial dummy run - stdlib sprintf allocates memory
#endif
	
	TEST( Printf, TestPrintf );
	TEST( 00, TestCML ); 
	TEST( 01, TestML );
	TEST( 02, TestMB );
	TEST( 03, TestStreams );
	TEST( 04, TestVector );
	TEST( 05, TestAlgorithms );
	TEST( 06, TestObjectVector );
	TEST( 07, TestString );
	TEST( 08, TestStringVector );
	TEST( 09, TestStringStreams );
	TEST( 10, TestFunctors );
	TEST( 11, TestSetAndMultiset );
	TEST( 12, ObjectSerialization );
	//TEST( 13, TestCoutCinCerr );
	TEST( 14, TestMap );
	TEST( 15, TestMultiMap );
	TEST( 16, TestUTF8 );
	TEST( 17, TestBitset );
	TEST( 18, TestIntegralTuples );
	TEST( 19, TestEnumArithmetic );
	TEST( 20, TestStackAndQueue );
	TEST( 21, TestUtility );
	TEST( 22, TestMatrixAlgorithms );
	TEST( 23, TestSimdAlgorithms );
	TEST( 24, TestHeapOperations );
	TEST( 25, TestFStream );
	TEST( Symbian, TestSymbianOSExtensions );
    return (EXIT_SUCCESS);
}

