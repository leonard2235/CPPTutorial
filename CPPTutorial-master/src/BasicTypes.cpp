#include "BasicTypes.hpp"

#include <algorithm>

ProtocolNameChecker::PointerType ProtocolNameChecker::instance;
const ProtocolNameChecker::LookupTable ProtocolNameChecker::lookupTable {
        {ProtocolType::PHY, "PHY"},
        {ProtocolType::MAC, "MAC"},
        {ProtocolType::RLC, "RLC"},
        {ProtocolType::PDCP, "PDCP"}
};

PDU::PDU(ProtocolType type, const Payload &payload) : data()
{
    *this << type;
    *this << payload;
    rewind();
}

PDU::PDU(Payload payload) : data()
{
    *this << payload;
    rewind();
}

PDU::PDU(ProtocolType type) : data()
{
    *this << type;
    rewind();
}

const ProtocolType PDU::getType() const
{
    return static_cast<ProtocolType>(data[0]);
}
