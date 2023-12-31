#ifndef _ROS_pr2_plugs_msgs_AlignBaseAction_h
#define _ROS_pr2_plugs_msgs_AlignBaseAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "pr2_plugs_msgs/AlignBaseActionGoal.h"
#include "pr2_plugs_msgs/AlignBaseActionResult.h"
#include "pr2_plugs_msgs/AlignBaseActionFeedback.h"

namespace pr2_plugs_msgs
{

  class AlignBaseAction : public ros::Msg
  {
    public:
      pr2_plugs_msgs::AlignBaseActionGoal action_goal;
      pr2_plugs_msgs::AlignBaseActionResult action_result;
      pr2_plugs_msgs::AlignBaseActionFeedback action_feedback;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->action_goal.serialize(outbuffer + offset);
      offset += this->action_result.serialize(outbuffer + offset);
      offset += this->action_feedback.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->action_goal.deserialize(inbuffer + offset);
      offset += this->action_result.deserialize(inbuffer + offset);
      offset += this->action_feedback.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "pr2_plugs_msgs/AlignBaseAction"; };
    const char * getMD5(){ return "ab27b6acd819da176d84314a6e891150"; };

  };

}
#endif