// sums.cpp - running sum of elements of an array
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)
#include <functional>
#include <numeric>
#include "array.h"

using namespace xll;

static AddInX xai_sums(
	FunctionX(XLL_FPX, _T("?xll_sums"), _T("ARRAY.SUMS"))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the running sum of numbers from Array."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.SUMS({2,1,2})</codeInline> ")
		_T("is the array <codeInline>{2,3,5}</codeInline>. ")
		_T("This function calls the C++ Standard Library routine <codeInline>std::partial_sum</codeInline> ")
		_T("declared in the &lt;algorithm&gt; header.")
		_T("</para><para>")
		_T("<codeInline>ARRAY.DELTAS</codeInline> is the inverse of this function. ")
		,
		xml::xlink(_T("ARRAY.DELTAS"))
	)
);
xfp* WINAPI
xll_sums(xfp* pa)
{
#pragma XLLEXPORT
	xfp* pb = pa;

	if (FPX* px = array::handle(pa))
		pa = px->get();

	std::partial_sum(pa->array, pa->array + size(*pa), &pa->array[0]); 

	return pb;
}
