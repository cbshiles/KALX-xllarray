// unique.cpp - remove consecutive duplicates
// Copyright (c) 2006-2010 KALX, LLC. All rights reserved. No warranty is made.
#include <algorithm>
#include "array.h"

using namespace xll;

static AddInX xai_array_unique(
	FunctionX(XLL_FPX, _T("?xll_array_unique"), _T("ARRAY.UNIQUE"))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Remove consecutive duplicates from Array."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.UNIQUE({1, 2, 2, 3, 2, 2})</codeInline> is ")
		_T("<codeInline>{1, 2, 3, 2}</codeInline>. ")
		_T("This function calls the C++ Standard Library routine <codeInline>std::unique()</codeInline> ")
		_T("declared in the &lt;algorithm&gt; header. ")
	)
);
xfp* WINAPI
xll_array_unique(xfp* pa)
{
#pragma XLLEXPORT
	static FPX x;
	double* pe;

	try {
		FPX *px = array::handle(pa);

		if (px) {
			pe = std::unique(px->array(), px->array() + px->size());
			x = *pa;
		}
		else {
			x = *pa;
			pe = std::unique(x.array(), x.array() + x.size());
		}

		if (x.columns() == 1)
			x.reshape(static_cast<xword>(pe - x.array()), 1);
		else
			x.reshape(1, static_cast<xword>(pe - x.array()));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return x.get();
}

#ifdef _DEBUG

#endif // _DEBUG