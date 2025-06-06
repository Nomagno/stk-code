//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2025 kimden
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef PACKET_TYPES_HPP
#define PACKET_TYPES_HPP

#include "network/network_string.hpp"

/**
 * IMPORTANT!
 * This class is meant to describe the overall structure of network packets
 * and operations on them, using only the "network/packet_types_base.hpp".
 * No other files should include the base header as it has no guards.
 * Include this header instead.
 */

//---------------------- Enums ------------------------------------------------

/** Lists all lobby events (LE). */

enum LobbyEvent: uint8_t
{
    LE_CONNECTION_REQUESTED = 1, // a connection to the server
    LE_CONNECTION_REFUSED, // Connection to server refused
    LE_CONNECTION_ACCEPTED, // Connection to server accepted
    LE_SERVER_INFO, // inform client about server info
    LE_REQUEST_BEGIN, // begin of kart selection
    LE_UPDATE_PLAYER_LIST, // inform client about player list update
    LE_KART_SELECTION, // Player selected kart
    LE_PLAYER_DISCONNECTED, // Client disconnected
    LE_CLIENT_LOADED_WORLD, // Client finished loading world
    LE_LOAD_WORLD, // Clients should load world
    LE_START_RACE, // Server to client to start race
    LE_START_SELECTION, // inform client to start selection
    LE_RACE_FINISHED, // race has finished, display result
    LE_RACE_FINISHED_ACK, // client went back to lobby
    LE_BACK_LOBBY, // Force clients to go back to lobby
    LE_VOTE, // Track vote
    LE_CHAT, // Client chat message
    LE_SERVER_OWNERSHIP, // Tell client he is now the server owner
    LE_KICK_HOST, // Server owner kicks some other peer in game
    LE_CHANGE_TEAM, // Client wants to change his team
    LE_BAD_TEAM, // Tell server owner that the team is unbalanced
    LE_BAD_CONNECTION, // High ping or too many packets loss
    LE_CONFIG_SERVER, // Server owner config server game mode or difficulty
    LE_CHANGE_HANDICAP_AND_TYRE, // Client changes handicap
    LE_CHANGE_COLOR, // Client changes color
    LE_LIVE_JOIN, // Client live join or spectate
    LE_LIVE_JOIN_ACK, // Server tell client live join or spectate succeed
    LE_KART_INFO, // Client or server exchange new kart info
    LE_CLIENT_BACK_LOBBY, // Client tell server to go back lobby
    LE_REPORT_PLAYER, // Client report some player in server
                        // (like abusive behaviour)
    LE_ASSETS_UPDATE, // Client tell server with updated assets
    LE_COMMAND, // Command
};

enum RejectReason : uint8_t
{
    RR_BUSY = 0,
    RR_BANNED = 1,
    RR_INCORRECT_PASSWORD = 2,
    RR_INCOMPATIBLE_DATA = 3,
    RR_TOO_MANY_PLAYERS = 4,
    RR_INVALID_PLAYER = 5
};

enum BackLobbyReason : uint8_t
{
    BLR_NONE = 0,
    BLR_NO_GAME_FOR_LIVE_JOIN = 1,
    BLR_NO_PLACE_FOR_LIVE_JOIN = 2,
    BLR_ONE_PLAYER_IN_RANKED_MATCH = 3,
    BLR_SERVER_ONWER_QUITED_THE_GAME = 4,
    BLR_SPECTATING_NEXT_GAME = 5
};

//---------------------- Initialization ---------------------------------------

#define DEFINE_CLASS(Name) \
struct Name { \
    public: \
    void toNetworkString(NetworkString* ns) const; \
    void fromNetworkString(NetworkString* ns);

#define SYNCHRONOUS(Value) bool isSynchronous() { return Value; }
#define DEFINE_FIELD(Type, Var) Type Var;
#define DEFINE_FIXED_FIELD(Type, Var, Value) Type Var;
#define DEFINE_VECTOR(Type, Size, Var) std::vector<Type> Var;
#define END_DEFINE_CLASS(Name) };

#include "network/packet_types_base.hpp"
#undef DEFINE_CLASS
#undef SYNCHRONOUS
#undef DEFINE_FIELD
#undef DEFINE_FIXED_FIELD
#undef DEFINE_VECTOR
#undef END_DEFINE_CLASS

//---------------------- To NetworkString -------------------------------------

#define DEFINE_CLASS(Name) \
inline void Name::toNetworkString(NetworkString* ns) const \
{ 

#define SYNCHRONOUS(Value) \
    ns->setSynchronous(Value);

#define DEFINE_FIELD(Type, Var) \
    ns->encode<Type>(Var);

#define DEFINE_FIXED_FIELD(Type, Var, Value) \
    ns->encode<Type>(Value);

#define DEFINE_VECTOR(Type, Size, Value) \
    for (unsigned Value##_cnt = 0; Value##_cnt < Size; ++Value##_cnt) { \
        Value[Value##_cnt].toNetworkString(ns); \
    }

#define END_DEFINE_CLASS(Name) \
}

#include "network/packet_types_base.hpp"
#undef DEFINE_CLASS
#undef SYNCHRONOUS
#undef DEFINE_FIELD
#undef DEFINE_FIXED_FIELD
#undef DEFINE_VECTOR
#undef END_DEFINE_CLASS

//---------------------- From NetworkString -----------------------------------

#define DEFINE_CLASS(Name) \
inline void Name::fromNetworkString(NetworkString* ns) \
{ 

#define SYNCHRONOUS(Value)

#define DEFINE_FIELD(Type, Var) \
    ns->decode<Type>(Var);

#define DEFINE_FIXED_FIELD(Type, Var, Value)

#define DEFINE_VECTOR(Type, Size, Var) \
    Var.resize(Size); \
    for (unsigned Var##_cnt = 0; Var##_cnt < Size; ++Var##_cnt) { \
        Var[Var##_cnt].fromNetworkString(ns); \
    }

#define END_DEFINE_CLASS(Name) \
}

#include "network/packet_types_base.hpp"
#undef DEFINE_CLASS
#undef SYNCHRONOUS
#undef DEFINE_FIELD
#undef DEFINE_FIXED_FIELD
#undef DEFINE_VECTOR
#undef END_DEFINE_CLASS

//---------------------- End --------------------------------------------------

#endif // PACKET_TYPES_HPP
