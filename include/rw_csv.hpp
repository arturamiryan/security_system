#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace cv;
using namespace cv::face;
using namespace std;
using namespace std::filesystem;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';', char kwch = '"') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line, path, classlabel, emp;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, emp, kwch);
        getline(liness, path, kwch);
        getline(liness, emp, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

static void read_csv_grayscale(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';', char kwch = '"') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line, path, classlabel, emp;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, emp, kwch);
        getline(liness, path, kwch);
        getline(liness, emp, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

void write_csv(const string& filename, const string& foldername)
{
    ofstream file(filename);
    if (!file)
    {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }

    vector<string> folders;
    for(auto& p: directory_iterator(foldername))
        folders.push_back(p.path().string());
    
    for(long unsigned int i = 0; i < folders.size(); ++i)
    {
        for(auto& p: directory_iterator(folders[i]))
            file << absolute(p) << ";" << folders[i][folders[i].size()-1] << endl;
    }

    file.close();
}
