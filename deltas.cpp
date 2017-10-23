// deltas.cpp - stl function
// Copyright (c) 2006-2010 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)
#include <numeric>
//#define EXCEL4
#include "array.h"

using namespace xll;

// ARRAY.DIFF(Array, Times?) ARRAY.DELTA ARRAY.SIGMA
static AddInX xai_deltas(
	FunctionX(XLL_FPX, _T("?xll_deltas"), _T("ARRAY.DELTAS"))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Calculates the differences of adjacent numbers in Array."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.DELTAS({2,3,5})</codeInline> ")
		_T("is the array <codeInline>{2,1,2}</codeInline>. ")
		_T("This function calls the C++ Standard Library routine <codeInline>std::adjacent_difference</codeInline> ")
		_T("declared in the &lt;algorithm&gt; header.")
		_T("</para><para>")
		_T("<codeInline>ARRAY.SUMS</codeInline> is the inverse of this function.")
		,
		xml::xlink(_T("ARRAY.SUMS"))
	)
);
xfp* WINAPI
xll_deltas(xfp* pa)
{
#pragma XLLEXPORT
	FPX* px = array::handle(pa);

	if (px)
		std::adjacent_difference(px->array(), px->array() + px->size(), &px->array()[0]);
	else
		std::adjacent_difference(pa->array, pa->array + size(*pa), &pa->array[0]);

	return pa;
}

#ifdef _DEBUG

int
test_deltas()
{
	FPX x(2, 3);

	for (xword i = 0; i < x.size(); ++i)
		x[i] = i;

	xll_deltas(x.get());

	ensure (x[0] == 0);
	ensure (x[1] == 1);
	ensure (x[2] == 1);
	ensure (x[3] == 1);
	ensure (x[4] == 1);
	ensure (x[5] == 1);

	ensure (ExcelX(xlfEvaluate, OPERX("=ARRAY.DELTAS({1,2,3,4})={1,1,1,1}")));

//	OPERX o1 = ExcelX(xlfEvaluate, OPERX("=ARRAY.DELTAS({1,2,3,4})"));
/*	ensure (o1.xltype == xltypeMulti);
	ensure (o1.rows() == 1);
	ensure (o1.columns() == 4);
	for (xword i = 0; i < o1.size(); ++i)
		ensure (o1[i] == 1);
*/
	return 1;
}

static Auto<OpenAfterX> xao_deltas(test_deltas);

#endif // _DEBUG