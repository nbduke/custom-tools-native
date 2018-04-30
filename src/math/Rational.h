#pragma once

namespace Tools { namespace Math {

	/*
	 * Represents a rational number by storing the numerator
	 * and denominator.
	 */
	class Rational
	{
	public:
#pragma region Constructors

		Rational();
		Rational(int p);
		Rational(int p, int q);

#pragma endregion

#pragma region Member methods

		int p() const;
		int q() const;
		virtual Rational getInverted() const;
		virtual Rational getReduced() const;
		virtual double getValue() const;

#pragma endregion

#pragma region Operators

		virtual Rational& operator=(int x);

		virtual double operator+(double x) const;
		virtual Rational operator+(int x) const;
		virtual Rational operator+(const Rational& other) const;

		virtual Rational operator-() const;
		virtual double operator-(double x) const;
		virtual Rational operator-(int x) const;
		virtual Rational operator-(const Rational& other) const;

		virtual double operator*(double x) const;
		virtual Rational operator*(int x) const;
		virtual Rational operator*(const Rational& other) const;

		virtual double operator/(double x) const;
		virtual Rational operator/(int x) const;
		virtual Rational operator/(const Rational& other) const;

		virtual bool operator==(double x) const;
		virtual bool operator==(int x) const;
		virtual bool operator==(const Rational& other) const;

		virtual bool operator!=(double x) const;
		virtual bool operator!=(int x) const;
		virtual bool operator!=(const Rational& other) const;

		virtual bool operator<(double x) const;
		virtual bool operator<(int x) const;
		virtual bool operator<(const Rational& other) const;
		virtual bool operator<=(double x) const;
		virtual bool operator<=(int x) const;
		virtual bool operator<=(const Rational& other) const;

		virtual bool operator>(double x) const;
		virtual bool operator>(int x) const;
		virtual bool operator>(const Rational& other) const;
		virtual bool operator>=(double x) const;
		virtual bool operator>=(int x) const;
		virtual bool operator>=(const Rational& other) const;

		virtual operator double() const;

#pragma endregion

	private:
		int compare(int x) const;
		int compare(const Rational& other) const;
		static int compare(int lhs, int rhs);

		int m_p;
		int m_q;
	};

}}
