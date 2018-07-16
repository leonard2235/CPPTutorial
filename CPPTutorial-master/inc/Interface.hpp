#pragma once
#include <vector>
#include <memory>
#include "Protocol.hpp"

/**
 * Network Element communicate with each other through different Interface according to the 3GPP protocol.
 * The interface consists of a branch of protocol to build protocol stack. When sending data to peer, the
 * data is encoded through the protocol stack from top to bottom. When receiving a frame from the peer,
 * the frame is decoded through the protocol stack from bottom to top.*/
class Interface
{
public:
    using SharedPtrType = std::shared_ptr<Protocol>;
    Interface(std::initializer_list<SharedPtrType> list)
    {
        protocolStack = std::move(list);
    }
    /**
     * Encode the user data through the protocol from top to bottom. Each protocol layer add its header
     * to the userData and pass the result data frame as user data to the lower layer.
     * @param userData    The user data that being sent to peer.
     * @return    The PDU that contains each protocol's header and user data which is read to be sent to
     *            peer.*/
    PDU sendData(const Payload &userData) const
    {
        auto iter = protocolStack.cbegin();
        PDU pdu = (*iter)->encode(userData);
        while(++iter != protocolStack.cend())
        {
            pdu = (*iter)->encode(pdu.getFullData());
        }
        return pdu;
    }

    /**
     * Decode the data that received from peer. Each protocol layer strip its header from pdu and pass the
     * sdu to its upper layer as input.
     * @param pdu    The PDU that received from peer.
     * @return    The user data that peer send. */
    Payload receiveData(PDU pdu) const
    {
        auto rit = protocolStack.crbegin();
        Payload payload = (*rit)->decode(pdu);
        while(++rit != protocolStack.crend())
        {
            PDU pdu{payload};
            payload = (*rit)->decode(pdu);
        }
        return payload;
    }

private:
    std::vector<SharedPtrType> protocolStack;
};

/**
 * The UUInterface simulates the interface between UE and gNB. it contains the protocol stack which
 * consists of PDCP, RLC, MAC, PHY from top to the bottom.*/
class UUInterface : public Interface
{
public:
    UUInterface() : Interface{std::make_shared<PDCP>(), std::make_shared<RLC>(),
                              std::make_shared<MAC>(), std::make_shared<PHY>()}
    {
    }
};
