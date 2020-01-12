#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

ros::Subscriber cmd_vel_sub;
ros::Publisher lf_pub,lm_pub,lb_pub,rf_pub,rm_pub,rb_pub;
    

void callback(const geometry_msgs::Twist twist){
    double y,omega;
    omega = twist.angular.z*3;
    y = twist.linear.x;
    
    double left_vel, right_vel;
    
    left_vel = 3*y - omega/1.2;
    right_vel = 3*y + omega/1.2;
    
    std_msgs::Float64 left_command, right_command;
    left_command.data = -left_vel;
    right_command.data = right_vel;
    
    lf_pub.publish(left_command);
    lm_pub.publish(left_command);
    lb_pub.publish(left_command);
    rf_pub.publish(right_command);
    rm_pub.publish(right_command);
    rb_pub.publish(right_command);
}
    

int main(int argc, char** argv){
    
    ros::init(argc,argv,"rover_base_controller");
    ros::NodeHandle n;
    
    
    lf_pub = n.advertise<std_msgs::Float64>("/rover/lf_controller/command",0);
    lm_pub = n.advertise<std_msgs::Float64>("/rover/lm_controller/command",0);
    lb_pub = n.advertise<std_msgs::Float64>("/rover/lb_controller/command",0);
    rf_pub = n.advertise<std_msgs::Float64>("/rover/rf_controller/command",0);
    rm_pub = n.advertise<std_msgs::Float64>("/rover/rm_controller/command",0);
    rb_pub = n.advertise<std_msgs::Float64>("/rover/rb_controller/command",0);
    
    cmd_vel_sub = n.subscribe<geometry_msgs::Twist>("/rover/cmd_vel",1,&callback);    
    
    ros::spin();
}
