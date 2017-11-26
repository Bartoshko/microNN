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

#include <vector>
#include <list>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

/* type and struct definitions */
struct Connection {
	double weight;
	double deltaWeight;
};

class Neuron;

typedef vector<Neuron> Layer;

/* class Neuron */

class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myIndex);
	~Neuron();
	void setOutputVal(double val)
	{
		m_outputVal = val;
		updateFireRate();
	}
	double getOutputVal(void) const
	{
		return m_outputVal;
	}
	void feedForward(const Layer &prevLayer);
	void calcOutputGradients(double targetVal);
	void calcHiddenGradients(const Layer &nextLayer);
	void updateInputWeights(Layer &prevLayer);
	std::vector<Connection> getOutputWeights() const
	{
		return m_outputWeights;
	}
	double getFireRate() const
	{
		return fireRate;
	}

private:
	static double eta;   // [0.0..1.0]
	static double alpha; // [0.0..n]
	static unsigned averageFireRate;
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);
	static double randomWeight(void)
	{
		return rand() / double(RAND_MAX);
	}
	double sumDOW(const Layer &nextLayer) const;
	void updateFireRate();
	double m_outputVal;
	vector<Connection> m_outputWeights;
	unsigned m_myIndex;
	double m_gradient;
	double fireRate;
	std::list<bool> lastShots;
};

double Neuron::eta = 0.15; // [0.0..1.0]
double Neuron::alpha = 0.5; // [0.0..n]

unsigned Neuron::averageFireRate = 500;

void Neuron::updateFireRate()
{
	lastShots.push_back(m_outputVal > 0);
	fireRate += lastShots.back() / double(averageFireRate);
	if (lastShots.size() > averageFireRate)
	{
		fireRate -= lastShots.front() / double(averageFireRate);
		lastShots.pop_front();
	}
}

void Neuron::updateInputWeights(Layer &prevLayer)
{

	for (unsigned n = 0; n < prevLayer.size(); ++n)
	{
		Neuron &neuron = prevLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

		double newDeltaWeight = eta * neuron.getOutputVal() * m_gradient + alpha * oldDeltaWeight;

		neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
		neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
	}
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
	double sum = 0.0;

	for (unsigned n = 0; n < nextLayer.size() - 1; ++n)
	{
		sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
	}

	return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
	double dow = sumDOW(nextLayer);
	m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::calcOutputGradients(double targetVal)
{
	double delta = targetVal - m_outputVal;
	m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

double Neuron::transferFunction(double x)
{
	return tanh(x); // [-1.0..1.0]
}

double Neuron::transferFunctionDerivative(double x)
{
	double tan = tanh(x);
	return 1.0-tan*tan;
}

void Neuron::feedForward(const Layer &prevLayer)
{
	double sum = 0.0;

	for (unsigned n = 0; n < prevLayer.size(); ++n)
	{
		sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights[m_myIndex].weight;
	}

	m_outputVal = Neuron::transferFunction(sum);
	updateFireRate();
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned c = 0; c < numOutputs; ++c)
	{
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight = randomWeight();
	}

	m_myIndex = myIndex;
}

Neuron::~Neuron() {}

/* class Net */

class Net
{
public:
	Net(const vector<unsigned> &topology);
	~Net();
	void feedForward(const vector<double> &inputs);
	void backProp(const vector<double> &targets);
	void getResults(vector<double> &result) const;
	double getRecentAverageError(void) const
	{
		return d_recentAverageError;
	}
	void saveNet(const string filename) const;
	void loadNet(const string filename);

private:
	vector<Layer> v_layers;
	double d_error;
	double d_recentAverageError;
	static double d_recentAverageSmoothingFactor;

	static unsigned u_savewidth;
	static unsigned u_saveprec;
	static double d_threshold;
};

void Net::saveNet(const string filename) const
{
	std::ofstream myfile;
	myfile.open(filename.c_str());
	myfile << setprecision(u_saveprec);

	myfile << "topology:";
	unsigned maximumLayer = 0;
	for (unsigned i = 0; i < v_layers.size(); i++)
	{
		myfile << " " << v_layers[i].size() - 1;
		if (v_layers[i].size() - 1 > maximumLayer)
		{
			maximumLayer = v_layers[i].size() - 1;
		}
	}
	for (unsigned i = 0; i < v_layers.size() - 1; i++)
	{
		myfile << std::endl << std::endl << "ANN from layer " << i
				<< " to layer " << i + 1 << ":" << std::endl
				<< std::setw(u_savewidth) << " ";
		for (unsigned j = 0; j < v_layers[i + 1].size() - 1; j++)
		{
			myfile << std::setw(u_savewidth) << j;
		}
		for (unsigned j = 0; j < v_layers[i].size(); j++)
		{
			myfile << std::endl << std::setw(u_savewidth) << j;
			for (unsigned k = 0; k < v_layers[i + 1].size() - 1; k++)
			{
				myfile << std::setw(u_savewidth)
						<< v_layers[i][j].getOutputWeights()[k].weight;
			}
		}
	}
	myfile << std::endl << std::endl
			<< "Neurons, that fire almost every time (threshold=" << d_threshold
			<< "):" << std::endl;
	myfile << setw(2) << " ";
	for (unsigned i = 0; i < v_layers.size(); i++)
	{
		myfile << setw(2) << i;
	}
	for (unsigned i = 0; i < maximumLayer; i++)
	{
		myfile << std::endl;
		myfile << setw(2) << i;
		for (unsigned j = 0; j < v_layers.size(); j++)
		{
			if (v_layers[j].size() - 1 > i)
			{
				myfile << setw(2) << (v_layers[j][i].getFireRate() > d_threshold ? "X" : "O");
			}
			else
				myfile << setw(2) << " ";
		}
	}
	myfile << std::endl << std::endl
			<< "Neurons, that fire almost not once (threshold=" << 1 - d_threshold
			<< "):" << std::endl;
	myfile << setw(2) << " ";
	for (unsigned i = 0; i < v_layers.size(); i++)
	{
		myfile << setw(2) << i;
	}
	for (unsigned i = 0; i < maximumLayer; i++)
	{
		myfile << std::endl;
		myfile << setw(2) << i;
		for (unsigned j = 0; j < v_layers.size(); j++)
		{
			if (v_layers[j].size() - 1 > i)
			{
				myfile << setw(2) << (v_layers[j][i].getFireRate() < 1 - d_threshold ? "X" : "O");
			}
			else
			{
				myfile << setw(2) << " ";
			}
		}
	}
	myfile.close();
}

void Net::loadNet(const string filename) {}

double Net::d_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over
unsigned Net::u_savewidth = 10;
unsigned Net::u_saveprec = 5;
double Net::d_threshold = 0.55;

void Net::getResults(vector<double> &results) const
{
	results.clear();

	for (unsigned n = 0; n < v_layers.back().size() - 1; ++n)
	{
		results.push_back(v_layers.back()[n].getOutputVal());
	}
}

void Net::backProp(const vector<double> &targets)
{
// Calculate overall net error (RMS of output neuron errors)

	Layer &outputLayer = v_layers.back();
	d_error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		double delta = targets[n] - outputLayer[n].getOutputVal();
		d_error += delta * delta;
	}
	d_error /= outputLayer.size() - 1; // get average error squared
	d_error = sqrt(d_error); // RMS

// Implement a recent average measurement

	d_recentAverageError = (d_recentAverageError * d_recentAverageSmoothingFactor + d_error) / (d_recentAverageSmoothingFactor + 1.0);

// Calculate output layer gradients

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		outputLayer[n].calcOutputGradients(targets[n]);
	}

// Calculate hidden layer gradients

	for (unsigned layerNum = v_layers.size() - 2; layerNum > 0; --layerNum)
	{
		Layer &hiddenLayer = v_layers[layerNum];
		Layer &nextLayer = v_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); ++n)
		{
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}


	for (unsigned layerNum = v_layers.size() - 1; layerNum > 0; --layerNum)
	{
		Layer &layer = v_layers[layerNum];
		Layer &prevLayer = v_layers[layerNum - 1];

		for (unsigned n = 0; n < layer.size() - 1; ++n)
		{
			layer[n].updateInputWeights(prevLayer);
		}
	}
}

void Net::feedForward(const vector<double> &inputs)
{
	assert(inputs.size() == v_layers[0].size() - 1);

	for (unsigned i = 0; i < inputs.size(); ++i)
	{
		v_layers[0][i].setOutputVal(inputs[i]);
	}

// forward propagate
	for (unsigned layerNum = 1; layerNum < v_layers.size(); ++layerNum)
	{
		Layer &prevLayer = v_layers[layerNum - 1];
		for (unsigned n = 0; n < v_layers[layerNum].size() - 1; ++n)
		{
			v_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

Net::Net(const vector<unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
	 {
		v_layers.push_back(Layer());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum)
		{
			v_layers.back().push_back(Neuron(numOutputs, neuronNum));
		}

		v_layers.back().back().setOutputVal(1.0);
	}
}

Net::~Net() {}

void showVectorVals(string label, vector<double> &v)
{
	cout << label << " ";
	for (unsigned i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}

	cout << endl;
}

double Randomizer(void)
{
	return rand() / double(RAND_MAX);
}
