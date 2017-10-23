// grade.cpp - indices that would sort array
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
// !!!Add Count arg for partial sorting like ARRAY.SORT!!!
#pragma warning(disable: 4996)
#include <numeric>
#include "array.h"

using namespace xll;

struct gradeup : public std::binary_function<double, double, bool> {	
	const double* pa_;
	gradeup(const double* pa)
		: pa_(pa)
	{ }
	bool operator()(const double& _Left, const double& _Right) const
	{
		return pa_[static_cast<xword>(_Left - 1)] < pa_[static_cast<xword>(_Right - 1)];
	}
};
struct gradedn : public std::binary_function<double, double, bool> {	
	const double* pa_;
	gradedn(const double* pa)
		: pa_(pa)
	{ }
	bool operator()(const double& _Left, const double& _Right) const
	{
		return pa_[static_cast<xword>(_Left - 1)] > pa_[static_cast<xword>(_Right - 1)];
	}
};

static AddInX xai_array_grade(
	FunctionX(XLL_FPX, _T("?xll_array_grade"), _T("ARRAY.GRADE"))
	.Arg(XLL_FPX, _T("Array"), _T("is an array of numbers "))
	.Arg(XLL_BOOLX, _T("_Decreasing"), _T("sorts from highest to lowest if true "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return indices that would sort Array in increasing order by default."))
	.Documentation(
		_T("Note that <codeInline>INDEX(Array, ARRAY.GRADE(Array))</codeInline> is the same as ")
		_T("<codeInline>ARRAY.SORT(Array)</codeInline>, where <codeInline>INDEX</codeInline> ")
		_T("is the Excel built-in function. ")
		_T("If <codeInline>Str</codeInline> is a range of strings, ")
		_T("<codeInline>INDEX(Str, ARRAY.GRADE(LEN(Str)))</codeInline> would sort them according ")
		_T("to their length. ")
		,
		xml::xlink(_T("ARRAY.SORT"))
	)
);
xfp* WINAPI
xll_array_grade(xfp* pa, BOOL decr)
{
#pragma XLLEXPORT
	static FPX x;

	x.reshape(pa->rows, pa->columns);
	for (xword i = 1; i <= x.size(); ++i)
		x[i - 1] = i;

	if (decr)
		std::sort(x.array(), x.array() + size(*pa), gradedn(pa->array)); 
	else
		std::sort(x.array(), x.array() + size(*pa), gradeup(pa->array)); 

	return x.get();
}

#ifdef _DEBUG

int
test_grade(void)
{
	try {
		OPER o;

		ensure (ExcelX(xlfEvaluate, OPERX(_T("ARRAY.GRADE({3,1,2})={2,3,1}"))));
		ensure (ExcelX(xlfEvaluate, OPERX(_T("ARRAY.GRADE({3,1,2}, TRUE)={1,3,2}"))));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_grade(test_grade);

#endif // _DEBUG
