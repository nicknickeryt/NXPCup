/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */

#pragma once
#include <cstdint>
#include <cstring>

#define MAX_LINES_NUMBER 2

enum class PacketType : uint8_t{
    RESPONSE_RESULT = 1,
    REQUEST_ERROR = 2,
    RESPONSE_ERROR = 3,
    LAMP_REQUEST = 0x16,
    LINE_NODE_REQUEST = 0x1c,
    LINE_NODE_RESPONSE = 0x1d
};

enum class LinesTypes : uint8_t{
    NO_LINES = 0x01,
    RIGHT_LINE = 0x02,
    LEFT_LINE = 0x03,
    BOTH_LINES = 0x04
};

struct PixyPacketRequest{
    struct __attribute__((__packed__)) Header{
        uint16_t sync;
        PacketType type;
        uint8_t payloadLength;
    } header;

    explicit PixyPacketRequest(PacketType packetType, uint8_t payloadLength) : header{0xc1ae, packetType, payloadLength} {}
    virtual void serialize(uint8_t *data, uint32_t* length) = 0;
};

struct PixyPacketResponse{
    struct __attribute__((__packed__)) Header{
        uint16_t sync = 0;
        PacketType type = PacketType::RESPONSE_ERROR;
        uint8_t payloadLength = 2;
        uint16_t checksum;
    } header;

    explicit PixyPacketResponse(PacketType type) : header{0xc1af, type, 0, 0}{}
    virtual void getHeader(uint8_t* data) = 0;
    virtual void deserialize(uint8_t *data) = 0;
};

struct SetLampRequest : public PixyPacketRequest{
    struct __attribute__((__packed__)) Payload{
        uint8_t upper = 0;
        uint8_t lower = 0;
    } payload;

    SetLampRequest(uint8_t upper, uint8_t lower) : PixyPacketRequest(PacketType::LAMP_REQUEST, 2), payload{upper, lower}{}

    void serialize(uint8_t *data, uint32_t* length) override {
        memcpy(data, &header, sizeof(header));
        memcpy(data + sizeof(header), &payload, sizeof(payload));
        *length = sizeof(header) + sizeof(payload);
    }
};

struct LineNodeRequest : public PixyPacketRequest{
    struct __attribute__((__packed__)) Payload{
        uint32_t lineNo = 0;
    } payload;

    explicit LineNodeRequest(uint32_t lineNo) : PixyPacketRequest(PacketType::LINE_NODE_REQUEST, 2), payload{lineNo}{}

    void serialize(uint8_t *data, uint32_t* length) override {
        memcpy(data, &header, sizeof(header));
        memcpy(data + sizeof(header), &payload, sizeof(payload));
        *length = sizeof(header) + sizeof(payload);
    }
};

struct LineNodeResponse : public PixyPacketResponse{
    struct __attribute__((__packed__)) Payload{
        LinesTypes linesType;
        uint16_t lines[MAX_LINES_NUMBER];
    } payload;

    LineNodeResponse() : PixyPacketResponse(PacketType::LINE_NODE_RESPONSE){}

    void getHeader(uint8_t* data) override {
        memcpy(&header, data, sizeof(header));
    }
    void deserialize(uint8_t *data) override {
        memcpy(&payload, data, header.payloadLength);
    }
};


