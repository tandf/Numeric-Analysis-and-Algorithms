// 2016011470 ������ ��ֵ��һ�δ���ҵ
// FaceMatch\Image.h ���� image ��

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
	// ͼ���࣬���ڴ洢ͼ�����������
public:
	Image(String ImgPath) { img = imread(ImgPath); }
	Image(int rows, int cols);
	void read_point_file(String PointPath); // �����ļ�·������ȡ�ļ��е���������Ϣ
	bool detect_points(); // �����ؼ�����
	void markPoints(); // ��ͼ���б�ǳ�������
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
	// ��Դ https://github.com/amusi/opencv-facial-landmark-detection
	// Summary: ����OpenCV��LBF�㷨���������ؼ�����
	// Author:  Amusi
	// Date:    2018-03-20
	// Reference:
	//		[1]Tutorial: https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/
	//		[2]Code: https://github.com/spmallick/learnopencv/tree/master/FacialLandmarkDetection

	// Note: OpenCV3.4�Լ���֧��Facemark

	// ���������������Face Detector��
	// [1]Haar Face Detector
	CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");

	// ����Facemark��Ķ���
	Ptr<Facemark> facemark = FacemarkLBF::create();

	// �������������ģ��
	facemark->loadModel("lbfmodel.yaml");

	// �Ҷ�ͼ
	Mat gray;

	// �洢�������ο������
	std::vector<Rect> faces;
	// ����Ƶ֡ת�����Ҷ�ͼ, ��ΪFace Detector�������ǻҶ�ͼ
	cvtColor(img, gray, COLOR_BGR2GRAY);

	// �������
	faceDetector.detectMultiScale(gray, faces);

	// �����ؼ��������
	std::vector< std::vector<Point2f> > landmarks;

	// ���������ؼ���������landmark detector��
	bool success = facemark->fit(img, faces, landmarks);

	for ( auto p : landmarks[0] ) {
		points.push_back(Point2d(p.x, p.y));
	}

	return success;

}


#endif // !IMAGE