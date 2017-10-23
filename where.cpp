// where.cpp - return indices of non zero mask elements
// Copyright (c) 2006-2010 KALX, LLC. All rights reserved. No warranty is made.
#include <algorithm>

#include "array.h"

using namespace xll;

static AddInX xai_array_where(
	FunctionX(XLL_FPX, _T("?xll_array_where"), _T("ARRAY.WHERE"))
	.Arg(XLL_FPX, _T("Mask"), ARRAY_OR_HANDLE)
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns indices of non-zero mask elements."))
	.Documentation(
		_T("For example, <codeInline>ARRAY.WHERE({1, 0, -1})</codeInline> ")
		_T("returns <codeInline>{1,3}</codeInline>. ")
		_T("Note <codeInline>INDEX(Array, ARRAY.WHERE(Mask)) = ARRAY.MASK(Array, Mask)</codeInline>. ")
		,
		xml::element()
		.content(xml::xlink(_T("ARRAY.MASK")))
		.content(xml::xlink(_T("ARRAY.MEMBER")))
	)
);
#pragma warning(push)
#pragma warning(disable: 4702)
xfp* WINAPI
xll_array_where(const xfp* pa)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		if (FPX* px = array::handle(pa))
			pa = px->get();

		xword i(0), n(static_cast<xword>(size(*pa)));

		if (pa->columns == 1)
			x.reshape(n, 1);
		else
			x.reshape(1, n);

		for (xword j = 0; j < n; ++j) {
			if (pa->array[j])
				x[i++] = j + 1;
		}

		if (pa->columns == 1)
			x.reshape(i, 1);
		else
			x.reshape(1, i);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return x.get();
}
#pragma warning(pop)