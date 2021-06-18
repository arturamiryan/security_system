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

int main()
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
    for(int i = 0; i < images.size(); ++i)
        crop_images[i] = prepare_image(images[i], gray_images[i]);

    // Training
    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train(crop_images, labels);
    model->save("../output/face-trainner.yml");
/*
    // TODO: check for file exist
    Mat test = imread("../assets/artur.jpg");
    // TODO: check for file exist
    Mat test_gray = imread("../assets/artur.jpg", 0);
    Mat fin_test = prepare_image(test, test_gray);
    int predictedLabel = model->predict(fin_test);
    cout << predictedLabel << endl;

    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, 1);
    cout << result_message << endl;
*/
    /*
    // First we'll use it to set the threshold of the LBPHFaceRecognizer
    // to 0.0 without retraining the model. This can be useful if
    // you are evaluating the model:
    //
    model->setThreshold(0.0);
    // Now the threshold of this model is set to 0.0. A prediction
    // now returns -1, as it's impossible to have a distance below
    // it
    predictedLabel = model->predict(fin_test);
    cout << "Predicted class = " << predictedLabel << endl;
    // Show some informations about the model, as there's no cool
    // Model data to display as in Eigenfaces/Fisherfaces.
    // Due to efficiency reasons the LBP images are not stored
    // within the model:
    cout << "Model Information:" << endl;
    string model_info = format("\tLBPH(radius=%i, neighbors=%i, grid_x=%i, grid_y=%i, threshold=%.2f)",
            model->getRadius(),
            model->getNeighbors(),
            model->getGridX(),
            model->getGridY(),
            model->getThreshold());
    cout << model_info << endl;
    // We could get the histograms for example:
    vector<Mat> histograms = model->getHistograms();
    // But should I really visualize it? Probably the length is interesting:
    cout << "Size of the histograms: " << histograms[0].total() << endl;
*/
    return 0;
}
