#include <ros/ros.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

using namespace cv;

cv::VideoWriter video;

void callback(const sensor_msgs::ImageConstPtr& msg){
    
    cv_bridge::CvImagePtr cv_ptr;
    try{
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        video.write(cv_ptr->image);
    } catch (cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s",e.what());
        return;
    }
    
    
    
}

int main (int argc, char** argv){
    
     time_t t = time(0);   // get time now
     struct tm * now = localtime( & t );

     char buffer [80];
     strftime (buffer,80,"%Y-%m-%d-%H:%M:%S",now);
     String s = buffer;
     s = "/home/rover/videos/" + s + ".avi";
     
    video = VideoWriter(s,CV_FOURCC('M','J','P','G'),30, Size(1280,720));
    
    ros::init(argc,argv,"rover_image_saver");
    ros::NodeHandle n;
    image_transport::ImageTransport it(n);
    image_transport::Subscriber img_sub = it.subscribe("rover/ground_camera/image_raw", 1, callback);
    ros::spin();
    video.release();
    return 0;
}
