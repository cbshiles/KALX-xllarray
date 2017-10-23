// sequence.cpp - sequence over an interval with given step size
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)
#include "array.h"

using namespace xll;

static AddInX xai_array_sequence(
	FunctionX(XLL_FPX, _T("?xll_array_sequence"), _T("ARRAY.SEQUENCE"))
	.Arg(XLL_DOUBLEX, _T("Start"), _T("is the first number in the sequence."))
	.Arg(XLL_DOUBLEX, _T("Stop"), _T("is the last number in the sequence. "))
	.Arg(XLL_DOUBLEX, _T("_Step"), _T("is the amount to increment. The default value is 1. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Array having one column from Start to Stop in Step increments."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.SEQUENCE(1, 3)</codeInline> ")
		_T("is the array <codeInline>{1;2;3}</codeInline>.")
		,
		xml::xlink(_T("ARRAY.INTERVAL"))
	)
);
xfp* WINAPI
xll_array_sequence(double a, double b, double da)
{
#pragma XLLEXPORT
	static FPX x;
	
	try {

		if (da > 0) {
			ensure (da > 0 && a < b);
		}
		else if (da < 0) {
			ensure (da < 0 && a > b);
		}

		if (da == 0)
			da = 1;
		double n_ = 1 + (b - a)/da;
		if (n_ >= limits<XLOPERX>::maxrows)
			return 0;

		xword n = static_cast<xword>(n_);

		if (n == 0)
			return 0;

		x.reshape(n, 1);

		for (xword i = 0; i < n; ++i) {
			x[i] = a + i*da;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return x.get();
}

#ifdef _DEBUG

int
test_sequence(void)
{
	try {
		OPERX o;

		o = ExcelX(xlfEvaluate, OPERX(_T("ARRAY.SEQUENCE(0, 1, .5)")));
		ensure (o.xltype == xltypeMulti);
		ensure (o.rows() == 3);
		ensure (o.columns() == 1);
		ensure (o[0] == 0);
		ensure (o[1] == .5);
		ensure (o[2] == 1);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_sequence(test_sequence);

#endif // _DEBUG