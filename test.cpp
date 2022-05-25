#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"

using namespace std;

cv::Mat rotate(cv::Mat img, int angle, int scale){
  cv::Point2f center(img.cols*0.5, img.rows*0.5);
  const cv::Mat affine_matrix = cv::getRotationMatrix2D(center, angle, scale);
  cv::Mat rotated;
  cv::warpAffine(img, rotated, affine_matrix, img.size());
  return rotated;
}

cv::Mat mask_background(cv::Mat img, int area_threshold, int BW_threshold){
  // BW_threshold:0 => 大津法 other => 閾値手動設定
  cv::Mat gray, bin;
  cv::Mat copy = img.clone();
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
  if(BW_threshold == 0){
    cv::threshold(gray, bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
  }
  else{
    cv::threshold(gray, bin, BW_threshold, 255, CV_THRESH_BINARY);
  }
  std::vector< std::vector< cv::Point > > contours;
  std::vector< std::vector< cv::Point > > contours_sub;
  cv::findContours(bin, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  for(int i=0; i<contours.size(); i++){
    double area = cv::contourArea(contours.at(i));
    if(area>=area_threshold){
      //輪郭を緑で描画(デバッグ用)
      // cv::polylines(L_trim_frame, contours.at(i), true, cv::Scalar(0, 255, 0), 2);
      // std::cout << area << std::endl;
      contours_sub.push_back(contours.at(i));
    }
  }
  cv::Mat mask_area = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
  cv::drawContours(mask_area, contours_sub, -1, cv::Scalar(255), -1);
  cv::Mat masked_img;
  copy.copyTo(masked_img, mask_area);
  return masked_img;
}

int main(int argh, char* argv[])
{
  cv::VideoCapture cap_left(0);//デバイスのオープン
  cv::VideoCapture cap_front(1);
  cv::VideoCapture cap_right(2);

  if(!cap_left.isOpened() || !cap_front.isOpened())//カメラデバイスが正常にオープンしたか確認．
    {
      return -1;
    }
  while(1)
    {
      cv::Mat L_frame;
      cv::Mat F_frame;
      cv::Mat R_frame;
      
      cap_left >> L_frame;
      cap_front >> F_frame;
      cap_right >> R_frame;

      cv::Mat L_trim_frame(L_frame, cv::Rect(60, 0, 480, 480));
      cv::Mat F_trim_frame(F_frame, cv::Rect(60, 0, 480, 480));
      cv::Mat R_trim_frame(R_frame, cv::Rect(60, 0, 480, 480));
      
      // 物体検出のプログラム
      // cv::Mat L_masked = mask_background(L_trim_frame, 1500, 80);
      // cv::Mat F_masked = mask_background(F_trim_frame, 1500, 0);
      // cv::Mat R_masked = mask_background(R_trim_frame, 1500, 70);
      
      // 縮小
      cv::resize(L_trim_frame, L_trim_frame, cv::Size(), 0.41, 0.41);//197x197
      cv::resize(F_trim_frame, F_trim_frame, cv::Size(), 0.41, 0.41);
      cv::resize(R_trim_frame, R_trim_frame, cv::Size(), 0.41, 0.41);

      // left rightの回転
      cv::Mat L_rotated = rotate(L_trim_frame, 270, 1);
      cv::Mat R_rotated = rotate(R_trim_frame, 90, 1);

      // 合成
      cv::Mat output_frame(cv::Size(1024, 600), CV_8UC3, CV_RGB(0, 0, 0));
      cv::Mat roi_left(output_frame, cv::Rect(212, 200, L_trim_frame.cols, L_trim_frame.cols));
      cv::Mat roi_front(output_frame, cv::Rect(412, 400, F_trim_frame.cols, F_trim_frame.rows));
      cv::Mat roi_right(output_frame, cv::Rect(612, 200, R_trim_frame.cols, R_trim_frame.rows));
      // L_rotated.copyTo(roi_left);
      // F_trim_frame.copyTo(roi_front);
      // R_rotated.copyTo(roi_right);
      L_rotated.copyTo(roi_left);
      F_trim_frame.copyTo(roi_front);
      R_rotated.copyTo(roi_right);

      // full screen
      cvNamedWindow("output", CV_WINDOW_NORMAL);
      cvSetWindowProperty( "output", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );
      cv::imshow("output", output_frame);
      int key = cv::waitKey(1);
      if(key == 113)//qボタンが押されたとき
	{
	  break;
	}
    }
  cv::destroyAllWindows();
  return 0;
}
