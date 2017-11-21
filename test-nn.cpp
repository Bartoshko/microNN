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
#include "./micronn.h"
/*****test******/

int main()
{
	vector<unsigned> topology;
	topology.push_back(100);
	topology.push_back(200);
	topology.push_back(100);
	Net microNN(topology);

	vector<double> inputs;
	microNN.FeedForward(inputs);

	vector<double> targets;
	microNN.BackPropagation(targets);

	vector<double> results;
	microNN.GetResults(results);
}

