#include <gtest/gtest.h>
#include <vector>
#include "BasicTypes.hpp"

namespace{
class PDUTest: public ::testing::Test
{
protected:
};

TEST_F(PDUTest, PDU_ctor)
{
    Payload payload = {1, 2, 3, 4, 5, 6, 7, 8};
    Payload fullData = {static_cast<BYTE>(ProtocolType::MAC), 1, 2, 3, 4, 5, 6, 7, 8};

    PDU pdu(ProtocolType::MAC, payload);
    EXPECT_EQ(pdu.getFullData(), fullData);

    ProtocolType type;
    pdu >> type;
    EXPECT_EQ(type, ProtocolType::MAC);

    Payload data;
    pdu >> data;
    EXPECT_EQ(payload, data);
}

TEST_F(PDUTest, PDU_handle_size_t_value)
{
    PDU pdu(ProtocolType::PHY);
    std::size_t s = 0x1122334455667788;
    pdu << s;
    Payload fullData = {static_cast<BYTE>(ProtocolType::PHY), 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
    EXPECT_EQ(pdu.getFullData(), fullData);

    ProtocolType type;
    pdu >> type;
    EXPECT_EQ(type, ProtocolType::PHY);

    std::size_t t = 0;
    pdu >> t;
    EXPECT_EQ(s, t);
}
}

