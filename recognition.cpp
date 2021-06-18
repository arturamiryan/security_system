#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "FaceDetector.h"
#include <iostream>
#include <fstream>
#include <sstream>
// MySQL library
#include "some_sql.hpp"

using namespace std;
using namespace cv;
using namespace cv::face;

#define WINDOW_NAME "Recognize"

Mat prepare_image(const Mat& gray_image)
{
    Mat image = gray_image;
    resize(image, image, Size(100,100), INTER_AREA);
    equalizeHist(image, image);

    return image;
}

int main(void)
{
	MYSQL *con;
	MYSQL_RES *res;
	MYSQL_ROW row;

	struct connection_details mysqlD;
	mysqlD.server = "localhost";
	mysqlD.user = "albus";
	mysqlD.password = "123qwe";
	mysqlD.database = "company_staff";
	// connect to the mysql database
	con = mysql_connection_setup(mysqlD);
	// get the results from executing commands
	res = mysql_perform_query(con, "select * from employees where database_id=999");
	row = mysql_fetch_row(res);

    string model = "../output/face-trainner.yml";
	ifstream file(model, ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the file exist.";
		CV_Error(Error::StsBadArg, error_message);
	}

	cv::VideoCapture video_capture;
	if (!video_capture.open(0)) return 0;

	FaceDetector face_detector;
	Ptr<LBPHFaceRecognizer> recognizer = LBPHFaceRecognizer::create();
	recognizer->read(model);

	vector<string> names(5);
	names[0] = "colin morgan";
	names[1] = "artur";
	names[2] = "bradley james";
	names[3] = "david tennant";
	names[4] = "billie piper";

	Mat frame, frame_gry, ui_frame;
	// Main cycle
	while (true)
	{
		video_capture >> frame;
		cvtColor(frame, frame_gry, COLOR_BGR2GRAY);

		vector<Rect> rectangles = face_detector.detect_face_rectangles(frame);
		if(rectangles.size() > 0)
		{
    		vector<Mat> roi;

    		cv::Scalar color(0, 0, 255);
    		for(const auto & r : rectangles)
    		{
    			cv::rectangle(frame, r, color, 4);
                Mat img = prepare_image(frame_gry(r));
    			roi.push_back(img);
    		}
            //imshow("roi", roi[0]);

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
    			if(conf[i] > 30 && conf[i] < 100)
				{
    				putText(frame, names[predictedLabel[i]], Point(rectangles[i].x, rectangles[i].y), FONT_ITALIC, 1, Scalar(255, 255, 255), 2, LINE_AA);

					// clean up the database result
					mysql_free_result(res);
					string queryy = "select * from employees where database_id=";
					queryy.append(to_string(predictedLabel[0]));
					res = mysql_perform_query(con, queryy);
					row = mysql_fetch_row(res);
				}
		}

		// UI init
		// 640x480 625x120
		ui_frame = Mat(Size(1280, 720), CV_8UC3);
		ui_frame = Scalar(49, 52, 49);
		/*
		cvui::init(WINDOW_NAME);
		cvui::text(ui_frame, 70, 130, "Name: ", 0.9);
		cvui::text(ui_frame, 180, 130, row[1], 0.9);
		cvui::text(ui_frame, 70, 230, "Position: ", 0.9);
		cvui::text(ui_frame, 200, 230, row[2], 0.9);
		cvui::text(ui_frame, 70, 330, "Birth Date: ", 0.9);
		cvui::text(ui_frame, 230, 330, row[3], 0.9);
		cvui::image(ui_frame, 625, 120, frame);
		*/
		cv::imshow("Recognize", ui_frame);
		int esc_key = 27;
		if (cv::waitKey(10) == esc_key) break;
	}

	// close database connection
	mysql_close(con);

    cv::destroyAllWindows();
    video_capture.release();

    return 0;
}
