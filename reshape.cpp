// reshape.cpp - reshap arrays
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#include "array.h"

using namespace xll;

static AddInX xai_array_reshape(
	FunctionX(XLL_FPX, _T("?xll_array_reshape"), _T("ARRAY.RESHAPE"))
	.Arg(XLL_LONGX, _T("Rows"), _T("is the number of rows in the resulting array "))
	.Arg(XLL_LONGX, _T("Columns"), _T("is the number of columns in the resulting array "))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Change the shape of Array to Rows and Columns."))
	.Documentation(_T("If Rows or Columns is 0 then an empty array is returned. "))
);
xfp* WINAPI
xll_array_reshape(xword r, xword c, xfp* pa)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		ensure (r >= 0);
		ensure (c >= 0);
		ensure (r < limits<XLOPERX>::maxrows);
		ensure (c < limits<XLOPERX>::maxcols);

		if (FPX* px = array::handle(pa)) {
			px->reshape(r, c);

			return pa;
		}

		// same size
		if (size(*pa) == r*c) {
			pa->rows = r;
			pa->columns = c;

			return pa;
		}

		x = *pa;
		x.reshape(r, c);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		
		return 0;
	}

	return x.get();
}

#ifdef _DEBUG

int
test_reshape()
{
	try {
		FPX a0(2, 3);
		ensure (a0.rows() == 2);
		ensure (a0.columns() == 3);

		a0.reshape(3, 2);
		ensure (a0.rows() == 3);
		ensure (a0.columns() == 2);

		a0.reshape(1, 2);
		ensure (a0.rows() == 1);
		ensure (a0.columns() == 2);

		a0.reshape(3, 4);
		ensure (a0.rows() == 3);
		ensure (a0.columns() == 4);

		xfp* pa = xll_array_reshape(5, 6, a0.get());
		ensure (pa->rows == 5);
		ensure (pa->columns == 6);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return 1;
}

static Auto<OpenAfterX> xao_test_reshape(test_reshape);

#endif // _DEBUG