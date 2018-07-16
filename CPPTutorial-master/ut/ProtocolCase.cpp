#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include "Protocol.hpp"

namespace
{
class ProtocolTest: public ::testing::Test
{
protected:
    Payload payload = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    Payload empty{};
    Payload hashPart = {0xFE, 0x4C, 0x61, 0xC5, 0xF2, 0x17, 0x3D, 0xCE,};

    void testCommonProtocol(const Protocol &prtcl, ProtocolType type, const Payload &payload)
    {
        using namespace std;
        PDU pdu = prtcl.encode(payload);
        EXPECT_EQ(type, pdu.getType());

        Payload target = {static_cast<Payload::value_type>(type)};
        target.insert(target.end(), payload.begin(), payload.end());
        EXPECT_EQ(target, pdu.getFullData());

        const Payload result = prtcl.decode(pdu);
        EXPECT_EQ(payload, result);
    }

    std::string getExceptionString(const Protocol &prtcl, ProtocolType wrongType)
    {
        auto expect = ProtocolNameChecker::getChecker()->find(prtcl.getType());
        auto get = ProtocolNameChecker::getChecker()->find(wrongType);
        return "The header shall be " + expect + ", but get " + get + "!";
    }

    void testException(const Protocol &prtcl, ProtocolType wrongType)
    {
        using namespace std;
        PDU pdu(wrongType);
        pdu << payload;
        try{
            prtcl.decode(pdu.getFullData());
        }
        catch(invalid_argument const &e)
        {
            EXPECT_EQ(e.what(), getExceptionString(prtcl, wrongType));
        }
        catch(...)
        {
            FAIL() << "Throw an unexpected exception!";
        }
    }
};

TEST_F(ProtocolTest, Protocol_PDCP)
{
    testCommonProtocol(PDCP(), ProtocolType::PDCP, payload);
    testCommonProtocol(PDCP(), ProtocolType::PDCP, empty);
    testException(PDCP(), ProtocolType::MAC);
}

TEST_F(ProtocolTest, Protocol_RLC)
{
    testCommonProtocol(RLC(), ProtocolType::RLC, payload);
    testCommonProtocol(RLC(), ProtocolType::RLC, empty);
    testException(RLC(), ProtocolType::MAC);
}

TEST_F(ProtocolTest, Protocol_MAC)
{
    testCommonProtocol(MAC(), ProtocolType::MAC, payload);
    testCommonProtocol(MAC(), ProtocolType::MAC, empty);
    testException(MAC(), ProtocolType::PDCP);
}

TEST_F(ProtocolTest, Protocol_PHY)
{
    PDU target(ProtocolType::PHY);
    target << static_cast<BYTE>(payload.size()) << payload;

    PHY phy;
    PDU pdu = phy.encode(payload);

    EXPECT_EQ(target.getFullData(), pdu.getFullData());
    const Payload result = phy.decode(pdu);
    EXPECT_EQ(payload, result);

    testException(phy, ProtocolType::PDCP);
}

TEST_F(ProtocolTest, Protocol_PHY_WITH_HASH)
{
    Payload target = {static_cast<Payload::value_type>(ProtocolType::PHY_WITH_HASH)};
    target.insert(target.end(), hashPart.begin(), hashPart.end());
    target.insert(target.end(), payload.begin(), payload.end());
    PHYWithHash phy{};
    PDU pdu = phy.encode(payload);

    EXPECT_EQ(target, pdu.getFullData());
    const Payload result = phy.decode(pdu);
    EXPECT_EQ(payload, result);

}

TEST_F(ProtocolTest, Protocol_PHY_WITH_HASH_Empty_Userdata)
{
    //ce3d17f2c5614cfe
    Payload target = {static_cast<Payload::value_type>(ProtocolType::PHY_WITH_HASH)};
    target.insert(target.end(), 8, 0x0);

    PHYWithHash phy{};
    PDU pdu = phy.encode(empty);

    EXPECT_EQ(target, pdu.getFullData());
    const Payload result = phy.decode(pdu);
    EXPECT_EQ(empty, result);
}
}
