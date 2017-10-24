#pragma once
#include "SystemVue/ModelBuilder.h"
#include "SystemVue/CircularBuffer.h"
#include "SystemVue/Matrix.h"
#include "SystemVue/MatrixCircularBuffer.h"
#include "SystemVue/DFParam.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

namespace SystemVueModelBuilder
{
	class ra_run : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(ra_run);
		ra_run();
		virtual ~ra_run();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();
		int getData(double** h_data, int* n, int* dim);
		int LinearRegression(double *data, int rows, double *a, double *b);

		/// define parameters
		int NUMBER_OF_ELEMENT;
		int NUMBER_OF_ATTRIBUTE;

		/// define ports
		IntCircularBuffer IN_ELEMENT;
		IntCircularBuffer IN_ATTRIBUTE;
		DoubleMatrixCircularBuffer IN_INPUTFILE;

		DoubleCircularBuffer Output;

	protected:
		DoubleMatrix in_tmp;
		DoubleMatrix out_tmp;

	};
}
