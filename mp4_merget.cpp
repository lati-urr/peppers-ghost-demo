#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;

cv::Mat rotate(cv::Mat img, int angle, int scale){
  cv::Point2f center(img.cols*0.5, img.rows*0.5);
  const cv::Mat affine_matrix = cv::getRotationMatrix2D(center, angle, scale);
  cv::Mat rotated;
  cv::warpAffine(img, rotated, affine_matrix, img.size());
  return rotated;
}

int main(){
  cv::Mat L_img, F_img, R_img;
  cv::VideoCapture L_cap("break_box_left.mp4");
  cv::VideoCapture F_cap("break_box_front.mp4");
  cv::VideoCapture R_cap("break_box_right.mp4");
  std::cout << cv::getBuildInformation() << std::endl;
  //int max_frame = F_cap.get(CV_CAP_PROP_FRAME_COUNT);
  cv::VideoWriter writer("out.avi", CV_FOURCC_DEFAULT, 24, cv::Size(1024, 600), true);
  /*  for(int i=0;i<max_frame;i++){
    L_cap >> L_img;
    F_cap >> F_img;
    R_cap >> R_img;

    cv::resize(L_img, L_img, cv::Size(), 0.36, 0.36);//346x194
    cv::resize(F_img, F_img, cv::Size(), 0.36, 0.36);
    cv::resize(R_img, R_img, cv::Size(), 0.36, 0.36);
    std::cout << L_img.cols << L_img.rows << std::endl;

    cv::Mat L_trim_img(L_img, cv::Rect(76, 0, 194, 194));
    cv::Mat F_trim_img(F_img, cv::Rect(76, 0, 194, 194));
    cv::Mat R_trim_img(R_img, cv::Rect(76, 0, 194, 194));

    cv::Mat L_rotated = rotate(L_trim_img, 270, 1);
    cv::Mat R_rotated = rotate(R_trim_img, 90, 1);

    cv::Mat output_frame(cv::Size(1024, 600), CV_8UC3, CV_RGB(0, 0, 0));
    cv::Mat roi_left(output_frame, cv::Rect(212, 200, L_trim_img.cols, L_trim_img.cols));
    cv::Mat roi_front(output_frame, cv::Rect(412, 400, F_trim_img.cols, F_trim_img.rows));
    cv::Mat roi_right(output_frame, cv::Rect(612, 200, R_trim_img.cols, R_trim_img.rows));
    L_rotated.copyTo(roi_left);
    F_trim_img.copyTo(roi_front);
    R_rotated.copyTo(roi_right);
    
    // full screen
    //cvNamedWindow("output", CV_WINDOW_NORMAL);
    //cvSetWindowProperty( "output", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );
    writer << output_frame;
    cv::imshow("output", output_frame);

    int key = cv::waitKey(1);
    if(key == 113)//qボタンが押されたとき
      {
	break;
      }
      }*/
  cv::destroyAllWindows();
  return 0;
}
