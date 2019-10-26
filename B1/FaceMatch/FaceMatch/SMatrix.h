// 2016011470 罗云鹏 数值第一次大作业
// FaceMatch\SMatrix.h 定义方阵及相关运算

#ifndef SMATRIX
#define SMATRIX

#include "Matrix.h"

class SMatrix : public Matrix {
public:
	SMatrix(int size) : Matrix(size, size){};
	void LUP_Descomposition(); // 进行 LUP 分解
	void transpose(); // 转置运算
	SMatrix* inv(); // 求逆

	double *L, *U;
	int *P;
};

void SMatrix::LUP_Descomposition() {
	this->L = new double[m * m];
	this->U = new double[m * m];
	this->P = new int[m];
	memset(L, 0, sizeof(double) * m * m);
	memset(U, 0, sizeof(double) * m * m);
	memset(P, 0, sizeof(int) * m);

	double* copy = new double[m * m];
	memcpy(copy, data, m * m * sizeof(double));

	int maxDataRow = 0;
	for ( int i = 0; i < m; i++ )
		P[i] = i;

	for ( int i = 0; i < m - 1; i++ ) {
		double maxData = 0.0;
		for ( int j = i; j < m; j++ )
			if ( abs(copy[j * m + i]) > maxData ) {
				maxData = abs(copy[j * m + i]);
				maxDataRow = j;
			}

		if ( 0 == maxData ) {
			std::cerr << "singular matrix, no inv" << std::endl;
			return;
		}

		int temp = P[i];
		P[i] = P[maxDataRow];
		P[maxDataRow] = temp;

		double dtemp = 0;
		for ( int j = 0; j < m; j++ ) {
			dtemp = copy[i*m + j];
			copy[i*m + j] = copy[maxDataRow*m + j];
			copy[maxDataRow*m + j] = dtemp;
		}

		double u = copy[i*m + i], l = 0;
		for ( int j = i + 1; j < m; j++ ) {
			l = copy[j * m + i] / u;
			copy[j * m + i] = l;
			for ( int k = i + 1; k < m; k++ )
				copy[j * m + k] -= copy[i * m + k] * l;
		}
	}

	for ( int i = 0; i < m; i++ ) {
		for ( int j = 0; j <= i; j++ )
			L[i*m + j] = i == j ? 1 : copy[i*m + j];

		for ( int k = i; k < m; k++ )
			U[i*m + k] = copy[i*m + k];
	}

}

SMatrix* SMatrix::inv() {
	SMatrix *invM = new SMatrix(m);
	double *inv_A_each = new double[m]();
	double *temp = new double[m]();
	double *b = new double[m]();
	memset(b, 0, m * sizeof(double));
	LUP_Descomposition();

	for ( int i = 0; i < m; i++ ) {
		for ( int i = 0; i < m; i++ )
			b[i] = 0;
		b[i] = 1;

		for ( int i = 0; i < m; i++ ) {
			temp[i] = b[P[i]];
			for ( int j = 0; j < i; j++ )
				temp[i] = temp[i] - L[i*m + j] * temp[j];
		}

		for ( int i = m - 1; i >= 0; i-- ) {
			inv_A_each[i] = temp[i];
			for ( int j = m - 1; j > i; j-- )
				inv_A_each[i] = inv_A_each[i] - U[i*m + j] * inv_A_each[j];
			inv_A_each[i] /= U[i*m + i];
		}

		memcpy(invM->data + i * m, inv_A_each, m * sizeof(double));
	}

	invM->transpose();
	return invM;
}

void SMatrix::transpose() {
	double temp;
	for ( int i = 0; i < m; ++i ) {
		for ( int j = 0; j < i; ++j ) {
			temp = this->element(i, j);
			this->element(i, j) = this->element(j, i);
			this->element(j, i) = temp;
		}
	}
}

#endif // !SMATRIX