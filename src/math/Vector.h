#pragma once

#include "Point.h"

#include <functional>
#include <iostream>
#include <string>


namespace CommonTools_Native { namespace Math {

	template <unsigned int DIM>
	class Vector
	{
	public:
		friend std::ostream& operator<<(std::ostream& out, const Vector<DIM>& vector)
		{
			out << vector.toString();
			return out;
		}

#pragma region Constructors and Destructor

		Vector()
			: m_data(new double[DIM])
		{
		}

		Vector(double fillValue)
			: Vector<DIM>()
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				m_data[i] = fillValue;
			}
		}

		Vector(const double values[DIM])
			: Vector<DIM>()
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				m_data[i] = values[i];
			}
		}

		Vector(const std::initializer_list<double>& values)
			: Vector<DIM>()
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

		Vector(const Point<DIM>& endPoint)
			: Vector<DIM>()
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				m_data[i] = endPoint[i];
			}
		}

		Vector(const Point<DIM>& startPoint, const Point<DIM>& endPoint)
			: Vector<DIM>()
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				m_data[i] = endPoint[i] - startPoint[i];
			}
		}

		Vector(const Vector<DIM>& other)
			: Vector<DIM>()
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				m_data[i] = other.m_data[i];
			}
		}

		Vector(Vector<DIM>&& other)
			: m_data(other.m_data)
		{
			other.m_data = nullptr;
		}

		virtual ~Vector()
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

		virtual double magnitude() const
		{
			double sumSqrs = 0;
			aggregateComponents(
				[&sumSqrs]
				(unsigned int, double value)
				{
					sumSqrs += value * value;
				});

			return sqrt(sumSqrs);
		}

		virtual Vector<DIM> unitVector() const
		{
			double mag = magnitude();
			Vector<DIM> result;

			aggregateComponents(
				[&mag, &result]
				(unsigned int index, double value)
				{
					result.m_data[index] = value / mag;
				});

			return result;
		}

		virtual bool isOrthogonalTo(const Vector<DIM>& other) const
		{
			return operator*(other) == 0;
		}

		virtual bool isParallelTo(const Vector<DIM>& other) const
		{
			return unitVector() == other.unitVector();
		}

		virtual Point<DIM> getPoint() const
		{
			return Point<DIM>(m_data);
		}

		virtual std::string toString() const
		{
			std::ostringstream output;

			output << "<";
			for (unsigned int i = 0; i < DIM - 1; ++i)
			{
				output << m_data[i] << ", ";
			}
			output << m_data[DIM - 1] << ">";

			return output.str();
		}

#pragma endregion

#pragma region Operators

		Vector<DIM>& operator=(const Vector<DIM>& other)
		{
			Vector<DIM> copy(other);
			std::swap(m_data, copy.m_data);
			return *this;
		}

		Vector<DIM>& operator=(Vector<DIM>&& other)
		{
			if (this != &other)
			{
				Vector<DIM> moved(std::move(other));
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

		virtual Vector<DIM> operator+(const Vector<DIM>& other) const
		{
			Vector<DIM> result;

			aggregateComponents(
				[&result, &other]
				(unsigned int index, double value)
				{
					result.m_data[index] = value + other[index];
				});

			return result;
		}

		virtual Vector<DIM> operator-(const Vector<DIM>& other) const
		{
			Vector<DIM> result;

			aggregateComponents(
				[&result, &other]
				(unsigned int index, double value)
				{
					result.m_data[index] = value - other[index];
				});

			return result;
		}

		virtual Vector<DIM> operator-() const
		{
			return operator*(-1);
		}

		virtual double operator*(const Vector<DIM>& other) const
		{
			double dotProduct = 0;

			aggregateComponents(
				[&dotProduct, &other]
				(unsigned int index, double value)
				{
					dotProduct += value * other.m_data[index];
				});

			return dotProduct;
		}

		virtual Vector<DIM> operator*(double scalar) const
		{
			Vector<DIM> result;

			aggregateComponents(
				[&result, &scalar]
				(unsigned int index, double value)
				{
					result.m_data[index] = value * scalar;
				});

			return result;
		}

		virtual Vector<DIM> operator/(double scalar) const
		{
			Vector<DIM> result;

			aggregateComponents(
				[&result, &scalar]
				(unsigned int index, double value)
				{
					result.m_data[index] = value / scalar;
				});

			return result;
		}

		virtual bool operator==(const Vector<DIM>& other) const
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				if (m_data[i] != other.m_data[i])
					return false;
			}

			return true;
		}

		virtual bool operator!=(const Vector<DIM>& other) const
		{
			return !operator==(other);
		}

#pragma endregion

	private:
		double* m_data;

		void aggregateComponents(const std::function<void(unsigned int, double)>& action) const
		{
			for (unsigned int i = 0; i < DIM; ++i)
			{
				action(i, m_data[i]);
			}
		}

		static void validateIndexElseCrash(unsigned int index)
		{
			if (index >= DIM)
			{
				throw std::invalid_argument("index out of range.");
			}
		}
	};

}}
