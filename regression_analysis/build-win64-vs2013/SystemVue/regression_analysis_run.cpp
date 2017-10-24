#pragma once
#include "regression_analysis_run.h"
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

using namespace std;

namespace SystemVueModelBuilder {

#ifndef SV_CODE_RA_RUN
	DEFINE_MODEL_INTERFACE(ra_run)
	{
		SET_MODEL_NAME("Regressioin_analysis_run");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("Regressioin_analysis");
		ADD_MODEL_HEADER_FILE("regression_analysis_run.h");
		model.SetModelCodeGenName("ra_run");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters


		// Add input/output ports
		DFPort IN_N = ADD_MODEL_INPUT(IN_ELEMENT);
		IN_N.SetDescription("Input the number of elements");
		DFPort IN_M = ADD_MODEL_INPUT(IN_ATTRIBUTE);
		IN_M.SetDescription("Input the number of attributes");
		DFPort IN_FILE = ADD_MODEL_INPUT(IN_INPUTFILE);
		IN_FILE.SetDescription("Input the data");


		DFPort OutputPort = ADD_MODEL_OUTPUT(Output);
		OutputPort.SetDescription("Output");

		return true;
	}
#endif

	ra_run::ra_run()
	{
	}

	ra_run::~ra_run()
	{
	}

	bool ra_run::Setup()
	{
		IN_ELEMENT.SetRate(1);
		IN_ATTRIBUTE.SetRate(1);
		IN_INPUTFILE.SetRate(1);
		Output.SetRate(2);

		return true;
	};

	bool ra_run::Initialize()
	{
		return true;
	}

	bool ra_run::Run()
	{

		int n;
		int dim;
		double* h_data;
		double a, b;
		getData(&h_data, &n, &dim);

		LinearRegression(h_data,n, &a, &b);

		free(h_data);


		return true;
	}


	bool ra_run::Finalize()
	{
		return true;
	}

	int ra_run::getData(double** h_data,
		int* n,
		int* dim)
	{
		int i, j;


		*n = IN_ELEMENT[0];
		*dim = IN_ATTRIBUTE[0];

		in_tmp = IN_INPUTFILE[0];


		*h_data = (double*)malloc(sizeof(double) * (*n) * (*dim));

		int cnt = 0;
		
		for (i = 0; i < *n; ++i)
		{
			for (j = 0; j < *dim; ++j)
			    *(*h_data + (cnt++)) = in_tmp(i,j);
		}
		return 0;
	}
	int ra_run::LinearRegression(double *data, int rows, double *a, double *b)
	{
		int m;
		double *p, Lxx = 0.0, Lxy = 0.0, xa = 0.0, ya = 0.0;
		
		for (p = data, m = 0; m < rows; m++)
		{
			xa += *p++;
			ya += *p++;
		}
		xa /= rows;                                     // X平均值
		ya /= rows;                                     // Y平均值

		
		for (p = data, m = 0; m < rows; m++, p += 2)
		{
			Lxx += ((*p - xa) * (*p - xa));             // Lxx = Sum((X - Xa)平方)
			Lxy += ((*p - xa) * (*(p + 1) - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
		}
		*b = Lxy / Lxx;                                 // b = Lxy / Lxx
		*a = ya - *b * xa;                              // a = Ya - b*Xa
		FILE* out=fopen("regression_analysis_out.txt","w+");

		

		fprintf(out,"%f %f\n",*a,*b);
		fclose(out);
		Output[0] = *a;
		Output[1] = *b;
		return 0;
	}

}