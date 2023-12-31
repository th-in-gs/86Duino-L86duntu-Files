#ifndef _ROS_SERVICE_GetRoutePlan_h
#define _ROS_SERVICE_GetRoutePlan_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geographic_msgs/RoutePath.h"
#include "uuid_msgs/UniqueID.h"

namespace geographic_msgs
{

static const char GETROUTEPLAN[] = "geographic_msgs/GetRoutePlan";

  class GetRoutePlanRequest : public ros::Msg
  {
    public:
      uuid_msgs::UniqueID network;
      uuid_msgs::UniqueID start;
      uuid_msgs::UniqueID goal;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->network.serialize(outbuffer + offset);
      offset += this->start.serialize(outbuffer + offset);
      offset += this->goal.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->network.deserialize(inbuffer + offset);
      offset += this->start.deserialize(inbuffer + offset);
      offset += this->goal.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return GETROUTEPLAN; };
    const char * getMD5(){ return "e56ac34268c6d575dabb30f42da4a47a"; };

  };

  class GetRoutePlanResponse : public ros::Msg
  {
    public:
      bool success;
      char * status;
      geographic_msgs::RoutePath plan;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
      uint32_t length_status = strlen( (const char*) this->status);
      memcpy(outbuffer + offset, &length_status, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->status, length_status);
      offset += length_status;
      offset += this->plan.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
      uint32_t length_status;
      memcpy(&length_status, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_status; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_status-1]=0;
      this->status = (char *)(inbuffer + offset-1);
      offset += length_status;
      offset += this->plan.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return GETROUTEPLAN; };
    const char * getMD5(){ return "28ee54f0ccb2ab28b46048ebc6fa5aff"; };

  };

  class GetRoutePlan {
    public:
    typedef GetRoutePlanRequest Request;
    typedef GetRoutePlanResponse Response;
  };

}
#endif
