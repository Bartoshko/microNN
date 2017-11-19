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
#include <cstdlib>
#include <cassert>
#include <cmath>

using namespace std;

// structure connections is conteiner to store specific connection information

struct Connection
{
	double weight;
	double deltaWeight;
};

// declared there because Layer is related to Neuron
class Neuron;

// each Leyer contains vector of Neurons
typedef vector<Neuron> Layer;

/*****NEURON******/

class Neuron
{
public:
	Neuron(unsigned neuronOutputsNum, unsigned myIndex);
	~Neuron();
	void SetOutput(double output);
	double GetOutput() const;
	void FeedForward(const Layer &previousLayer);

private:
	unsigned n_myIndex;
	double n_Output;
	vector<Connections> n_outWeights;
	static double RandomNum(void);
	static double Transfer(double value);
	static double TransferDerivative(double value);
};

Neuron::Neuron(unsigned neuronOutputsNum, unsigned myIndex)
{
	for(unsigned c; c < neuronOutputsNum; ++c)
	{
		n_outWeights.push_back(Connection());
		n_outWeights.back().weight = Neuron::RandomNum();
	}
	n_myIndex = myIndex
}
static double Neuron::RandomNum(void)
{
	return rand() / double(RAND_MAX);
}
static double Transfer(double value)
{
	// there we are using hyperbolic tension function
	return tanh(value);
}
static double TransferDerivative(double value)
{
	return 1.0 - value * value;
}

void Neuron::SetOutput(double output)
{
	n_Output = output;
}
double Neuron::GetOutput() const
{
	return n_Output;
}
void Neuron::FeedForward(const Layer &previousLayer)
{
	double sum = 0.0;
	// sum outpots from previous layer tah in this case are inputs for neuron

	// include bias node from previos layer

	for(unsigned neuronN n = 0; neuronN < previousLayer.size(); ++neuronN)
	{
		sum += 	previousLayer[neuronN].GetOutput() * previousLayer[neuronN].n_outWeights[n_myIndex].weight;
	}
	n_Output = Neuron::Transfer(sum);
}

/*****NET******/

class Net
{
public:
	Net(const vector<unsigned> &topology);
	~Net();
	void FeedForward(const vector<double> &inputs);
	void BackPropagation(const vector<double> &targets);
	void GetResults(vector<double> &results) const;

private:
	vector<Layer> n_layers; // this is n_layer[number of layer][number of neuron] contains all layers that contains neurons
};

Net::Net(const vector<unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for(unsigned layerN = 0; layerN < numLayers; ++layerN)
	{
		n_layers.push_back(Layer()); // creates new struture named Layer and appends it on the end of the vector n_layer

		// calculate number of outputs (connections) for each neuron based on information of neurons quantity in next layer, for output layer there will be no connections set
		unsigned outputsNum = layerN == topology.size() - 1 ? 0 : topology[layerN + 1];

		// now each Layer needs to be filled with i_th neurons and add bias neuron to Layer
		for(unsigned neuronNum = 0; neuronNum <= topology[layerN]; ++neuronNum)
		{
			// creates new Neuron on the end of the layer vector
			n_layers.back().push_back(Neuron(outputsNum, neuronNum));
			cout << "Made a new Neuron" << endl;
		}
	}
}

Net::~Net()
{

}

void Net::FeedForward(const vector<double> &inputs)
{
	//check if number of inputs is the same as number of neurons in firs layer
	assert(inputs.size() == n_layers[0].size() - 1);

	//asssign the input values in to the input neurons
	for(unsigned iter = 0; iter < inputs.size(); ++inter)
	{
		n_layers[0][iter].SetOutputValue(inputs[i]);
	}

	// then do the forward propagation of input by looping through each neuron and than call FeedForward fonction of neuron that will than based on all inputs from neurons of layerNumber - 1 calculate weights and feed forward neurons in the next layers
	for(unsigned layerNumber = 1; layerNumber < n_layers.size(); ++layerNumber)
	{
		Layer &previousLayer = n_layers.[layerNumber - 1];
		for(unsigned neuronInLayerNum = 0; neuronInLayerNum < n_layers[layerNumber].size(); -1; ++neuronInLayerNum)
		{
			n_layers[layerNumber][neuronInLayerNum].FeedForward();
		}
	}
}

void Net::BackPropagation(const vector<double> &targets)
{

}
void Net::GetResults(vector<double> &results) const
{

}

/*****test******/

int main()
{
	vector<unsigned> topology;
	topology.push_back(3);
	topology.push_back(2);
	topology.push_back(1);
	Net microNN(topology);

	vector<double> inputs;
	microNN.FeedForward(inputs);

	vector<double> targets;
	microNN.BackPropagation(targets);

	vector<double> results;
	microNN.GetResults(results);
}

