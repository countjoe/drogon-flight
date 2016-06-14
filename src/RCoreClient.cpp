/*
 * RCoreClient.cpp
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

#include "RCoreClient.h"

#include <unistd.h>


uint8_t read_byte(void* data, int* offset) 
{
    uint8_t value;
    memcpy(&value, (void*)(((long)data)+(*offset)), sizeof(uint8_t));
    (*offset) += sizeof(uint8_t);
    return value;
}

void write_byte(void* data, int* offset, uint8_t value) 
{
    memcpy((void*)(((long)data)+(*offset)), &value, sizeof(uint8_t));
    (*offset) += sizeof(uint8_t);
}

short read_short(void* data, int* offset) 
{
    short value;
    memcpy(&value, (void*)(((long)data)+(*offset)), sizeof(short));
    (*offset) += sizeof(short);
    return be16toh(value);
}

void write_short(void* data, int* offset, short value) 
{
    short enc_value = htobe16(value);
    memcpy((void*)(((long)data)+(*offset)), &enc_value, sizeof(short));
    (*offset) += sizeof(short);
}

int read_int(void* data, int* offset) 
{
    int value;
    memcpy(&value, (void*)(((long)data)+(*offset)), sizeof(int));
    (*offset) += sizeof(int);
    return be32toh(value);
}

void write_int(void* data, int* offset, int value) 
{
    int enc_value = htobe32(value);
    memcpy((void*)(((long)data)+(*offset)), &enc_value, sizeof(int));
    (*offset) += sizeof(int);
}

double read_double(void* data, int* offset) 
{
    uint64_t bits;
    union {
        uint64_t enc;
        double value;
    } uval;

    memcpy(&bits, (void*)(((long)data)+(*offset)), sizeof(uint64_t));
    (*offset) += sizeof(uint64_t);
    uval.enc = be64toh(bits);
    return uval.value;
}

void write_double(void* data, int* offset, double value) 
{
    uint64_t enc_value;
    union {
        uint64_t enc;
        double value;
    } uval;
    
    uval.value = value;

    enc_value = htobe64(uval.enc);

    memcpy((void*)(((long)data)+(*offset)), &enc_value, sizeof(uint64_t));
    (*offset) += sizeof(uint64_t);
}

void write_str(void* data, int* offset, const char* value, bool include_len = false) 
{
    int len = strlen(value);
    if (include_len) write_int(data, offset, len);
    for (int i = 0; i < len; i++) {
        write_byte(data, offset, value[i]);
    }
}

void write_bytea(void* data, int* offset, uint8_t* value, int len, bool include_len = false) 
{
    if (include_len) write_int(data, offset, len);
    for (int i = 0; i < len; i++) {
        write_byte(data, offset, value[i]);
    }
}

void print_hex(uint8_t* data, int len) 
{
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}


RCoreClient::RCoreClient(const char* host) 
{
    char addr[100];

    context = new zmq::context_t(1);

    mgt_socket = new zmq::socket_t(*context, ZMQ_REQ);
    sprintf(addr, "tcp://%s:%d", host, PORT_MGT);
    mgt_socket->connect (addr);

    sub_socket = new zmq::socket_t(*context, ZMQ_SUB);
    sprintf(addr, "tcp://%s:%d", host, PORT_PUBSUB);
    sub_socket->connect (addr);

    uint8_t arm_types[] = { MSG_DATA_TYPE_BYTE };
    event_type_arm = register_event_type("flight_arm", arm_types, 1);

    uint8_t motor_types[] = { MSG_DATA_TYPE_DOUBLE };
    event_type_motor = register_event_type("flight_motor", motor_types, 1);
}

bool RCoreClient::read()
{
    bool ok = sub_socket->recv(&sub_msg, ZMQ_NOBLOCK);
    if (ok) {
        event_data = (uint8_t*)sub_msg.data();
        event_data_len = sub_msg.size();

        printf("RECEIVED: ");
        print_hex(event_data, event_data_len);

        event_data_offset = 0;
        event_type = read_short(event_data, &event_data_offset);
        has_read_data = false;
    }
    return ok;
}

bool RCoreClient::is_arm_data()
{
    return event_type == event_type_arm;
}

uint8_t RCoreClient::get_arm_data()
{
    if (!is_arm_data()) {
        throw std::runtime_error("Attempt to read non-arm data");
    }
    if (has_read_data) {
        throw std::runtime_error("Attempt to read already-read data");
    }
    has_read_data = true;
    return read_byte(event_data, &event_data_offset);
}

bool RCoreClient::is_motor_data()
{
    return event_type == event_type_motor;
}

double RCoreClient::get_motor_data()
{
    if (!is_motor_data()) {
        throw std::runtime_error("Attempt to read non-motor data");
    }
    if (has_read_data) {
        throw std::runtime_error("Attempt to read already-read data");
    }
    has_read_data = true;
    return read_double(event_data, &event_data_offset);
}


void RCoreClient::close()
{
    delete mgt_socket;
    delete sub_socket;
    delete context;
}

int RCoreClient::register_event_type(const char * name, uint8_t* data_types, int ntypes)
{
    uint8_t cmd_buffer[255];
    int offset = 0;

    memset(cmd_buffer, 0, 255);

    write_short(cmd_buffer, &offset, EVT_TYPE_MGT_REGISTER_EVENT_TYPE);
    write_str(cmd_buffer, &offset, name, true);
    write_bytea(cmd_buffer, &offset, data_types, ntypes, false);

    zmq::message_t req(offset);
    zmq::message_t resp;

    memcpy(req.data(), cmd_buffer, offset);

    mgt_socket->send(req);

    mgt_socket->recv(&resp);

    uint8_t* resp_data = (uint8_t*)resp.data();

    offset = 0;
    read_short(resp_data, &offset); //unused, no need to verify resp type
    int eventTypeId = read_int(resp_data, &offset);

    uint8_t sub_filter[sizeof(short)];
    offset = 0;
    write_short(sub_filter, &offset, (short)eventTypeId);

    sub_socket->setsockopt(ZMQ_SUBSCRIBE, sub_filter, sizeof(short));

    return eventTypeId;
}
