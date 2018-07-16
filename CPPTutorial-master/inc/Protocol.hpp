#pragma once
#include <stdexcept>
#include "BasicTypes.hpp"

class Protocol
{
public:
    virtual PDU encode(const Payload &payload) const;

    virtual const Payload decode(const PDU &pdu) const;

    /* Why we don't use data member to define the Protocol Type? The reason is if we use data member to record type
     * that may cause the problem that the class type may mismatch with coding mistake (by assigned wrong value to it).
     * Using static member may have problem when writing sub-class, forget to add this value.
     * Using pure virtual function force the sub class to implement it. */
    virtual ProtocolType getType() const = 0;

    virtual ~Protocol() = default;
protected:
    void checkType(ProtocolType pduType) const;
};

class PHY : public Protocol
{
public:
    virtual PDU encode(const Payload &payload) const override;

    virtual const Payload decode(const PDU &pdu) const override;

    virtual ProtocolType getType() const override
    {
        return ProtocolType::PHY;
    }
};

class MAC : public Protocol
{
public:
    virtual ProtocolType getType() const override
    {
        return ProtocolType::MAC;
    }
};

class RLC : public Protocol
{
public:
    virtual ProtocolType getType() const override
    {
        return ProtocolType::RLC;
    }
};

class PDCP : public Protocol
{
public:
    virtual ProtocolType getType() const override
    {
        return ProtocolType::PDCP;
    }
};

class PHYWithHash : public Protocol
{
public:
    virtual PDU encode(const Payload &payload) const override;

    virtual const Payload decode(const PDU &pdu) const override;

    virtual ProtocolType getType() const override
    {
        return ProtocolType::PHY_WITH_HASH;
    }
private:
    static const int HASH_KEY_SIZE = sizeof(std::size_t) / sizeof(BYTE);
};
