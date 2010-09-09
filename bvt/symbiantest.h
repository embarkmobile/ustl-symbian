// This file is part of the ustl library, an STL implementation.
//
//

#ifndef SYMBIANTEST_H_4980DC406FEEBBDF2235E42C4EFF1A4E
#define SYMBIANTEST_H_4980DC406FEEBBDF2235E42C4EFF1A4E

#include <ustl.h>
using namespace ustl;

extern ostringstream& TestCout;
#define cout TestCout

#define StdBvtMain( x )
class CConsoleBase;
extern CConsoleBase* console; 

void TestCML ();
void TestML ();
void TestMB ();
void TestStreams ();
void TestVector ();
void TestAlgorithms ();
void TestObjectVector ();
void TestString ();
void TestStringVector ();
void TestStringStreams ();
void TestFunctors ();
void TestSetAndMultiset ();
void ObjectSerialization ();
void TestCoutCinCerr ();
void TestMap ();
void TestMultiMap ();
void TestUTF8 ();
void TestBitset ();
void TestIntegralTuples ();
void TestEnumArithmetic ();
void TestStackAndQueue ();
void TestUtility ();
void TestMatrixAlgorithms ();
void TestSimdAlgorithms ();
void TestHeapOperations ();
void TestFStream();
void TestSymbianOSExtensions();
void TestPrintf();
	
#endif

