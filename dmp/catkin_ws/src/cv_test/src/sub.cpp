#include <ros/ros.h>
#include <iostream>
#include <sstream>
#include <image_transport/image_transport.h>
#include <std_msgs/String.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Image.h>
#include <message_filters/time_sequencer.h>

using namespace std;
using namespace cv;

void imageCallback(const sensor_msgs::ImageConstPtr& color_img)
{
  //ros::Duration diff = ros::Time::now() - color_img->header.stamp;
  //if( (double)diff.nsec/1000000000.0 > 0.5 )
    //return ;
  cv_bridge::CvImagePtr cv_img;
  cv::Mat frame;
  try
  {
      cv_img = cv_bridge::toCvCopy(color_img,
      sensor_msgs::image_encodings::MONO8);
      frame = cv_img->image;
  }
  catch (cv_bridge::Exception& e)
  {
      ROS_ERROR("cv_bridge exception: \%s", e.what());
      return ;
  }
  cv::imshow("cap", frame);
  cv::waitKey(1);
}

int main( int argc, char** argv)
{
  ros::init(argc, argv, "sub");
  ros::NodeHandle nh;
  //message_filters::Subscriber<sensor_msgs::ImagePtr> sub(nh, "image_out", 1);
  //message_filters::TimeSequencer<sensor_msgs::ImagePtr> seq(sub, ros::Duration(0.1), ros::Duration(0.01), 10);
  //seq.registerCallback(imageCallback);
  ros::Subscriber sub = nh.subscribe("image_out", 2, &imageCallback);
  ros::spin();
  return 0;
}
