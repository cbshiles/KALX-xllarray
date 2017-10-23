// buffer.cpp - buffer rows on top or bottom of array
// Copyright (c) 2006-2012 KALX, LLC. All rights reserved. No warranty is made.
#include "array.h"
#include "command.h"

using namespace xll;

//!!! not working !!!
static AddInX X_(xai_array_buffer)(
	FunctionX(XLL_HANDLEX, TX_("?xll_array_buffer"), _T("ARRAY.BUFFER"))
	.Uncalced()
	.Arg(XLL_FPX, _T("Array"), _T("is an array of numbers to be buffered."))
	.Arg(XLL_LONGX, _T("Rows"), _T("is the number rows in the buffer."))
	.Arg(XLL_BOOLX, _T("_Reset"), _T("is a boolean that sets the buffer to Array when true. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Append Array to the top (if positive) or bottom (if negative) Rows of internal buffer and return handle. "))
	.Documentation(
		_T("If rows is 0 append to the beginning/top and if rows is -1 append to the end/bottom. ")
	)
);
HANDLEX WINAPI
X_(xll_array_buffer)(xfp* pa, LONG n, BOOL b)
{
#pragma XLLEXPORT
	handlex hx;

	try {
		locus l;
		OPERX h = l.get();
		handle<FPX> h_((h.xltype == xltypeNum && h.val.num) ? h.val.num : handle<FPX>(new FPX()).get());

		ensure (h_);

		if (b) {
			h_->reshape(0, 0);
		}
		else {
			h_->reshape(h_->size(), 1);
		}

		if (n >= 0)
			array::splice_command(*h_, 0, pa);
		else
			array::splice_command(*h_, -1, pa);

		if (n > 1 || n < -1)
			array::take_command(*h_, n*size(*pa));

		h_->reshape(h_->size()/size(*pa), size(*pa));

		l.set(OPERX(h_.get()));

		hx = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return hx;
}
