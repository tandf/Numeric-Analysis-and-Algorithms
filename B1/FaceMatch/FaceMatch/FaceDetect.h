// Summary: ����OpenCV��LBF�㷨���������ؼ�����
// Author:  Amusi
// Date:    2018-03-20
// Reference:
//		[1]Tutorial: https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/
//		[2]Code: https://github.com/spmallick/learnopencv/tree/master/FacialLandmarkDetection

// Note: OpenCV3.4�Լ���֧��Facemark

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

using namespace cv;
using namespace cv::face;


bool main(int argc,char** argv)
{
    // ���������������Face Detector��
    // [1]Haar Face Detector
    CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");

    // ����Facemark��Ķ���
    Ptr<Facemark> facemark = FacemarkLBF::create();

    // �������������ģ��
    facemark->loadModel("lbfmodel.yaml");

    // �洢��Ƶ֡�ͻҶ�ͼ�ı���
    Mat image = imread("1.jpg"), gray;

    // �洢�������ο������
    std::vector<Rect> faces;
    // ����Ƶ֡ת�����Ҷ�ͼ, ��ΪFace Detector�������ǻҶ�ͼ
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // �������
    faceDetector.detectMultiScale(gray, faces);

    // �����ؼ��������
    std::vector< std::vector<Point2f> > landmarks;

    // ���������ؼ���������landmark detector��
    bool success = facemark->fit(image,faces,landmarks);

    if(success)
    {

    }

}
