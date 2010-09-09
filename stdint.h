// File: stdint.h
// Dummy implementation of the standard for Symbian OS
//
// Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
// This file is free software, distributed under the MIT License.

#ifndef __STDINT_H
#define __STDINT_H
#include <STDDEF.H>

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef int int32_t;
typedef short int16_t;
typedef char int8_t;

typedef size_t ssize_t;

typedef unsigned int uoff_t;
typedef int value_type;		
typedef size_t size_type;

typedef unsigned int uintptr_t; // Pointer as an unsigned integer???
typedef int intptr_t; // Pointer as an signed integer???

typedef long long INT64_C;
typedef unsigned long long UINT64_C;

// Kludge some numbers to keep ulimits.h happy in ARMI
#ifndef FLT_MAX
# define FLT_MIN 1.175494351E-38F
# define FLT_MAX 3.402823466E+38F
#define DBL_MIN FLT_MIN
#define DBL_MAX FLT_MAX
#define LDBL_MIN FLT_MIN
#define LDBL_MAX FLT_MAX
#endif

#endif
