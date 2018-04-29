#pragma once

#include <iostream>
#include <string>


namespace CommonTools_Native { namespace Math {

	template <unsigned int DIM>
	class Vector;

	template <unsigned int DIM>
	class Point
	{
	public:
		friend std::ostream& operator<<(std::ostream& out, const Point<DIM>& point)
		{
			out << point.toString();
			return out;
		}

#pragma region Constructors and Destructor

		Point()
			: m_data(new double[DIM])
		{
		}

		Point(double fillValue)
			: Point<DIM>()
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				m_data[i] = fillValue;
			}
		}

		Point(const double values[DIM])
			: Point<DIM>()
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				m_data[i] = values[i];
			}
		}

		Point(const std::initializer_list<double>& values)
			: Point<DIM>()
		{
			if (values.size() != DIM)
			{
				throw std::invalid_argument("Wrong number of values in initializer list.");
			}

			unsigned int index = 0;
			for (auto iter = std::begin(values); iter != std::end(values); ++iter)
			{
				m_data[index++] = *iter;
			}
		}

		Point(const Point<DIM>& other)
			: Point<DIM>()
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				m_data[i] = other.m_data[i];
			}
		}

		Point(Point<DIM>&& other)
			: m_data(other.m_data)
		{
			other.m_data = nullptr;
		}

		virtual ~Point()
		{
			if (m_data != nullptr)
			{
				delete[] m_data;
			}
		}

#pragma endregion

#pragma region Member methods

		unsigned int dimensions() const
		{
			return DIM;
		}

		virtual double distanceTo(const Point<DIM>& other) const
		{
			Vector<DIM> v(*this, other);
			return v.magnitude();
		}

		virtual std::string toString() const
		{
			std::ostringstream output;

			output << "(";
			for (unsigned int i = 0; i < DIM - 1; ++i)
			{
				output << m_data[i] << ", ";
			}
			output << m_data[DIM - 1] << ")";

			return output.str();
		}

#pragma endregion

#pragma region Operators

		Point<DIM>& operator=(const Point<DIM>& other)
		{
			Point<DIM> copy(other);
			std::swap(m_data, copy.m_data);
			return *this;
		}

		Point<DIM>& operator=(Point<DIM>&& other)
		{
			if (this != &other)
			{
				Point<DIM> moved(std::move(other));
				std::swap(m_data, moved.m_data);
			}

			return *this;
		}

		double& operator[](unsigned int index)
		{
			validateIndexElseCrash(index);
			return m_data[index];
		}

		double operator[](unsigned int index) const
		{
			validateIndexElseCrash(index);
			return m_data[index];
		}

		virtual Point<DIM> operator+(const Vector<DIM>& vector) const
		{
			Point<DIM> result;

			for (unsigned int i = 0; i < DIM; ++i)
			{
				result.m_data[i] = m_data[i] + vector[i];
			}

			return result;
		}

		virtual Point<DIM> operator-(const Vector<DIM>& vector) const
		{
			Point<DIM> result;

			for (unsigned int i = 0; i < DIM; ++i)
			{
				result.m_data[i] = m_data[i] - vector[i];
			}

			return result;
		}

		virtual bool operator==(const Point<DIM>& other) const
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				if (m_data[i] != other.m_data[i])
					return false;
			}

			return true;
		}

		virtual bool operator!=(const Point<DIM>& other) const
		{
			return !operator==(other);
		}

#pragma endregion

	private:
		double* m_data;

		void validateIndexElseCrash(unsigned int index) const
		{
			if (index >= DIM)
			{
				throw std::invalid_argument("index out of range.");
			}
		}
	};

}}
