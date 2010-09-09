// This file is part of the ustl library, an STL implementation.
//
// Copyright (c) Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
//
// This file is free software, distributed under the MIT License.
//
// Printf support declarations.
//

#ifndef PRINTFSUPPORT_H_
#define PRINTFSUPPORT_H_
#include <stdio.h>
#include <stdarg.h>

size_t printf_length(char const *fmt, va_list ap);
size_t vsnprintf(char * str, size_t size, 
		const char * format, va_list ap);

#endif /*PRINTFSUPPORT_H_*/
