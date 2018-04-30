#pragma once

namespace Tools { namespace Math {

	/*
	 * Euclid's algorithm for finding the greatest common factor of a and b.
	 */
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

	/*
	 * Returns the least common multiple of a and b.
	 */
	int LeastCommonMultiple(int a, int b)
	{
		int gcf = GreatestCommonFactor(a, b);
		return a * b / gcf;
	}

}}
