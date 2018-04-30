#pragma once

#include "IPipelineStage.h"


namespace Tools { namespace Parallel {

	/*
	 * Represents a pipeline stage that can consume input.
	 */
	template<class T>
	class IConsumerStage : public IPipelineStage
	{
	public:
		virtual ~IConsumerStage() { }

		virtual bool hasInputs() const = 0;
		virtual void addInput(T& input) = 0;
	};

}}
