#include <vector>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <functional>
#include <math.h>

using namespace std;

template<typename T>class Matrix {
public:
    int _rows;
    int _cols;
    vector<T> _vals;

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

    bool isSquare() {
        return _rows == _cols;
    }

    int getRows() { return _rows; }

    int getCols() { return _cols; }

    Matrix negative() {
        Matrix output(_rows, _cols);
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
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) * target.at(x, y);
            }
        }
        return output;
    }

    void zeros() {
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                at(x, y) = 0;
            }
        }
    }

    void ones() {
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                at(x, y) = 1;
            }
        }
    }

    float sumElements() {
        float sum = 0;
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                sum += at(x, y);
            }
        }
        return sum;
    }

    float max() {
        float max = at(0,0);
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                max = ((max <= at(x, y))? at(x, y) : max);
            }
        }
        return max;
    }

    Matrix logSoftmax() {

        float max = this -> max();

        cout << "max: " << max << endl;

        Matrix<float> targetMatrix = this -> addScaler(-max);

        cout << "unactivated after normalisation:" << endl;
        targetMatrix.print();

        targetMatrix = targetMatrix.applyFunction([](const float &val){
                    return exp(val);
                });

        cout << "output after exp:" << endl;
        targetMatrix.print();

        float sum = targetMatrix.sumElements();
        sum = log(sum);

        targetMatrix = targetMatrix.addScaler(-sum);

        cout << "output after sum:" << endl;
        targetMatrix.print();

        targetMatrix = targetMatrix.addScaler(-max);

        cout << "output after logsoftmax:" << endl;
        targetMatrix.print();

        return targetMatrix;
    }

    Matrix Softmax() {
        Matrix targetMatrix(_rows, _cols);

        // 1. Find max for stability
        float maxVal = at(0, 0);
        for (int x = 1; x < _rows; x++) {
            maxVal = (maxVal <= at(x, 0)) ? at(x, 0) : maxVal;
        }

        // 2. Compute exponentials and sum
        float sumExp = 0.0f;
        for (int x = 0; x < _rows; x++) {
            float val = exp(at(x, 0) - maxVal);
            targetMatrix.at(x, 0) = val;
            sumExp += val;
        }

        // 3. Normalize
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
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) + target.at(x, y);
            }
        }
        return output;
    }

    Matrix applyFunction(function<T(const T&)> func) {
        Matrix output(_rows, _cols);
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = func(at(x, y));
            }
        }
        return output;
    }

    void initWith(uniform_real_distribution<T>& dist, std::mt19937& gen) {
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                at(x, y) = dist(gen);
            }
        }
    }

    Matrix multiplyScaler(float s) {
        Matrix output(_rows, _cols);
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) * s;
            }
        }
        return output;

    }

    Matrix addScaler(float s) {
        Matrix output(_rows, _cols);
        for (int x = 0; x < output._rows; x++) {
            for (int y = 0; y < output._cols; y++) {
                output.at(x, y) = at(x, y) + s;
            }
        }
        return output;
    }

    Matrix transpose() {
        Matrix output(_cols, _rows);
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                output.at(y, x) = at(x, y);
            }
        }
        return output;
    }

    Matrix cofactor(int row, int col) {
        Matrix output(_rows - 1, _cols - 1);
        int i = 0;
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                if (x == col || y == row) continue;
                output._vals[i++] = at(x, y);
            }
        }
        return output;
    }

    T determinant() {
        assert(_rows == _cols);
        T output = T();

        if (_rows == 1) {
            return _vals[0];
        }
        else {
            int32_t sign = 1;
            for (int x = 0; x < _cols; x++) {
                output += sign * at(0, x) * cofactor(0, x).determinant();
                sign *= -1;
            }
        }
        return output;
    }

    Matrix adjoint() {
        assert(_rows == _cols);
        Matrix output(_rows, _cols);
        int32_t sign = 1;
        for (int x = 0; x < _rows; x++) {
            for (int y = 0; y < _cols; y++) {
                output.at(x, y) = sign * cofactor(x, y).determinant();
                sign *= -1;
            }
        }
        output = output.transpose();

        return output;
    }

    Matrix inverse() {
        Matrix adj = adjoint();
        T factor = determinant();
        for (int x = 0; x < adj._cols; x++) {
            for (int y = 0; y < adj._rows; y++) {
                adj.at(x, y) = adj.at(x, y) / factor;
            }
        }
        return adj;
    }
}; // class Matrix

//ReLU activation function - Rectified Linear Unit
inline float ReLU(float val){
    return ((val <= 0)? 0 : val);
}

template<typename T> void LogMatrix(Matrix<T>& mat) {
    for (int x = 0; x < mat._rows; x++) {
        for (int y = 0; y < mat._cols; y++) {
            cout << setw(10) << mat.at(x, y) << " ";
        }
        cout << endl;
    }
}



