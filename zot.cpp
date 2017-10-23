// zot.cpp - zero, one, two, ...
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.

#include "array.h"

using namespace xll;

static AddInX xai_array_zot(
	FunctionX(XLL_FPX, _T("?xll_array_zot"), _T("ARRAY.ZOT"))
	.Arg(XLL_WORDX, _T("Rows"), _T("is the number of rows "))
	.Arg(XLL_WORDX, _T("_Columns"), _T("is the optional number of columns. The default value is 1 "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Array with specified Rows and Columns containing 0, 1, 2,... in row major order."))
	.Documentation(_T("Zero, one, two..."))
);
#pragma warning(push)
#pragma warning(disable: 4702)
xfp* WINAPI
xll_array_zot(xword r, xword c)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		if (c == 0)
			c = 1;

		x.reshape(r, c);

		for (xword i = 0; i < r*c; ++i) {
			x[i] = i;
		}
	}
	catch (const char* ex) {
		XLL_ERROR(ex);
	}

	return x.get();
}
#pragma warning(pop)

#ifdef _DEBUG

static int
test_zot(void)
{
	try {

		// general XFP<X> tests
		FPX x(2, 3);
		ensure (x.rows() == 2);
		ensure (x.columns() == 3);
		ensure (x.size() == 6);

		x.reshape(3, 4);
		ensure (x.rows() == 3);
		ensure (x.columns() == 4);
		ensure (x.size() == 12);

		FPX y(x);
		ensure (y.rows() == 3);
		ensure (y.columns() == 4);
		ensure (y.size() == 12);

		y.reshape(2,3);
		x = y;
		ensure (x.rows() == 2);
		ensure (x.columns() == 3);
		ensure (x.size() == 6);

		xfp* px = xll_array_zot(2, 3);

		ensure (px->rows == 2);
		ensure (px->columns == 3);
		ensure (size(*px) == 6);
		ensure (px->array[0] == 0);
		ensure (px->array[5] == 5);

	}
	catch (const char* ex) {
		XLL_ERROR(ex);
	}

	return 1;
}

static Auto<OpenAfterX> xao_test_zot(test_zot);
#endif // _DEBUG