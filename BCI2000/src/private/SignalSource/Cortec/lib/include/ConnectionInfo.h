/**********************************************************************
* Copyright 2015-2018, CorTec GmbH
* All rights reserved.
*
* Redistribution, modification, adaptation or translation is not permitted.
*
* CorTec shall be liable a) for any damage caused by a willful, fraudulent or grossly 
* negligent act, or resulting in injury to life, body or health, or covered by the 
* Product Liability Act, b) for any reasonably foreseeable damage resulting from 
* its breach of a fundamental contractual obligation up to the amount of the 
* licensing fees agreed under this Agreement. 
* All other claims shall be excluded. 
* CorTec excludes any liability for any damage caused by Licensee’s 
* use of the Software for regular medical treatment of patients.
**********************************************************************/
#ifndef IMPLANTAPI_CONNECTION_INFO_H
#define IMPLANTAPI_CONNECTION_INFO_H
#include <map>

namespace cortec { namespace implantapi {

    /**
    * @brief  Enumeration for different connections.
    *
    * A connection is always defined between two devices, e.g. between implant and external unit.
    *
    * @see ConnectionState
    */
    enum class ConnectionType
    {
        /// Connection between external unit and implant.
        EXT_TO_IMPLANT,
        /// Connection between PC and external unit.
        PC_TO_EXT
    };

    /**
    * @brief Enumeration for the state of a connection, e.g., connected or disconnected.
    *
    * @see ConnectionType
    */
    enum class ConnectionState
    {
        /// Represents the state 'disconnected'.
        DISCONNECTED,

        /// Represents the state 'connected'.
        CONNECTED,

        /// Represents the state 'connected'.
        UNKNOWN
    };

    /**
    * The connection_info_t data structure contains information about the state of several connections.
    * Depending on the actual hardware configuration, some connection types may be present, some not. For example, a
    * BIC3232 implant with an external unit has two connections: one between implant and external unit which is of type 
    * EXT_TO_IMPLANT, and one between PC and external unit which is of type PC_TO_EXT. Each connection can either be 
    * CONNECTED or DISCONNECTED. Consider a different hardware configuration consisting of an implant and an IRDA 
    * dongle. In this configuration, only one connection of type EXT_TO_IMPLANT is present.
    */
    typedef std::map<ConnectionType, ConnectionState> connection_info_t;

}}



#endif //IMPLANTAPI_CONNECTION_INFO_H