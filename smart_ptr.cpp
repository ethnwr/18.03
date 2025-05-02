#include <iostream>
#include <vector>
#include <random>
#include <stdexcept>
#include <memory> // для unique_ptr

using namespace std;

class Matrix {
private:
    unique_ptr<double[]> data; 
    int rows;
    int cols;

    int getIndex(int row, int col) const {
        return row * cols + col;
    }

public:

    Matrix() : data(nullptr), rows(0), cols(0) {}

 
    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        if (rows <= 0 || cols <= 0) {
            throw invalid_argument("Rows and cols must be positive.");
        }
        data = unique_ptr<double[]>(new double[rows * cols]()); 
    }

    // Конструктор копирования 
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = unique_ptr<double[]>(new double[rows * cols]);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                data[getIndex(i, j)] = other.data[other.getIndex(i, j)];
            }
        }
    }


    // Конструктор перемещения
    Matrix(Matrix&& other) noexcept : data(move(other.data)), rows(other.rows), cols(other.cols) {
        other.rows = 0;
        other.cols = 0;
    }

    // Оператор присваивания перемещением
    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            data = move(other.data);
            rows = other.rows;
            cols = other.cols;

            other.rows = 0;
            other.cols = 0;
        }
        return *this;
    }

    // Оператор присваивания копированием
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            if (rows != other.rows || cols != other.cols) {
                rows = other.rows;
                cols = other.cols;
                data = unique_ptr<double[]>(new double[rows * cols]);
            }

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    data[getIndex(i, j)] = other.data[other.getIndex(i, j)];
                }
            }
        }
        return *this;
    }

    ~Matrix() {} 

    // Получение элемента по индексу 
    double GetElement(int row, int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Index out of range.");
        }
        return data[getIndex(row, col)];
    }
    // Установка элемента по индексу 
    void SetElement(int row, int col, double value) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Index out of range.");
        }
        data[getIndex(row, col)] = value;
    }
    // Оператор () 
    double& operator()(int row, int col) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Index out of range.");
        }
        return data[getIndex(row, col)];
    }
    const double& operator()(int row, int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Index out of range.");
        }
        return data[getIndex(row, col)];
    }

    // Заполнение матрицы одним значением
    void Fill(double value) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                data[getIndex(i, j)] = value;
            }
        }
    }
    // Заполнение матрицы случайными числами 
    void FillRandom() {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                data[getIndex(i, j)] = dis(gen);
            }
        }
    }

   // Сложение матриц
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Matrices must have the same dimensions for addition.");
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.data[result.getIndex(i, j)] = data[getIndex(i, j)] + other.data[other.getIndex(i, j)];
            }
        }
        return result;
    }
    // Вычитание матриц 
    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Matrices must have the same dimensions for subtraction.");
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.data[result.getIndex(i, j)] = data[getIndex(i, j)] - other.data[other.getIndex(i, j)];
            }
        }
        return result;
    }
    // Умножение на число
    Matrix operator*(double scalar) const {
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.data[result.getIndex(i, j)] = data[getIndex(i, j)] * scalar;
            }
        }
        return result;
    }
    // Умножение матрицы на матрицу
    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw invalid_argument("Number of columns in the first matrix must be equal to the number of rows in the second matrix for multiplication.");
        }
        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                for (int k = 0; k < cols; ++k) {
                    result.data[result.getIndex(i, j)] += data[getIndex(i, k)] * other.data[other.getIndex(k, j)];
                }
            }
        }
        return result;
    }
    // Транспонирование
    Matrix Transpose() const {
        Matrix result(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.data[result.getIndex(j, i)] = data[getIndex(i, j)];
            }
        }
        return result;
    }
    // Создание диагональной матрицы 
    static Matrix CreateDiagonal(const vector<double>& diagonal) {
        int size = diagonal.size();
        Matrix result(size, size);
        for (int i = 0; i < size; ++i) {
            result.SetElement(i, i, diagonal[i]);
        }
        return result;
    }
    // Вычисление определителя 
    double Determinant() const {
        if (rows != cols) {
            throw invalid_argument("Determinant can only be calculated for square matrices.");
        }

        if (rows == 2) {
            return data[getIndex(0, 0)] * data[getIndex(1, 1)] - data[getIndex(0, 1)] * data[getIndex(1, 0)];
        }
        throw runtime_error("Determinant calculation is not implemented for matrices larger than 2x2.");
    }
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }
    // Оператор вывода 
    friend ostream& operator<<(ostream& os, const Matrix& matrix) {
        for (int i = 0; i < matrix.GetRows(); ++i) {
            for (int j = 0; j < matrix.GetCols(); ++j) {
                os << matrix.GetElement(i, j) << " ";
            }
            os << endl;
        }
        return os;
    }
};

int main() {
    try {
        Matrix matrix1(2, 3);
        matrix1.FillRandom();
        cout << "Matrix 1:\n" << matrix1 << endl;

        Matrix matrix2(2, 3);
        matrix2.Fill(1.0);
        cout << "Matrix 2:\n" << matrix2 << endl;

        Matrix matrixSum = matrix1 + matrix2;
        cout << "Matrix Sum:\n" << matrixSum << endl;

        Matrix matrixMultiplied = matrix1 * 2.0;
        cout << "Matrix Multiplied by 2:\n" << matrixMultiplied << endl;

        Matrix matrixTransposed = matrix1.Transpose();
        cout << "Matrix Transposed:\n" << matrixTransposed << endl;

        vector<double> diagonal = { 1.0, 2.0, 3.0 };
        Matrix diagonalMatrix = Matrix::CreateDiagonal(diagonal);
        cout << "Diagonal Matrix:\n" << diagonalMatrix << endl;

        cout << "Determinant of 2x2 Diagonal Matrix: " << Matrix::CreateDiagonal({ 2.0, 3.0 }).Determinant() << endl;

        Matrix matrix3x2(3, 2);
        matrix3x2.FillRandom();

        Matrix matrix2x3(2, 3);
        matrix2x3.FillRandom();

        cout << "Matrix 3x2:\n" << matrix3x2 << endl;
        cout << "Matrix 2x3:\n" << matrix2x3 << endl;

        Matrix matrixMultRes = matrix2x3 * matrix3x2;

        cout << "Multiplication result\n" << matrixMultRes << endl;

        Matrix matrixCopy = matrix1; 
        cout << "Matrix Copy:\n" << matrixCopy << endl;

        Matrix matrixAssign(1, 1);
        matrixAssign = matrix1;
        cout << "Matrix Assigned:\n" << matrixAssign << endl;

     
        Matrix matrixMove(2, 2);
        matrixMove.Fill(5.0);
        cout << "Matrix Before Move:\n" << matrixMove << endl;

        Matrix matrixMoved = move(matrixMove); 

        cout << "Matrix After Move:\n" << matrixMoved << endl;

    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
