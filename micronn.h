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
	virtual ~Neuron();
	static double eta, alpha;
	static double RandomNum(void)
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
	void SetOutput(double output);
	double GetOutput() const;
	void FeedForward(const Layer &previousLayer);
	void CalculateOutputGradients(double target);
	void CalculateHiddenGradients(Layer &nextLayer);
	double SumDOW(const Layer &nextLayer) const;
	void UpdateInputWeights(Layer &prevLayer);
private:
	unsigned n_myIndex;
	double n_Output, n_gradient;
	vector<Connection> n_outWeights;
};

double Neuron::eta = 0.16;
double Neuron::alpha = 0.52;

Neuron::Neuron(unsigned neuronOutputsNum, unsigned myIndex)
{
	for(unsigned c = 0; c < neuronOutputsNum; ++c)
	{
		n_outWeights.push_back(Connection());
		n_outWeights.back().weight = Neuron::RandomNum();
		n_outWeights.back().deltaWeight = 0.0;
	}
	n_myIndex = myIndex;
}
Neuron::~Neuron(){}

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
	// sum outpots from previous layer in this case are inputs for neuron
	double sum = 0.0;

	// include bias node from previos layer

	for(unsigned neuronN = 0; neuronN < previousLayer.size(); ++neuronN)
	{
		sum += previousLayer[neuronN].GetOutput() * previousLayer[neuronN].n_outWeights[n_myIndex].weight;
	}
	n_Output = Neuron::Transfer(sum);
}

void Neuron::CalculateOutputGradients(double target)
{
	double dv = target - n_Output;
	n_gradient = dv * Neuron::TransferDerivative(n_Output);
}

void Neuron::CalculateHiddenGradients(Layer &nextLayer)
{
	double dow = SumDOW(nextLayer);
	n_gradient = dow * Neuron::TransferDerivative(n_Output);
}

double Neuron::SumDOW(const Layer &nextLayer) const
{
	double sum = 0.0;
	// sum contribution of the error at the nodes we feed
	for(unsigned n = 0; n < nextLayer.size() - 1; ++n)
	{
		sum += n_outWeights[n].weight * nextLayer[n].n_gradient;
	}
	return sum;
}

void Neuron::UpdateInputWeights(Layer &prevLayer)
{
	// weights that are going to be updated are in Connection container in the neuron in the  previous layer
	for(unsigned n = 0; n < prevLayer.size(); ++n)
	{
		Neuron &neuron = prevLayer[n];

		cout << neuron.n_outWeights[n_myIndex].deltaWeight;

		// double oldDeltaWeight = neuron.n_outWeights[n_myIndex].deltaWeight;

		// eta is overall learning rate
		// alpha is momentum of the learning
		// double newDeltaWeight = eta * neuron.GetOutput()  * n_gradient + alpha * oldDeltaWeight;

		// neuron.n_outWeights[n_myIndex].deltaWeight = newDeltaWeight;
		// neuron.n_outWeights[n_myIndex].weight += newDeltaWeight;

	}
}

/*****NET******/

class Net
{
public:
	Net(const vector<unsigned> &topology);
	virtual ~Net();
	void FeedForward(const vector<double> &inputs);
	void BackPropagation(const vector<double> &targets);
	void GetResults(vector<double> &results) const;

private:
	vector<Layer> n_layers; // this is n_layer[number of layer][number of neuron] contains all layers that contains neurons
	double n_error, n_recentAverageError, n_recentAverageErrorSmoothingFactor;
};

Net::Net(const vector<unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for(unsigned layerN = 0; layerN < numLayers; ++layerN)
	{
		n_layers.push_back(Layer()); // creates new struture named Layer and appends it on the end of the vector n_layer

		// calculate number of outputs (connections) for each neuron based on information of neurons quantity in next layer, for output layer there will be no connections set
		unsigned outputsNum = layerN == topology.size() - 1 ? 0 : topology[layerN + 1];

		// now each Layer needs to be filled with i_th neurons and add bias neuron to Layer besides output layer that will not have bias neuron
		unsigned neuronsPerLayer = topology[layerN];
		if(layerN == numLayers - 1)
		{
			neuronsPerLayer = topology[layerN] - 1;
		}
		for(unsigned neuronNum = 0; neuronNum <= neuronsPerLayer; ++neuronNum)
		{
			// creates new Neuron on the end of the layer vector
			n_layers.back().push_back(Neuron(outputsNum, neuronNum));
			cout << "Made a new Neuron in layer: " << layerN << " neuron number: " << neuronNum << endl;
		}
		// force bieas nodes output value to 1.0
		if(layerN != numLayers - 1)
		{
			n_layers.back().back().SetOutput(1.0);
		}
	}
}

Net::~Net(){}

void Net::FeedForward(const vector<double> &inputs)
{
	//check if number of inputs is the same as number of neurons in first layer
	assert(inputs.size() == n_layers[0].size() - 1);

	//asssign the input values in to the input

	for(unsigned iter = 0; iter < inputs.size(); ++iter)
	{
		n_layers[0][iter].SetOutput(inputs[iter]);
	}

	// then do the forward propagation of input by looping through each neuron and than call FeedForward function of neuron that will than based on all inputs from neurons of layerNumber - 1 calculate weights and feed forward neurons in the layerNumber
	for(unsigned layerNumber = 1; layerNumber < n_layers.size(); ++layerNumber)
	{
		Layer &previousLayer = n_layers[layerNumber - 1];
		for(unsigned neuron_Num = 0; neuron_Num < n_layers[layerNumber].size() - 1; ++neuron_Num)
		{
			n_layers[layerNumber][neuron_Num].FeedForward(previousLayer);
		}
	}
}

void Net::BackPropagation(const vector<double> &targets)
{
 /*
 * There is where network learns, it needs to do some calculations:
 * a - calculate overall net error using root means squere error
 * a.a - implement a recent average measurment
 * b - calculate output layer gradiend
 * c - calculate hidden layer gradient
 * d - for all layers from outputs back to first hidden layer update connections weights
 */

	// a
	Layer &outputLayer = n_layers.back();
	n_error = 0.0;

	unsigned ne; // neuron index for looping through layer neurons
	unsigned layerNumber; // layer index for looping through net layers
	for(ne = 0; ne < outputLayer.size() - 1; ++ne)
	{
		double dt = targets[ne] - outputLayer[ne].GetOutput();
		n_error += dt * dt;
	}
	n_error /= outputLayer.size() - 1;
	n_error = sqrt(n_error);

	// a.a
	n_recentAverageError = (n_recentAverageError * n_recentAverageErrorSmoothingFactor + n_error) / (n_recentAverageErrorSmoothingFactor + 1.0);

	// b
	for(ne = 0; ne < outputLayer.size() - 1; ++ne)
	{
		outputLayer[ne].CalculateOutputGradients(targets[ne]);
	}

	// c
	for(layerNumber = n_layers.size() - 2; layerNumber > 0; --layerNumber)
	{
		Layer &hiddenLayer = n_layers[layerNumber];
		Layer &nextLayer = n_layers[layerNumber + 1];
		for(ne = 0; ne < hiddenLayer.size(); ++ne)
		{
			hiddenLayer[ne].CalculateHiddenGradients(nextLayer);
		}
	}

	// d
	for(layerNumber = n_layers.size() - 1; layerNumber > 0; --layerNumber)
	{
		Layer &layer = n_layers[layerNumber];
		Layer &prevLayer = n_layers[layerNumber - 1];

		for(ne = 0; ne < layer.size(); ++ne)
		{
			layer[ne].UpdateInputWeights(prevLayer);
		}
	}

}

void Net::GetResults(vector<double> &results) const
{
	results.clear();
	for(unsigned neurOut = 0; neurOut < n_layers.back().size(); ++neurOut)
	{
		results.push_back(n_layers.back()[neurOut].GetOutput());
	}
}