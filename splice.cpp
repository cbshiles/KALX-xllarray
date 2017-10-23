// splice.cpp - splice two arrays together
// Copyright (c) 2010 KALX, LLC. All rights reserved. No warranty is made.
#include "array.h"

using namespace xll;

static AddInX xai_array_splice(
	FunctionX(XLL_FPX, _T("?xll_array_splice"), _T("ARRAY.SPLICE"))
	.Arg(XLL_FPX,  _T("Array1"), _T("is an array or handle to an array that gets spliced "))
	.Arg(XLL_LONGX, _T("Index"), _T("is the index at which the array will be spliced "))
	.Arg(XLL_FPX, _T("Array2"), _T("is the array or handle to the array that is spliced into Array1 "))
	.Arg(XLL_LONGX, _T("_Drop"), _T("is the number of elements of Array1 to be removed after (if positive) or before (if negative) Index "))
	.Arg(XLL_BOOLX, _T("_Stack"), _T("is a boolean indicating Array1 should be placed above Array2 "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Splice Array2 into Array1 at Index skipping Drop elements of Array1."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.SPLICE({1,2,3}, 2, {4,5})</codeInline> ")
		_T("is the array <codeInline>{1,2,4,5,3}</codeInline>, ")
		_T("<codeInline>ARRAY.SPLICE({1,2,3}, 2, {4,5}, 1)</codeInline> ")
		_T("is the array <codeInline>{1,2,4,5}</codeInline>, and ")
		_T("<codeInline>ARRAY.SPLICE({1,2,3}, 2, {4,5}, -1)</codeInline> ")
		_T("is the array <codeInline>{1,4,5,3}</codeInline>. ")
		_T("In general, ")
		_T("<codeInline>ARRAY.SPLICE(a, i, b, e)</codeInline> returns ")
		_T("<codeInline>a[1]...a[i]b[1]...b[-1]a[i+e+1]...a[-1]</codeInline> if e is positive ")
		_T("and <codeInline>a[1]...a[i+e]b[1]...b[-1]a[i]...a[-1]</codeInline> if e is negative. ")
		,
		xml::element()
		.content(xml::xlink(_T("ARRAY.JOIN")))
		.content(xml::xlink(_T("ARRAY.STACK")))
	)
);
xfp* WINAPI
xll_array_splice(xfp* pa1, LONG i, xfp* pa2, LONG e, BOOL stack)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		FPX* px = array::handle(pa1);

		if (px) {
			array::splice_command(*px, i, pa2, e);
			x = *pa1;
		}
		else {
			x = *pa1;
			array::splice_command(x, i, pa2, e);
		}

		// special cases
		if (stack)
			ensure (i == -1 && e == 0 && pa1->columns == pa2->columns);

		if (stack || i == -1 && e == 0 && pa1->rows > 1 && pa1->columns == pa2->columns) {
			x.reshape(pa1->rows + pa2->rows, pa1->columns);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return x.get();
}

static AddInX xai_array_join(
	FunctionX(XLL_FPX, _T("?xll_array_join"), _T("ARRAY.JOIN"))
	.Arg(XLL_FPX,  _T("Array1"), _T("is an array or handle to an array that gets joined."))
	.Arg(XLL_FPX, _T("Array2"), _T("is the array or handle to the array that is joined into Array1."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Join Array1 and Array2."))
	.Documentation(
		_T("If they are not single rows then both arrays must have the same number of columns.")
		,
		xml::element()
		.content(xml::xlink(_T("ARRAY.SPLICE")))
		.content(xml::xlink(_T("ARRAY.STACK")))
	)
);
xfp* WINAPI
xll_array_join(xfp* pa1, xfp* pa2)
{
#pragma XLLEXPORT
	if (pa1->columns != pa2->columns && !(pa1->rows == 1 && pa2->rows == 1)) {
		XLL_ERROR("ARRAY.JOIN: both arrays must have the same number of columns.");

		return 0;
	}

	return xll_array_splice(pa1, -1, pa2, 0, false);
}

static AddInX xai_array_stack(
	FunctionX(XLL_FPX, _T("?xll_array_stack"), _T("ARRAY.STACK"))
	.Arg(XLL_FPX,  _T("Array1"), _T("is an array or handle to an array that gets stacked above Array2."))
	.Arg(XLL_FPX, _T("Array2"), _T("is the array or handle to the array that is stacked below Array1."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Stack Array1 and Array2."))
	.Documentation(
		_T("Both arrays must have the same number of columns.")
		,
		xml::element()
		.content(xml::xlink(_T("ARRAY.JOIN")))
		.content(xml::xlink(_T("ARRAY.SPLICE")))
	)
);
xfp* WINAPI
xll_array_stack(xfp* pa1, xfp* pa2)
{
#pragma XLLEXPORT
	if (pa1->columns != pa2->columns && !(pa1->rows == 1 && pa2->rows == 1)) {
		XLL_ERROR("ARRAY.STACK: both arrays must have the same number of columns.");

		return 0;
	}

	return xll_array_splice(pa1, -1, pa2, 0, true);
}

#ifdef _DEBUG

#define EVAL(s) ExcelX(xlfEvaluate, OPERX(_T(s)))

int
test_splice()
{
	FPX a(2, 1), b(3, 1);
	a[0] = 1;
	a[1] = 2;
	b[0] = 3;
	b[1] = 4;
	b[2] = 5;

	FPX a0;
	a0 = a;
	array::splice_command(a0, 0, b.get(), 0);
	ensure (a0.size() == 5);
	ensure (a0[0] == 3);
	ensure (a0[1] == 4);
	ensure (a0[2] == 5);
	ensure (a0[3] == 1);
	ensure (a0[4] == 2);


	a0 = a;
	array::splice_command(a0, 1, b.get(), 0);
	ensure (a0.size() == 5);
	ensure (a0[0] == 1);
	ensure (a0[1] == 3);
	ensure (a0[2] == 4);
	ensure (a0[3] == 5);
	ensure (a0[4] == 2);

	a0 = a;
	array::splice_command(a0, -2, b.get(), 0);
	ensure (a0.size() == 5);
	ensure (a0[0] == 1);
	ensure (a0[1] == 3);
	ensure (a0[2] == 4);
	ensure (a0[3] == 5);
	ensure (a0[4] == 2);


	a0 = a;
	array::splice_command(a0, -1, b.get(), 0);
	ensure (a0.size() == 5);
	ensure (a0[0] == 1);
	ensure (a0[1] == 2);
	ensure (a0[2] == 3);
	ensure (a0[3] == 4);
	ensure (a0[4] == 5);

	a0 = a;
	array::splice_command(a0, 2, b.get(), 0);
	ensure (a0.size() == 5);
	ensure (a0[0] == 1);
	ensure (a0[1] == 2);
	ensure (a0[2] == 3);
	ensure (a0[3] == 4);
	ensure (a0[4] == 5);

	// tests for elide
	a0 = a;
	array::splice_command(a0, 0, b.get(), 1);
	ensure (a0.size() == 4);
	ensure (a0[0] == 3);
	ensure (a0[1] == 4);
	ensure (a0[2] == 5);
	ensure (a0[3] == 2);

	a0 = a;
	array::splice_command(a0, 1, b.get(), 1);
	ensure (a0.size() == 4);
	ensure (a0[0] == 1);
	ensure (a0[1] == 3);
	ensure (a0[2] == 4);
	ensure (a0[3] == 5);

	OPERX o1, o2;
	o1 = EVAL("ARRAY.MAKE()");
	ensure (o1.xltype == xltypeMulti);
	ensure (o1.val.array.rows == 1);
	ensure (o1.val.array.columns == 1);
	ensure (o1.val.array.lparray[0].xltype == xltypeNum);
	ensure (_isnan(o1.val.array.lparray[0].val.num));

	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, 0, {4, 5}")=EVAL("{4,5,1,2,3}"));
	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, 1, {4, 5}")=EVAL("{1,4,5,2,3}"));
	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, 2, {4, 5}")=EVAL("{1,2,4,5,3}"));
	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, 2, {4, 5}")=EVAL("{1,2,4,5,3}"));
	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, 3, {4, 5}")=EVAL("{1,2,3,4,5}"));
	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, -1, {4, 5}")=EVAL("{1,2,3,4,5}"));
	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, 2, {4, 5}, 1")=EVAL("{1,2,4,5}"));
	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, 2, {4, 5}, -1")=EVAL("{1,4,5,3}"));
	ensure (EVAL("=ARRAY.SPLICE({1,2,3}, 2, {4, 5}, -2")=EVAL("{4,5,3}"));

	ensure (EVAL("=ROWS(ARRAY.JOIN({1,2,3},{4,5})) = 1"));
	ensure (EVAL("=COLUMNS(ARRAY.JOIN({1,2,3},{4,5})) = 5"));
	ensure (EVAL("=INDEX(ARRAY.JOIN({1,2,3},{4,5}),1) = 1"));
	ensure (EVAL("=INDEX(ARRAY.JOIN({1,2,3},{4,5}),5) = 5"));

	ensure (EVAL("=ROWS(ARRAY.JOIN({1,2,3;4,5,6},{7,8,9})) = 3"));
	ensure (EVAL("=COLUMNS(ARRAY.JOIN({1,2,3;4,5,6},{7,8,9})) = 3"));
	ensure (EVAL("=INDEX(ARRAY.JOIN({1,2,3;4,5,6},{7,8,9}),1,1) = 1"));
	ensure (EVAL("=INDEX(ARRAY.JOIN({1,2,3;4,5,6},{7,8,9}),3,3) = 9"));

	ensure (EVAL("=ROWS(ARRAY.JOIN({1;2;3;4},{5;6})) = 6"));
	ensure (EVAL("=COLUMNS(ARRAY.JOIN({1;2;3;4},{5;6})) = 1"));
	ensure (EVAL("=INDEX(ARRAY.JOIN({1;2;3;4},{5;6}),1) = 1"));
	ensure (EVAL("=INDEX(ARRAY.JOIN({1;2;3;4},{5;6}),6) = 6"));

	ensure (EVAL("=ROWS(ARRAY.JOIN({1,2,3},{4,5,6})) = 1"));
	ensure (EVAL("=COLUMNS(ARRAY.JOIN({1,2,3},{4,5,6})) = 6"));
	ensure (EVAL("=INDEX(ARRAY.JOIN({1,2,3},{4,5,6}),1) = 1"));
	ensure (EVAL("=INDEX(ARRAY.JOIN({1,2,3},{4,5,6}),6) = 6"));

	ensure (EVAL("=ROWS(ARRAY.STACK({1,2,3},{4,5,6})) = 2"));
	ensure (EVAL("=COLUMNS(ARRAY.STACK({1,2,3},{4,5,6})) = 3"));
	ensure (EVAL("=INDEX(ARRAY.STACK({1,2,3},{4,5,6}),1,1) = 1"));
	ensure (EVAL("=INDEX(ARRAY.STACK({1,2,3},{4,5,6}),2,3) = 6"));

	return 1;
}

static Auto<OpenAfterX> xaa_splice(test_splice);

#endif // _DEBUG