// running.cpp - running binary operator applied to an array
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)
#include <functional>
#include <numeric>
#include "array.h"

using namespace xll;

static AddInX xai_array_running(
	FunctionX(XLL_FPX, _T("?xll_array_running"), _T("ARRAY.RUNNING"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a function that takes two numeric arguments."))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE _T(" "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the running Function of numbers from Array."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.RUNNING(FUNCTION.ADD, {2,1,2})</codeInline> ")
		_T("is the array <codeInline>{2,3,5}</codeInline>. ")
/*		,
		xml::xlink(_T("ARRAY.DELTAS"))
*/	)
);
xfp* WINAPI
xll_array_running(HANDLEX f, xfp* pa)
{
#pragma XLLEXPORT
	if (FPX* px = array::handle(pa))
		pa = px->get();

	OPERX F(f);
	for (xword i = 1; i < size(*pa); ++i)
		pa->array[i] = ExcelX(xlUDF, F, OPERX(pa->array[i]), OPERX(pa->array[i - 1]));

	return pa;
}
