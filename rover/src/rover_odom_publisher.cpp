#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/JointState.h>
#include <cmath>
#include <ecl/linear_algebra.hpp>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

using namespace ecl::linear_algebra;

typedef Matrix<float,3,1> Vector3f;
typedef Matrix<float,8,1> Vector8f;

bool is_init = false;
double lastPos[6];
double x = 0,y = 0,th = 0+M_PI/2;

ros::Time Lasttime;

ros::Publisher odom_pub;


void callback(const sensor_msgs::JointState ptr){
	

	double radius = 0.14;
	//ptr = [lb,lf,lm,rb,rf,rm]
	double wheel[6] = {-ptr.position[1]*radius,-ptr.position[2]*radius,-ptr.position[0]*radius,ptr.position[4]*radius,ptr.position[5]*radius,ptr.position[3]*radius};
	
	double dp[6];
	double wheel_vel[6] = {-ptr.velocity[1]*radius,-ptr.velocity[2]*radius,-ptr.velocity[0]*radius,ptr.velocity[4]*radius,ptr.velocity[5]*radius,ptr.velocity[3]*radius};
	
	
	if (!is_init){
		for(int i = 0; i < 6; i++){
			lastPos[i] = wheel[i]; 
		}
		Lasttime = ros::Time::now();
		is_init = true;
	}

	auto nowTime = ros::Time::now();
	auto dt = (Lasttime - nowTime).toSec();
	Lasttime = nowTime;


	for (int i = 0; i < 6;i++){
		dp[i] = wheel[i] - lastPos[i];
		lastPos[i] = wheel[i];
	}

	double left_dp = (dp[0]+dp[1]+dp[2])/3;
	double right_dp = (dp[3]+dp[4]+dp[5])/3;


	double vx,vy,omega;
	vx = (left_dp+right_dp)/2;
	vy = 0;
	omega = (right_dp - left_dp) / 1.40;
	
	double delta_x = (vx * cos(th) - vy * sin(th)) * 1;
	double delta_y = (vx * sin(th) + vy * cos(th)) * 1;
	double delta_th = omega * 1;

	x += delta_x;
	y += delta_y;
	th += delta_th;
	
	static tf::TransformBroadcaster odom_broadcaster;
	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    	//first, we'll publish the transform over tf
    	geometry_msgs::TransformStamped odom_trans;
    	odom_trans.header.stamp = ros::Time::now();
    	odom_trans.header.frame_id = "odom";
    	odom_trans.child_frame_id = "base_link";

    	odom_trans.transform.translation.x = x;
    	odom_trans.transform.translation.y = y;
    	odom_trans.transform.translation.z = 0.0;
    	odom_trans.transform.rotation = odom_quat;

    	//send the transform
    	odom_broadcaster.sendTransform(odom_trans);

	ROS_INFO("The odometry result is: X: [%f] Y: [%f] Theta: [%f] | Heading: [%f]",x,y,th*180/3.14159,atan2(sin(th),cos(th))*180/3.14159);

	//ROS_INFO("THE ODOMETRY RESULT: vx: [%f] vy: [%f] vz: [%f]",vx,vy,omega);
	//ROS_INFO("|[%f],[%f]||[%f],[%f]||[%f],[%f]||[%f],[%f]|",vel(0,0),vel(1,0),vel(2,0),vel(3,0),vel(4,0),vel(5,0),vel(6,0),vel(7,0));


	double left_vel = (wheel_vel[0]+wheel_vel[1]+wheel_vel[2]);
	double right_vel = (wheel_vel[3]+wheel_vel[4]+wheel_vel[5]);

	double vel_x = (-(left_vel+right_vel)/2 * sin(th)) * 1;
	double vel_y = ((left_vel+right_vel)/2 * cos(th)) * 1; 

	nav_msgs::Odometry odom;
	odom.header.stamp = ros::Time::now();
	odom.header.frame_id = "odom";

	odom.pose.pose.position.x = x;
	odom.pose.pose.position.y = y;
	odom.pose.pose.position.z = 0;

	odom.pose.pose.orientation = odom_quat;

	odom.child_frame_id = "base_link";
	odom.twist.twist.linear.x = vel_x;
	odom.twist.twist.linear.y = vel_y;
	odom.twist.twist.angular.z = (right_vel-left_vel)/1.4;

	odom_pub.publish(odom);
}





int main(int argc, char** argv){
  double x,y,theta;


  ros::init(argc,argv,"odom_publisher");

  ros::NodeHandle n;

  ros::Subscriber sub;

  odom_pub = n.advertise<nav_msgs::Odometry>("odom",50);

  sub = n.subscribe<sensor_msgs::JointState>("rover/joint_states",1,&callback);

  ros::spin();
  
  return 0;
}
