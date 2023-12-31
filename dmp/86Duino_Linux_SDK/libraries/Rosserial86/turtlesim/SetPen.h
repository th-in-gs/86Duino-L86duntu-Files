#ifndef _ROS_SERVICE_SetPen_h
#define _ROS_SERVICE_SetPen_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace turtlesim
{

static const char SETPEN[] = "turtlesim/SetPen";

  class SetPenRequest : public ros::Msg
  {
    public:
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t width;
      uint8_t off;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->r >> (8 * 0)) & 0xFF;
      offset += sizeof(this->r);
      *(outbuffer + offset + 0) = (this->g >> (8 * 0)) & 0xFF;
      offset += sizeof(this->g);
      *(outbuffer + offset + 0) = (this->b >> (8 * 0)) & 0xFF;
      offset += sizeof(this->b);
      *(outbuffer + offset + 0) = (this->width >> (8 * 0)) & 0xFF;
      offset += sizeof(this->width);
      *(outbuffer + offset + 0) = (this->off >> (8 * 0)) & 0xFF;
      offset += sizeof(this->off);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->r =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->r);
      this->g =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->g);
      this->b =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->b);
      this->width =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->width);
      this->off =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->off);
     return offset;
    }

    const char * getType(){ return SETPEN; };
    const char * getMD5(){ return "9f452acce566bf0c0954594f69a8e41b"; };

  };

  class SetPenResponse : public ros::Msg
  {
    public:

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return SETPEN; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class SetPen {
    public:
    typedef SetPenRequest Request;
    typedef SetPenResponse Response;
  };

}
#endif
