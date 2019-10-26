// 2016011470 罗云鹏 数值第一次大作业
// FaceMatch\Matrix.h 定义矩阵类及相关运算

#ifndef MATRIX
#define MATRIX

#include <iostream>
class Matrix{
public:
    Matrix(int m, int n);
	Matrix(int m, int n, double* d) : m(m), n(n), data(d) {};
	~Matrix() { delete data; }
    Matrix* operator*(const Matrix &target); // 定义矩阵乘法
    double& element(int x, int y){ return data[x * n + y]; };
    const double& element(int x, int y) const{ return data[x * n + y]; };
	void print();

    double *data;
    int m, n;

};

Matrix::Matrix(int sizem, int sizen){
    m = sizem;
    n = sizen;
    data = new double[m * n];
	memset(data, 0, m * n * sizeof(double));
}

Matrix* Matrix::operator*(const Matrix &target) {
	Matrix* result = new Matrix(this->m, target.n);

	if ( this->n == target.m ) {
		// 矩阵乘法
		for ( int i = 0; i < result->m; i++ )
			for ( int j = 0; j < result->n; j++ ) {
				result->element(i, j) = 0;
				for ( int k = 0; k < this->n; k++ )
					result->element(i, j) += this->element(i, k) * target.element(k, j);
			}
	}

	else
		std::cerr << "matrix size does not match" << std::endl;

    return result;
}

void Matrix::print() {
    for(int i=0;i<this->m;i++) {
        for(int j=0;j<this->n;j++) {
			std::cout << this->element(i, j) << "\t";
        }
        std::cout<<std::endl;
    }
}

#endif // !MATRIX
