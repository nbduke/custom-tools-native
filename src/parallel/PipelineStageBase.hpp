namespace Tools { namespace Parallel {

	const unsigned int c_waitMilliseconds = 10;

	template<class Input>
	PipelineStageBase<Input>::PipelineStageBase(
		int stageId,
		const std::function<void(int, std::exception_ptr)>& handleErrorFunction)
		: m_stageId(stageId)
		, m_handleError(handleErrorFunction)
		, m_isTaskRunning(false)
		, m_shouldTaskContinue(false)
		, m_isFlushing(false)
		, m_processInputsTask(concurrency::task_from_result())
	{
	}

	template<class Input>
	PipelineStageBase<Input>::~PipelineStageBase()
	{
		deactivate().wait();
	}

	template<class Input>
	int PipelineStageBase<Input>::stageId() const
	{
		return m_stageId;
	}

	template<class Input>
	bool PipelineStageBase<Input>::isActive()
	{
		concurrency::reader_writer_lock::scoped_lock_read readerLock(m_taskLifetimeLock);
		return isRunningOrScheduled();
	}

	template<class Input>
	bool PipelineStageBase<Input>::isFlushing()
	{
		concurrency::reader_writer_lock::scoped_lock_read readerLock(m_isFlushingLock);
		return m_isFlushing;
	}

	template<class Input>
	bool PipelineStageBase<Input>::hasInputs() const
	{
		return !m_inputQueue.empty();
	}

	template<class Input>
	bool PipelineStageBase<Input>::isRunningOrScheduled()
	{
		return m_isTaskRunning || m_shouldTaskContinue;
	}

	template<class Input>
	bool PipelineStageBase<Input>::shouldTaskContinue()
	{
		concurrency::reader_writer_lock::scoped_lock_read readerLock(m_taskLifetimeLock);
		return m_shouldTaskContinue;
	}

	template<class Input>
	void PipelineStageBase<Input>::activate()
	{
		concurrency::reader_writer_lock::scoped_lock writerLock(m_taskLifetimeLock);
		if (isRunningOrScheduled())
		{
			return;
		}

		m_shouldTaskContinue = true;
		m_processInputsTask = concurrency::create_task([this](){ processInputs(); });
	}

	template<class Input>
	concurrency::task<void> PipelineStageBase<Input>::deactivate()
	{
		concurrency::reader_writer_lock::scoped_lock writerLock(m_taskLifetimeLock);
		m_shouldTaskContinue = false;
		return m_processInputsTask;
	}

	template<class Input>
	concurrency::task<void> PipelineStageBase<Input>::flushOne()
	{
		concurrency::reader_writer_lock::scoped_lock writerLock(m_isFlushingLock);
		m_isFlushing = true;
		return m_processInputsTask;
	}

	template<class Input>
	concurrency::task<void> PipelineStageBase<Input>::flushAll()
	{
		return flushOne();
	}

	template<class Input>
	void PipelineStageBase<Input>::addInput(Input& input)
	{
		if (!isFlushing())
		{
			m_inputQueue.push(input);
		}
	}

	template<class Input>
	void PipelineStageBase<Input>::processInputs()
	{
		initializeTask();

		while (shouldTaskContinue())
		{
			try
			{
				Input input;
				if (m_inputQueue.try_pop(input))
				{
					processInput(input);
				}
				else if (isFlushing() && !hasInputs())
				{
					break;
				}
				else
				{
					concurrency::wait(c_waitMilliseconds);
				}
			}
			catch (...)
			{
				onError(std::current_exception());
			}
		}

		cleanupTask();
	}

	template<class Input>
	void PipelineStageBase<Input>::initializeTask()
	{
		setIsTaskRunning();
	}

	template<class Input>
	void PipelineStageBase<Input>::setIsTaskRunning()
	{
		concurrency::reader_writer_lock::scoped_lock writerLock(m_taskLifetimeLock);
		m_isTaskRunning = true;
	}

	template<class Input>
	void PipelineStageBase<Input>::onError(std::exception_ptr error)
	{
		try
		{
			if (m_handleError)
			{
				m_handleError(m_stageId, error);
			}
		}
		catch (...) {}
	}

	template<class Input>
	void PipelineStageBase<Input>::cleanupTask()
	{
		resetIsRunningAndShouldContinue();
		resetIsFlushing();
	}

	template<class Input>
	void PipelineStageBase<Input>::resetIsRunningAndShouldContinue()
	{
		concurrency::reader_writer_lock::scoped_lock writerLock(m_taskLifetimeLock);
		m_isTaskRunning = false;
		m_shouldTaskContinue = false;
	}

	template<class Input>
	void PipelineStageBase<Input>::resetIsFlushing()
	{
		concurrency::reader_writer_lock::scoped_lock writerLock(m_isFlushingLock);
		m_isFlushing = false;
	}

}}
