#pragma once

#include "IPipelineStage.h"


namespace CommonTools_Native { namespace Parallel {

	template<class T>
	class IConsumerStage : public IPipelineStage
	{
	public:
		virtual ~IConsumerStage() { }

		virtual bool hasInputs() const = 0;
		virtual void addInput(T& input) = 0;
	};

}}
