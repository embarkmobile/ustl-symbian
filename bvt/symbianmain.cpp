// SymbianMain.cpp
//
// Copyright (C) Symbian Software Ltd 2000-2005.  All rights reserved.
// Modifications Copyright (C) Penrillian 2006 <http://www.penrillian.com>
// Subject to MIT licence.
//
// Replacement for ecrt0.lib, which is missing for UIQ Beta 3 SDK.

#include <stdlib.h>
#include <stdio.h>
#include <e32base.h>
#include <e32cons.h>
#include <libc\sys\reent.h> // CloseSTDLIB

_LIT(KTxtEPOC32EX,"EPOC32EX");
_LIT(KTxtExampleCode,"USTL Test");
_LIT(KFormatFailed,"failed: leave code=%d");
_LIT(KTxtOK,"ok");
_LIT(KTxtPressAnyKey," [press any key]");

LOCAL_C void callExampleL();
extern int main( int argc, char** argv); 

// public
CConsoleBase* console; // write all your messages to this

GLDEF_C TInt E32Main() // main function called by E32
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	TRAPD(error,callExampleL()); // more initialization, then do example
	__ASSERT_ALWAYS(!error,User::Panic(KTxtEPOC32EX,error));
	CloseSTDLIB();
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND;
	return 0; // and return
    }

LOCAL_C void callExampleL() // initialize and call example code under cleanup stack
    {
	console=Console::NewL(KTxtExampleCode,TSize(KConsFullScreen,KConsFullScreen));
	CleanupStack::PushL(console);
	char* initStrings = "Application";
	TRAPD(error,(void)main( 1, &initStrings )); // perform example function
	if (error)
		console->Printf(KFormatFailed, error);
	else
		console->Printf(KTxtOK);
	console->Printf(KTxtPressAnyKey);
	console->Getch(); // get and ignore character
	CleanupStack::PopAndDestroy(); // close console
    }

