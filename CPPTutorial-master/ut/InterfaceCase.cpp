#include <gtest/gtest.h>
#include <vector>
#include "InterfaceFactory.hpp"

namespace
{
class InterfaceTest: public ::testing::Test
{
protected:
    Payload payload = {1, 2, 3, 4, 5, 6, 7, 8};
};

TEST_F(InterfaceTest, testUU_GNB_Interface)
{
    auto interface = InterfaceFactory::createInterface(InterfaceType::UU_GNB);
    PDU pdu = interface->sendData(payload);
    Payload target = {static_cast<BYTE>(ProtocolType::PHY), static_cast<BYTE>(payload.size() + 3), static_cast<BYTE>(ProtocolType::MAC),
                      static_cast<BYTE>(ProtocolType::RLC), static_cast<BYTE>(ProtocolType::PDCP)};
    target.insert(target.end(), payload.begin(), payload.end());
    EXPECT_EQ(target, pdu.getFullData());

    Payload result = interface->receiveData(pdu);
    EXPECT_EQ(payload, result);
}
}

