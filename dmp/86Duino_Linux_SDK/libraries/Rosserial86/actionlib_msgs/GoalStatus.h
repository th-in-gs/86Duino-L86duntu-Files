#ifndef _ROS_actionlib_msgs_GoalStatus_h
#define _ROS_actionlib_msgs_GoalStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "actionlib_msgs/GoalID.h"

namespace actionlib_msgs
{

  class GoalStatus : public ros::Msg
  {
    public:
      actionlib_msgs::GoalID goal_id;
      uint8_t status;
      char * text;
      enum { PENDING =  0    };
      enum { ACTIVE =  1    };
      enum { PREEMPTED =  2    };
      enum { SUCCEEDED =  3    };
      enum { ABORTED =  4    };
      enum { REJECTED =  5    };
      enum { PREEMPTING =  6    };
      enum { RECALLING =  7    };
      enum { RECALLED =  8    };
      enum { LOST =  9    };

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->goal_id.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->status);
      uint32_t length_text = strlen( (const char*) this->text);
      memcpy(outbuffer + offset, &length_text, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->text, length_text);
      offset += length_text;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->goal_id.deserialize(inbuffer + offset);
      this->status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->status);
      uint32_t length_text;
      memcpy(&length_text, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_text; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_text-1]=0;
      this->text = (char *)(inbuffer + offset-1);
      offset += length_text;
     return offset;
    }

    const char * getType(){ return "actionlib_msgs/GoalStatus"; };
    const char * getMD5(){ return "d388f9b87b3c471f784434d671988d4a"; };

  };

}
#endif