#ifndef _ROS_pr2_common_action_msgs_TuckArmsActionResult_h
#define _ROS_pr2_common_action_msgs_TuckArmsActionResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "pr2_common_action_msgs/TuckArmsResult.h"

namespace pr2_common_action_msgs
{

  class TuckArmsActionResult : public ros::Msg
  {
    public:
      std_msgs::Header header;
      actionlib_msgs::GoalStatus status;
      pr2_common_action_msgs::TuckArmsResult result;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->status.serialize(outbuffer + offset);
      offset += this->result.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->status.deserialize(inbuffer + offset);
      offset += this->result.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "pr2_common_action_msgs/TuckArmsActionResult"; };
    const char * getMD5(){ return "a151ea69df95c9525872b19d347d7f8e"; };

  };

}
#endif