#include "nn.hpp"

random_device rd; // Seed
mt19937 gen(rd()); // Mersenne Twister generator

// Constructor creates the neural network of the given topology and learning rate
// and initializes the neural network's weights and biases
NeuralNetwork::NeuralNetwork(vector<int> topology, float learningRate):
        _topology(topology),
        _neuronMatrix({}),
        _unactivatedMatrix({}),
        _biasMatrix({}),
        _weightMatrix({}),
        _learningRate(learningRate)
        {
            float boundary;

            int topology_size = topology.size();
            for (int i = 1; i < topology_size; ++i) {
                Matrix<float> weightMatrix(topology[i], topology[i-1]);

                if (i == topology_size - 1) {
                    boundary = XavierInit(topology[i], topology[i-1]);
                } else {
                    boundary = HeInit(topology[i-1]);
                }
                uniform_real_distribution<float> dis(-boundary, boundary);
                weightMatrix.initWith(dis, gen);

                _weightMatrix.push_back(weightMatrix);
                Matrix<float> biasMatrix(topology[i], 1);

                biasMatrix.zeros();
                _biasMatrix.push_back(biasMatrix);
            }
            _neuronMatrix.resize(topology.size());
            _unactivatedMatrix.resize(topology.size());
        }

//constructor initializes the neural network with trained values
NeuralNetwork::NeuralNetwork(vector<int> topology, float learningRate, const char *bias_file, const char *weight_file):
        _topology(topology),
        _neuronMatrix({}),
        _unactivatedMatrix({}),
        _biasMatrix({}),
        _weightMatrix({}),
        _learningRate(learningRate)
        {
            FILE *fp_bias = fopen(bias_file, "r");
            FILE *fp_weight = fopen(weight_file, "r");

            int topology_size = topology.size();
            for(int i = 1; i < topology_size; ++i){
                
                Matrix<float> weightMatrix(topology[i], topology[i-1]);
                for(int x = 0; x < topology[i]; x++) {
                    for(int y = 0; y < topology[i-1]; y++) {
                        float temp;
                        if(fscanf(fp_weight, "%f ", &temp) != 1) {
                            assert(false && "Read weight file error");
                        }
                        weightMatrix.at(x, y) = temp;
                    }
                }
                _weightMatrix.push_back(weightMatrix);

                Matrix<float> biasMatrix(topology[i], 1);
                for(int x = 0; x < topology[i]; x++) {
                    float temp;
                    if(fscanf(fp_bias, "%f ", &temp) != 1) {
                        assert(false && "Read bias file error");
                    }
                    biasMatrix.at(x, 0) = temp;
                }
                _biasMatrix.push_back(biasMatrix);
            }

            fclose(fp_bias);
            fclose(fp_weight);
            _neuronMatrix.resize(topology.size());
            _unactivatedMatrix.resize(topology.size());
        }

bool NeuralNetwork::feedForward(Matrix<float> neuronMatrix){   
    //check that the input size matches the given topology 
    assert (neuronMatrix.getRows() == _topology[0]);

    Matrix<float> UnactivatedMatrix = neuronMatrix;

    int weightMatrix_size = _weightMatrix.size();

    for(int i = 0; i < weightMatrix_size; ++i){
        _neuronMatrix.at(i) = neuronMatrix;
        _unactivatedMatrix.at(i) = UnactivatedMatrix;
       
        //z = W*x + b
        UnactivatedMatrix = (_weightMatrix.at(i)).multiply(_neuronMatrix[i]);
        UnactivatedMatrix = UnactivatedMatrix.add(_biasMatrix[i]);

        //a = activation_function(z)
        if (i == (weightMatrix_size - 1)) {
            neuronMatrix = UnactivatedMatrix.Softmax();
            //neuronMatrix.print();
        } else {       
            neuronMatrix = UnactivatedMatrix.applyFunction(ReLU);
        }
    }
    
    _neuronMatrix.at(_weightMatrix.size()) = neuronMatrix;
    _unactivatedMatrix.at(_weightMatrix.size()) = UnactivatedMatrix;

    return true;
}

bool NeuralNetwork::backPropagate(vector<float> target){
    // δ^(L) = a^(L) - y -> output layer
    // δ^(l) = (W^(l+1)T x δ^(l+1)) ⊙ f'(z^(l)) -> hidden layers
    // ΔW = -α * δ^(l) x a^(l-1)T
    // Δb = -α * δ^(l)

    if((int)target.size() != _topology.back()) {
        return false;
    }

    //Get delta for output layer
    Matrix<float> activatedOutput = _neuronMatrix.back();
    Matrix<float> targetOutput(target.size(), 1); // 10x1 matrix
    targetOutput._vals = target;

    Matrix<float> prev_delta; // store δ^(l+1)
    int weightMatrix_size = _weightMatrix.size();
    for (int i = weightMatrix_size - 1; i >= 0; i--) {
        
        Matrix<float> delta;
        if (i == weightMatrix_size - 1) { //for Softmax the derivitave cancels out
            delta = activatedOutput.add(targetOutput.negative());
            prev_delta = delta;
        } else {
            delta = (_weightMatrix[i+1].transpose().multiply(prev_delta)).multiplyElements(_unactivatedMatrix[i+1].applyFunction(DReLU));
            prev_delta = delta;
        }

        Matrix<float> deltaBias = delta.multiplyScaler(-_learningRate);
        //cout << "deltaBias[ " << i << "]" << endl;
        //deltaBias.print();
        _biasMatrix[i] = _biasMatrix[i].add(deltaBias);


        Matrix<float> deltaWeight = deltaBias.multiply(_neuronMatrix[i].transpose());
        //cout << "deltaWeight[ " << i << "]" << endl;
        //deltaWeight.print();
        _weightMatrix[i] = _weightMatrix[i].add(deltaWeight);
    }

    return true;
}

// function to retrive final output
vector<float> NeuralNetwork::getPredictions(){
    return _neuronMatrix.back()._vals;
}

void NeuralNetwork::print(const char *bias_file, const char *weight_file)
{
    FILE *fp = fopen(bias_file, "w");

    int biasMatrix_size = _biasMatrix.size();
    for(int i = 0; i < biasMatrix_size; ++i){
        //fprintf(fp, "-> \n");
        for (int x = 0; x < this->_biasMatrix[i]._rows; x++){
            for (int y = 0; y < this->_biasMatrix[i]._cols; y++){
                fprintf(fp, "%f ", this->_biasMatrix[i].at(x, y));
                //printf("%f ", this->_biasMatrix[i].at(x, y));
            }
            fprintf(fp, "\n");
            //printf("\n");
        }
    }
    fclose(fp);

    fp = fopen(weight_file, "w");

    int weightMatrix_size = _weightMatrix.size();
    for(int i = 0; i < weightMatrix_size; ++i){
        //fprintf(fp, "-> \n");
        for (int x = 0; x < this->_weightMatrix[i]._rows; x++){
            for (int y = 0; y < this->_weightMatrix[i]._cols; y++){
                fprintf(fp, "%f ", this->_weightMatrix[i].at(x, y));
                //printf("%f ", this->_weightMatrix[i].at(x, y));
            }
            fprintf(fp, "\n");
            //printf("\n");
        }
    }
    fclose(fp);
}
