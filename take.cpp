// take.cpp - take elements or rows from array.
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)

#include "array.h"

using namespace xll;

static AddInX xai_array_take(
	FunctionX(XLL_FPX, _T("?xll_array_take"), _T("ARRAY.TAKE"))
	.Arg(XLL_LONGX, _T("Count"), _T("is the number elements to take "))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Take Count elements from Array."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.TAKE({1,2,3}, 1)</codeInline> is the array <codeInline>{1}</codeInline> ")
		_T("and <codeInline>ARRAY.TAKE({1,2,3}, -2)</codeInline> is the array <codeInline>{2, 3}</codeInline>. ")
		,
		xml::xlink(_T("ARRAY.DROP"))
	)
);
xfp* WINAPI
xll_array_take(LONG n, xfp* pa)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		FPX *px = array::handle(pa);
		if (px) {
			array::take_command(*px, n);
			x = *pa;
		}
		else {
			x = *pa;
			array::take_command(x, n);
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
test_take()
{
	FPX x(4, 1);

	for (xword i = 0; i < x.size(); ++i)
		x[i] = i;

	FPX x1(x);
	array::take_command(x1, 2);
	ensure (x1.size() == 2);
	ensure (x1[0] == 0);
	ensure (x1[1] == 1);

	FPX x2(x);
	array::take_command(x2, -2);
	ensure (x2.size() == 2);
	ensure (x2[0] == 2);
	ensure (x2[1] == 3);

	return 1;
}

static Auto<OpenAfterX> xao_take(test_take);

#endif // _DEBUG
