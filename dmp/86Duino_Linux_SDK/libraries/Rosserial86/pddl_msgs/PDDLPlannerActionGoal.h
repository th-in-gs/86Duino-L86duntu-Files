#ifndef _ROS_pddl_msgs_PDDLPlannerActionGoal_h
#define _ROS_pddl_msgs_PDDLPlannerActionGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalID.h"
#include "pddl_msgs/PDDLPlannerGoal.h"

namespace pddl_msgs
{

  class PDDLPlannerActionGoal : public ros::Msg
  {
    public:
      std_msgs::Header header;
      actionlib_msgs::GoalID goal_id;
      pddl_msgs::PDDLPlannerGoal goal;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->goal_id.serialize(outbuffer + offset);
      offset += this->goal.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->goal_id.deserialize(inbuffer + offset);
      offset += this->goal.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "pddl_msgs/PDDLPlannerActionGoal"; };
    const char * getMD5(){ return "b3c09ffd26e66295a3538094a62334be"; };

  };

}
#endif