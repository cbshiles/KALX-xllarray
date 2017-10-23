// drop.cpp - drop elements or rows from array.
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)
//#define EXCEL4
#include "array.h"

using namespace xll;

static AddInX xai_array_drop(
	FunctionX(XLL_FPX, _T("?xll_array_drop"), _T("ARRAY.DROP"))
	.Arg(XLL_LONGX, _T("Count"), _T("is the number elements to drop "))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Drop Count elements from Array."))
	.Documentation(
		_T("If <codeInline>Count</codeInline> is positive the elments are dropped from the beginning of the array. ")
		_T("If <codeInline>Count</codeInline> is negative the elments are dropped from the end of the array. ")
		_T("For example, <codeInline>ARRAY.DROP({1,2,3}, 1)</codeInline> is the array ")
		_T("<codeInline>{2, 3}</codeInline> and <codeInline>ARRAY.DROP({1,2,3}, -2)</codeInline> ")
		_T("is the array <codeInline>{1}</codeInline>. ")
		,
		xml::xlink(_T("ARRAY.TAKE"))
	)
);
xfp* WINAPI
xll_array_drop(LONG n, xfp* pa)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		xword absn = static_cast<xword>(abs(n));
		if (absn >= size(*pa)) {
			x.reshape(0, 1);

			return x.get();
		}
		FPX *px = array::handle(pa);
		if (px) {
			array::drop_command(*px, n);
			x = *pa;
		}
		else {
			x = *pa;
			array::drop_command(x, n);
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
test_drop()
{
	FPX x(4, 1);

	for (xword i = 0; i < x.size(); ++i)
		x[i] = i;

	FPX x1(x);
	array::drop_command(x1, 2);
	ensure (x1.size() == 2);
	ensure (x1[0] == 2);
	ensure (x1[1] == 3);

	FPX x2(x);
	array::drop_command(x2, -2);
	ensure (x2.size() == 2);
	ensure (x2[0] == 0);
	ensure (x2[1] == 1);

	return 1;
}

static Auto<OpenAfterX> xao_test_drop(test_drop);

#endif // _DEBUG
