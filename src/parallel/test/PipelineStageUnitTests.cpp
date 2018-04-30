#include "stdafx.h"

#include "fake\FakeConsumerStage.h"
#include "..\PipelineStage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Tools::Parallel;
using namespace Fake;
using namespace std;

const int c_anyStageId = 1;
int s_anyInput = 20;


namespace Test
{
	TEST_CLASS(PipelineStageUnitTests)
	{
#pragma region constructor

		TEST_METHOD(constructor_OutputTypeIsVoidWithNullProcessInputFunction_ThrowsInvalidArgumentException)
		{
			// Act
			auto action = []()
			{
				auto stage = make_shared<PipelineStage<int, void>>(
					c_anyStageId,
					nullptr /*processInputFunction*/);
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(constructor_OutputTypeIsNonVoidWithNullProcessInputFunction_ThrowsInvalidArgumentException)
		{
			// Act
			auto action = []()
			{
				auto stage = make_shared<PipelineStage<int, double>>(
					c_anyStageId,
					nullptr /*processInputFunction*/);
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(constructor_WithNullHandleErrorFunction_Passes)
		{
			// Arrange
			auto processInput = GetStandardProcessInputFunction();
			bool testPassed = true;

			// Act
			try
			{
				auto stage = make_shared<PipelineStage<int, int>>(
					c_anyStageId,
					processInput,
					nullptr /*handleErrorFunction*/);
			}
			catch (...)
			{
				testPassed = false;
			}

			// Assert
			Assert::IsTrue(testPassed, L"An exception must not be thrown.");
		}

#pragma endregion

#pragma region stageId

		TEST_METHOD(stageId_AnyIdPassedToConstructor_ReturnsThatId)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			int stageId = stage->stageId();

			// Assert
			Assert::AreEqual(c_anyStageId, stageId, L"stageId must return the value passed to the constructor.");
		}

#pragma endregion

#pragma region isActive

		TEST_METHOD(isActive_BeforeActivate_ReturnsFalse)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			bool isActive = stage->isActive();

			// Assert
			Assert::IsFalse(isActive, L"The stage must not be active initially.");
		}

#pragma endregion

#pragma region isFlushing

		TEST_METHOD(isFlushing_BeforeFlush_ReturnsFalse)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			bool isFlushing = stage->isFlushing();

			// Assert
			Assert::IsFalse(isFlushing, L"The stage must not be flushing initially.");
		}

#pragma endregion

#pragma region hasInputs

		TEST_METHOD(hasInputs_BeforeAddInput_ReturnsFalse)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			bool hasInputs = stage->hasInputs();

			// Assert
			Assert::IsFalse(hasInputs, L"The stage must not have any buffered inputs initially.");
		}

		TEST_METHOD(hasInputs_AfterAddInput_ReturnsTrue)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->addInput(s_anyInput);

			// Act
			bool hasInputs = stage->hasInputs();

			// Assert
			Assert::IsTrue(hasInputs, L"The stage must buffer inputs passed to addInput.");
		}

#pragma endregion

#pragma region addInput

		TEST_METHOD(addInput_StageIsFlushing_DoesNotBufferTheInput)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->flushOne();

			// Act
			stage->addInput(s_anyInput);

			// Assert
			Assert::IsFalse(stage->hasInputs(), L"Inputs must not be buffered while the stage is flushing.");
		}

#pragma endregion

#pragma region activate

		TEST_METHOD(activate_AtAnyTime_isActiveReturnsTrue)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			stage->activate();

			// Assert
			Assert::IsTrue(stage->isActive(), L"The stage must be active once activate is called.");
		}

		TEST_METHOD(activate_StageWasAlreadyActive_StageRemainsActive)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->activate();

			// Act
			stage->activate();

			// Assert
			Assert::IsTrue(stage->isActive(), L"The stage must remain active if activate is called more than once.");
		}

		TEST_METHOD(activate_AfterDeactivate_isActiveReturnsTrue)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->deactivate().wait();

			// Act
			stage->activate();

			// Assert
			Assert::IsTrue(stage->isActive(), L"The stage must be able to be activated after being deactivated.");
		}

		TEST_METHOD(activate_WithBufferedInputs_BeginsProcessingInputs)
		{
			// Arrange
			int expectedInput = s_anyInput;
			auto stage = GetPipelineStage(GetValidatingProcessInputFunction(expectedInput));
			stage->addInput(expectedInput);

			// Act & Assert
			stage->activate();
			stage->flushOne().wait(); // wait for the stage to finish processing its inputs
		}

#pragma endregion

#pragma region deactivate

		TEST_METHOD(deactivate_StageIsNotActive_isActiveReturnsFalse)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			stage->deactivate();

			// Assert
			Assert::IsFalse(stage->isActive(), L"The stage must not be active after calling deactivate.");
		}

		TEST_METHOD(deactivate_TwiceInARow_stageRemainsDeactivated)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->deactivate().wait();

			// Act
			stage->deactivate();

			// Assert
			Assert::IsFalse(stage->isActive(), L"The stage must remain unactive if deactivate is called more than once.");
		}

		TEST_METHOD(deactivate_AfterActivate_isActiveReturnsFalseOnceTaskCompletes)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->activate();

			// Act
			stage->deactivate().wait();

			// Assert
			Assert::IsFalse(stage->isActive(), L"An active stage must not be active after deactivate is called and the task completes.");
		}

		TEST_METHOD(deactivate_WithManyBufferedInputs_DoesNotProcessAllInputsBeforeTaskCompletes)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			AddAnyInputs(stage, 1000);
			stage->activate();

			// Act
			stage->deactivate().wait();

			// Assert
			Assert::IsTrue(stage->hasInputs(), L"The stage should not have processed all of its inputs.");
		}

#pragma endregion

#pragma region flushOne

		TEST_METHOD(flushOne_AtAnyTime_isFlushingReturnsTrue)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			stage->flushOne();

			// Assert
			Assert::IsTrue(stage->isFlushing(), L"The stage must be flushing after flushOne is called.");
		}

		TEST_METHOD(flushOne_StageIsActive_isFlushingReturnsFalseAfterTaskCompletes)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->activate();

			// Act
			stage->flushOne().wait();

			// Assert
			Assert::IsFalse(stage->isFlushing(), L"isFlushing must return false after flushing completes.");
		}

		TEST_METHOD(flushOne_WithAnyBufferedInputs_ProcessesAllInputsBeforeTaskCompletes)
		{
			// Arrange
			vector<int> outputs;
			auto stage = GetAccumulatorStage(outputs);

			int expectedOutputsCount = 50;
			AddAnyInputs(stage, expectedOutputsCount);
			stage->activate();

			// Act
			stage->flushOne().wait();

			// Assert
			Assert::IsFalse(stage->hasInputs(), L"All inputs must be removed from the queue..");
			Assert::AreEqual(expectedOutputsCount, static_cast<int>(outputs.size()), L"All outputs must be processed.");
		}

		TEST_METHOD(flushOne_StageIsActive_isActiveReturnsFalseAfterTaskCompletes)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->activate();

			// Act
			stage->flushOne().wait();

			// Assert
			Assert::IsFalse(stage->isActive(), L"The stage must deactivate after flushing completes.");
		}

		TEST_METHOD(flushOne_StageHasConsumer_ConsumerIsNotFlushed)
		{
			// Arrange
			auto stage1 = GetStandardPipelineStage();
			auto stage2 = GetFakeStage();
			stage1->connect(stage2);

			// Act
			stage1->flushOne().wait();

			// Assert
			Assert::IsFalse(stage2->isFlushing(), L"flushOne must not flush any consumers.");
		}

#pragma endregion

#pragma region flushAll

		TEST_METHOD(flushAll_AtAnyTime_isFlushingReturnsTrue)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			stage->flushAll();

			// Assert
			Assert::IsTrue(stage->isFlushing(), L"The stage must be flushing after flushAll is called.");
		}

		TEST_METHOD(flushAll_StageIsActive_isFlushingReturnsFalseAfterStageCompletes)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->activate();

			// Act
			stage->flushAll().wait();

			// Assert
			Assert::IsFalse(stage->isFlushing(), L"isFlushing must return false after flushing completes.");
		}

		TEST_METHOD(flushAll_WithAnyBufferedInputs_ProcessesAllInputsBeforeTaskCompletes)
		{
			// Arrange
			vector<int> outputs;
			auto stage = GetAccumulatorStage(outputs);

			int expectedOutputsCount = 50;
			AddAnyInputs(stage, expectedOutputsCount);
			stage->activate();

			// Act
			stage->flushOne().wait();

			// Assert
			Assert::IsFalse(stage->hasInputs(), L"All inputs must be removed from the queue.");
			Assert::AreEqual(expectedOutputsCount, static_cast<int>(outputs.size()), L"All inputs must be processed.");
		}

		TEST_METHOD(flushAll_StageIsActive_isActiveReturnsFalseAfterTaskCompletes)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			stage->activate();

			// Act
			stage->flushAll().wait();

			// Assert
			Assert::IsFalse(stage->isActive(), L"The stage must deactivate after flushing completes.");
		}

		TEST_METHOD(flushAll_StageHasConsumer_ConsumerIsAlsoFlushed)
		{
			// Arrange
			auto stage1 = GetStandardPipelineStage();
			auto stage2 = GetFakeStage();
			stage1->connect(stage2);

			// Act
			stage1->flushAll().wait();

			// Assert
			Assert::IsTrue(stage2->isFlushing(), L"flushAll must flush all consumers.");
		}

		TEST_METHOD(flushAll_FirstStageInChain_AllOutputsReachFinalStage)
		{
			// Arrange
			auto stage1 = GetStandardPipelineStage();
			auto stage2 = GetStandardPipelineStage();
			auto end = GetFakeStage();
			stage1->connect(stage2);
			stage2->connect(end);

			int expectedOutputsCount = 100;
			AddAnyInputs(stage1, expectedOutputsCount);
			stage1->activate();
			stage2->activate();

			// Act
			stage1->flushAll().wait();

			// Assert
			Assert::AreEqual(expectedOutputsCount, static_cast<int>(end->m_inputs.size()), L"When flushAll is called on a stage, all outputs of that stage must reach the final stage.");
		}

#pragma endregion

#pragma region connect

		TEST_METHOD(connect_WithNullConsumer_ThrowsInvalidArgumentException)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			auto action = [&stage]()
			{
				stage->connect(nullptr /*consumer*/);
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(connect_ToTwoConsumersWithSameStageids_SecondConsumerNotConnected)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto anyConsumer = GetFakeStage();
			auto consumerWithSameStageId = GetFakeStage();

			stage->connect(anyConsumer);

			// Act
			stage->connect(consumerWithSameStageId);

			// Assert
			stage->addInput(s_anyInput);
			stage->activate();
			stage->flushOne().wait();

			AssertConnected(stage, anyConsumer);
			AssertNotConnected(stage, consumerWithSameStageId);
		}

		TEST_METHOD(connect_ToTwoConsumersWithDifferentStageIds_BothConsumersConnected)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto anyConsumer = GetFakeStage(1);
			auto consumerWithDifferentStageId = GetFakeStage(2);

			stage->connect(anyConsumer);

			// Act
			stage->connect(consumerWithDifferentStageId);

			// Assert
			AssertConnected(stage, anyConsumer);
			AssertConnected(stage, consumerWithDifferentStageId);
		}

#pragma endregion

#pragma region disconnect

		TEST_METHOD(disconnect_WithNullConsumer_ThrowsInvalidArgumentException)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();

			// Act
			auto action = [&stage]()
			{
				stage->disconnect(nullptr /*consumer*/);
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(disconnect_FromStageThatIsNotConnected_Passes)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto anyConsumer = GetFakeStage();
			bool testPassed = true;

			// Act
			try
			{
				stage->disconnect(anyConsumer);
			}
			catch (...)
			{
				testPassed = false;
			}

			// Assert
			Assert::IsTrue(testPassed, L"An exception must not be thrown if disconnect is called with a stage that isn't connected.");
		}

		TEST_METHOD(disconnect_FromConnectedConsumer_ConsumerNoLongerReceivesInputs)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto anyConsumer = GetFakeStage();
			stage->connect(anyConsumer);

			// Act
			stage->disconnect(anyConsumer);

			// Assert
			AssertNotConnected(stage, anyConsumer);
		}

#pragma endregion

#pragma region disconnectAll

		TEST_METHOD(disconnectAll_NoConsumers_Passes)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			bool testPassed = true;

			// Act
			try
			{
				stage->disconnectAll();
			}
			catch (...)
			{
				testPassed = false;
			}

			// Assert
			Assert::IsTrue(testPassed, L"An exception must not be thrown if disconnectAll is called when there are no consumers.");
		}

		TEST_METHOD(disconnectAll_ConsumersWereConnected_AllConsumersDisconnected)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto anyConsumer = GetFakeStage(1);
			auto anyOtherConsumer = GetFakeStage(2);
			stage->connect(anyConsumer);
			stage->connect(anyOtherConsumer);

			// Act
			stage->disconnectAll();

			// Assert
			AssertNotConnected(stage, anyConsumer);
			AssertNotConnected(stage, anyOtherConsumer);
		}

#pragma endregion

#pragma region swap

		TEST_METHOD(swap_WithNullCurrentStage_ThrowsInvalidArgumentException)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto anyConsumer = GetFakeStage();

			// Act
			auto action = [&]()
			{
				stage->swap(nullptr /*current*/, anyConsumer);
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(swap_WithNullReplacementStage_ThrowsInvalidArgumentException)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto anyConsumer = GetFakeStage();

			// Act
			auto action = [&]()
			{
				stage->swap(anyConsumer, nullptr /*replacement*/);
			};

			// Assert
			Assert::ExpectException<invalid_argument>(action);
		}

		TEST_METHOD(swap_WithCurrentStageNotConnected_ReplacementNotConnected)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto current = GetFakeStage();
			auto replacement = GetFakeStage();

			// Act
			stage->swap(current, replacement);

			// Assert
			AssertNotConnected(stage, replacement);
		}

		TEST_METHOD(swap_WithReplacementStageAlreadyConnected_CurrentStageNotDisconnected)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto current = GetFakeStage(1);
			auto replacement = GetFakeStage(3);
			stage->connect(current);
			stage->connect(replacement);

			// Act
			stage->swap(current, replacement);

			// Assert
			AssertConnected(stage, current);
		}

		TEST_METHOD(swap_WithCurrentStageConnectedAndReplacementStageNotConnected_DisconnectsCurrentStageAndConnectsReplacementStage)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto current = GetFakeStage(2);
			auto replacement = GetFakeStage(4);
			stage->connect(current);

			// Act
			stage->swap(current, replacement);

			// Assert
			AssertNotConnected(stage, current);
			AssertConnected(stage, replacement);
		}

		TEST_METHOD(swap_WithCurrentStageConnectedAndReplacementHasSameId_DisconnectsCurrentStageAndConnectsReplacementStage)
		{
			// Arrange
			auto stage = GetStandardPipelineStage();
			auto current = GetFakeStage(2);
			auto replacement = GetFakeStage(2);
			stage->connect(current);

			// Act
			stage->swap(current, replacement);

			// Assert
			AssertNotConnected(stage, current);
			AssertConnected(stage, replacement);
		}

#pragma endregion

#pragma region Error handling

		TEST_METHOD(ProcessInputFunctionThrows_WithNullHandleErrorFunction_Passes)
		{
			// Arrange
			auto anyException = exception("error");
			auto stage = GetPipelineStage(GetProcessInputFunctionThatThrows(anyException));
			bool testPassed = true;

			try
			{
				// Act
				ProcessAnyInput(stage);
			}
			catch (...)
			{
				testPassed = false;
			}

			// Assert
			Assert::IsTrue(testPassed, L"The error must not propagate out of the stage.");
		}

		TEST_METHOD(ProcessInputFunctionThrows_WithValidHandleErrorFunction_HandleErrorFunctionCalledWithExpectedArgs)
		{
			// Arrange
			int expectedStageId = c_anyStageId;
			auto expectedError = exception("AHHHH!!");
			auto processInput = GetProcessInputFunctionThatThrows(expectedError);
			auto handleError = GetValidatingHandleErrorFunction(expectedStageId, expectedError);
			auto stage = make_shared<PipelineStage<int, int>>(expectedStageId, processInput, handleError);

			// Act & Assert
			ProcessAnyInput(stage);
		}

		TEST_METHOD(ProcessInputFunctionThrows_WithHandleErrorFunctionThatAlsoThrows_Passes)
		{
			// Arrange
			auto anyException = exception("NO!!!");
			auto processInput = GetProcessInputFunctionThatThrows(anyException);
			auto handleError = [anyException](int, exception_ptr){ throw anyException; };
			auto stage = make_shared<PipelineStage<int, int>>(c_anyStageId, processInput, handleError);
			bool testPassed = true;

			// Act
			try
			{
				ProcessAnyInput(stage);
			}
			catch (...)
			{
				testPassed = false;
			}

			// Assert
			Assert::IsTrue(testPassed, L"The error must not propagate out of the stage.");
		}

#pragma endregion

	private:
#pragma region Test language

		void AssertConnected(
			const shared_ptr<IConsumerStage<int>>& stage,
			const shared_ptr<IConsumerStage<int>>& otherStage)
		{
			ProcessAnyInput(stage);
			Assert::IsTrue(otherStage->hasInputs(), L"Outputs must reach a connected stage.");
		}

		void AssertNotConnected(
			const shared_ptr<IConsumerStage<int>>& stage,
			const shared_ptr<IConsumerStage<int>>& otherStage)
		{
			ProcessAnyInput(stage);
			Assert::IsFalse(otherStage->hasInputs(), L"Outputs must not reach a stage that is not connected.");
		}

		void AddAnyInputs(const shared_ptr<IConsumerStage<int>>& stage, int inputsCount)
		{
			for (int i = 0; i < inputsCount; ++i)
			{
				stage->addInput(i);
			}
		}

		void ProcessAnyInput(const shared_ptr<IConsumerStage<int>>& stage)
		{
			stage->addInput(s_anyInput);
			stage->activate();
			stage->flushOne().wait();
		}

		function<int(int&)> GetStandardProcessInputFunction()
		{
			return [](int& x){ return x * x; };
		}

		function<int(int&)> GetValidatingProcessInputFunction(int& expectedInput)
		{
			return [expectedInput](int& actualInput)
			{
				Assert::AreEqual(expectedInput, actualInput, L"The expected and actual inputs must match.");
				return 0;
			};
		}

		function<void(int, exception_ptr)> GetValidatingHandleErrorFunction(
			int expectedStageId,
			exception& expectedError)
		{
			return [expectedStageId, expectedError]
				(int actualStageId, exception_ptr actualErrorPtr)
			{
				Assert::AreEqual(expectedStageId, actualStageId, L"The expected stage ID must be passed to the error handler.");

				try
				{
					rethrow_exception(actualErrorPtr);
				}
				catch (exception& actualError)
				{
					Assert::AreEqual(expectedError.what(), actualError.what(), L"The expected error must be passed to the error handler.");
				}
				catch (...)
				{
					Assert::Fail(L"The expected error was not passed to the error handler.");
				}
			};
		}

		function<int(int&)> GetProcessInputFunctionThatThrows(exception& e)
		{
			return [e](int&) -> int { throw e; };
		}

		shared_ptr<PipelineStage<int, int>> GetStandardPipelineStage()
		{
			return GetPipelineStage(GetStandardProcessInputFunction());
		}

		shared_ptr<PipelineStage<int, int>> GetPipelineStage(const function<int(int&)>& processInputFunction)
		{
			return make_shared<PipelineStage<int, int>>(c_anyStageId, processInputFunction);
		}

		shared_ptr<PipelineStage<int, void>> GetAccumulatorStage(vector<int>& outputs)
		{
			return make_shared<PipelineStage<int, void>>(c_anyStageId, [&outputs](int& input){ outputs.push_back(input); });
		}

		shared_ptr<FakeConsumerStage<int>> GetFakeStage()
		{
			return GetFakeStage(c_anyStageId);
		}

		shared_ptr<FakeConsumerStage<int>> GetFakeStage(int stageId)
		{
			return make_shared<FakeConsumerStage<int>>(stageId);
		}

#pragma endregion
	};
}