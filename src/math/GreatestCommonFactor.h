#pragma once

namespace CommonTools_Native { namespace Math {

	// Euclid's Algorithm
	int GreatestCommonFactor(int a, int b)
	{
		while (b != 0)
		{
			int temp = b;
			b = a % b;
			a = temp;
		}

		return a;
	}

	int LeastCommonMultiple(int a, int b)
	{
		int gcf = GreatestCommonFactor(a, b);
		return a * b / gcf;
	}

}}
