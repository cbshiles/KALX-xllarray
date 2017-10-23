// sort.cpp - sort elements of an array
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)
#include <numeric>
#include "array.h"

using namespace xll;

static AddInX xai_array_sort(
	FunctionX(XLL_FPX, _T("?xll_array_sort"), _T("ARRAY.SORT"))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Arg(XLL_BOOLX, _T("_Descending"), _T("sorts from highest to lowest if true "))
	.Arg(XLL_WORDX, _T("_Count"), _T("is the number of initial elements to sort "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Sort the first Count numbers in Array in increasing order by default."))
	.Documentation(
		_T("This function calls the C++ Standard Library routine <codeInline>std::sort</codeInline> ")
		_T("declared in the &lt;algorithm&gt; header if the count is not specified or zero, ")
		_T("otherwise <codeInline>std::partial_sort</codeInline> is used.")
		,
		xml::xlink(_T("ARRAY.GRADE"))
	)
);
xfp* WINAPI
xll_array_sort(xfp* pa, BOOL decr, xword n)
{
#pragma XLLEXPORT
	xword na = size(*pa);

	try {
		FPX* px = array::handle(pa);

		if (n == 0) {
			if (decr) {
				if (px)
					std::sort(px->begin(), px->end(), std::greater<double>()); 
				else
					std::sort(&pa->array[0], &pa->array[0] + na, std::greater<double>()); 
			}
			else {
				if (px)
					std::sort(px->begin(), px->end()); 
				else
					std::sort(&pa->array[0], &pa->array[0] + na); 
			}
		}
		else {
			if (n > na) {
				XLL_WARNING("ARRAY.SORT: partial sort size is greater than the number of array elements");

				n = na;
			}

			if (decr) {
				if (px)
					std::partial_sort(px->begin(), px->begin() + n, px->end(), std::greater<double>());
				else
					std::partial_sort(&pa->array[0], &pa->array[0] + n, &pa->array[0] + na, std::greater<double>());
			}
			else {
				if (px)
					std::partial_sort(px->begin(), px->begin() + n, px->end());
				else
					std::partial_sort(&pa->array[0], &pa->array[0] + n, &pa->array[0] + na);
			}
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return pa;
}

#ifdef _DEBUG

#define EVAL(x) ExcelX(xlfEvaluate, OPERX(_T(x)))
int
test_sort(void)
{
	try {
		OPER o;

		ensure (EVAL("{1,2,3}=ARRAY.SORT({3,1,2})"));
		ensure (EVAL("{3,2,1}=ARRAY.SORT({3,1,2}, TRUE)"));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_sort(test_sort);

#endif // _DEBUG

