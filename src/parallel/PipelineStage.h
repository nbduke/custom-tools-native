#pragma once

#include "PipelineStageBase.h"
#include "IConnectable.h"

#include <functional>
#include <map>


namespace Tools { namespace Parallel {

#pragma region PipelineStage<Input, Output>

	/*
	 * PipelineStage is a concurrent data structure ideal for constructing
	 * a data pipeline. Each PipelineStage is templated on an input and
	 * output type and represents one stage of execution in the pipeline.
	 */
	template<class Input, class Output>
	class PipelineStage
		: public PipelineStageBase<Input>
		, public IConnectable<Output>
		, public std::enable_shared_from_this<PipelineStage<Input, Output>>
	{
	public:
#pragma region Constructors and Destructor

		PipelineStage(
			int stageId,
			const std::function<Output(Input&)>& processInputFunction);

		PipelineStage(
			int stageId,
			const std::function<Output(Input&)>& processInputFunction,
			const std::function<void(int, std::exception_ptr)>& handleErrorFunction);

		PipelineStage(const PipelineStage<Input, Output>& other) = delete;

#pragma endregion

#pragma region PipelineStageBase overrides

		concurrency::task<void> flushAll() override;
		protected: void processInput(Input& input) override;

#pragma endregion

	public:
#pragma region IConnectable implementations

		void connect(const std::shared_ptr<IConsumerStage<Output>>& consumer) override;
		void disconnect(const std::shared_ptr<IConsumerStage<Output>>& consumer) override;
		void disconnectAll() override;
		void swap(
			const std::shared_ptr<IConsumerStage<Output>>& current,
			const std::shared_ptr<IConsumerStage<Output>>& replacement) override;

#pragma endregion

	private:
		concurrency::task<void> flushConsumers();

		std::function<Output(Input&)> m_processInput;
		std::map<int, std::shared_ptr<IConsumerStage<Output>>> m_consumers;
		concurrency::reader_writer_lock m_consumersLock;
	};

#pragma endregion

#pragma region PipelineStage<Input, void>

	/*
	 * Partial template specialization of PipelineStage with a void output type.
	 * This is known as a final pipeline stage.
	 */
	template<class Input>
	class PipelineStage<Input, void> : public PipelineStageBase<Input>
	{
	public:
		PipelineStage(
			int stageId,
			const std::function<void(Input&)>& processInputFunction);

		PipelineStage(
			int stageId,
			const std::function<void(Input&)>& processInputFunction,
			const std::function<void(int, std::exception_ptr)>& handleErrorFunction);

		PipelineStage(const PipelineStage<Input, void>& other) = delete;

	protected:
		void processInput(Input& input) override;

	private:
		std::function<void(Input&)> m_processInput;
	};

#pragma endregion

}}

#include "PipelineStage.hpp"
#include "FinalPipelineStage.hpp"
