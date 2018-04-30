#include "stdafx.h"

#include "..\Rational.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Tools::Math;

using namespace std;

const int c_anyPrime = 5;
const int c_anyLargerPrime = 11;


namespace Test
{
	TEST_CLASS(RationalUnitTests)
	{
#pragma region constructor

		TEST_METHOD(constructor_WithNoArguments_SetsNumeratorToZeroAndDenominatorToOne)
		{
			// Act
			Rational r;

			// Assert
			AssertMembersAreEqual(0, 1, r);
		}

		TEST_METHOD(constructor_WithAnyPositiveNumerator_SetsNumeratorToGivenValueAndDenominatorToOne)
		{
			// Arrange
			const int numerator = 24;

			// Act
			Rational r(numerator);

			// Assert
			AssertMembersAreEqual(numerator, 1, r);
		}

		TEST_METHOD(constructor_WithAnyNegativeNumerator_SetsNumeratorToGivenValueAndDenominatorToOne)
		{
			// Arrange
			const int numerator = -1;

			// Act
			Rational r(numerator);

			// Assert
			AssertMembersAreEqual(numerator, 1, r);
		}

		TEST_METHOD(constructor_WithZero_SetsNumeratorToZeroAndDenominatorToOne)
		{
			// Act
			Rational r(0);

			// Assert
			AssertMembersAreEqual(0, 1, r);
		}

		TEST_METHOD(constructor_WithAnyNumeratorAndZeroDenominator_ThrowsInvalidArgumentException)
		{
			// Arrange
			const int anyNumerator = -10;

			// Act
			auto action = [=]()
			{
				Rational r(anyNumerator, 0);
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(constructor_WithZeroNumeratorAndPositiveDenominator_SetsNumeratorToZeroAndDenominatorToGivenValue)
		{
			// Arrange
			const int denominator = 9;

			// Act
			Rational r(0, denominator);

			// Assert
			AssertMembersAreEqual(0, denominator, r);
		}

		TEST_METHOD(constructor_WithZeroNumeratorAndNegativeDenominator_SetsNumeratorToZeroAndDenominatorToAbsoluteGivenValue)
		{
			// Arrange
			const int denominator = -237;

			// Act
			Rational r(0, denominator);

			// Assert
			AssertMembersAreEqual(0, -denominator, r);
		}

		TEST_METHOD(constructor_WithPositiveNumeratorAndPositiveDenominator_SetsNumeratorAndDenominatorToGivenValues)
		{
			// Arrange
			const int numerator = 2;
			const int denominator = 3;

			// Act
			Rational r(numerator, denominator);

			// Assert
			AssertMembersAreEqual(numerator, denominator, r);
		}

		TEST_METHOD(constructor_WithPositiveNumeratorAndNegativeDenominator_SetsNumeratorToNegatedValueAndDenominatorToAbsoluteValue)
		{
			// Arrange
			const int numerator = 3;
			const int denominator = -4;

			// Act
			Rational r(numerator, denominator);

			// Assert
			AssertMembersAreEqual(-numerator, -denominator, r);
		}

		TEST_METHOD(constructor_WithNegativeNumeratorAndPositiveDenominator_SetsNumeratorAndDenominatorToGivenValues)
		{
			// Arrange
			const int numerator = -5;
			const int denominator = 2;

			// Act
			Rational r(numerator, denominator);

			// Assert
			AssertMembersAreEqual(numerator, denominator, r);
		}

		TEST_METHOD(constructor_WithNegativeNumeratorAndNegativeDenominator_SetsNumeratorAndDenominatorToAbsoluteGivenValues)
		{
			// Arrange
			const int numerator = -5;
			const int denominator = -3;

			// Act
			Rational r(numerator, denominator);

			// Assert
			AssertMembersAreEqual(-numerator, -denominator, r);
		}

		TEST_METHOD(constructor_NumeratorAndDenominatorAreTheSame_SetsNumeratorAndDenominatorToGivenValues)
		{
			// Arrange
			const int numerator = 300;
			const int denominator = numerator;

			// Act
			Rational r(numerator, denominator);

			// Assert
			AssertMembersAreEqual(numerator, denominator, r);
		}

#pragma endregion

#pragma region p

		TEST_METHOD(p_AnyRational_ReturnsNumerator)
		{
			// Arrange
			const int expectedNumerator = 6;
			const int anyDenominator = 5;
			Rational r(expectedNumerator, anyDenominator);

			// Act
			int actualNumerator = r.p();

			// Assert
			Assert::AreEqual(expectedNumerator, actualNumerator, L"The expected and actual numerators must match.");
		}

#pragma endregion

#pragma region q

		TEST_METHOD(q_AnyRational_ReturnsDenominator)
		{
			// Arrange
			const int anyNumerator = -4;
			const int expectedDenominator = 70;
			Rational r(anyNumerator, expectedDenominator);

			// Act
			int actualDenominator = r.q();

			// Assert
			Assert::AreEqual(expectedDenominator, actualDenominator, L"The expected and actual denominators must match.");
		}

#pragma endregion

#pragma region getInverted

		TEST_METHOD(getInverted_AnyPositiveRational_ReturnsRationalWithNumeratorAndDenominatorSwapped)
		{
			// Arrange
			auto r = AnyPositiveRational();

			// Act
			auto inverted = r.getInverted();

			// Assert
			AssertMembersAreEqual(r.q(), r.p(), inverted);
		}

		TEST_METHOD(getInverted_AnyNegativeRational_ReturnsRationalWithNumeratorAndDenominatorSwappedExceptSignStaysWithNumerator)
		{
			// Arrange
			auto r = AnyNegativeRational();

			// Act
			auto inverted = r.getInverted();

			// Assert
			AssertMembersAreEqual(-r.q(), -r.p(), inverted);
		}

		TEST_METHOD(getInverted_RationalIsZero_ThrowsInvalidArgumentException)
		{
			// Arrange
			Rational r;

			// Act
			auto action = [&r]()
			{
				auto inverted = r.getInverted();
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

#pragma endregion

#pragma region getReduced

		void getReduced_AnyRational_ResultMatchesExpected(const Rational& r, const Rational& expected)
		{
			// Act
			Rational actual = r.getReduced();

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(getReduced_NumeratorIsZeroAndAnyDenominator_ReturnsRationalWithNumeratorZeroAndDenominatorOne)
		{
			// Arrange
			const int anyDenominator = 13;
			Rational r(0, anyDenominator);
			Rational expected;

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_RationalIsIrreducible_ReturnsSameRational)
		{
			// Arrange
			Rational r(2, 25);
			Rational expected = r;

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_NumeratorAndDenominatorAreTheSame_ReturnsRationalWithNumeratorAndDenominatorSetToOne)
		{
			// Arange
			Rational r(50, 50);
			Rational expected(1, 1);

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_NumeratorIsNegatedDenominator_ReturnsRationalWithNumeratorNegativeOneAndDenominatorOne)
		{
			// Arrange
			Rational r(-3, 3);
			Rational expected(-1, 1);

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_NumeratorIsMultipleOfDenominator_ReturnsRationalWithNumeratorSetToMultipleFactorAndDenominatorOne)
		{
			// Arrange
			const int anyDenominator = 4;
			const int numerator = anyDenominator * 12;
			Rational r(numerator, anyDenominator);
			Rational expected(numerator / anyDenominator, 1);

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_DenominatorIsMultipleOfNumerator_ReturnsRationalWithNumeratorOneAndDenominatorSetToMultipleFactor)
		{
			// Arrange
			const int anyNumerator = 5;
			const int denominator = anyNumerator * -7;
			Rational r(anyNumerator, denominator);
			Rational expected(1, denominator / anyNumerator);

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_NumeratorAndDenominatorHavePositiveGreatestCommonFactorAndNumeratorIsLarger_ReturnsRationalWithNumeratorAndDenominatorDividedByGreatestCommonFactor)
		{
			// Arrange
			const int anyGreatestCommonFactor = 4;
			const int numerator = anyGreatestCommonFactor * c_anyLargerPrime;
			const int denominator = anyGreatestCommonFactor * c_anyPrime;
			Rational r(numerator, denominator);
			Rational expected(numerator / anyGreatestCommonFactor, denominator / anyGreatestCommonFactor);

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_NumeratorAndDenominatorHavePositiveGreatestCommonFactorAndNumeratorIsSmaller_ReturnsRationalWithNumeratorAndDenominatorDividedByGreatestCommonFactor)
		{
			// Arrange
			const int anyGreatestCommonFactor = 2;
			const int numerator = anyGreatestCommonFactor * c_anyPrime;
			const int denominator = anyGreatestCommonFactor * c_anyLargerPrime;
			Rational r(numerator, denominator);
			Rational expected(numerator / anyGreatestCommonFactor, denominator / anyGreatestCommonFactor);

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_NumeratorAndDenominatorHaveNegativeGreatestCommonFactorAndNumeratorIsLarger_ReturnsRationalWithNumeratorAndDenominatorDividedByGreatestCommonFactor)
		{
			// Arrange
			const int anyGreatestCommonFactor = -5;
			const int numerator = -anyGreatestCommonFactor * c_anyLargerPrime;
			const int denominator = anyGreatestCommonFactor * c_anyPrime;
			Rational r(numerator, denominator);
			Rational expected(numerator / anyGreatestCommonFactor, denominator / anyGreatestCommonFactor);

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

		TEST_METHOD(getReduced_NumeratorAndDenominatorHaveNegativeGreatestCommonFactorAndNumeratorIsSmaller_ReturnsRationalWithNumeratorAndDenominatorDividedByGreatestCommonFactor)
		{
			// Arrange
			const int anyGreatestCommonFactor = -9;
			const int numerator = -anyGreatestCommonFactor * c_anyPrime;
			const int denominator = anyGreatestCommonFactor * c_anyLargerPrime;
			Rational r(numerator, denominator);
			Rational expected(numerator / anyGreatestCommonFactor, denominator / anyGreatestCommonFactor);

			// Act & Assert
			getReduced_AnyRational_ResultMatchesExpected(r, expected);
		}

#pragma endregion

#pragma region getValue

		TEST_METHOD(getValue_AnyRational_ReturnsNumeratorDividedByDenominatorAsADouble)
		{
			// Arrange
			const int anyNumerator = 33;
			const int anyDenominator = -48;
			Rational r(anyNumerator, anyDenominator);
			double expectedValue = anyNumerator / static_cast<double>(anyDenominator);

			// Act
			double actualValue = r.getValue();

			// Assert
			Assert::AreEqual(expectedValue, actualValue, L"getValue must return the expected quotient.");
		}

#pragma endregion

#pragma region operator=

		TEST_METHOD(assignmentOperatorToInt_AnyRationalAndAnyInteger_UpdatesRationalWithNumeratorSetToGivenValueAndDenominatorOne)
		{
			// Arrange
			auto anyRational = AnyRational();
			const int anyValue = 99;

			// Act
			anyRational = anyValue;

			// Assert
			AssertMembersAreEqual(anyValue, 1, anyRational);
		}

#pragma endregion

#pragma region operator+

		TEST_METHOD(additionOperator_AnyRationalAndAnyDouble_ReturnsEvaluatedRationalPlusGivenValue)
		{
			// Arrange
			auto anyRational = AnyRational();
			const double anyValue = 3.1416;
			double expectedValue = anyValue + anyRational.getValue();

			// Act
			double actualValue = anyRational + anyValue;

			// Assert
			Assert::AreEqual(expectedValue, actualValue);
		}

		TEST_METHOD(additionOperator_AnyRationalAndAnyInteger_ReturnsRationalRepresentingSumOfRationalAndGivenValue)
		{
			// Arrange
			auto anyRational = AnyRational();
			const int anyValue = 2;
			Rational expected(anyRational.p() + anyValue * anyRational.q(), anyRational.q());

			// Act
			Rational actual = anyRational + anyValue;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(additionOperator_WithTwoRationalsWithSameDenominator_ReturnsRationalRepresentingSumOfRationals)
		{
			// Arrange
			const int anyDenominator = 3;
			const int numeratorA = 2;
			const int numeratorB = 5;
			Rational lhs(numeratorA, anyDenominator);
			Rational rhs(numeratorB, anyDenominator);
			Rational expected(numeratorA + numeratorB, anyDenominator);

			// Act
			Rational actual = lhs + rhs;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(additionOperator_WithTwoRationalsWithDifferentDenominators_ReturnsRationalRepresentingSumOfRationals)
		{
			// Arrange
			const int numeratorA = 2;
			const int denominatorA = 3;
			const int numeratorB = 5;
			const int denominatorB = 4;
			Rational lhs(numeratorA, denominatorA);
			Rational rhs(numeratorB, denominatorB);
			Rational expected(numeratorA * denominatorB + numeratorB * denominatorA, denominatorA * denominatorB);

			// Act
			Rational actual = lhs + rhs;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(additionOperator_AnyTwoRationals_OperatorIsCommutative)
		{
			// Arrange
			auto lhs = AnyRational();
			auto rhs = AnyOtherRational();

			// Act
			auto sumA = lhs + rhs;
			auto sumB = rhs + lhs;

			// Assert
			AssertRationalsAreEqual(sumA, sumB);
		}

#pragma endregion

#pragma region operator-

		TEST_METHOD(subtractionOperator_AnyRationalAndNoArgument_ReturnsRationalWithNegatedNumerator)
		{
			// Arrange
			const int anyNumerator = -4;
			const int anyDenominator = 16;
			Rational r(anyNumerator, anyDenominator);
			Rational expected(-anyNumerator, anyDenominator);

			// Act
			Rational actual = -r;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(subtractionOperator_AnyRationalAndAnyDouble_ReturnsEvaluatedRationalMinusGivenValue)
		{
			// Arrange
			auto anyRational = AnyRational();
			const double anyValue = 2.718;
			double expectedValue = anyRational.getValue() - anyValue;

			// Act
			double actualValue = anyRational - anyValue;

			// Assert
			Assert::AreEqual(expectedValue, actualValue);
		}

		TEST_METHOD(subtractionOperator_AnyRationalAndAnyInteger_ReturnsRationalRepresentingDifferenceOfRationalAndGivenValue)
		{
			// Arrange
			auto anyRational = AnyRational();
			const int anyValue = 11;
			Rational expected(anyRational.p() - anyValue * anyRational.q(), anyRational.q());

			// Act
			Rational actual = anyRational - anyValue;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(subtractionOperator_WithTwoRationalsWithSameDenominator_ReturnsRationalRepresentingDifferenceOfRationals)
		{
			// Arrange
			const int anyDenominator = 30;
			const int numeratorA = 2;
			const int numeratorB = 50;
			Rational lhs(numeratorA, anyDenominator);
			Rational rhs(numeratorB, anyDenominator);
			Rational expected(numeratorA - numeratorB, anyDenominator);

			// Act
			Rational actual = lhs - rhs;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(subtractionOperator_WithTwoRationalsWithDifferentDenominators_ReturnsRationalRepresentingDifferenceOfRationals)
		{
			// Arrange
			const int numeratorA = 13;
			const int denominatorA = 3;
			const int numeratorB = 45;
			const int denominatorB = 54;
			Rational lhs(numeratorA, denominatorA);
			Rational rhs(numeratorB, denominatorB);
			Rational expected(numeratorA * denominatorB - numeratorB * denominatorA, denominatorA * denominatorB);

			// Act
			Rational actual = lhs - rhs;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(subtractionOperator_AnyTwoRationals_OperatorIsAntiCommutative)
		{
			// Arrange
			auto lhs = AnyRational();
			auto rhs = AnyOtherRational();

			// Act
			auto diffA = lhs - rhs;
			auto diffB = rhs - lhs;

			// Assert
			AssertRationalsAreEqual(diffA, -diffB);
		}

#pragma endregion

#pragma region operator*

		TEST_METHOD(multiplicationOperator_AnyRationalAndAnyDouble_ReturnsEvaluatedRationalTimesGivenValue)
		{
			// Arrange
			auto anyRational = AnyRational();
			const double anyValue = -8.5;
			double expectedValue = anyRational.getValue() * anyValue;

			// Act
			double actualValue = anyRational * anyValue;

			// Assert
			Assert::AreEqual(expectedValue, actualValue);
		}

		TEST_METHOD(multiplicationOperator_AnyRationalAndAnyInteger_ReturnsRationalRepresentingProductOfRationalAndGivenValue)
		{
			// Arrange
			auto anyRational = AnyRational();
			const int anyValue = 2;
			Rational expected(anyRational.p() * anyValue, anyRational.q());

			// Act
			auto actual = anyRational * anyValue;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(multiplicationOperator_AnyTwoRationals_ReturnsRationalRepresentingProductOfRationals)
		{
			// Arrange
			auto lhs = AnyRational();
			auto rhs = AnyOtherRational();
			Rational expected(lhs.p() * rhs.p(), lhs.q() * rhs.q());

			// Act
			auto actual = lhs * rhs;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(multiplicationOperator_AnyTwoRationals_OperatorIsCommutative)
		{
			// Arrange
			auto lhs = AnyRational();
			auto rhs = AnyOtherRational();

			// Act
			auto productA = lhs * rhs;
			auto productB = rhs * lhs;

			// Assert
			AssertRationalsAreEqual(productA, productB);
		}

#pragma endregion

#pragma region operator/

		TEST_METHOD(divisionOperator_AnyRationalAndAnyNonzeroDouble_ReturnsEvaluatedRationalDividedByGivenValue)
		{
			// Arrange
			auto anyRational = AnyRational();
			const double anyValue = 1.111111;
			double expectedValue = anyRational.getValue() / anyValue;

			// Act
			auto actualValue = anyRational / anyValue;

			// Assert
			Assert::AreEqual(expectedValue, actualValue);
		}

		TEST_METHOD(divisionOperator_AnyRationalAndAnyNonzeroInteger_ReturnsRationalRepresentingQuotientOfRationalAndGivenValue)
		{
			// Arrange
			auto anyRational = AnyRational();
			const int anyValue = 350;
			Rational expected(anyRational.p(), anyRational.q() * anyValue);

			// Act
			auto actual = anyRational / anyValue;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(divisionOperator_AnyRationalAndZero_ThrowsInvalidArgumentException)
		{
			// Arrange
			auto anyRational = AnyRational();

			// Act
			auto action = [anyRational]()
			{
				auto quotient = anyRational / 0;
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(divisionOperator_AnyNonzeroRationals_ReturnsRationalRepresentingQuotientOfRationals)
		{
			// Arrange
			auto lhs = AnyRational();
			auto rhs = AnyOtherRational();
			Rational expected(lhs.p() * rhs.q(), rhs.p() * lhs.q());

			// Act
			auto actual = lhs / rhs;

			// Assert
			AssertRationalsAreEqual(expected, actual);
		}

		TEST_METHOD(divisionOperator_RightHandSideIsRationalEqualToZero_ThrowsInvalidArgumentException)
		{
			// Arrange
			auto lhs = AnyRational();
			Rational rhs;

			// Act
			auto action = [&]()
			{
				auto quotient = lhs / rhs;
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(divisionOperator_AnyNonzeroRationals_ReversingOperandsProducesInverseResult)
		{
			// Arrange
			auto lhs = AnyRational();
			auto rhs = AnyOtherRational();

			// Act
			auto quotientA = lhs / rhs;
			auto quotientB = rhs / lhs;

			// Assert
			AssertRationalsAreEqual(quotientA, quotientB.getInverted());
		}

#pragma endregion

#pragma region operator==

		TEST_METHOD(EqualityOperator_WithDoubleThatEqualsEvaluatedRational_ReturnsTrue)
		{
			// Arrange
			const int numerator = 1;
			const int denominator = 2;
			Rational r(numerator, denominator);
			const double value = 0.5;

			// Act
			bool result = (r == value);

			// Assert
			Assert::IsTrue(result);
		}

		TEST_METHOD(EqualityOperator_WithDoubleThatDoesNotEqualEvaluatedRational_ReturnsFalse)
		{
			// Arrange
			const int numerator = 2;
			const int denominator = 3;
			Rational r(numerator, denominator);
			const double value = 0.667;

			// Act
			bool result = (r == value);

			// Assert
			Assert::IsFalse(result);
		}

		TEST_METHOD(EqualityOperator_WithIntegerThatEqualsEvaluatedRational_ReturnsTrue)
		{
			// Arrange
			const int value = 1;
			const int anyDenominator = 3;
			const int numerator = anyDenominator * value;
			Rational r(numerator, anyDenominator);

			// Act
			bool result = (r == value);

			// Assert
			Assert::IsTrue(result);
		}

		TEST_METHOD(EqualityOperator_WithIntegerThatDoesNotEqualEvaluatedRational_ReturnsFalse)
		{
			// Arrange
			const int numerator = -4;
			const int denominator = 2;
			const int value = 1;
			Rational r(numerator, denominator);

			// Act
			bool result = (r == value);

			// Assert
			Assert::IsFalse(result);
		}

		TEST_METHOD(EqualityOperator_WithRationalWithSameNumeratorAndDenominator_ReturnsTrue)
		{
			// Arrange
			const int anyNumerator = 23;
			const int anyDenominator = 26;
			Rational lhs(anyNumerator, anyDenominator);
			Rational rhs(anyNumerator, anyDenominator);

			// Act
			bool result = (lhs == rhs);

			// Assert
			Assert::IsTrue(result);
		}

		TEST_METHOD(EqualityOperator_WithRationalThatEvaluatesToSameValue_ReturnsTrue)
		{
			// Arrange
			const int anyNumerator = 2;
			const int anyDenominator = 80;
			const int multiplier = 4;
			Rational lhs(anyNumerator, anyDenominator);
			Rational rhs(anyNumerator * multiplier, anyDenominator * multiplier);

			// Act
			bool result = (lhs == rhs);

			// Assert
			Assert::IsTrue(result);
		}

		TEST_METHOD(EqualityOperator_WithRationalThatEvaluatesToDifferentValue_ReturnsFalse)
		{
			// Arrange
			const int numeratorA = 10;
			const int denominatorA = 3;
			const int numeratorB = 21;
			const int denominatorB = 7;
			Rational lhs(numeratorA, denominatorA);
			Rational rhs(numeratorB, denominatorB);

			// Act
			bool result = (lhs == rhs);

			// Assert
			Assert::IsFalse(result);
		}

#pragma endregion

	private:
#pragma region Test language

		void AssertRationalsAreEqual(const Rational& expected, const Rational& actual)
		{
			AssertMembersAreEqual(expected.p(), expected.q(), actual);
		}

		void AssertMembersAreEqual(int expectedNumerator, int expectedDenominator, const Rational& actual)
		{
			Assert::AreEqual(expectedNumerator, actual.p());
			Assert::AreEqual(expectedDenominator, actual.q());
		}

		Rational AnyRational()
		{
			return Rational(123, 456);
		}

		Rational AnyOtherRational()
		{
			return Rational(-22, 77);
		}

		Rational AnyPositiveRational()
		{
			return Rational(2, 6);
		}

		Rational AnyNegativeRational()
		{
			return Rational(14, -13);
		}

#pragma endregion
	};
}