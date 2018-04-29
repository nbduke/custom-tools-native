#pragma once

#include <ppltasks.h>


namespace CommonTools_Native { namespace Parallel {

	class IPipelineStage
	{
	public:
		virtual ~IPipelineStage() { }

		virtual int stageId() const = 0;
		virtual bool isActive() = 0;
		virtual bool isFlushing() = 0;
		virtual void activate() = 0;
		virtual concurrency::task<void> deactivate() = 0;
		virtual concurrency::task<void> flushOne() = 0;
		virtual concurrency::task<void> flushAll() = 0;
	};

}}
