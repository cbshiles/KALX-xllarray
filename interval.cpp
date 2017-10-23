// interval.cpp - interval containing given number of points
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#include "array.h"

using namespace xll;

static AddInX xai_array_interval(
	FunctionX(XLL_FPX, _T("?xll_array_interval"), _T("ARRAY.INTERVAL"))
	.Arg(XLL_DOUBLEX, _T("Start"), _T("is the first number in the array "))
	.Arg(XLL_DOUBLEX, _T("Stop"), _T("is the last number in the array "))
	.Arg(XLL_WORDX,  _T("Rows"), _T("is the number of rows in the interval "))
	.Arg(XLL_WORDX,  _T("_Columns"), _T("is the number of columns in the interval, default value is 1 "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns an array having Count rows and one column from Start to Stop in equal increments."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.INTERVAL(0, 1, 3)</codeInline> ")
		_T("is the array <codeInline>{0;0.5;1}</codeInline>. ")
		,
		xml::xlink(_T("ARRAY.SEQUENCE"))
	)
);
xfp* WINAPI
xll_array_interval(double a, double b, xword r, xword c)
{
#pragma XLLEXPORT
	static FPX x; // not threadsafe

	if (r == 0)
		r = 1;
	if (c == 0)
		c = 1;

	x.reshape(r, c);
	double da = (x.size() == 1) ? 0 : (b - a)/(x.size() - 1);

	for (xword i = 0; i < x.size(); ++i) {
		x[i] = a + i*da;
	}

	return x.get();
}

#ifdef _DEBUG

static int 
test_interval(void)
{
	try {
		xfp* px = xll_array_interval(1, 2, 6, 1);

		ensure (px->rows == 6);
		ensure (px->columns == 1);
		ensure (size(*px) == 6);
		ensure (px->array[0] == 1);
		ensure (px->array[1] == 1. + 1./5);
		ensure (px->array[5] == 2);

		FPX x = *xll_array_interval(1, 5, 1, 5);
		ensure (x.size() == 5);
		ensure (x[0] == 1);
		ensure (x[4] == 5);
		ensure (x(0, 4) == 5);

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return 1;
}

static Auto<OpenAfterX> xao_test_interval(test_interval);

#endif // _DEBUG
