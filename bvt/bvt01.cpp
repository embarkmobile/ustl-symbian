// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//

#include "stdtest.h"

static void WriteCML (const cmemlink& l)
{
    cout << "memlink{" << l.size() << "}: ";
    const char* pc = reinterpret_cast<const char*>(l.cdata());
    size_t nc = l.size();
    if (pc[nc - 1] == 0)
	-- nc;
    cout.write (l.cdata(), nc);
    cout << endl;
}

void TestML (void)
{
    char str[] = "abcdefghijklmnopqrstuvwzyz";
    memlink::const_pointer cstr = str;

    memlink a, b;
    a.static_link (str);
    if (a.begin() != str)
	cout << "begin() failed on memlink" << endl;
    a.link (VectorRange(str));
    if (a.begin() + 5 != &str[5])
	cout << "begin() + 5 failed on memlink" << endl;
    if (0 != memcmp (a.begin(), str, VectorSize(str)))
	cout << "memcmp failed on memlink" << endl;
    WriteCML (a);
    b.link (cstr, VectorSize(str));
    if (b.data() != cstr)
	cout << "begin() of const failed on cmemlink" << endl;
    if (b.cmemlink::begin() != cstr)
	cout << "begin() failed on cmemlink" << endl;
    WriteCML (b);
    if (!(a == b))
	cout << "operator== failed on cmemlink" << endl;
    b.resize (VectorSize(str) - 2);
    a = b;
    if (a.data() != b.data())
	cout << "begin() after assignment failed on cmemlink" << endl;
    a.relink (str, VectorSize(str) - 1);
    WriteCML (a);
    a.insert (a.begin() + 5, 9);
    a.fill (a.begin() + 5, "-", 1, 9);
    WriteCML (a);
    a.erase (a.begin() + 9, 7);
    a.fill (a.end() - 7, "=", 1, 7);
    WriteCML (a);
    a.fill (a.begin() + 5, "TEST", 4, 3); 
    WriteCML (a);
    a.copy (cstr, VectorSize(str) - 1);
    WriteCML (a);
}

StdBvtMain (TestML)

