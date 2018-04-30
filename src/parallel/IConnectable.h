#pragma once

#include "IConsumerStage.h"


namespace Tools { namespace Parallel {

	/*
	 * Represents an object that can be connected to an IConsumer.
	 */
	template<class T>
	class IConnectable
	{
	public:
		virtual ~IConnectable() { }

		virtual void connect(const std::shared_ptr<IConsumerStage<T>>& consumer) = 0;
		virtual void disconnect(const std::shared_ptr<IConsumerStage<T>>& consumer) = 0;
		virtual void disconnectAll() = 0;
		virtual void swap(
			const std::shared_ptr<IConsumerStage<T>>& current,
			const std::shared_ptr<IConsumerStage<T>>& replacement) = 0;
	};

}}
