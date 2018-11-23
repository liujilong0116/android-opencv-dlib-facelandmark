#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
using namespace cv;
using namespace std;

#define MAKE_RGBA(r, g, b, a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#define RGBA_A(p) (((p) & 0xFF000000) >> 24)

CascadeClassifier ccf;


extern "C"
void JNICALL
Java_com_example_administrator_facelandmark_1test_MainActivity_findface(JNIEnv *env, jclass type,
                                                                     jobject bitmap) {
    if (bitmap == NULL) {
        return;
    }else{
        AndroidBitmapInfo info;
        memset(&info, 0, sizeof(info));
        AndroidBitmap_getInfo(env, bitmap, &info);
        void *pixels = NULL;
        AndroidBitmap_lockPixels(env, bitmap, &pixels);
        Mat src(info.height, info.width, CV_8UC4, pixels);
        cvtColor(src, src, CV_BGRA2RGB);

        Mat img_v, img_hsv, img_struct,anchor_color_hsv, anchor_color, anchor1;
        int r = 30;
        double eps = 0.01;
        vector<cv::Mat> hsv_vec;
        cvtColor(src, img_hsv, CV_BGR2HSV);


        dlib::shape_predictor pose_model;
        ccf.load("/sdcard/facelandmark/haarcascade_frontalface_alt.xml");
        dlib::deserialize("/sdcard/facelandmark/shape_predictor_68_face_landmarks.dat") >> pose_model;
        vector<Rect> faces;
        Mat gray;
        cvtColor(src, gray, CV_BGR2GRAY);
        equalizeHist(gray, gray);
        ccf.detectMultiScale(gray, faces, 1.1, 3, 0, Size(300, 300), Size(2000, 2000));
        for (vector<Rect>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
        {
            rectangle(src, *iter, Scalar(0, 0, 255), 2, 8); //画出脸部矩形
        }
        dlib::cv_image<dlib::bgr_pixel> cimg(src);
        dlib::rectangle faces1;
        faces1.set_top(faces[0].tl().y);
        faces1.set_bottom(faces[0].br().y);
        faces1.set_left(faces[0].tl().x);
        faces1.set_right(faces[0].br().x);
        vector<dlib::full_object_detection> shapes;
        shapes.push_back(pose_model(cimg, faces1));

        if (!shapes.empty())
        {
            for (int j = 0; j < shapes.size(); j++)
            {
                for (int i = 0; i < 68; i++)
                {
                    circle(src, cv::Point(shapes[j].part(i).x(), shapes[j].part(i).y()),
                           10, Scalar(0, 255, 0), -1);
                }
            }
        }

        Mat output = src.clone();

        int a = 0, r1 = 0, g = 0, b = 0;
        for (int y = 0; y < info.height; ++y) {
            // From left to right
            for (int x = 0; x < info.width; ++x) {
                int *pixel = NULL;
                pixel = ((int *) pixels) + y * info.width + x;
                r1 = output.at<Vec3b>(y, x)[0];
                g = output.at<Vec3b>(y, x)[1];
                b = output.at<Vec3b>(y, x)[2];
                a = RGBA_A(*pixel);
                *pixel = MAKE_RGBA(r1, g, b, a);
            }
        }
        AndroidBitmap_unlockPixels(env, bitmap);
    }
}
