#include <vector>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <functional>
#include <math.h>
#include <omp.h>

using namespace std;

template<typename T>class Matrix {
public:
    int _rows;
    int _cols;
    vector<T> _vals;

    static int omp_threads;

public:
    Matrix(int rows, int cols) 
    : _rows(rows),
      _cols(cols),
      _vals({}) {
        _vals.resize(rows * cols, T());
    }

    Matrix()
    : _rows(0),
    _cols(0),
    _vals({}) {}

    T& at(int row, int col) {
        return _vals[row * _cols + col];
    }

    int getRows() { return _rows; }

    int getCols() { return _cols; }

    Matrix negative() {
        Matrix output(_rows, _cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = -at(x, y);
            }
        }
        return output;
    }

    Matrix multiply(Matrix& target) {
        assert(target._rows == _cols);
        Matrix output(_rows, target._cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                T result = T();
                for (int k = 0; k < _cols; k++)
                    result += at(x, k) * target.at(k, y);
                output.at(x, y) = result;
            }
        }
        return output;
    }

    Matrix multiply(Matrix&& target) {
        assert(target._rows == _cols);
        Matrix output(_rows, target._cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                T result = T();
                for (int k = 0; k < _cols; k++)
                    result += at(x, k) * target.at(k, y);
                output.at(x, y) = result;
            }
        }
        return output;
    }

    Matrix multiplyElements(Matrix& target) {
        assert(_rows == target._rows && _cols == target._cols);
        Matrix output(_rows, _cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) * target.at(x, y);
            }
        }
        return output;
    }

    Matrix multiplyElements(Matrix&& target) {
        assert(_rows == target._rows && _cols == target._cols);
        Matrix output(_rows, _cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) * target.at(x, y);
            }
        }
        return output;
    }

    void zeros() {

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                at(x, y) = 0;
            }
        }
    }

    void ones() {

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                at(x, y) = 1;
            }
        }
    }

    Matrix Softmax() {
        Matrix targetMatrix(_rows, _cols);

        // 1. Find max for stability
        float maxVal = at(0, 0);

        #pragma omp parallel for reduction(max:maxVal) \
        if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 1; x < _rows; x++) {
            float v = at(x, 0);
            if (v >= maxVal) {
                maxVal = v;
            }
        }

        // 2. Compute exponentials and sum
        float sumExp = 0.0f;

        #pragma omp parallel for reduction(+:sumExp) \
        if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < _rows; x++) {
            float val = exp(at(x, 0) - maxVal);
            targetMatrix.at(x, 0) = val;
            sumExp += val;
        }

        // 3. Normalize
        #pragma omp parallel for \
        if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < _rows; x++) {
            targetMatrix.at(x, 0) /= sumExp;
        }

        return targetMatrix;
    }

    void print() {
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                printf("%f ", at(x, y));
            }
            printf("\n");
        }
    }

    Matrix add(Matrix& target) {
        assert(_rows == target._rows && _cols == target._cols);
        Matrix output(_rows, _cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) + target.at(x, y);
            }
        }
        return output;
    }

    Matrix add(Matrix&& target) {
        assert(_rows == target._rows && _cols == target._cols);
        Matrix output(_rows, _cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) + target.at(x, y);
            }
        }
        return output;
    }

    Matrix applyFunction(function<T(const T&)> func) {
        Matrix output(_rows, _cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = func(at(x, y));
            }
        }
        return output;
    }

    void initWith(uniform_real_distribution<T>& dist, std::mt19937& gen) {

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                at(x, y) = dist(gen);
            }
        }
    }

    Matrix multiplyScaler(float s) {
        Matrix output(_rows, _cols);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) * s;
            }
        }
        return output;

    }

    Matrix transpose() {
        Matrix output(_cols, _rows);

        #pragma omp parallel if(omp_threads > 0) num_threads(omp_threads)
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                output.at(y, x) = at(x, y);
            }
        }
        return output;
    }

}; // class Matrix

// Init static variable num of threads
template<typename T>
int Matrix<T>::omp_threads = 0;

//ReLU activation function - Rectified Linear Unit
inline float ReLU(float val){
    return ((val <= 0)? 0 : val);
}



