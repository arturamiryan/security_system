#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "FaceDetector.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;
using namespace cv::face;

Mat prepare_image(const Mat& gray_image)
{
    Mat image = gray_image;
    resize(image, image, Size(100,100), INTER_AREA);
    equalizeHist(image, image);

    return image;
}

int recognize(Mat cap_frame)
{
	if(cap_frame.empty())
		return 999;
    string model = "../output/face-trainner.yml";
	ifstream file(model, ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the file exist.";
		CV_Error(Error::StsBadArg, error_message);
	}

	FaceDetector face_detector;
	Ptr<LBPHFaceRecognizer> recognizer = LBPHFaceRecognizer::create();
	recognizer->read(model);

	Mat frame, frame_gry;
	frame = cap_frame;

	cvtColor(frame, frame_gry, COLOR_BGR2GRAY);

	vector<Rect> rectangles = face_detector.detect_face_rectangles(frame);
	if(rectangles.size() > 0)
	{
		vector<Mat> roi;

		for(const auto & r : rectangles)
		{
            Mat img = prepare_image(frame_gry(r));
			roi.push_back(img);
		}

		vector<int> predictedLabel(roi.size());
		vector<double> conf(roi.size());
		for(int i = 0; i < roi.size(); ++i)
		{
			predictedLabel[i] = -1;
			conf[i] = 0.0;
		}
		for(int i = 0; i < roi.size(); ++i)
		{
			recognizer->predict(roi[i], predictedLabel[i], conf[i]);
		}
		//cout << predictedLabel[0] << " with " << conf[0] << endl;

	    for(int i = 0; i < conf.size(); ++i)
		{
			cout << predictedLabel[0] << " with " << conf[0] << endl;
			if(conf[i] > 30 && conf[i] < 120)
				return predictedLabel[i];
			else
				return 999;
		}
	}
	return 999;
}
