#pragma once
#include "Interface.hpp"
#include <map>
#include <memory>

enum class NetworkElementType {
	GNB, UE, SGW, PGW, MME
};

class NetworkElement
{
public:
	NetworkElement(NetworkElementType type) : type{type}{}
	NetworkElement() = delete; //Not allow create a NE without type information.

	NetworkElementType getType()
	{
		return type;
	}

	PDU sendData(const NetworkElement &peer, Payload payload)
	{
		auto interface = interfaces[peer.getType()];
		return interface->sendData(payload);
	}
private:
	std::map<NetworkElementType, std::shared_ptr<Interface>> interfaces;
	NetworkElementType type;
};
