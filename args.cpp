// args.cpp - convert individual arguments to an array
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma warning(disable: 4996)
#include "array.h"

using namespace xll;

static AddInX xai_array_arg_end(
	FunctionX(XLL_FPX, _T("?xll_array_arg_end"), _T("ARRAY.ARG.END"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Indicate the end of arguments to ARRAY.ARGS."))
	.Documentation(
		_T("This function returns Not a Number (NaN). ")
	)
);
xfp* WINAPI
xll_array_arg_end()
{
#pragma XLLEXPORT
	static xfp x;

	x.rows = 1;
	x.columns = 1;
	x.array[0] = std::numeric_limits<double>::quiet_NaN();

	return &x;
}

static AddInX xai_array_arg_zero(
	FunctionX(XLL_FPX, _T("?xll_array_arg_zero"), _T("ARRAY.ARG.ZERO"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Indicate an argument of zero (0) to ARRAY.ARGS."))
	.Documentation(
		_T("This function returns negative zero. ")
	)
);
xfp* WINAPI
xll_array_arg_zero()
{
#pragma XLLEXPORT
	static xfp x;

	x.rows = 1;
	x.columns = 1;
	x.array[0] = _copysign(0, -1);

	return &x;
}

// negative zero
inline bool isnzero(double x)
{
	return !x && _copysign(1, x) < 0;
}

static AddInX xai_array_arg(
	FunctionX(XLL_FPX, _T("?xll_array_arg"), _T("ARRAY.ARG"))
	.Arg(XLL_DOUBLEX, _T("Number"), _T("is a number to be passed to ARRAY.ARGS. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return zero as negative zero other wise the Number. "))
	.Documentation(
		_T("Some functions like to take a single argument array. ")
	)
);
xfp* WINAPI
xll_array_arg(double x0)
{
#pragma XLLEXPORT
	static xfp x;

	x.rows = 1;
	x.columns = 1;
	x.array[0] = x0 ? x0 : _copysign(0, -1);

	return &x;
}

static AddInX xai_array_args(
	FunctionX(XLL_FPX, _T("?xll_array_args"), _T("ARRAY.ARGS"))
	.Arg(_T("BBBB") _T("BBBB") _T("BBBB") _T("BBBB") _T("B"), _T("Number, ..."), _T("are up to 16 numeric arguments. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Gather the numeric arguments into a single array. "))
	.Documentation(
		_T("Some functions like to take a single argument array. This groups the individual arguments and returns a one row array. ")
	)
);
xfp* WINAPI
xll_array_args(double x0)
{
#pragma XLLEXPORT
	static FPX x;

	x.reshape(0, 0);
	for (double* px = &x0; px - &x0 < 16 && (*px || isnzero(*px)) && !_isnan(*px); ++px) {
		x.push_back(isnzero(*px) ? 0 : *px);
	}


	return x.get();
}
