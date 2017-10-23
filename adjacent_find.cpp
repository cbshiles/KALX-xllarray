// adjacent_find.cpp - stl function
// Copyright (c) 2006-2012 KALX, LLC. All rights reserved. No warranty is made.
#include <numeric>
#include "array.h"

using namespace xll;

// ARRAY.SAME???
static AddInX xai_adjacent_find(
	FunctionX(XLL_LONGX, _T("?xll_adjacent_find"), _T("ARRAY.SAME"))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the index of first of two adjacent equal elements or 0 if not found."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.SAME({1;2;3;4})</codeInline> is 0 and ")
		_T("<codeInline>ARRAY.SAME({1;2;2;3})</codeInline> is 2. ")
		_T("This function calls the C++ Standard Library routine <codeInline>std::adjacent_find</codeInline> ")
		_T("declared in the &lt;algorithm&gt; header. ")
	)
);
LONG WINAPI
xll_adjacent_find(const xfp* pa) //!!! bool all - find all indices
{
#pragma XLLEXPORT
	const double *pd, *pb, *pe;
	FPX* px = array::handle(pa);

	if (px) {
		pb = px->array();
		pe = px->array() + px->size();
	}
	else {
		pb = pa->array;
		pe = pa->array + size(*pa);
	}

	pd = std::adjacent_find(pb, pe);

	return static_cast<LONG>(pd == pe ? 0 : pd - pb + 1);
}

#ifdef _DEBUG

int
test_a_f()
{
	OPERX o1 = ExcelX(xlfEvaluate, OPERX(_T("=ARRAY.SAME({1;2;3;4})")));
	ensure (o1.xltype == xltypeNum);
	ensure (o1.val.num == 0);

	OPERX o2 = ExcelX(xlfEvaluate, OPERX(_T("=ARRAY.SAME({1;2;2;3})")));
	ensure (o2.xltype == xltypeNum);
	ensure (o2.val.num == 2);

	OPERX o3 = ExcelX(xlfEvaluate, OPERX(_T("=ARRAY.SET({1;2;2;4})")));
	ensure (o3.xltype == xltypeNum);

	OPERX cmd = ExcelX(xlfConcatenate
		, OPERX(_T("=ARRAY.SAME(ARRAY.GET("))
		, o3
		, OPERX(_T("))")));
	OPERX o4 = ExcelX(xlfEvaluate, cmd);
	ensure (o4.xltype == xltypeNum);
	ensure (o4.val.num == 2);

	return 1;
}

static Auto<OpenAfterX> xao_a_f(test_a_f);

#endif // _DEBUG