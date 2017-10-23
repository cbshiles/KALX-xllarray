// getset.cpp - get and set in memory arrays
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.

#include "array.h"

using namespace xll;

static AddInX xai_array_set(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_array_set"), _T("ARRAY.SET"))
	.Arg(XLL_FPX, _T("Array"), _T("is an array of numbers "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to an in memory Array."))
	.Documentation(
		_T("Use <codeInline>ARRAY.GET(Handle)</codeInline> to retrieve the array.")
/*		,
		xml::element()
		.content(xml::xlink(_T("ARRAY.GET")))
		.content(xml::xlink(_T("ARRAY.MAKE")))
*/	)
);
HANDLEX WINAPI
xll_array_set(xfp* pa)
{
#pragma XLLEXPORT
	HANDLEX xh(0);

	try {
		handle<FPX> h(new FPX(*pa));
		xh = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return xh;
}

static AddInX xai_array_get(
	FunctionX(XLL_FPX, _T("?xll_array_get"), _T("ARRAY.GET"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an array "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return an array from a Handle to an in-memory array."))
	.Documentation(
		_T("Use the handle returned by <codeInline>ARRAY.SET</codeInline>.")
		,
		xml::element()
		.content(xml::xlink(_T("ARRAY.SET")))
		.content(xml::xlink(_T("ARRAY.MAKE")))
	)
);
xfp* WINAPI
xll_array_get(HANDLEX h)
{
#pragma XLLEXPORT
	xfp* p(0);

	try {
		if (h) {
			handle<FPX> h_(h); // checked???
			if (h_)
				p = h_->get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return p;
}

#ifdef _DEBUG

int
test_getset(void)
{
	ensure (ExcelX(xlfEvaluate, OPERX(_T("ARRAY.GET(ARRAY.SET(ARRAY.MAKE(2, 3, 4)))={4,4,4;4,4,4}"))));
	
	return 1;
}

static Auto<OpenAfterX> xao_test_getset(test_getset);

#endif // _DEBUG
