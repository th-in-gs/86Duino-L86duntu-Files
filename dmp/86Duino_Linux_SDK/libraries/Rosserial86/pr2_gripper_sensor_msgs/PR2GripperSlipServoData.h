#ifndef _ROS_pr2_gripper_sensor_msgs_PR2GripperSlipServoData_h
#define _ROS_pr2_gripper_sensor_msgs_PR2GripperSlipServoData_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"
#include "pr2_gripper_sensor_msgs/PR2GripperSensorRTState.h"

namespace pr2_gripper_sensor_msgs
{

  class PR2GripperSlipServoData : public ros::Msg
  {
    public:
      ros::Time stamp;
      float deformation;
      float left_fingertip_pad_force;
      float right_fingertip_pad_force;
      float joint_effort;
      bool slip_detected;
      bool deformation_limit_reached;
      bool fingertip_force_limit_reached;
      bool gripper_empty;
      pr2_gripper_sensor_msgs::PR2GripperSensorRTState rtstate;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->stamp.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->stamp.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->stamp.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->stamp.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->stamp.sec);
      *(outbuffer + offset + 0) = (this->stamp.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->stamp.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->stamp.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->stamp.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->stamp.nsec);
      int32_t * val_deformation = (int32_t *) &(this->deformation);
      int32_t exp_deformation = (((*val_deformation)>>23)&255);
      if(exp_deformation != 0)
        exp_deformation += 1023-127;
      int32_t sig_deformation = *val_deformation;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = (sig_deformation<<5) & 0xff;
      *(outbuffer + offset++) = (sig_deformation>>3) & 0xff;
      *(outbuffer + offset++) = (sig_deformation>>11) & 0xff;
      *(outbuffer + offset++) = ((exp_deformation<<4) & 0xF0) | ((sig_deformation>>19)&0x0F);
      *(outbuffer + offset++) = (exp_deformation>>4) & 0x7F;
      if(this->deformation < 0) *(outbuffer + offset -1) |= 0x80;
      int32_t * val_left_fingertip_pad_force = (int32_t *) &(this->left_fingertip_pad_force);
      int32_t exp_left_fingertip_pad_force = (((*val_left_fingertip_pad_force)>>23)&255);
      if(exp_left_fingertip_pad_force != 0)
        exp_left_fingertip_pad_force += 1023-127;
      int32_t sig_left_fingertip_pad_force = *val_left_fingertip_pad_force;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = (sig_left_fingertip_pad_force<<5) & 0xff;
      *(outbuffer + offset++) = (sig_left_fingertip_pad_force>>3) & 0xff;
      *(outbuffer + offset++) = (sig_left_fingertip_pad_force>>11) & 0xff;
      *(outbuffer + offset++) = ((exp_left_fingertip_pad_force<<4) & 0xF0) | ((sig_left_fingertip_pad_force>>19)&0x0F);
      *(outbuffer + offset++) = (exp_left_fingertip_pad_force>>4) & 0x7F;
      if(this->left_fingertip_pad_force < 0) *(outbuffer + offset -1) |= 0x80;
      int32_t * val_right_fingertip_pad_force = (int32_t *) &(this->right_fingertip_pad_force);
      int32_t exp_right_fingertip_pad_force = (((*val_right_fingertip_pad_force)>>23)&255);
      if(exp_right_fingertip_pad_force != 0)
        exp_right_fingertip_pad_force += 1023-127;
      int32_t sig_right_fingertip_pad_force = *val_right_fingertip_pad_force;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = (sig_right_fingertip_pad_force<<5) & 0xff;
      *(outbuffer + offset++) = (sig_right_fingertip_pad_force>>3) & 0xff;
      *(outbuffer + offset++) = (sig_right_fingertip_pad_force>>11) & 0xff;
      *(outbuffer + offset++) = ((exp_right_fingertip_pad_force<<4) & 0xF0) | ((sig_right_fingertip_pad_force>>19)&0x0F);
      *(outbuffer + offset++) = (exp_right_fingertip_pad_force>>4) & 0x7F;
      if(this->right_fingertip_pad_force < 0) *(outbuffer + offset -1) |= 0x80;
      int32_t * val_joint_effort = (int32_t *) &(this->joint_effort);
      int32_t exp_joint_effort = (((*val_joint_effort)>>23)&255);
      if(exp_joint_effort != 0)
        exp_joint_effort += 1023-127;
      int32_t sig_joint_effort = *val_joint_effort;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = (sig_joint_effort<<5) & 0xff;
      *(outbuffer + offset++) = (sig_joint_effort>>3) & 0xff;
      *(outbuffer + offset++) = (sig_joint_effort>>11) & 0xff;
      *(outbuffer + offset++) = ((exp_joint_effort<<4) & 0xF0) | ((sig_joint_effort>>19)&0x0F);
      *(outbuffer + offset++) = (exp_joint_effort>>4) & 0x7F;
      if(this->joint_effort < 0) *(outbuffer + offset -1) |= 0x80;
      union {
        bool real;
        uint8_t base;
      } u_slip_detected;
      u_slip_detected.real = this->slip_detected;
      *(outbuffer + offset + 0) = (u_slip_detected.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->slip_detected);
      union {
        bool real;
        uint8_t base;
      } u_deformation_limit_reached;
      u_deformation_limit_reached.real = this->deformation_limit_reached;
      *(outbuffer + offset + 0) = (u_deformation_limit_reached.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->deformation_limit_reached);
      union {
        bool real;
        uint8_t base;
      } u_fingertip_force_limit_reached;
      u_fingertip_force_limit_reached.real = this->fingertip_force_limit_reached;
      *(outbuffer + offset + 0) = (u_fingertip_force_limit_reached.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->fingertip_force_limit_reached);
      union {
        bool real;
        uint8_t base;
      } u_gripper_empty;
      u_gripper_empty.real = this->gripper_empty;
      *(outbuffer + offset + 0) = (u_gripper_empty.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->gripper_empty);
      offset += this->rtstate.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->stamp.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->stamp.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->stamp.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->stamp.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->stamp.sec);
      this->stamp.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->stamp.nsec);
      uint32_t * val_deformation = (uint32_t*) &(this->deformation);
      offset += 3;
      *val_deformation = ((uint32_t)(*(inbuffer + offset++))>>5 & 0x07);
      *val_deformation |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<3;
      *val_deformation |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<11;
      *val_deformation |= ((uint32_t)(*(inbuffer + offset)) & 0x0f)<<19;
      uint32_t exp_deformation = ((uint32_t)(*(inbuffer + offset++))&0xf0)>>4;
      exp_deformation |= ((uint32_t)(*(inbuffer + offset)) & 0x7f)<<4;
      if(exp_deformation !=0)
        *val_deformation |= ((exp_deformation)-1023+127)<<23;
      if( ((*(inbuffer+offset++)) & 0x80) > 0) this->deformation = -this->deformation;
      uint32_t * val_left_fingertip_pad_force = (uint32_t*) &(this->left_fingertip_pad_force);
      offset += 3;
      *val_left_fingertip_pad_force = ((uint32_t)(*(inbuffer + offset++))>>5 & 0x07);
      *val_left_fingertip_pad_force |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<3;
      *val_left_fingertip_pad_force |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<11;
      *val_left_fingertip_pad_force |= ((uint32_t)(*(inbuffer + offset)) & 0x0f)<<19;
      uint32_t exp_left_fingertip_pad_force = ((uint32_t)(*(inbuffer + offset++))&0xf0)>>4;
      exp_left_fingertip_pad_force |= ((uint32_t)(*(inbuffer + offset)) & 0x7f)<<4;
      if(exp_left_fingertip_pad_force !=0)
        *val_left_fingertip_pad_force |= ((exp_left_fingertip_pad_force)-1023+127)<<23;
      if( ((*(inbuffer+offset++)) & 0x80) > 0) this->left_fingertip_pad_force = -this->left_fingertip_pad_force;
      uint32_t * val_right_fingertip_pad_force = (uint32_t*) &(this->right_fingertip_pad_force);
      offset += 3;
      *val_right_fingertip_pad_force = ((uint32_t)(*(inbuffer + offset++))>>5 & 0x07);
      *val_right_fingertip_pad_force |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<3;
      *val_right_fingertip_pad_force |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<11;
      *val_right_fingertip_pad_force |= ((uint32_t)(*(inbuffer + offset)) & 0x0f)<<19;
      uint32_t exp_right_fingertip_pad_force = ((uint32_t)(*(inbuffer + offset++))&0xf0)>>4;
      exp_right_fingertip_pad_force |= ((uint32_t)(*(inbuffer + offset)) & 0x7f)<<4;
      if(exp_right_fingertip_pad_force !=0)
        *val_right_fingertip_pad_force |= ((exp_right_fingertip_pad_force)-1023+127)<<23;
      if( ((*(inbuffer+offset++)) & 0x80) > 0) this->right_fingertip_pad_force = -this->right_fingertip_pad_force;
      uint32_t * val_joint_effort = (uint32_t*) &(this->joint_effort);
      offset += 3;
      *val_joint_effort = ((uint32_t)(*(inbuffer + offset++))>>5 & 0x07);
      *val_joint_effort |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<3;
      *val_joint_effort |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<11;
      *val_joint_effort |= ((uint32_t)(*(inbuffer + offset)) & 0x0f)<<19;
      uint32_t exp_joint_effort = ((uint32_t)(*(inbuffer + offset++))&0xf0)>>4;
      exp_joint_effort |= ((uint32_t)(*(inbuffer + offset)) & 0x7f)<<4;
      if(exp_joint_effort !=0)
        *val_joint_effort |= ((exp_joint_effort)-1023+127)<<23;
      if( ((*(inbuffer+offset++)) & 0x80) > 0) this->joint_effort = -this->joint_effort;
      union {
        bool real;
        uint8_t base;
      } u_slip_detected;
      u_slip_detected.base = 0;
      u_slip_detected.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->slip_detected = u_slip_detected.real;
      offset += sizeof(this->slip_detected);
      union {
        bool real;
        uint8_t base;
      } u_deformation_limit_reached;
      u_deformation_limit_reached.base = 0;
      u_deformation_limit_reached.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->deformation_limit_reached = u_deformation_limit_reached.real;
      offset += sizeof(this->deformation_limit_reached);
      union {
        bool real;
        uint8_t base;
      } u_fingertip_force_limit_reached;
      u_fingertip_force_limit_reached.base = 0;
      u_fingertip_force_limit_reached.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->fingertip_force_limit_reached = u_fingertip_force_limit_reached.real;
      offset += sizeof(this->fingertip_force_limit_reached);
      union {
        bool real;
        uint8_t base;
      } u_gripper_empty;
      u_gripper_empty.base = 0;
      u_gripper_empty.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->gripper_empty = u_gripper_empty.real;
      offset += sizeof(this->gripper_empty);
      offset += this->rtstate.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "pr2_gripper_sensor_msgs/PR2GripperSlipServoData"; };
    const char * getMD5(){ return "a49728a2e0c40706b3c9b74046f006aa"; };

  };

}
#endif