// reverse.cpp - remove consecutive duplicates
// Copyright (c) 2006-2010 KALX, LLC. All rights reserved. No warranty is made.
#include <algorithm>
#include "array.h"

using namespace xll;

static AddInX xai_array_reverse(
	FunctionX(XLL_FPX, _T("?xll_array_reverse"), _T("ARRAY.REVERSE"))
	.Arg(XLL_FPX, _T("Array"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Remove consecutive duplicates from Array."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.REVERSE({1, 2, 3})</codeInline> is ")
		_T("<codeInline>{3, 2, 1}</codeInline>. ")
		_T("This function calls the C++ Standard Library routine <codeInline>std::reverse()</codeInline> ")
		_T("declared in the &lt;algorithm&gt; header. ")
	)
);
xfp* WINAPI
xll_array_reverse(xfp* pa)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		FPX *px = array::handle(pa);

		if (px) {
			std::reverse(px->array(), px->array() + px->size());
			x = *pa;
		}
		else {
			x = *pa;
			std::reverse(x.array(), x.array() + x.size());
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return x.get();
}

#ifdef _DEBUG

#endif // _DEBUG