/*
 * RCoreClient.h
 *
 * This file is part of Drogon.
 *
 * Drogon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Drogon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Drogon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Joseph Monti <joe.monti@gmail.com>
 * Copyright (c) 2013 Joseph Monti All Rights Reserved, http://joemonti.org/
 */
#ifndef __RCORECLIENT_H__
#define __RCORECLIENT_H__

#include <zmq.hpp>
#include <map>

#define PORT_MGT    12210
#define PORT_PUBSUB 12211

#define MSG_DATA_TYPE_BYTE      0
#define MSG_DATA_TYPE_INT       1
#define MSG_DATA_TYPE_LONG      2
#define MSG_DATA_TYPE_FLOAT     3
#define MSG_DATA_TYPE_DOUBLE    4
#define MSG_DATA_TYPE_STRING    5
#define MSG_DATA_TYPE_BYTEA     6
#define MSG_DATA_TYPE_JSON      7

#define EVT_TYPE_MGT_REGISTER_EVENT_TYPE        1
#define EVT_TYPE_MGT_REGISTER_EVENT_TYPE_RESP   2
#define EVT_TYPE_MGT_READ_EVENT_TYPE            3
#define EVT_TYPE_MGT_READ_EVENT_TYPE_RESP       4
#define EVT_TYPE_MGT_EVENT_RESP                 5

class RCoreClient {
  public:
    RCoreClient(const char* host);

    bool read();

    bool is_arm_data();
    uint8_t get_arm_data();

    bool is_motor_data();
    double get_motor_data();

    void close();

  private:
    int register_event_type(const char * name, uint8_t* data_types, int ntypes);

    zmq::context_t* context;

    zmq::socket_t* mgt_socket;
    zmq::socket_t* sub_socket;

    zmq::message_t sub_msg;

    int event_type;
    uint8_t* event_data;
    int event_data_len;
    int event_data_offset;
    bool has_read_data;

    int event_type_arm;
    int event_type_motor;
};

#endif // __RCORECLIENT_H__
