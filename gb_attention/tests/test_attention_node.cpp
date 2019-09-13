/*********************************************************************
*  Software License Agreement (BSD License)
*
*   Copyright (c) 2019, Intelligent Robotics
*   All rights reserved.
*
*   Redistribution and use in source and binary forms, with or without
*   modification, are permitted provided that the following conditions
*   are met:

*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above
*      copyright notice, this list of conditions and the following
*      disclaimer in the documentation and/or other materials provided
*      with the distribution.
*    * Neither the name of Intelligent Robotics nor the names of its
*      contributors may be used to endorse or promote products derived
*      from this software without specific prior written permission.

*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*   POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Francisco Martín fmrico@gmail.com */

#include <ros/ros.h>

#include <bica_graph/graph_client.h>

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "test_attention_node");
  ros::NodeHandle n;

  ros::Rate rate(1);
  ros::Time start = ros::Time::now();

  bica_graph::GraphClient graph_client;

  ROS_INFO("Creating nodes...");
  graph_client.add_node("leia", "robot");
  graph_client.add_node("table_1", "table");
  graph_client.add_node("table_2", "table");
  graph_client.add_node("table_3", "table");
  graph_client.add_node("waiting_zone", "zone");

  ROS_INFO("done");

  start = ros::Time::now();
  while (ros::ok() && (ros::Time::now() - start).toSec() < 1.0 )
  {
    ROS_INFO("spinnging 0");
    ros::spinOnce();
    rate.sleep();
  }

  graph_client.set_tf_identity("base_footprint", "leia");

  graph_client.add_edge("leia", "want_see", "table_1");
  graph_client.add_edge("leia", "want_see", "table_2");
  tf2::Transform tf_r2t(tf2::Quaternion(0, 0, 0, 1), tf2::Vector3(1.40, 0, 0));
  graph_client.add_edge("leia", tf_r2t, "table_1", true);
  tf2::Transform tf_r2t2(tf2::Quaternion(0, 0, 0, 1), tf2::Vector3(1.4, 1.6, 0));
  graph_client.add_edge("leia", tf_r2t2, "table_2", true);
  tf2::Transform tf_r2t3(tf2::Quaternion(0, 0, 0, 1), tf2::Vector3(1.4, 3.2, 0));
  graph_client.add_edge("leia", tf_r2t3, "table_3", true);
  tf2::Transform tf_r2p(tf2::Quaternion(0, 0, 0, 1), tf2::Vector3(1.4, -1.6, 0));
  graph_client.add_edge("leia", tf_r2t, "waiting_zone", true);



  while (ros::ok())
  {

    graph_client.add_edge("leia", "want_see", "table_1");
    graph_client.add_edge("leia", "want_see", "table_2");
    graph_client.add_edge("leia", "want_see", "table_3");
    graph_client.add_edge("leia", "want_see", "waiting_zone");

    start = ros::Time::now();
    while (ros::ok() && (ros::Time::now() - start).toSec() < 30.0 )
    {
      ros::spinOnce();
      rate.sleep();
    }

    ROS_INFO("removing want see 2");
    graph_client.remove_edge("leia", "want_see", "table_2");

    start = ros::Time::now();
    while (ros::ok() && (ros::Time::now() - start).toSec() < 15.0 )
    {
      ROS_INFO("spinnging 2");
      ros::spinOnce();
      rate.sleep();
    }

  }

  return 0;
}
