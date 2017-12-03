/*

BARTOSHCO MICRO NEURO NETWORK C++

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
Copyright (c) 2017-2022 Bartoshco Lenart

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:
t
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFT

*/

#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

const double e = 2.71828182845904523536;

class ActivationFunction
{
public:
	ActivationFunction(){};
	~ActivationFunction(){};
	static double hyperbolic(const double &x)
	{
		return tanh(x);
	}
	static double hyperbolicDerivative(const double &x)
	{
		double tan = ActivationFunction::hyperbolic(x);
		return 1.0 - tan * tan;
	}
	static double sigmoid(const double &x)
	{
		return x / (1 + abs(x));
	}
	static double sigmoidDerivative(const double &x)
	{
		double sig_x = ActivationFunction::sigmoid(x);
		return 1.0 - sig_x * sig_x;
	}
	static double binnaryStep(const double &x)
	{
		return x >= 0 ? 1 : 0;
	}
	static double binnaryStepDerivative(const double &x)
	{
		return x == 0 ? 1 : 0;
	}
	static double stepLinear(const double &x)
	{
		return x >= 0 ? x : 0;
	}
	static double stepLinearDerivative(const double &x)
	{
		return x >= 0 ? 1 : 0;
	}
	static double softPlus(const double &x)
	{
		return log(1 +  pow(e, x));
	}
	static double softPlusDerivative(const double &x)
	{
		return 1 / (1 +  pow(e, -x));
	}
	static double sinusoid(const double &x)
	{
		return sin(x);
	}
	static double sinusoidDerivative(const double &x)
	{
		return cos(x);
	}
	static double gaussian(const double &x)
	{
		return pow(e, pow(-x, 2));
	}
	static double gaussianDerivative(const double &x)
	{
		return 2 * x * pow(e, pow(-x, 2));
	}
	static double arcTan(const double &x)
	{
		return atan(x);
	}
	static double arcTanDerivative(const double &x)
	{
		return 1 / (x * x + 1);
	}
	static double sinc(const double &x)
	{
		return x == 0 ? 1 : sin(x) / x;
	}
	static double sincDerivative(const double &x)
	{
		return x == 0 ? 0 : cos(x) / x - sin(x) / x * x;
	}
};
