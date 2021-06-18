#include "opencv2/imgproc.hpp"
#include "rw_csv.hpp"
#include "FaceDetector.h"

// detect face, cut, resize, normalize
Mat prepare_image(const Mat& image, const Mat& gray_image)
{
    FaceDetector face_detector;
    Mat crop_image = image;

    vector<Rect> rectangles = face_detector.detect_face_rectangles(image);
    crop_image = gray_image(rectangles[0]);
    resize(crop_image, crop_image, Size(100,100), INTER_AREA);
    equalizeHist(crop_image, crop_image);

    //imshow("Image", crop_image);
    //int k = waitKey(0); // Wait for a keystroke in the window

    return crop_image;
}

void train()
{
    const string csv_file = "../output/att_faces.csv";
    // TODO: check for file exist
    write_csv(csv_file, "../assets/att_faces");

    vector<Mat> images;
    vector<Mat> gray_images;
    vector<int> labels;
    vector<int> gray_labels;
    read_csv(csv_file, images, labels);
    read_csv_grayscale(csv_file, gray_images, gray_labels);

    // Prepair for training
    vector<Mat> crop_images(images.size());
    for(long unsigned int i = 0; i < images.size(); ++i)
        crop_images[i] = prepare_image(images[i], gray_images[i]);

    // Training
    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train(crop_images, labels);
    model->save("../output/face-trainner.yml");

	return;
}
