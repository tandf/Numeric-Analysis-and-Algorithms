// 2016011470 罗云鹏 数值第一次大作业
// FaceMatch\Interpolation.h 插值函数

#ifndef INTERPOLATION
#define INTERPOLATION

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "Image.h"
#include <math.h>

double S(double x);

void getMapping(double& x, double& y, double inputx, double inputy, std::vector<Point2d> &points, Matrix *factors) {
	// 使用计算得到的参数矩阵，计算出从目标矩阵坐标映射到原图的位置
	x = factors->element(factors->m - 3, 0) +
		inputx * factors->element(factors->m - 2, 0) +
		inputy * factors->element(factors->m - 1, 0);
	y = factors->element(factors->m - 3, 1) +
		inputx * factors->element(factors->m - 2, 1) +
		inputy * factors->element(factors->m - 1, 1);
	double u, r2;
	int i = 0;
	for (auto p : points) {
		r2 = pow(p.x - inputy, 2) + pow(p.y - inputx, 2);
		u = r2 ? r2 * log(r2) : 0;
		x += factors->element(i, 0) * u;
		y += factors->element(i, 1) * u;
		i++;
	}
}

void neighbour(double x, double y, cv::Vec3b& p, const Image& source) {
	// 最近邻插值
	int ix, iy;
	ix = (int)round(x);
	iy = (int)round(y);
	if ( ix >= 0 && ix < source.rows() && iy >= 0 && iy < source.cols() ) {
		p[0] = source.img.at<Vec3b>(ix, iy)[0];
		p[1] = source.img.at<Vec3b>(ix, iy)[1];
		p[2] = source.img.at<Vec3b>(ix, iy)[2];
	}
}

void bilinear(double x, double y, cv::Vec3b& p, const Image& source) {
	// 双线性插值
	double u = x - floor(x), v = y - floor(y), temp;
	int ix = (int)floor(x), iy = (int)floor(y);
	if ( ix >= 0 && ix < source.rows() - 1 && iy >= 0 && iy < source.cols() - 1 ) {
		for ( int i = 0; i < 3; i++ ) {
			temp = (1 - u)*(1 - v)*source.img.at<Vec3b>(ix, iy)[i]
				+ u * (1 - v)*source.img.at<Vec3b>(ix + 1, iy)[i]
				+ (1 - u)*v*source.img.at<Vec3b>(ix, iy + 1)[i]
				+ u * v*source.img.at<Vec3b>(ix + 1, iy + 1)[i];
			p[i] = temp > 255 ? 255 :
				(temp < 0 ? 0 : (uchar)temp);
		}
	}
}

void bicubic(double x, double y, cv::Vec3b& p, const Image& source) {
	// 双三次插值
	double u = x - floor(x), v = y - floor(y), temp1, temp2;
	int ix = (int)floor(x), iy = (int)floor(y);
	if ( ix > 0 && ix < source.rows() - 2 && iy > 0 && iy < source.cols() - 2 ) {
		for ( int c = 0; c < 3; c++ ) {
			temp2 =  0.0;
			for ( int i = 0; i < 4; i++ ) {
				temp1 = 0;
				for ( int j = 0; j < 4; j++ )
					temp1 += S(v - j + 1) * source.img.at<Vec3b>(ix + i - 1, iy + j - 1)[c];
				temp2 += temp1 * S(u - i + 1);
			}
			p[c] = temp2 > 255 ? 255 : 
				(temp2 < 0 ? 0 : (uchar)temp2);
		}
	}
}

double S(double x) {
	x = abs(x);
	if ( x <= 1 )
		return 1 - 2 * pow(x, 2) + pow(x, 3);
	else if ( x <= 2 )
		return 4 - 8 * x + 5 * pow(x, 2) - pow(x, 3);
	else
		return 0;
}
#endif // !INTERPOLATION