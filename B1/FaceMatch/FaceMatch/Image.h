// 2016011470 罗云鹏 数值第一次大作业
// FaceMatch\Image.h 定义 image 类

#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/face.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::face;

class Image {
	// 图像类，用于存储图像矩阵及特征点
public:
	Image(String ImgPath) { img = imread(ImgPath); }
	Image(int rows, int cols);
	void read_point_file(String PointPath); // 传入文件路径，读取文件中的特征点信息
	bool detect_points(); // 人脸关键点检测
	void markPoints(); // 在图形中标记出特征点
	const int& rows() const { return img.rows; }
	const int& cols() const { return img.cols; }

	Mat img;
	std::vector<Point2d> points;
};

Image::Image(int rows, int cols) {
	img = Mat(rows, cols, CV_8UC3);
}

void Image::read_point_file(String PointPath) {
	std::ifstream file;
	file.open(PointPath, std::ios::in);
	double x, y;
	while ( true ) {
		file >> x >> y;
		if ( !file.eof() )
			points.push_back(Point2d(x, y));
		else
			break;
	}
}
void Image::markPoints() {
	if ( points.size() )
		for ( int i = 0; i < points.size(); i++ ) {
			circle(img, points[i], 1, Scalar(0, 0, 0), FILLED);
		}
	else
		std::cerr << "Image has no points to mark!" << std::endl;
}

bool Image::detect_points() {
	// 来源 https://github.com/amusi/opencv-facial-landmark-detection
	// Summary: 利用OpenCV的LBF算法进行人脸关键点检测
	// Author:  Amusi
	// Date:    2018-03-20
	// Reference:
	//		[1]Tutorial: https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/
	//		[2]Code: https://github.com/spmallick/learnopencv/tree/master/FacialLandmarkDetection

	// Note: OpenCV3.4以及上支持Facemark

	// 加载人脸检测器（Face Detector）
	// [1]Haar Face Detector
	CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");

	// 创建Facemark类的对象
	Ptr<Facemark> facemark = FacemarkLBF::create();

	// 加载人脸检测器模型
	facemark->loadModel("lbfmodel.yaml");

	// 灰度图
	Mat gray;

	// 存储人脸矩形框的容器
	std::vector<Rect> faces;
	// 将视频帧转换至灰度图, 因为Face Detector的输入是灰度图
	cvtColor(img, gray, COLOR_BGR2GRAY);

	// 人脸检测
	faceDetector.detectMultiScale(gray, faces);

	// 人脸关键点的容器
	std::vector< std::vector<Point2f> > landmarks;

	// 运行人脸关键点检测器（landmark detector）
	bool success = facemark->fit(img, faces, landmarks);

	for ( auto p : landmarks[0] ) {
		points.push_back(Point2d(p.x, p.y));
	}

	return success;

}


#endif // !IMAGE