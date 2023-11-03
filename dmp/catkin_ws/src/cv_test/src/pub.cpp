#include <ros/ros.h>
#include <iostream>
#include <sstream>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/Image.h>

using namespace std;
using namespace cv;

int main( int argc, char** argv)
{
  ros::init(argc, argv, "pub");
  ros::NodeHandle nh;
  ros::Publisher out_img_pub = 
    nh.advertise<sensor_msgs::Image>("image_out", 2, false);

  Mat frame;
  VideoCapture cap(0);
  if(!cap.isOpened())
  {
    ROS_ERROR("fail to open!");
    return -1;
  }

  cap.set(CV_CAP_PROP_FRAME_HEIGHT,192);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 256);
  cap.set(CV_CAP_PROP_FPS, 5);

  ros::Rate loop_rate(5);
  cv_bridge::CvImage cv_img;
  stringstream ss;

  for(int i=0; ros::ok(); i++)
  {
    cap.read(frame);
    cvtColor(frame, frame, CV_BGR2GRAY);
    ss << "frame " << i;
    cv_img.encoding = "mono8";
    cv_img.header.stamp = ros::Time::now();
    cv_img.header.frame_id = ss.str();
    cv_img.image = frame;
    out_img_pub.publish( cv_img.toImageMsg() );
    //ROS_INFO("Publish Frame %d !", i);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
