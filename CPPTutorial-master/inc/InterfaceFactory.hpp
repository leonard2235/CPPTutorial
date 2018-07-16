#pragma once
#include <stdexcept>
#include <memory>
#include "Interface.hpp"


/* A simple Factory class for building all kind of interface*/
class InterfaceFactory
{
public:
    static std::shared_ptr<Interface> createInterface(InterfaceType type)
    {
    	/* This switch is very ugly, this interface can be moved to Inferface class as virtual function
    	 * and create a bunch of sub class.*/
        switch(type)
        {
        case InterfaceType::UU_GNB:
            return std::shared_ptr<Interface>(new UUInterface());
        default:
            throw std::invalid_argument("The interface not support yet!");
        }
    }
};
