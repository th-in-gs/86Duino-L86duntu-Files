#ifndef _ROS_moveit_msgs_MoveGroupFeedback_h
#define _ROS_moveit_msgs_MoveGroupFeedback_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace moveit_msgs
{

  class MoveGroupFeedback : public ros::Msg
  {
    public:
      char * state;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_state = strlen( (const char*) this->state);
      memcpy(outbuffer + offset, &length_state, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->state, length_state);
      offset += length_state;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_state;
      memcpy(&length_state, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_state; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_state-1]=0;
      this->state = (char *)(inbuffer + offset-1);
      offset += length_state;
     return offset;
    }

    const char * getType(){ return "moveit_msgs/MoveGroupFeedback"; };
    const char * getMD5(){ return "af6d3a99f0fbeb66d3248fa4b3e675fb"; };

  };

}
#endif