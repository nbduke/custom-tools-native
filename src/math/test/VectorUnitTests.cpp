#include "stdafx.h"

#include "..\Vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CommonTools_Native::Math;

namespace CommonTools_NativeTests
{
	TEST_CLASS(VectorUnitTests)
	{
	public:
		template<unsigned int DIM>
		void dimensions_WithAnyVector_ReturnsExpectedDimensions(
			Vector<DIM> vector, unsigned int expectedDimensions)
		{
			// Arrange

			// Act
			unsigned int actualDimensions = vector.dimensions();

			// Assert
			Assert::AreEqual(expectedDimensions, actualDimensions);
		}

		TEST_METHOD(dimensions_Vector2_Returns2)
		{
			dimensions_WithAnyVector_ReturnsExpectedDimensions(Vector<2>(), 2);
		}

		TEST_METHOD(dimensions_Vector3_Returns3)
		{
			dimensions_WithAnyVector_ReturnsExpectedDimensions(Vector<3>(), 3);
		}

		TEST_METHOD(ctor_WithFillValue_VectorContainsOnlyFillValue)
		{
			// Arrange
			double fillValue = 3.14;
			const unsigned int dimensions = 3;

			// Act
			Vector<dimensions> vector(3.14);

			// Assert
			for (unsigned int i = 0; i < dimensions; ++i)
			{
				Assert::AreEqual(fillValue, vector[i]);
			}
		}

		TEST_METHOD(ctor_WithArray_VectorContainsArrayElements)
		{
			// Arrange
			const unsigned int dimensions = 5;
			double fillValues[dimensions] = { 1.1, 2.2, 3.3, 4.4, 5.5 };

			// Act
			Vector<dimensions> vector(fillValues);

			// Assert
			for (unsigned int i = 0; i < dimensions; ++i)
			{
				Assert::AreEqual(fillValues[i], vector[i]);
			}
		}

		TEST_METHOD(copyCtor_OriginalInitializedWithArray_CopyHasSameElementsAsOriginal)
		{
			// Arrange
			const unsigned int dimensions = 5;
			double fillValues[dimensions] = { 1.1, 2.2, 3.3, 4.4, 5.5 };
			Vector<dimensions> original(fillValues);

			// Act
			Vector<dimensions> copy(original);

			// Assert
			for (unsigned int i = 0; i < dimensions; ++i)
			{
				Assert::AreEqual(original[i], copy[i]);
			}
		}


		template<unsigned int DIM>
		void magnitude_AnyVector_ReturnsExpectedMagnitude(
			Vector<DIM> vector, double expectedMagnitude)
		{
			// Arrange

			// Act
			double actualMagnitude = vector.magnitude();

			// Assert
			Assert::AreEqual(expectedMagnitude, actualMagnitude);
		}

		TEST_METHOD(magnitude_Vector3WithArbitraryValues_ReturnsExpectedMagnitude)
		{
			double fillValues[3] = { 1, 3, 5 };
			Vector<3> vector(fillValues);
			magnitude_AnyVector_ReturnsExpectedMagnitude(vector, sqrt(35.0));
		}

		TEST_METHOD(magnitude_Vector8WithAllZeros_Returns0)
		{
			Vector<8> vector(0.0);
			magnitude_AnyVector_ReturnsExpectedMagnitude(vector, 0.0);
		}

		TEST_METHOD(magnitude_Vector4FilledWithNegativeOnes_ReturnsPositiveMagnitude)
		{
			Vector<4> vector(-1.0);
			magnitude_AnyVector_ReturnsExpectedMagnitude(vector, 2.0);
		}


		template<unsigned int DIM>
		void vectorMultiply_AnyTwoVectors_ReturnsExpectedProduct(
			Vector<DIM> a, Vector<DIM> b, double expectedProduct)
		{
			// Arrange

			// Act
			double actualProduct = a * b;

			// Assert
			Assert::AreEqual(expectedProduct, actualProduct);
		}

		TEST_METHOD(vectorMultiply_ArbitraryVector3s_ReturnsExpectedProduct)
		{
			Vector<3> a(2);
			Vector<3> b(10);
			double expectedProduct = 60;
			vectorMultiply_AnyTwoVectors_ReturnsExpectedProduct(a, b, expectedProduct);
		}
	};
}