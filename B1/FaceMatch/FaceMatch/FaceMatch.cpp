// 2016011470 罗云鹏 数值第一次大作业
// FaceMatch\FaceMatch.cpp 主程序

#include "pch.h"
#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

#include "Image.h"
#include "Matrix.h"
#include "SMatrix.h"
#include "Interpolation.h"
#include "cmdline.h"
#include <vector>

int main(int argc, char *argv[]) {
	//////////// 命令行参数读取部分 ////////////
	cmdline::parser cmd;
	cmd.add<std::string>("path", '\0', "directory path for images and points, relative path supported", false, "./");
	cmd.add<std::string>("source", 's', "source image file name", true, "");
	cmd.add<std::string>("target", 't', "source image file name", true, "");
	cmd.add<std::string>("sourcepset", '1', "point set for source image", true, "");
	cmd.add<std::string>("targetpset", '2', "point set for target image", true, "");
	cmd.add<std::string>("method", 'm', "interpolation method", false, "neighbour", cmdline::oneof<std::string>("neighbour", "bilinear", "bicubic"));
	cmd.add("all", 'a', "show source image, target image, and generated images with three methods");
	cmd.add("pointmark", 'p', "mark points");
	cmd.add("detect", 'd', "detect points");

	cmd.parse_check(argc, argv);

	std::cout << "path: " << cmd.get<std::string>("path") << std::endl;
	std::cout << "source: " << cmd.get<std::string>("source") << std::endl;
	std::cout << "target: " << cmd.get<std::string>("target") << std::endl;
	std::cout << "sourcepset: " << cmd.get<std::string>("sourcepset") << std::endl;
	std::cout << "targetpset: " << cmd.get<std::string>("targetpset") << std::endl;
	std::cout << "method: " << cmd.get<std::string>("method") << std::endl;
	std::cout << "all: " << (cmd.exist("all") ? "true" : "false") << std::endl;
	std::cout << "pointMark: " << (cmd.exist("pointmark") ? "true" : "false") << std::endl;
	std::cout << "detect: " << (cmd.exist("detect") ? "true" : "false") << std::endl;

	String path = cmd.get<std::string>("path");
	String sourceImg = cmd.get<std::string>("source");
	String targetImg = cmd.get<std::string>("target");
	String sourcepset = cmd.get<std::string>("sourcepset");
	String targetpset = cmd.get<std::string>("targetpset");
	String method = cmd.get<std::string>("method");
	bool showAll = cmd.exist("all");
	bool pointMark = cmd.exist("pointmark");
	bool detect = cmd.exist("detect");
	//////////// 命令行参数读取部分 ////////////

	Image img1(path + sourceImg);
	Image img2(path + targetImg);

	if ( detect ) {
		if ( img1.detect_points() && img2.detect_points() )
			std::cout << "Points successfully detected!" << std::endl;
		else {
			std::cout << "Detection failed! Exiting program." << std::endl;
			waitKey(0);
			return 0;
		}
	}
	else {
		img1.read_point_file(path + sourcepset);
		img2.read_point_file(path + targetpset);
	}

	int imgPointN = (int)img2.points.size();
	SMatrix L(imgPointN + 3);

	// 构造矩阵 L 中 K 矩阵部分
	for ( int i = 1; i < imgPointN; i++ ) {
		for ( int j = 0; j < i; j++ ) {
			double rs = pow(img2.points[i].x - img2.points[j].x, 2) + pow(img2.points[i].y - img2.points[j].y, 2);
			L.element(i, j) = L.element(j, i) = rs ? rs * log(rs) : 0;
		}
	}

	// 构造矩阵 L 中 P 矩阵部分
	for ( int i = 0; i < imgPointN; ++i ) {
		L.element(L.m - 3, i) = L.element(i, L.m - 3) = 1;
		L.element(L.m - 2, i) = L.element(i, L.m - 2) = img2.points[i].y;
		L.element(L.m - 1, i) = L.element(i, L.m - 1) = img2.points[i].x;
	}

	// 求得 L 的逆
	SMatrix &Ainv = *L.inv();

	// 构造矩阵 Y
	Matrix Y(imgPointN + 3, 2);
	for ( int i = 0; i < imgPointN; ++i ) {
		Y.element(i, 0) = img1.points[i].y;
		Y.element(i, 1) = img1.points[i].x;
	}

	// 计算映射方程的参数
	Matrix *factors = Ainv * Y;

	double targetx, targety;

	// 根据用户选择决定计算三种插值方法中哪一种

	int rows = max(img1.rows(), img2.rows());
	int cols = max(img1.cols(), img2.cols());

	if ( method == "neighbour" || showAll) {
		Image neighbourI(rows, cols);
		for ( int i = 0; i < neighbourI.rows(); i++ ) {
			for ( int j = 0; j < neighbourI.cols(); j++ ) {
				getMapping(targetx, targety, i, j, img2.points, factors);
				neighbour(targetx, targety, neighbourI.img.at<Vec3b>(i, j), img1);
			}
		}

		if ( pointMark ) {
			neighbourI.points = img2.points;
			neighbourI.markPoints();
		}

		namedWindow("neighbour");
		imshow("neighbour", neighbourI.img);
		imwrite(path + "out_neighbour.jpg", neighbourI.img);
	}

	if ( method == "bilinear" || showAll ) {
		Image bilinearI(rows, cols);
		for ( int i = 0; i < bilinearI.rows(); i++ ) {
			for ( int j = 0; j < bilinearI.cols(); j++ ) {
				getMapping(targetx, targety, i, j, img2.points, factors);
				bilinear(targetx, targety, bilinearI.img.at<Vec3b>(i, j), img1);
			}
		}

		if ( pointMark ) {
			bilinearI.points = img2.points;
			bilinearI.markPoints();
		}

		namedWindow("bilinear");
		imshow("bilinear", bilinearI.img);
		imwrite(path + "out_bilinear.jpg", bilinearI.img);
	}


	if ( method == "bicubic" || showAll ) {
		Image bicubicI(rows, cols);
		for ( int i = 0; i < bicubicI.rows(); i++ ) {
			for ( int j = 0; j < bicubicI.cols(); j++ ) {
				getMapping(targetx, targety, i, j, img2.points, factors);
				bicubic(targetx, targety, bicubicI.img.at<Vec3b>(i, j), img1);
			}
		}

		if ( pointMark ) {
			bicubicI.points = img2.points;
			bicubicI.markPoints();
		}

		namedWindow("bicubic");
		imshow("bicubic", bicubicI.img);
		imwrite(path + "out_bicubic.jpg", bicubicI.img);
	}

	if ( showAll ) {
		if ( pointMark ) {
			img1.markPoints();
			img2.markPoints();
		}
		namedWindow("original picture");
		imshow("original picture", img1.img);
		namedWindow("target picture");
		imshow("target picture", img2.img);
	}

	waitKey(0);

	delete[] &Ainv, factors;
	return 0;
}

