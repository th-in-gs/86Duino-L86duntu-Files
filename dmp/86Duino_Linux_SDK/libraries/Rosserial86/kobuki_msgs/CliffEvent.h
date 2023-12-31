#ifndef _ROS_kobuki_msgs_CliffEvent_h
#define _ROS_kobuki_msgs_CliffEvent_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace kobuki_msgs
{

  class CliffEvent : public ros::Msg
  {
    public:
      uint8_t sensor;
      uint8_t state;
      uint16_t bottom;
      enum { LEFT =  0 };
      enum { CENTER =  1 };
      enum { RIGHT =  2 };
      enum { FLOOR =  0 };
      enum { CLIFF =  1 };

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->sensor >> (8 * 0)) & 0xFF;
      offset += sizeof(this->sensor);
      *(outbuffer + offset + 0) = (this->state >> (8 * 0)) & 0xFF;
      offset += sizeof(this->state);
      *(outbuffer + offset + 0) = (this->bottom >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->bottom >> (8 * 1)) & 0xFF;
      offset += sizeof(this->bottom);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->sensor =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->sensor);
      this->state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->state);
      this->bottom =  ((uint16_t) (*(inbuffer + offset)));
      this->bottom |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->bottom);
     return offset;
    }

    const char * getType(){ return "kobuki_msgs/CliffEvent"; };
    const char * getMD5(){ return "c5b106efbb1427a94f517c5e05f06295"; };

  };

}
#endif