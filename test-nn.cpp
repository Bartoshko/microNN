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

#include <iostream>
#include <vector>
#include <unistd.h>
#include "./Net.h"
/*****test******/

using namespace std;

int main()
{
	vector<unsigned> topology;
	vector<double> inputs;
	vector<double> targets;
	vector<double> results;
	unsigned hiddenLayersQuantity = 1;
	unsigned neuronsPerLayer = 2;


	/* Inputs */
	topology.push_back(2);
	/* Creating the N N */
	while(hiddenLayersQuantity > 0)
	{
		topology.push_back(neuronsPerLayer);
		--hiddenLayersQuantity;
	}
	/* Outputs */
	topology.push_back(1);
	Net microNN(topology);

	/*Train network and get results*/
	unsigned sesions = 1000;

	double input_A, input_B;

	while(sesions > 0)
	{
		// clear inputs and targets before each feed and lesson
		inputs.clear();
		targets.clear();

		input_A = Randomizer();
		input_B = Randomizer();
		if(input_A > .5)
		{
			input_A = 1.0;
		}
		else
		{
			input_A = 0.0;
		}

		if(input_B > .5)
		{
			input_B = 1.0;
		}
		else
		{
			input_B = 0.0;
		}

		inputs.push_back(input_A);
		inputs.push_back(input_B);

		double expectedValue;
		if((input_A == 1.0 && input_B == 1.0) || (input_A == 0.0 && input_B == 0.0))
		{
			expectedValue  = 0.0;
		}
		else
		{
			expectedValue  = 1.0;
		}


		microNN.feedForward(inputs);
		microNN.getResults(results);

		targets.push_back(expectedValue);

		microNN.backProp(targets);

		cout << " | A: " << input_A << " B: " << input_B << " | " << endl;
		cout << " | R " << results[0] << " : ";
		cout << "E " << expectedValue;
		cout << " | DIFF | " << abs(expectedValue - results[0]) << endl;

		// sleep(2); // sleep for one second;

		--sesions;
	}
}