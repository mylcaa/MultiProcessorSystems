#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <random>

#include "nn.hpp"

using namespace std;

// input is picture 28x28
#define INPUT_NEURON_NUM 784
// output is a number [0, 9]
#define OUTPUT_NEURON_NUM 10

#define EPOCH 10000
#define PIXEL_MAX_VAL 255
#define LEARNING_RATE 0.001

#define TRAIN_FILE_PATH "train/train_"
#define TEST_FILE_PATH "test/test_"

void train(vector<int>, const char *, const char *);
void test(vector<int>, const char *, const char *);
int findLabel(vector<float>);
int findLabelOutput(vector<float>);

int main(int argc, char* argv[])
{
    //input num of threads...

    //create topology of neural network
    vector<int> topology;
    int numLayers;

    cout << "input num of hidden layers:" << endl;
    cin >> numLayers;

    topology.push_back(INPUT_NEURON_NUM);

    int num;
    for(int i = 1; i <= numLayers; ++i){
        cout << "input num of neurons in layer[" << i << "]:" << endl;
        cin >> num;
        topology.push_back(num);
    }

    topology.push_back(OUTPUT_NEURON_NUM);
    
    const char *weight_file = "weights.txt";
    const char *bias_file = "biases.txt";

    string choice;

    while (choice != "quit") {
            cout << "train, test or quit?" << endl;
            cin >> choice;

            if(choice == "train") {
                train(topology, bias_file, weight_file);
            } 
            
            if (choice == "test") {
                test(topology, bias_file, weight_file);
            }
    }

    return 0;
}

void train(vector<int> topology, const char * bias_file, const char * weight_file){

    //creating neural network
    NeuralNetwork nn(topology, LEARNING_RATE);

    string train;

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist60000(0,59999);

    vector<float> targetOutput(OUTPUT_NEURON_NUM);
    Matrix<float> targetInput(INPUT_NEURON_NUM, 1);
    
    //training the neural network with randomized data
    cout << "training start\n";

    for(int i = 0; i < EPOCH; i++)
    {
        //get random index
        int index = dist60000(rng);
        
        //form file path
        train = TRAIN_FILE_PATH;
        train.append(to_string(index));
        train.append(".txt");
        const char * ctrain = train.c_str();
    
        FILE *fp = fopen(ctrain, "r");
        assert(fp != NULL);
        
        for (int j = 0; j < (INPUT_NEURON_NUM+OUTPUT_NEURON_NUM); ++j) {
            if (j < OUTPUT_NEURON_NUM) {
                fscanf(fp, "%d ", &index);
                targetOutput.at(j) = (float)index;
            } else {
                fscanf(fp, "%d ", &index);
                targetInput._vals[j - OUTPUT_NEURON_NUM] = (float)index/PIXEL_MAX_VAL;
            }
        }

        fclose(fp);

        nn.feedForward(targetInput);
        nn.backPropagate(targetOutput);
    }
    cout << "training complete\n";
    
    nn.print(bias_file, weight_file);
}

void test(vector<int> topology, const char * bias_file, const char * weight_file) {

    NeuralNetwork nn(topology, LEARNING_RATE, bias_file, weight_file);

    //random_device dev;
    //mt19937 rng(dev());
    //uniform_int_distribution<mt19937::result_type> dist10000(0,9999);

    vector<float> targetOutput(OUTPUT_NEURON_NUM);
    //MNIST dataset pictures are 28x28
    Matrix<float> targetInput(INPUT_NEURON_NUM, 1);
    int temp;
    string test;

    int correct_guess = 0;

    for(int i = 0; i < 10000; ++i){

        //get random index
        //int index = dist10000(rng);

        //form file path
        test = TEST_FILE_PATH;
        test.append(to_string(i));
        test.append(".txt");
        const char * ctest = test.c_str();
        
        FILE *fp = fopen(ctest, "r");
        assert(fp != NULL);
        
        for(int j = 0; j < (INPUT_NEURON_NUM + OUTPUT_NEURON_NUM); ++j){
            if(j < OUTPUT_NEURON_NUM){
                fscanf(fp, "%d ", &temp);
                targetOutput.at(j) = (float)temp;
            }else{
                fscanf(fp, "%d ", &temp);
                targetInput._vals[j - OUTPUT_NEURON_NUM] = (float)temp/PIXEL_MAX_VAL;
            }
        }
        fclose(fp);


        nn.feedForward(targetInput);
        vector<float> preds = nn.getPredictions();

        int trained_label = findLabelOutput(preds);
        int actual_label = findLabel(targetOutput);

        if(trained_label == actual_label)
            correct_guess++;

        //cout << "should've gotten: " << actual_label << " actually got: " << trained_label << endl;
        
    }

    cout << "accuracy of simple neural network: " << ((float)correct_guess/100) << "%" << endl; 

}

int findLabel(vector<float> output){

    int label = 0;
    for(auto pt: output){
        if(pt == 0)
            label++;
        else
            break;
    }
    return label;
}

int findLabelOutput(vector<float> output){

    int label = 0;
    float max = output.at(label);

    for (int i = 0; i < output.size(); ++i) {
        if (max < output.at(i)) {
            label = i;
            max = output.at(i);
        }
    }
    return label;
}