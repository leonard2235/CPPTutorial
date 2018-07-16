#pragma once

#include <vector>
#include <stdexcept>
#include <map>
#include <string>
#include <memory>

using BYTE = unsigned char;
/* To simplify the coding, the payload is holds in vector. This will increase unnecessary temporary objects */
using Payload = std::vector<BYTE>;

/* To make the excise easy, each protocol header is only one byte, the value is indicate which protocol is*/
enum class ProtocolType : BYTE
{
    PHY = 1, MAC = 2, RLC = 3, PDCP = 4, PHY_WITH_HASH = 128
};

class ProtocolNameChecker
{
public:
    using PointerType = std::shared_ptr<const ProtocolNameChecker>;
    static auto getChecker()
    {
        if(not instance){
            instance = PointerType(new ProtocolNameChecker());
        }
        return instance;
    }
    std::string find(ProtocolType type) const
    {
        return lookupTable.at(type);
    }

private:
    static PointerType instance;
    ProtocolNameChecker() {};
    using LookupTable = std::map<ProtocolType, std::string>;
    static const LookupTable lookupTable;
};

/* Interface type is named base <Interface name>_<NE name> */
enum class InterfaceType
{
    UU_GNB, UU_UE, S1U_GNB
};

/* */
class PDU
{
public:
    PDU(ProtocolType type, const Payload &payload);

    PDU(Payload payload);

    PDU(ProtocolType type);

    PDU() = delete;

    const ProtocolType getType() const;

    const Payload getFullData() const
    {
        return data;
    }

    void rewind() const
    {
        readPos = 0;
    }

    PDU& operator << (ProtocolType type)
    {
        data.push_back(static_cast<BYTE>(type));
        return *this;
    }

    PDU& operator << (BYTE val)
    {
        data.push_back(val);
        return *this;
    }

    PDU& operator << (std::size_t value)
    {
        BYTE *p = (BYTE*)&value;
        data.insert(data.end(), (BYTE*)&value, (BYTE*)&value + SIZE_T_SIZE);
        return *this;
    }

    PDU& operator << (const Payload &userData)
    {
        data.insert(data.end(), userData.begin(), userData.end());
        return *this;
    }

    const PDU& operator >> (ProtocolType &target) const
    {
        target = static_cast<ProtocolType>(data[readPos++]);
        return *this;
    }

    const PDU& operator >> (BYTE &target) const
    {
        target = data[readPos++];
        return *this;
    }

    const PDU& operator >> (std::size_t &target) const
    {
        if(data.size() - readPos < SIZE_T_SIZE)
            throw std::invalid_argument("No enough data left for std::size_t value.");
        target = 0;
        for(int i = 0; i < SIZE_T_SIZE; i++)
        {
            target |= (static_cast<std::size_t>(data[readPos++]) << (i * 8));
        }
        return *this;
    }

    const PDU& operator >> (Payload &target) const
    {
        target.insert(target.end(), data.begin() + readPos, data.end());
        readPos = data.size();
        return *this;
    }

    const std::size_t getSize() const
    {
        return this->data.size();
    }

private:
    /* How many BYTEs the std::size_t will takes*/
    static const int SIZE_T_SIZE = sizeof(std::size_t) / sizeof(BYTE);

    /* Couldn't use iterator type to records the read position, due to the data maybe re-allocated,
     * when the vector's size is growing*/
    /* Use mutable qualifier to make those function which modify the readPos const. That may cause
     * the PDU class not thread safe, but in this context the PDU object isn't shared by multi-threads.*/
    mutable int readPos;

    Payload data;
};

