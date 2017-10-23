// mask.cpp - remove elements or rows not in a mask
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)
#include <algorithm>
#include <functional>

#include "array.h"

using namespace std;
using namespace xll;

static void
array_mask(FPX& x, xfp* pm)
{
	ensure (x.size() == size(*pm));

	xword i = 0;
	for (xword j = 0; j < x.size(); ++j) {
		if (pm->array[j]) {
			x[i] = x[j];
			++i;
		}
	}

	if (x.columns() == 1)
		x.reshape(i, 1);
	else
		x.reshape(1, i);
}

static AddInX xai_array_mask(
	FunctionX(XLL_FPX, _T("?xll_array_mask"), _T("ARRAY.MASK"))
	.Arg(XLL_FPX, _T("Mask"), _T("contains zero elements at corresponding array indices that are to be removed "))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Mask elements from Array."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.MASK({1,0,1}, {1,2,3})</codeInline> is the array ")
		_T("<codeInline>{1,3}</codeInline>.")
		,
		xml::element()
		.content(xml::xlink(_T("ARRAY.MEMBER")))
		.content(xml::xlink(_T("ARRAY.WHERE")))
	)
);
xfp* WINAPI
xll_array_mask( xfp* pm, xfp* pa)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		FPX* px = array::handle(pa);

		if (px) {
			array_mask(*px, pm);
			x = *pa;
		}
		else {
			x = *pa;
			array_mask(x, pm);
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
test_mask()
{
	FPX x(4,1), m(4, 1);

	x[0] = 1; x[1] = 2; x[2] = 4; x[3] = 4;
	m[0] = 1; m[1] = 1; m[2] = 1; m[3] = 1;

	FPX x1(x);
	array_mask(x1, m.get());
	ensure (x1.size() == 4);
	ensure (x1[0] = 1);
	ensure (x1[1] = 2);
	ensure (x1[2] = 3);
	ensure (x1[3] = 4);

	FPX x2(x);
	m[0] = 0;
	array_mask(x2, m.get());
	ensure (x2.size() == 3);
	ensure (x2[0] = 2);
	ensure (x2[1] = 3);
	ensure (x2[2] = 4);

	FPX x3(x);
	m[0] = 1; m[3] = 0;
	array_mask(x3, m.get());
	ensure (x3.size() == 3);
	ensure (x3[0] = 1);
	ensure (x3[1] = 2);
	ensure (x3[2] = 3);


	FPX x4(x);
	m[3] = 1; m[1] = m[2] = 0;
	array_mask(x4, m.get());
	ensure (x4.size() == 2);
	ensure (x4[0] = 2);
	ensure (x4[1] = 3);

	return 1;
}

static Auto<OpenAfterX> xao_test_mask(test_mask);

#endif // _DEBUG