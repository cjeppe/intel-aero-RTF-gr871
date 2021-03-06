#include "ros/ros.h"
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <gazebo_msgs/ModelStates.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

gazebo_msgs::ModelStates gazebo_model_state;
void callback(const gazebo_msgs::ModelStates::ConstPtr& msg){
    gazebo_model_state = *msg;

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "true_position");
    ros::NodeHandle nh;

    ros::Subscriber gazebo_state_sub = nh.subscribe<gazebo_msgs::ModelStates>
            ("gazebo/model_states", 7, callback); 

    ros::Publisher true_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("mavros/mocap/pose", 7); 

  ros::Rate loop_rate(7);

  
int count = 1;
  while (ros::ok())
  {

    geometry_msgs::PoseStamped pose;

    if (gazebo_model_state.pose.size() >= 3) 
    {
       pose.pose = gazebo_model_state.pose[2];
    }
    
    pose.header.stamp=ros::Time::now();
    pose.header.seq=count;
    pose.header.frame_id=1;

    true_pos_pub.publish(pose);
    ros::spinOnce();
    count++;
    loop_rate.sleep();
  
  }

  //ros::spin();
  return 0;
}
