namespace Tools { namespace Parallel {

	template<class Input, class Output>
	PipelineStage<Input, Output>::PipelineStage(
		int stageId,
		const std::function<Output(Input&)>& processInputFunction)
		: PipelineStage<Input, Output>(
			stageId,
			processInputFunction,
			nullptr /*handleErrorFunction*/)
	{
	}

	template<class Input, class Output>
	PipelineStage<Input, Output>::PipelineStage(
		int stageId,
		const std::function<Output(Input&)>& processInputFunction,
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

	template<class Input, class Output>
	concurrency::task<void> PipelineStage<Input, Output>::flushAll()
	{
		auto flushOneTask = flushOne();
		std::weak_ptr<PipelineStage<Input, Output>> wpThis(shared_from_this());

		auto flushAllTask = flushOneTask.then([wpThis]()
		{
			auto spThis = wpThis.lock();
			if (spThis != nullptr)
			{
				return spThis->flushConsumers();
			}
			else
			{
				return concurrency::task_from_result();
			}
		});

		return flushAllTask;
	}

	template<class Input, class Output>
	concurrency::task<void> PipelineStage<Input, Output>::flushConsumers()
	{
		std::vector<concurrency::task<void>> flushConsumersTasks;
		concurrency::reader_writer_lock::scoped_lock_read readerLock(m_consumersLock);

		for (auto iter = m_consumers.begin(); iter != m_consumers.end(); ++iter)
		{
			auto& consumer = iter->second;
			flushConsumersTasks.push_back(consumer->flushAll());
		}

		return concurrency::when_all(flushConsumersTasks.begin(), flushConsumersTasks.end());
	}

	template<class Input, class Output>
	void PipelineStage<Input, Output>::connect(const std::shared_ptr<IConsumerStage<Output>>& consumer)
	{
		if (consumer == nullptr)
		{
			throw std::invalid_argument("Invalid consumer.");
		}

		concurrency::reader_writer_lock::scoped_lock writerLock(m_consumersLock);
		unsigned int id = consumer->stageId();
		if (m_consumers.count(id) == 0)
		{
			m_consumers[id] = consumer;
		}
	}

	template<class Input, class Output>
	void PipelineStage<Input, Output>::disconnect(const std::shared_ptr<IConsumerStage<Output>>& consumer)
	{
		if (consumer == nullptr)
		{
			throw std::invalid_argument("Invalid consumer.");
		}

		concurrency::reader_writer_lock::scoped_lock writerLock(m_consumersLock);
		m_consumers.erase(consumer->stageId());
	}

	template<class Input, class Output>
	void PipelineStage<Input, Output>::disconnectAll()
	{
		concurrency::reader_writer_lock::scoped_lock writerLock(m_consumersLock);
		m_consumers.clear();
	}

	template<class Input, class Output>
	void PipelineStage<Input, Output>::swap(
		const std::shared_ptr<IConsumerStage<Output>>& current,
		const std::shared_ptr<IConsumerStage<Output>>& replacement)
	{
		if (current == nullptr || replacement == nullptr)
		{
			throw std::invalid_argument("Invalid consumer.");
		}

		concurrency::reader_writer_lock::scoped_lock writerLock(m_consumersLock);
		int currentId = current->stageId();
		int replacementId = replacement->stageId();

		if (m_consumers.count(currentId) > 0)
		{
			if (currentId == replacementId)
			{
				m_consumers[currentId] = replacement;
			}
			else if (m_consumers.count(replacementId) == 0)
			{
				m_consumers.erase(currentId);
				m_consumers[replacementId] = replacement;
			}
		}
	}

	template<class Input, class Output>
	void PipelineStage<Input, Output>::processInput(Input& input)
	{
		Output output = m_processInput(input);

		concurrency::reader_writer_lock::scoped_lock_read readerLock(m_consumersLock);
		for (auto iter = m_consumers.begin(); iter != m_consumers.end(); ++iter)
		{
			// Pass output to each of the consumers
			auto& consumer = iter->second;
			consumer->addInput(output);
		}
	}

}}
