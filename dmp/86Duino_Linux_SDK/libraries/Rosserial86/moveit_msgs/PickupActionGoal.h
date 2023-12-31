#ifndef _ROS_moveit_msgs_PickupActionGoal_h
#define _ROS_moveit_msgs_PickupActionGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalID.h"
#include "moveit_msgs/PickupGoal.h"

namespace moveit_msgs
{

  class PickupActionGoal : public ros::Msg
  {
    public:
      std_msgs::Header header;
      actionlib_msgs::GoalID goal_id;
      moveit_msgs::PickupGoal goal;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->goal_id.serialize(outbuffer + offset);
      offset += this->goal.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->goal_id.deserialize(inbuffer + offset);
      offset += this->goal.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "moveit_msgs/PickupActionGoal"; };
    const char * getMD5(){ return "9e12196da542c9a26bbc43e9655a1906"; };

  };

}
#endif