#ifndef _ROS_create_node_TurtlebotSensorState_h
#define _ROS_create_node_TurtlebotSensorState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace create_node
{

  class TurtlebotSensorState : public ros::Msg
  {
    public:
      std_msgs::Header header;
      uint8_t bumps_wheeldrops;
      bool wall;
      bool cliff_left;
      bool cliff_front_left;
      bool cliff_front_right;
      bool cliff_right;
      bool virtual_wall;
      uint8_t motor_overcurrents;
      uint8_t dirt_detector_left;
      uint8_t dirt_detector_right;
      uint8_t remote_opcode;
      uint8_t buttons;
      float distance;
      float angle;
      uint8_t charging_state;
      uint16_t voltage;
      int16_t current;
      int8_t temperature;
      uint16_t charge;
      uint16_t capacity;
      uint16_t wall_signal;
      uint16_t cliff_left_signal;
      uint16_t cliff_front_left_signal;
      uint16_t cliff_front_right_signal;
      uint16_t cliff_right_signal;
      uint8_t user_digital_outputs;
      uint8_t user_digital_inputs;
      uint16_t user_analog_input;
      uint8_t charging_sources_available;
      uint8_t oi_mode;
      uint8_t song_number;
      bool song_playing;
      uint8_t number_of_stream_packets;
      int32_t requested_velocity;
      int32_t requested_radius;
      int32_t requested_right_velocity;
      int32_t requested_left_velocity;
      enum { OI_MODE_OFF =  0 };
      enum { OI_MODE_PASSIVE =  1 };
      enum { OI_MODE_SAFE =  2 };
      enum { OI_MODE_FULL =  3 };
      enum { REMOTE_LEFT =  129 };
      enum { REMOTE_FORWARD =  130 };
      enum { REMOTE_RIGHT =  131 };
      enum { REMOTE_SPOT =  132 };
      enum { REMOTE_MAX =  133 };
      enum { REMOTE_SMALL =  134 };
      enum { REMOTE_MEDIUM =  135 };
      enum { REMOTE_LARGE =  136 };
      enum { REMOTE_CLEAN =  136 };
      enum { REMOTE_PAUSE =  137 };
      enum { REMOTE_POWER =  138 };
      enum { REMOTE_ARC_LEFT =  139 };
      enum { REMOTE_ARC_RIGHT =  140 };
      enum { REMOTE_DRIVE_STOP =  141 };
      enum { REMOTE_SEND_ALL =  142 };
      enum { REMOTE_SEEK_DOCK =  143 };
      enum { REMOTE_RESERVED =  240 };
      enum { REMOTE_FORCE_FIELD =  242 };
      enum { REMOTE_GREEN_BUOY =  244 };
      enum { REMOTE_GREEN_BUOY_AND_FORCE_FIELD =  246 };
      enum { REMOTE_RED_BUOY =  248 };
      enum { REMOTE_RED_BUOY_AND_FORCE_FIELD =  250 };
      enum { REMOTE_RED_BUOY_AND_GREEN_BUOY =  252 };
      enum { REMOTE_RED_BUOY_AND_GREEN_BUOY_AND_FORCE_FIELD =  254 };
      enum { REMOTE_NONE =  255 };
      enum { CHARGING_NOT_CHARGING =  0 };
      enum { CHARGING_CHARGING_RECOVERY =  1 };
      enum { CHARGING_CHARGING =  2 };
      enum { CHARGING_TRICKLE_CHARGING =  3 };
      enum { CHARGING_WAITING =  4 };
      enum { CHARGING_CHARGING_ERROR =  5 };

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->bumps_wheeldrops >> (8 * 0)) & 0xFF;
      offset += sizeof(this->bumps_wheeldrops);
      union {
        bool real;
        uint8_t base;
      } u_wall;
      u_wall.real = this->wall;
      *(outbuffer + offset + 0) = (u_wall.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->wall);
      union {
        bool real;
        uint8_t base;
      } u_cliff_left;
      u_cliff_left.real = this->cliff_left;
      *(outbuffer + offset + 0) = (u_cliff_left.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->cliff_left);
      union {
        bool real;
        uint8_t base;
      } u_cliff_front_left;
      u_cliff_front_left.real = this->cliff_front_left;
      *(outbuffer + offset + 0) = (u_cliff_front_left.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->cliff_front_left);
      union {
        bool real;
        uint8_t base;
      } u_cliff_front_right;
      u_cliff_front_right.real = this->cliff_front_right;
      *(outbuffer + offset + 0) = (u_cliff_front_right.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->cliff_front_right);
      union {
        bool real;
        uint8_t base;
      } u_cliff_right;
      u_cliff_right.real = this->cliff_right;
      *(outbuffer + offset + 0) = (u_cliff_right.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->cliff_right);
      union {
        bool real;
        uint8_t base;
      } u_virtual_wall;
      u_virtual_wall.real = this->virtual_wall;
      *(outbuffer + offset + 0) = (u_virtual_wall.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->virtual_wall);
      *(outbuffer + offset + 0) = (this->motor_overcurrents >> (8 * 0)) & 0xFF;
      offset += sizeof(this->motor_overcurrents);
      *(outbuffer + offset + 0) = (this->dirt_detector_left >> (8 * 0)) & 0xFF;
      offset += sizeof(this->dirt_detector_left);
      *(outbuffer + offset + 0) = (this->dirt_detector_right >> (8 * 0)) & 0xFF;
      offset += sizeof(this->dirt_detector_right);
      *(outbuffer + offset + 0) = (this->remote_opcode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->remote_opcode);
      *(outbuffer + offset + 0) = (this->buttons >> (8 * 0)) & 0xFF;
      offset += sizeof(this->buttons);
      int32_t * val_distance = (int32_t *) &(this->distance);
      int32_t exp_distance = (((*val_distance)>>23)&255);
      if(exp_distance != 0)
        exp_distance += 1023-127;
      int32_t sig_distance = *val_distance;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = (sig_distance<<5) & 0xff;
      *(outbuffer + offset++) = (sig_distance>>3) & 0xff;
      *(outbuffer + offset++) = (sig_distance>>11) & 0xff;
      *(outbuffer + offset++) = ((exp_distance<<4) & 0xF0) | ((sig_distance>>19)&0x0F);
      *(outbuffer + offset++) = (exp_distance>>4) & 0x7F;
      if(this->distance < 0) *(outbuffer + offset -1) |= 0x80;
      int32_t * val_angle = (int32_t *) &(this->angle);
      int32_t exp_angle = (((*val_angle)>>23)&255);
      if(exp_angle != 0)
        exp_angle += 1023-127;
      int32_t sig_angle = *val_angle;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = (sig_angle<<5) & 0xff;
      *(outbuffer + offset++) = (sig_angle>>3) & 0xff;
      *(outbuffer + offset++) = (sig_angle>>11) & 0xff;
      *(outbuffer + offset++) = ((exp_angle<<4) & 0xF0) | ((sig_angle>>19)&0x0F);
      *(outbuffer + offset++) = (exp_angle>>4) & 0x7F;
      if(this->angle < 0) *(outbuffer + offset -1) |= 0x80;
      *(outbuffer + offset + 0) = (this->charging_state >> (8 * 0)) & 0xFF;
      offset += sizeof(this->charging_state);
      *(outbuffer + offset + 0) = (this->voltage >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->voltage >> (8 * 1)) & 0xFF;
      offset += sizeof(this->voltage);
      union {
        int16_t real;
        uint16_t base;
      } u_current;
      u_current.real = this->current;
      *(outbuffer + offset + 0) = (u_current.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_current.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->current);
      union {
        int8_t real;
        uint8_t base;
      } u_temperature;
      u_temperature.real = this->temperature;
      *(outbuffer + offset + 0) = (u_temperature.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->temperature);
      *(outbuffer + offset + 0) = (this->charge >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->charge >> (8 * 1)) & 0xFF;
      offset += sizeof(this->charge);
      *(outbuffer + offset + 0) = (this->capacity >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->capacity >> (8 * 1)) & 0xFF;
      offset += sizeof(this->capacity);
      *(outbuffer + offset + 0) = (this->wall_signal >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->wall_signal >> (8 * 1)) & 0xFF;
      offset += sizeof(this->wall_signal);
      *(outbuffer + offset + 0) = (this->cliff_left_signal >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->cliff_left_signal >> (8 * 1)) & 0xFF;
      offset += sizeof(this->cliff_left_signal);
      *(outbuffer + offset + 0) = (this->cliff_front_left_signal >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->cliff_front_left_signal >> (8 * 1)) & 0xFF;
      offset += sizeof(this->cliff_front_left_signal);
      *(outbuffer + offset + 0) = (this->cliff_front_right_signal >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->cliff_front_right_signal >> (8 * 1)) & 0xFF;
      offset += sizeof(this->cliff_front_right_signal);
      *(outbuffer + offset + 0) = (this->cliff_right_signal >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->cliff_right_signal >> (8 * 1)) & 0xFF;
      offset += sizeof(this->cliff_right_signal);
      *(outbuffer + offset + 0) = (this->user_digital_outputs >> (8 * 0)) & 0xFF;
      offset += sizeof(this->user_digital_outputs);
      *(outbuffer + offset + 0) = (this->user_digital_inputs >> (8 * 0)) & 0xFF;
      offset += sizeof(this->user_digital_inputs);
      *(outbuffer + offset + 0) = (this->user_analog_input >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->user_analog_input >> (8 * 1)) & 0xFF;
      offset += sizeof(this->user_analog_input);
      *(outbuffer + offset + 0) = (this->charging_sources_available >> (8 * 0)) & 0xFF;
      offset += sizeof(this->charging_sources_available);
      *(outbuffer + offset + 0) = (this->oi_mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->oi_mode);
      *(outbuffer + offset + 0) = (this->song_number >> (8 * 0)) & 0xFF;
      offset += sizeof(this->song_number);
      union {
        bool real;
        uint8_t base;
      } u_song_playing;
      u_song_playing.real = this->song_playing;
      *(outbuffer + offset + 0) = (u_song_playing.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->song_playing);
      *(outbuffer + offset + 0) = (this->number_of_stream_packets >> (8 * 0)) & 0xFF;
      offset += sizeof(this->number_of_stream_packets);
      union {
        int32_t real;
        uint32_t base;
      } u_requested_velocity;
      u_requested_velocity.real = this->requested_velocity;
      *(outbuffer + offset + 0) = (u_requested_velocity.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_requested_velocity.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_requested_velocity.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_requested_velocity.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->requested_velocity);
      union {
        int32_t real;
        uint32_t base;
      } u_requested_radius;
      u_requested_radius.real = this->requested_radius;
      *(outbuffer + offset + 0) = (u_requested_radius.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_requested_radius.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_requested_radius.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_requested_radius.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->requested_radius);
      union {
        int32_t real;
        uint32_t base;
      } u_requested_right_velocity;
      u_requested_right_velocity.real = this->requested_right_velocity;
      *(outbuffer + offset + 0) = (u_requested_right_velocity.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_requested_right_velocity.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_requested_right_velocity.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_requested_right_velocity.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->requested_right_velocity);
      union {
        int32_t real;
        uint32_t base;
      } u_requested_left_velocity;
      u_requested_left_velocity.real = this->requested_left_velocity;
      *(outbuffer + offset + 0) = (u_requested_left_velocity.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_requested_left_velocity.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_requested_left_velocity.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_requested_left_velocity.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->requested_left_velocity);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->bumps_wheeldrops =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->bumps_wheeldrops);
      union {
        bool real;
        uint8_t base;
      } u_wall;
      u_wall.base = 0;
      u_wall.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->wall = u_wall.real;
      offset += sizeof(this->wall);
      union {
        bool real;
        uint8_t base;
      } u_cliff_left;
      u_cliff_left.base = 0;
      u_cliff_left.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->cliff_left = u_cliff_left.real;
      offset += sizeof(this->cliff_left);
      union {
        bool real;
        uint8_t base;
      } u_cliff_front_left;
      u_cliff_front_left.base = 0;
      u_cliff_front_left.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->cliff_front_left = u_cliff_front_left.real;
      offset += sizeof(this->cliff_front_left);
      union {
        bool real;
        uint8_t base;
      } u_cliff_front_right;
      u_cliff_front_right.base = 0;
      u_cliff_front_right.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->cliff_front_right = u_cliff_front_right.real;
      offset += sizeof(this->cliff_front_right);
      union {
        bool real;
        uint8_t base;
      } u_cliff_right;
      u_cliff_right.base = 0;
      u_cliff_right.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->cliff_right = u_cliff_right.real;
      offset += sizeof(this->cliff_right);
      union {
        bool real;
        uint8_t base;
      } u_virtual_wall;
      u_virtual_wall.base = 0;
      u_virtual_wall.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->virtual_wall = u_virtual_wall.real;
      offset += sizeof(this->virtual_wall);
      this->motor_overcurrents =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->motor_overcurrents);
      this->dirt_detector_left =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->dirt_detector_left);
      this->dirt_detector_right =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->dirt_detector_right);
      this->remote_opcode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->remote_opcode);
      this->buttons =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->buttons);
      uint32_t * val_distance = (uint32_t*) &(this->distance);
      offset += 3;
      *val_distance = ((uint32_t)(*(inbuffer + offset++))>>5 & 0x07);
      *val_distance |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<3;
      *val_distance |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<11;
      *val_distance |= ((uint32_t)(*(inbuffer + offset)) & 0x0f)<<19;
      uint32_t exp_distance = ((uint32_t)(*(inbuffer + offset++))&0xf0)>>4;
      exp_distance |= ((uint32_t)(*(inbuffer + offset)) & 0x7f)<<4;
      if(exp_distance !=0)
        *val_distance |= ((exp_distance)-1023+127)<<23;
      if( ((*(inbuffer+offset++)) & 0x80) > 0) this->distance = -this->distance;
      uint32_t * val_angle = (uint32_t*) &(this->angle);
      offset += 3;
      *val_angle = ((uint32_t)(*(inbuffer + offset++))>>5 & 0x07);
      *val_angle |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<3;
      *val_angle |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<11;
      *val_angle |= ((uint32_t)(*(inbuffer + offset)) & 0x0f)<<19;
      uint32_t exp_angle = ((uint32_t)(*(inbuffer + offset++))&0xf0)>>4;
      exp_angle |= ((uint32_t)(*(inbuffer + offset)) & 0x7f)<<4;
      if(exp_angle !=0)
        *val_angle |= ((exp_angle)-1023+127)<<23;
      if( ((*(inbuffer+offset++)) & 0x80) > 0) this->angle = -this->angle;
      this->charging_state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->charging_state);
      this->voltage =  ((uint16_t) (*(inbuffer + offset)));
      this->voltage |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->voltage);
      union {
        int16_t real;
        uint16_t base;
      } u_current;
      u_current.base = 0;
      u_current.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_current.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->current = u_current.real;
      offset += sizeof(this->current);
      union {
        int8_t real;
        uint8_t base;
      } u_temperature;
      u_temperature.base = 0;
      u_temperature.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->temperature = u_temperature.real;
      offset += sizeof(this->temperature);
      this->charge =  ((uint16_t) (*(inbuffer + offset)));
      this->charge |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->charge);
      this->capacity =  ((uint16_t) (*(inbuffer + offset)));
      this->capacity |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->capacity);
      this->wall_signal =  ((uint16_t) (*(inbuffer + offset)));
      this->wall_signal |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->wall_signal);
      this->cliff_left_signal =  ((uint16_t) (*(inbuffer + offset)));
      this->cliff_left_signal |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->cliff_left_signal);
      this->cliff_front_left_signal =  ((uint16_t) (*(inbuffer + offset)));
      this->cliff_front_left_signal |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->cliff_front_left_signal);
      this->cliff_front_right_signal =  ((uint16_t) (*(inbuffer + offset)));
      this->cliff_front_right_signal |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->cliff_front_right_signal);
      this->cliff_right_signal =  ((uint16_t) (*(inbuffer + offset)));
      this->cliff_right_signal |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->cliff_right_signal);
      this->user_digital_outputs =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->user_digital_outputs);
      this->user_digital_inputs =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->user_digital_inputs);
      this->user_analog_input =  ((uint16_t) (*(inbuffer + offset)));
      this->user_analog_input |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->user_analog_input);
      this->charging_sources_available =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->charging_sources_available);
      this->oi_mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->oi_mode);
      this->song_number =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->song_number);
      union {
        bool real;
        uint8_t base;
      } u_song_playing;
      u_song_playing.base = 0;
      u_song_playing.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->song_playing = u_song_playing.real;
      offset += sizeof(this->song_playing);
      this->number_of_stream_packets =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->number_of_stream_packets);
      union {
        int32_t real;
        uint32_t base;
      } u_requested_velocity;
      u_requested_velocity.base = 0;
      u_requested_velocity.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_requested_velocity.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_requested_velocity.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_requested_velocity.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->requested_velocity = u_requested_velocity.real;
      offset += sizeof(this->requested_velocity);
      union {
        int32_t real;
        uint32_t base;
      } u_requested_radius;
      u_requested_radius.base = 0;
      u_requested_radius.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_requested_radius.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_requested_radius.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_requested_radius.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->requested_radius = u_requested_radius.real;
      offset += sizeof(this->requested_radius);
      union {
        int32_t real;
        uint32_t base;
      } u_requested_right_velocity;
      u_requested_right_velocity.base = 0;
      u_requested_right_velocity.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_requested_right_velocity.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_requested_right_velocity.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_requested_right_velocity.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->requested_right_velocity = u_requested_right_velocity.real;
      offset += sizeof(this->requested_right_velocity);
      union {
        int32_t real;
        uint32_t base;
      } u_requested_left_velocity;
      u_requested_left_velocity.base = 0;
      u_requested_left_velocity.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_requested_left_velocity.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_requested_left_velocity.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_requested_left_velocity.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->requested_left_velocity = u_requested_left_velocity.real;
      offset += sizeof(this->requested_left_velocity);
     return offset;
    }

    const char * getType(){ return "create_node/TurtlebotSensorState"; };
    const char * getMD5(){ return "d8f8ec7fa031fc9cc88e8319cd08a785"; };

  };

}
#endif