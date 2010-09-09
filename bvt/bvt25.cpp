// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//

#include "stdtest.h"

void TestFStream (void)
{
    fstream fs ("c:\\bvt25.in"); 
    if (!fs)
    	cout << "Failed to open bvt25.in" << endl;
    string buf;
    TInt size = fs.size();
    buf.resize (size);
    if (buf.size() == 0)
    	cout << "fstream.size() returned " << buf.size() << endl;
    fs.read (buf.begin(), buf.size());
    cout << buf;
    fs.close();
}

StdBvtMain (TestFStream)

