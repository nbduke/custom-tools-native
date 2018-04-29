/*
* PipelineStageBase.h
*
* PipelineStageBase is the base class of every template instantiation of
* PipelineStage. It encapsulates most of the core logic and functionality
* of a pipeline stage.
*/
#pragma once

#include "IConsumerStage.h"

#include <concrt.h>
#include <concurrent_queue.h>


namespace CommonTools_Native { namespace Parallel {

	template<class Input>
	class PipelineStageBase : public IConsumerStage<Input>
	{
	public:
#pragma region Constructors and Destructor

		PipelineStageBase(
			int stageId,
			const std::function<void(int, std::exception_ptr)>& handleErrorFunction);

		virtual ~PipelineStageBase();

		PipelineStageBase(const PipelineStageBase<Input>& other) = delete;

#pragma endregion

#pragma region IPipelineStage implementations

		int stageId() const override;
		bool isActive() override;
		bool isFlushing() override;
		virtual void activate() override;
		virtual concurrency::task<void> deactivate() override;
		virtual concurrency::task<void> flushOne() override;
		virtual concurrency::task<void> flushAll() override;

#pragma endregion

#pragma region IConsumerStage implementations

		bool hasInputs() const override;
		void addInput(Input& input) override;

#pragma endregion

	protected:
		virtual void processInput(Input& input) = 0;

	private:
		bool isRunningOrScheduled();
		bool shouldTaskContinue();
		void processInputs();
		void initializeTask();
		void setIsTaskRunning();
		void onError(std::exception_ptr error);
		void cleanupTask();
		void resetIsRunningAndShouldContinue();
		void resetIsFlushing();

		int m_stageId;
		bool m_isTaskRunning;
		bool m_shouldTaskContinue;
		bool m_isFlushing;
		std::function<void(int, std::exception_ptr)> m_handleError;
		concurrency::task<void> m_processInputsTask;
		concurrency::concurrent_queue<Input> m_inputQueue;
		concurrency::reader_writer_lock m_taskLifetimeLock;
		concurrency::reader_writer_lock m_isFlushingLock;
	};

}}

#include "PipelineStageBase.hpp"
