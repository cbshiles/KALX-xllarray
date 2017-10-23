// command.h - command functions
// Copyright (c) 2006-2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once

namespace array {

	inline void
	drop_command(xll::FPX& x, long n)
	{
		long nx = x.size();

		if (n == 0)
			return;

		if (abs(n) >= nx) {
			x.reshape(0,0);

			return;
		}

		if (n > 0) {
			memmove(x.array(), x.array() + n, (nx - n)*sizeof(double));
		}

		if (x.columns() == 1)
			x.reshape(static_cast<xll::traits<XLOPERX>::xword>(nx - abs(n)), 1);
		else
			x.reshape(1, static_cast<xll::traits<XLOPERX>::xword>(nx - abs(n)));
	}

	inline void
	take_command(xll::FPX& x, long n)
	{
		long nx = x.size();

		if (abs(n) > nx) {

			return;
		}

		if (n < 0) {
			n = -n;
			memmove(x.array(), x.array() + nx - n, n*sizeof(double));
		}

		if (n != 0) {
			ensure (n < xll::limits<XLOPERX>::maxrows);
			if (x.columns() == 1)
				x.reshape(static_cast<xll::traits<XLOPERX>::xword>(n), 1);
			else
				x.reshape(1, static_cast<xll::traits<XLOPERX>::xword>(n));
		}
		else {
			x.reshape(0, 0);
		}
	}

	//!!!test boundary cases!!!
	inline void
	splice_command(xll::FPX& a, long i, const xll::traits<XLOPERX>::xfp* pb, long e = 0)
	{
		xll::traits<XLOPERX>::xword n;
		long na = a.size();
		long nb = xll::size(*pb);

//		size_t absi = i >= 0 ? i : -i;

		long e_ = abs(e);

		if (nb == 0) {
			ensure (e_ <= na);
			if (e > 0) {
				n = static_cast<xll::traits<XLOPERX>::xword>(na - e_);
				if (a.columns() == 1)
					a.reshape(n, 1);
				else
					a.reshape(1, n);
			}
				
			return;
		}

		if (na == 0) {
			ensure (e == 0);
			a = *pb;

			return;
		}

		ensure (i >= -na);
		ensure (i <= na);

		if (i < 0)
			i = na + i + 1 - e_;

		if (e >= 0) {
			ensure (e_ <= na - i);
		}
		else {
			ensure (e_ <= i);
		}

		n = static_cast<xll::traits<XLOPERX>::xword>(na + nb - e_);
		if (a.columns() == 1 && pb->columns == 1) {
			ensure (na + nb - e_ < xll::limits<XLOPERX>::maxrows);
			a.reshape(n, 1);
		}
		else {
			ensure (na + nb - e_ < xll::limits<XLOPERX>::maxcols);
			a.reshape(1, n);
		}

		if (e >= 0) {
			if (na - i - e_ > 0)
				memmove(a.array() + i + nb, a.array() + i + e, (na - i - e_)*sizeof(double));
			memcpy(a.array() + i, pb->array, nb*sizeof(double));
		}
		else {
			if (na - i > 0)
				memmove(a.array() + i - e_ + nb, a.array() + i, (na - i)*sizeof(double));
			memcpy(a.array() + i - e_, pb->array, nb*sizeof(double));
		}
	}
	// push *pb on top (n >= 0) or bottom (n < 0) of a
	inline void
	push_command(xll::FPX& a, const xll::traits<XLOPERX>::xfp* pb, long n)
	{
		xll::traits<XLOPERX>::xword na = a.size();
		xll::traits<XLOPERX>::xword nb = static_cast<xll::traits<XLOPERX>::xword>(xll::size(*pb));

		ensure (na != xll::limits<XLOPERX>::maxrows - 1);

		xll::traits<XLOPERX>::xword absn = static_cast<xll::traits<XLOPERX>::xword>(abs(n));

		if (a.is_empty()) {
			a = *pb;
		}
		else if (n >= 0) {
			ensure (a.columns() == nb);
			if (n == 0 || a.rows() < absn) {
				a.reshape(a.rows() + 1, nb);
				memmove(a.begin() + nb, a.begin(), na*sizeof(double));
				memcpy(a.begin(), pb->array, nb*sizeof(double));
			}
			else {
				memmove(a.begin() + nb, a.begin(), (na - nb)*sizeof(double));
				memcpy(a.begin(), pb->array, nb*sizeof(double));
			}
		}
		else { // n < 0
			ensure (a.columns() == nb);
			if (n == -1 || a.rows() < absn) {
				a.reshape(a.rows() + 1, nb);
				memcpy(a.begin() + na, pb->array, nb*sizeof(double));
			}
			else {
				memmove(a.begin(), a.begin() + nb, na*sizeof(double));
				memcpy(a.begin() + na, pb->array, nb*sizeof(double));
			}
		}
	}

} // namespace array