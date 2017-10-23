// array.h - struct FP related functions
// Copyright (c) 2006-2011 KALX, LLC. All rights reserved. No warranty is made.
//#define EXCEL12
#include "xll/xll.h"
#include "command.h"

typedef xll::traits<XLOPERX>::xcstr xcstr;
typedef xll::traits<XLOPERX>::xfp xfp;
typedef xll::traits<XLOPERX>::xword xword;

#ifndef CATEGORY
#define CATEGORY _T("Array")
#endif

#ifndef ARRAY_PREFIX
#define ARRAY_PREFIX _T("ARRAY.")
#endif

#ifndef XML_DIR
#define XML_DIR _T("Code/xllarray/")
#endif

#define ARRAY_OR_HANDLE _T("is an array of numbers or a handle to an array. ")
#define IS_ARRAY _T("is an array of numbers. ")
#define IS_FUNCTION _T("is the register id of a user defined function. ")
#define IS_ROWS _T("is the number of rows in the array. ")
#define IS_COLUMNS _T("is the number of columns in the array. ")

namespace array {

	// Convert 1-based cyclic index into 0-based actual index
	inline xword 
	index(long i, xword n)
	{
		ensure (i != 0);
		ensure (n != 0);

		i = i > 0 ? (i - 1)%n : n - 1 - (-i - 1)%n; // like Excel's MOD

		ensure (i >= 0);
		ensure (static_cast<xword>(i) < n);

		return static_cast<xword>(i);
	}

	// Return pointer if FP has size 1 and is known, otherwise 0.
	inline xll::FPX*
	handle(const xfp* px)
	{
		if (xll::size(*px) > 1)
			return 0;
	
		xll::handle<xll::FPX> h((HANDLEX)px->array[0], true);
	
		return h;
	}

} // namespace array