#ifndef _ROS_pr2_plugs_msgs_RechargeActionGoal_h
#define _ROS_pr2_plugs_msgs_RechargeActionGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalID.h"
#include "pr2_plugs_msgs/RechargeGoal.h"

namespace pr2_plugs_msgs
{

  class RechargeActionGoal : public ros::Msg
  {
    public:
      std_msgs::Header header;
      actionlib_msgs::GoalID goal_id;
      pr2_plugs_msgs::RechargeGoal goal;

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

    const char * getType(){ return "pr2_plugs_msgs/RechargeActionGoal"; };
    const char * getMD5(){ return "ba67bcbf78c1cbe2b51f324aac5bd073"; };

  };

}
#endif