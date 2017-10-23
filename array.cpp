// array.cpp - create arrays
// Copyright (c) 2006-20012 KALX, LLC. All rights reserved. No warranty is made.
//#define EXCEL4
#include "array.h"

using namespace xll;
using namespace array;

#ifdef _DEBUG
static AddInX xai_array_doc(
	DocumentX(CATEGORY)
	.Documentation(xml::File(XML_DIR _T("array.xml")))
);	
#endif
static AddInX xai_array_make(
	FunctionX(XLL_FPX, _T("?xll_array_make"), _T("ARRAY.MAKE"))
	.Arg(XLL_WORDX, _T("Rows"), _T("is the number of rows in the array"))
	.Arg(XLL_WORDX, _T("_Columns"), _T("is the number of columns in the array. The default value is 1"))
	.Arg(XLL_DOUBLEX, _T("_Value"), _T("is the value supplied for every array entry. The default value is 0 "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns an array with given Rows and Columns filled with Value"))
	.Documentation(
		_T("If the number of rows or columns is 0 then an empty array is returned.")
		,
		xml::xlink(_T("ARRAY.ZOT"))
	)
);
xfp* WINAPI
xll_array_make(xword r, xword c, double v)
{
#pragma XLLEXPORT
	static FPX x;

	if (c == 0)
		c = 1;
	x.reshape(r, c);
	if (x.size() > 0)
		x = v;

	return x.get();
}

#ifdef _DEBUG

int
test_array()
{
	try {
		xfp* px = xll_array_make(2, 3, 4);
		ensure (px->rows == 2);
		ensure (px->columns == 3);
		ensure (px->array[5] == 4);

		ensure (Excel<XLOPERX>(xlfEvaluate, OPERX(_T("ARRAY.ISEMPTY(ARRAY.MAKE())"))));
		ensure (!Excel<XLOPERX>(xlfEvaluate, OPERX(_T("ARRAY.ISEMPTY(ARRAY.MAKE(1))"))));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

static Auto<OpenAfterX> xao_test_array(test_array);

#endif // _DEBUG
