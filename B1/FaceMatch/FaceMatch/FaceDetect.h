// Summary: 利用OpenCV的LBF算法进行人脸关键点检测
// Author:  Amusi
// Date:    2018-03-20
// Reference:
//		[1]Tutorial: https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/
//		[2]Code: https://github.com/spmallick/learnopencv/tree/master/FacialLandmarkDetection

// Note: OpenCV3.4以及上支持Facemark

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

using namespace cv;
using namespace cv::face;


bool main(int argc,char** argv)
{
    // 加载人脸检测器（Face Detector）
    // [1]Haar Face Detector
    CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");

    // 创建Facemark类的对象
    Ptr<Facemark> facemark = FacemarkLBF::create();

    // 加载人脸检测器模型
    facemark->loadModel("lbfmodel.yaml");

    // 存储视频帧和灰度图的变量
    Mat image = imread("1.jpg"), gray;

    // 存储人脸矩形框的容器
    std::vector<Rect> faces;
    // 将视频帧转换至灰度图, 因为Face Detector的输入是灰度图
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // 人脸检测
    faceDetector.detectMultiScale(gray, faces);

    // 人脸关键点的容器
    std::vector< std::vector<Point2f> > landmarks;

    // 运行人脸关键点检测器（landmark detector）
    bool success = facemark->fit(image,faces,landmarks);

    if(success)
    {

    }

}
