#include <iostream>
#include <cmath>
#include <vector>

#include "./ActivationFunction.h"

using namespace std;

int main(void)
{
	double inputs[] = {1.0, 2.0, 3.0, 4.0, 5.0};

	double hyperbolicResults[5];
	double hyperbolicDerivativeResults[5];
	double sigmoidResults[5];
	double sigmoidDerivativeResults[5];
	double binaryStepResults[5];
	double binaryStepDerivativeResults[5];
	double stepLinearResults[5];
	double stepLinearDerivativeResults[5];
	double softplusResults[5];
	double softplusDerivativeResults[5];
	double sinusoidResults[5];
	double sinusoidDerivativeResults[5];
	double gaussianResults[5];
	double gausianDerivativeResults[5];
	double arcTanResults[5];
	double arcTanDerivativeResults[5];
	double sincResults[5];
	double sincDerivativeResults[5];

	for(unsigned i = 0; i < sizeof(inputs) / sizeof(*inputs); ++i)
	{
		hyperbolicResults[i] = ActivationFunction::hyperbolic(inputs[i]);
		hyperbolicDerivativeResults[i] = ActivationFunction::hyperbolicDerivative(inputs[i]);
		sigmoidResults[i] = ActivationFunction::sigmoid(inputs[i]);
		sigmoidDerivativeResults[i] = ActivationFunction::sigmoidDerivative(inputs[i]);
		binaryStepResults[i] = ActivationFunction::binnaryStep(inputs[i]);
		binaryStepDerivativeResults[i] = ActivationFunction::binnaryStepDerivative(inputs[i]);
		stepLinearResults[i] = ActivationFunction::stepLinear(inputs[i]);
		stepLinearDerivativeResults[i] = ActivationFunction::stepLinearDerivative(inputs[i]);
		softplusResults[i] = ActivationFunction::softPlus(inputs[i]);
		softplusDerivativeResults[i] = ActivationFunction::softPlusDerivative(inputs[i]);
		sinusoidResults[i] = ActivationFunction::sinusoid(inputs[i]);
		sinusoidDerivativeResults[i] = ActivationFunction::sinusoidDerivative(inputs[i]);
		gaussianResults[i] = ActivationFunction::gaussian(inputs[i]);
		gausianDerivativeResults[i] = ActivationFunction::gaussianDerivative(inputs[i]);
		arcTanResults[i] = ActivationFunction::arcTan(inputs[i]);
		arcTanDerivativeResults[i] = ActivationFunction::arcTanDerivative(inputs[i]);
		sincResults[i] = ActivationFunction::sinc(inputs[i]);
		sincDerivativeResults[i] = ActivationFunction::sincDerivative(inputs[i]);

		cout << "hyperbolic" << endl;
		cout << hyperbolicResults[i] << endl;
		cout << "hyperbolic Derivative" << endl;
		cout << hyperbolicDerivativeResults[i] << endl;
		cout << "sigmiod" << endl;
		cout << sigmoidResults[i] << endl;
		cout << "hyperbolic Derivative" << endl;
		cout << sigmoidDerivativeResults[i] << endl;
		cout << "binary step" << endl;
		cout << binaryStepResults[i] << endl;
		cout << "binary step Derivative" << endl;
		cout << binaryStepDerivativeResults[i] << endl;
		cout << "linear" << endl;
		cout << stepLinearResults[i] << endl;
		cout << "linear Derivative" << endl;
		cout << stepLinearDerivativeResults[i] << endl;
		cout << "soft plus" << endl;
		cout << softplusResults[i] << endl;
		cout << "soft plus Derivative" << endl;
		cout << softplusDerivativeResults[i] << endl;
		cout << "sinusoid" << endl;
		cout << sinusoidResults[i] << endl;
		cout << "sinusoid Derivative" << endl;
		cout << sinusoidDerivativeResults[i] << endl;
		cout << "gaussian" << endl;
		cout << gaussianResults[i] << endl;
		cout << "gaussian Derivative" << endl;
		cout << gausianDerivativeResults[i] << endl;
		cout << "arcus tangent" << endl;
		cout << arcTanResults[i] << endl;
		cout << "arcus tangent Derivative" << endl;
		cout << arcTanDerivativeResults[i] << endl;
		cout << "sinc" << endl;
		cout << sincResults[i] << endl;
		cout << "sinc Derivative" << endl;
		cout << sincDerivativeResults[i] << endl;
	}

	return 0;
}
