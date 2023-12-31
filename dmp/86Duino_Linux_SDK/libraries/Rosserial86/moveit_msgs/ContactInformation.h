#ifndef _ROS_moveit_msgs_ContactInformation_h
#define _ROS_moveit_msgs_ContactInformation_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Vector3.h"

namespace moveit_msgs
{

  class ContactInformation : public ros::Msg
  {
    public:
      std_msgs::Header header;
      geometry_msgs::Point position;
      geometry_msgs::Vector3 normal;
      float depth;
      char * contact_body_1;
      uint32_t body_type_1;
      char * contact_body_2;
      uint32_t body_type_2;
      enum { ROBOT_LINK = 0 };
      enum { WORLD_OBJECT = 1 };
      enum { ROBOT_ATTACHED = 2 };

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->position.serialize(outbuffer + offset);
      offset += this->normal.serialize(outbuffer + offset);
      int32_t * val_depth = (int32_t *) &(this->depth);
      int32_t exp_depth = (((*val_depth)>>23)&255);
      if(exp_depth != 0)
        exp_depth += 1023-127;
      int32_t sig_depth = *val_depth;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = (sig_depth<<5) & 0xff;
      *(outbuffer + offset++) = (sig_depth>>3) & 0xff;
      *(outbuffer + offset++) = (sig_depth>>11) & 0xff;
      *(outbuffer + offset++) = ((exp_depth<<4) & 0xF0) | ((sig_depth>>19)&0x0F);
      *(outbuffer + offset++) = (exp_depth>>4) & 0x7F;
      if(this->depth < 0) *(outbuffer + offset -1) |= 0x80;
      uint32_t length_contact_body_1 = strlen( (const char*) this->contact_body_1);
      memcpy(outbuffer + offset, &length_contact_body_1, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->contact_body_1, length_contact_body_1);
      offset += length_contact_body_1;
      *(outbuffer + offset + 0) = (this->body_type_1 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->body_type_1 >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->body_type_1 >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->body_type_1 >> (8 * 3)) & 0xFF;
      offset += sizeof(this->body_type_1);
      uint32_t length_contact_body_2 = strlen( (const char*) this->contact_body_2);
      memcpy(outbuffer + offset, &length_contact_body_2, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->contact_body_2, length_contact_body_2);
      offset += length_contact_body_2;
      *(outbuffer + offset + 0) = (this->body_type_2 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->body_type_2 >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->body_type_2 >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->body_type_2 >> (8 * 3)) & 0xFF;
      offset += sizeof(this->body_type_2);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->position.deserialize(inbuffer + offset);
      offset += this->normal.deserialize(inbuffer + offset);
      uint32_t * val_depth = (uint32_t*) &(this->depth);
      offset += 3;
      *val_depth = ((uint32_t)(*(inbuffer + offset++))>>5 & 0x07);
      *val_depth |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<3;
      *val_depth |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<11;
      *val_depth |= ((uint32_t)(*(inbuffer + offset)) & 0x0f)<<19;
      uint32_t exp_depth = ((uint32_t)(*(inbuffer + offset++))&0xf0)>>4;
      exp_depth |= ((uint32_t)(*(inbuffer + offset)) & 0x7f)<<4;
      if(exp_depth !=0)
        *val_depth |= ((exp_depth)-1023+127)<<23;
      if( ((*(inbuffer+offset++)) & 0x80) > 0) this->depth = -this->depth;
      uint32_t length_contact_body_1;
      memcpy(&length_contact_body_1, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_contact_body_1; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_contact_body_1-1]=0;
      this->contact_body_1 = (char *)(inbuffer + offset-1);
      offset += length_contact_body_1;
      this->body_type_1 =  ((uint32_t) (*(inbuffer + offset)));
      this->body_type_1 |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->body_type_1 |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->body_type_1 |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->body_type_1);
      uint32_t length_contact_body_2;
      memcpy(&length_contact_body_2, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_contact_body_2; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_contact_body_2-1]=0;
      this->contact_body_2 = (char *)(inbuffer + offset-1);
      offset += length_contact_body_2;
      this->body_type_2 =  ((uint32_t) (*(inbuffer + offset)));
      this->body_type_2 |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->body_type_2 |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->body_type_2 |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->body_type_2);
     return offset;
    }

    const char * getType(){ return "moveit_msgs/ContactInformation"; };
    const char * getMD5(){ return "116228ca08b0c286ec5ca32a50fdc17b"; };

  };

}
#endif