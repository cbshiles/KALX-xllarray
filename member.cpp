// member.cpp - set membership
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
// Compute prime numbers:
// ARRAY.MASK(Sieve, NOT(ARRAY.MEMBER(Sieve, ARRAY.MULTIPLY(Sieve, TRANSPOSE(Sieve))))
#include <algorithm>
#include "array.h"

using namespace xll;

static AddInX xai_array_member(
	FunctionX(XLL_FPX, _T("?xll_array_member"), _T("ARRAY.MEMBER"))
	.Arg(XLL_FPX, _T("Elements"), _T("is an array of numbers to be checked for membership "))
	.Arg(XLL_FPX, _T("Array"), _T("is an array of numbers defining a set "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return mask of Elements that belong to Array."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.MEMBER({1,2,3,4}, {2,4,6})</codeInline> v")
		_T("is <codeInline>{0,1,0,1}</codeInline>.")
/*		,
		xml::element()
		.content(xml::xlink(_T("ARRAY.MASK")))
		.content(xml::xlink(_T("ARRAY.WHERE")))
*/	)
);
xfp* WINAPI
xll_array_member(xfp* pe, xfp* ps)
{
#pragma XLLEXPORT

	double* b = begin(*pe);
	double* e = end(*pe);

	FPX* px = array::handle(ps);
	if (px) {
		for (xword i = 0; i < px->size(); ++i) {
			(*px)[i] = (e != std::find(b, e, (*px)[i]));
		}
	}
	else {	
		for (xword i = 0; i < size(*ps); ++i) {
			ps->array[i] = (e != std::find(b, e, ps->array[i]));
		}
	}

	return ps;
}

#ifdef _DEBUG

int
test_member()
{
	try {
		OPER o;

		ensure (ExcelX(xlfEvaluate, OPERX(_T("ARRAY.MEMBER({2,4,6}, {1,2,3,4})={0,1,0,1}"))));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

static Auto<OpenAfterX> xao_test_member(test_member);

#endif // _DEBUG