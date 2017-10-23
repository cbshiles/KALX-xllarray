// array.cpp - create arrays
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
//#define EXCEL4
#include "array.h"

using namespace xll;
using namespace array;

static AddInX xai_array_empty(
	FunctionX(XLL_BOOLX, _T("?xll_array_empty"), _T("ARRAY.ISEMPTY"))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns true if Array is empty."))
	.Documentation(
		_T("This tests if the array is 1 by 1 and has a NaN value.")
	)
);
BOOL WINAPI
xll_array_empty(xfp* pa)
{
#pragma XLLEXPORT
	BOOL b(TRUE);

	FPX* px = array::handle(pa);
	if (px)
		b = px->is_empty();
	else
		b = is_empty(*pa);

	return b;
}

#ifdef _DEBUG

int
test_array_empty()
{
	try {
		ensure (ExcelX(xlfEvaluate, OPERX(_T("ARRAY.ISEMPTY(ARRAY.MAKE(0,0))"))));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

static Auto<OpenAfterX> xao_test_array_empty(test_array_empty);


#endif // _DEBUG