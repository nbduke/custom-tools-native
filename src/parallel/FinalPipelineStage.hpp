namespace Tools { namespace Parallel {

	template<class Input>
	PipelineStage<Input, void>::PipelineStage(
		int stageId,
		const std::function<void(Input&)>& processInputFunction)
		: PipelineStage<Input, void>(
			stageId,
			processInputFunction,
			nullptr /*handleErrorFunction*/)
	{
	}

	template<class Input>
	PipelineStage<Input, void>::PipelineStage(
		int stageId,
		const std::function<void(Input&)>& processInputFunction,
		const std::function<void(int, std::exception_ptr)>& handleErrorFunction)
		: PipelineStageBase<Input>(
			stageId,
			handleErrorFunction)
		, m_processInput(processInputFunction)
	{
		if (m_processInput == nullptr)
		{
			throw std::invalid_argument("PipelineStage requires a valid process input function.");
		}
	}

	template<class Input>
	void PipelineStage<Input, void>::processInput(Input& input)
	{
		m_processInput(input);
	}

}}
