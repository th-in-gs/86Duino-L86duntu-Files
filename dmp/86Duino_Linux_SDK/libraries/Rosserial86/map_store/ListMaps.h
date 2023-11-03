#ifndef _ROS_SERVICE_ListMaps_h
#define _ROS_SERVICE_ListMaps_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "map_store/MapListEntry.h"

namespace map_store
{

static const char LISTMAPS[] = "map_store/ListMaps";

  class ListMapsRequest : public ros::Msg
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

    const char * getType(){ return LISTMAPS; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class ListMapsResponse : public ros::Msg
  {
    public:
      uint8_t map_list_length;
      map_store::MapListEntry st_map_list;
      map_store::MapListEntry * map_list;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset++) = map_list_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < map_list_length; i++){
      offset += this->map_list[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint8_t map_list_lengthT = *(inbuffer + offset++);
      if(map_list_lengthT > map_list_length)
        this->map_list = (map_store::MapListEntry*)realloc(this->map_list, map_list_lengthT * sizeof(map_store::MapListEntry));
      offset += 3;
      map_list_length = map_list_lengthT;
      for( uint8_t i = 0; i < map_list_length; i++){
      offset += this->st_map_list.deserialize(inbuffer + offset);
        memcpy( &(this->map_list[i]), &(this->st_map_list), sizeof(map_store::MapListEntry));
      }
     return offset;
    }

    const char * getType(){ return LISTMAPS; };
    const char * getMD5(){ return "ca5a33dd106a2c24cfd54c927d214957"; };

  };

  class ListMaps {
    public:
    typedef ListMapsRequest Request;
    typedef ListMapsResponse Response;
  };

}
#endif
