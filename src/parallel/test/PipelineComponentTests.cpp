#include "stdafx.h"

#include "..\PipelineStage.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CommonTools_Native::Parallel;
using namespace std;

namespace CommonTools_NativeTests
{
	const int c_iterations = 1000;

	TEST_CLASS(PipelineComponentTests)
	{
	public:

		TEST_METHOD(TestPipeline)
		{
			auto stage = make_shared<PipelineStage<int, double>>(0, [](int x){ return x * 3.14; });
			auto finalStage = make_shared<PipelineStage<double, void>>(1, [](double x){ cout << x; });

			stage->connect(finalStage);
			stage->activate();
			finalStage->activate();

			for (int i = 0; i < c_iterations; ++i)
			{
				stage->addInput(i);
			}

			stage->flushAll().wait();
		}

	};
}
