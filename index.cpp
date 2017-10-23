// index.cpp - index into arrays
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.

#include "array.h"

using namespace xll;

static AddInX xai_array(
	FunctionX(XLL_FPX, _T("?xll_array_index"), _T("ARRAY.INDEX"))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Arg(XLL_FPX, _T("Index"), _T("is an array of row indices."))
	.Arg(XLL_LPOPERX, _T("_Columns"), _T("is an optional array of column indicies. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return Array values at Index using 1-based cyclic index."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.INDEX({1,2,3},4) = 1</codeInline> and ")
		_T("<codeInline>ARRAY.INDEX({1,2,3},-1) = 3</codeInline>. ")
		_T("Note that an index of 0 is considered an error. ")
	)
);
xfp* WINAPI
xll_array_index(const xfp* pa, const xfp* pr, const LPOPERX pc)
{
#pragma XLLEXPORT
	static FPX x;

	xword nr = size(*pr);
	xword nc = pc->size();
	if (!nc)
		nc = 1; // pc->xltype == xltypeMissing

	if (FPX* px = array::handle(pa))
		pa = px->get();

	if (pa->columns == 1)
		x.reshape(nr, 1);
	else if (pa->rows == 1)
		x.reshape(1, nr);
	else
		x.reshape(nr, nc);

	if ((pa->rows == 1 || pa->columns == 1) && nc == 1) {
		xword na = static_cast<xword>(size(*pa));
		for (xword i = 0; i < nr; ++i) {
			if (pr->array[i] == 0)
				return 0; // avoid index throwing
			x[i] = pa->array[array::index(static_cast<LONG>(pr->array[i]), na)];
		}
	}
	else {
		for (xword i = 0; i < nr; ++i) {
			for (xword j = 0; j < nc; ++j) {
				if (pr->array[i] == 0 || (*pc)[j] == 0)
					return 0; // avoid index throwing
				xword ir = array::index(static_cast<LONG>(pr->array[i]), pa->rows);
				xword jc = array::index(static_cast<LONG>((*pc)[j]), pa->columns); 
				x(i, j) = xll::index(*pa, ir, jc);
			}
		}
	}

	return x.get();
}

#ifdef _DEBUG

#define EVAL(x) ExcelX(xlfEvaluate, OPERX(_T(x)))

int
test_index()
{
	OPERX x;

	try {
		ensure (EVAL("ARRAY.INDEX({1;2;3;4},{0,-1,4,-4})").xltype == xltypeErr);
		ensure (EVAL("ARRAY.INDEX({1;2;3;4},{1,-1,4,-4})={1;4;4;1}")); 
		ensure (EVAL("ARRAY.INDEX({1,2,3,4},{1,-1,4,-5})={1,4,4,4}")); 
		ensure (!EVAL("AND(ARRAY.INDEX({1,2,3,4},{1,-1,4,-5})={1,1,4,4})")); 
		ensure (EVAL("ARRAY.INDEX({1,2,3,4,5,6},0)={1}")[0].xltype == xltypeErr); 
		ensure (EVAL("ARRAY.INDEX({1;2;3;4;5;6},1)={1}")); 
		ensure (EVAL("ARRAY.INDEX({1,2,3,4,5,6},-1)={6}")); 

		ensure (EVAL("ARRAY.INDEX({1,2;3,4},{1,2},1)={1;3}")); 
		ensure (EVAL("ARRAY.INDEX({1,2;3,4},2,{1,2})={3,4}")); 
		ensure (EVAL("ARRAY.INDEX({1,2;3,4},{1,1},{2,2})={2,2;2,2}")); 
		ensure (EVAL("ARRAY.INDEX({1,2;3,4},{3,-2},{2,2})={2,2;2,2}")); 
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

Auto<OpenAfterX> xao_test_index(test_index);

#endif // _DEBUG