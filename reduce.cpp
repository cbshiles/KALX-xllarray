// reduce.cpp - aggregate rows of an array
#include "array.h"

using namespace xll;

static AddInX xai_array_aggregate(
	FunctionX(XLL_FPX, _T("?xll_array_reduce"), _T("ARRAY.REDUCE"))
	.Arg(XLL_FPX, _T("Groups"), _T("is an array of the number of rows to reduce."))
	.Arg(XLL_FPX, _T("Array"), _T("is and array or handle to an array."))
//	.Arg(XLL_LPOPERX, _T("_Reduce"), _T("is an optional array of functions used to reduce rows. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Apply reduction to Array row Groups using _Reduce for each column."))
	.Documentation()
);
xfp* WINAPI
xll_array_reduce(const xfp* pg, const xfp* pa/*, LPOPERX pr*/)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		FPX *px = array::handle(pa);
		if (px) {
			array::reduce_command(*px, pg);
			x = *pa;
		}
		else {
			x = *pa;
			array::reduce_command(x, pg);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return x.get();
}
}