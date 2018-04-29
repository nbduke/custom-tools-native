#pragma once

#include "..\..\IConsumerStage.h"


namespace CommonTools_NativeTests
{
	template<class Input>
	class FakeConsumerStage : public CommonTools_Native::Parallel::IConsumerStage<Input>
	{
	public:
		FakeConsumerStage(int stageId)
			: m_stageId(stageId)
			, m_isActive(false)
			, m_isFlushingOne(false)
			, m_isFlushingAll(false)
		{
		}

		virtual int stageId() const override
		{
			return m_stageId;
		}

		virtual bool isActive() override
		{
			return m_isActive;
		}

		virtual bool isFlushing() override
		{
			return m_isFlushingOne || m_isFlushingAll;
		}

		virtual void activate() override
		{
			m_isActive = true;
		}

		virtual concurrency::task<void> deactivate() override
		{
			m_isActive = false;
			return concurrency::task_from_result();
		}

		virtual concurrency::task<void> flushOne() override
		{
			m_isFlushingOne = true;
			return concurrency::task_from_result();
		}

		virtual concurrency::task<void> flushAll() override
		{
			m_isFlushingAll = true;
			return concurrency::task_from_result();
		}

		virtual bool hasInputs() const override
		{
			return m_inputs.size() > 0;
		}

		virtual void addInput(Input& input) override
		{
			m_inputs.push_back(input);
		}


		bool m_isActive;
		bool m_isFlushingOne;
		bool m_isFlushingAll;
		std::vector<Input> m_inputs;

	private:
		int m_stageId;
	};
}
