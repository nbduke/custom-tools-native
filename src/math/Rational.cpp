#include "Rational.h"

#include <stdexcept>
#include <cmath>

#include "GreatestCommonFactor.h"

using namespace std;


namespace CommonTools_Native { namespace Math {

	Rational::Rational()
		: Rational(0)
	{
	}

	Rational::Rational(int p)
		: Rational(p, 1)
	{
	}

	Rational::Rational(int p, int q)
		: m_p(p), m_q(q)
	{
		if (m_q == 0)
		{
			throw invalid_argument("Cannot divide by zero.");
		}

		if (m_q < 0)
		{
			m_p = -m_p;
			m_q = -m_q;
		}
	}

	int Rational::p() const
	{
		return m_p;
	}

	int Rational::q() const
	{
		return m_q;
	}

	Rational Rational::getInverted() const
	{
		return Rational(m_q, m_p);
	}

	Rational Rational::getReduced() const
	{
		if (m_p == 0)
		{
			return Rational();
		}

		int gcf = GreatestCommonFactor(abs(m_p), m_q);
		int reducedP = m_p / gcf;
		int reducedQ = m_q / gcf;
		return Rational(reducedP, reducedQ);
	}

	double Rational::getValue() const
	{
		return m_p / static_cast<double>(m_q);
	}

	Rational& Rational::operator=(int x)
	{
		m_p = x;
		m_q = 1;
		return *this;
	}

	double Rational::operator+(double x) const
	{
		return getValue() + x;
	}

	Rational Rational::operator+(int x) const
	{
		int p = m_p + x * m_q;
		return Rational(p, m_q);
	}

	Rational Rational::operator+(const Rational& other) const
	{
		if (m_q == other.m_q)
		{
			return Rational(m_p + other.m_p, m_q);
		}
		else
		{
			int p = m_p * other.m_q + other.m_p * m_q;
			int q = m_q * other.m_q;
			return Rational(p, q);
		}
	}

	Rational Rational::operator-() const
	{
		return Rational(-m_p, m_q);
	}

	double Rational::operator-(double x) const
	{
		return getValue() - x;
	}

	Rational Rational::operator-(int x) const
	{
		return operator+(-x);
	}

	Rational Rational::operator-(const Rational& other) const
	{
		return operator+(-other);
	}

	double Rational::operator*(double x) const
	{
		return getValue() * x;
	}

	Rational Rational::operator*(int x) const
	{
		return Rational(m_p * x, m_q);
	}

	Rational Rational::operator*(const Rational& other) const
	{
		int p = m_p * other.m_p;
		int q = m_q * other.m_q;
		return Rational(p, q);
	}

	double Rational::operator/(double x) const
	{
		return getValue() / x;
	}

	Rational Rational::operator/(int x) const
	{
		return Rational(m_p, m_q * x);
	}

	Rational Rational::operator/(const Rational& other) const
	{
		return operator*(other.getInverted());
	}

	bool Rational::operator==(double x) const
	{
		return getValue() == x;
	}

	bool Rational::operator==(int x) const
	{
		return compare(x) == 0;
	}

	bool Rational::operator==(const Rational& other) const
	{
		return compare(other) == 0;
	}

	bool Rational::operator!=(double x) const
	{
		return getValue() != x;
	}

	bool Rational::operator!=(int x) const
	{
		return !operator==(x);
	}

	bool Rational::operator!=(const Rational& other) const
	{
		return !operator==(other);
	}

	bool Rational::operator<(double x) const
	{
		return getValue() < x;
	}

	bool Rational::operator<(int x) const
	{
		return compare(x) < 0;
	}

	bool Rational::operator<(const Rational& other) const
	{
		return compare(other) < 0;
	}

	bool Rational::operator<=(double x) const
	{
		return getValue() <= x;
	}

	bool Rational::operator<=(int x) const
	{
		return !operator>(x);
	}

	bool Rational::operator<=(const Rational& other) const
	{
		return !operator>(other);
	}

	bool Rational::operator>(double x) const
	{
		return getValue() > x;
	}

	bool Rational::operator>(int x) const
	{
		return compare(x) > 0;
	}

	bool Rational::operator>(const Rational& other) const
	{
		return compare(other) > 0;
	}

	bool Rational::operator>=(double x) const
	{
		return getValue() >= x;
	}

	bool Rational::operator>=(int x) const
	{
		return !operator<(x);
	}

	bool Rational::operator>=(const Rational& other) const
	{
		return !operator<(other);
	}

	int Rational::compare(int x) const
	{
		return compare(m_p, m_q * x);
	}

	int Rational::compare(const Rational& other) const
	{
		int lhs = m_p * other.m_q;
		int rhs = other.m_p * m_q;
		return compare(lhs, rhs);
	}

	/*static*/ int Rational::compare(int lhs, int rhs)
	{
		if (lhs < rhs)
			return -1;
		else if (lhs > rhs)
			return 1;
		else
			return 0;
	}

	Rational::operator double() const
	{
		return getValue();
	}

}}
