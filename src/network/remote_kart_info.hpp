//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013-2015 SuperTuxKart-Team
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

/*! \file remote_kart_info.hpp
 */

#ifndef HEADER_REMOTE_KART_INFO_HPP
#define HEADER_REMOTE_KART_INFO_HPP

#include "network/kart_data.hpp"

#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <irrString.h>
#include <cstdint>

enum KartTeam : int8_t
{
    KART_TEAM_NONE=-1,
    KART_TEAM_RED=0,
    KART_TEAM_BLUE=1,
};

struct KartTeamSet
{
    int state;
    KartTeamSet(int x = 0): state(x) {}

    bool has  (KartTeam team) const { return ((state >> (team + 1)) & 1); }
    bool empty()              const { return state == 0;                  }
    
    KartTeamSet operator + (KartTeam team)   const { return KartTeamSet(state | (1 << (team + 1)));  }
    KartTeamSet operator - (KartTeam team)   const { return KartTeamSet(state & ~(1 << (team + 1))); }
    KartTeamSet operator ^ (KartTeam team)   const { return KartTeamSet(state ^ (1 << (team + 1)));  }
    KartTeamSet operator & (KartTeamSet rhs) const { return KartTeamSet(state & rhs.state);          }

    void add      (KartTeam team)   { state |= (1 << (team + 1));  }
    void remove   (KartTeam team)   { state &= ~(1 << (team + 1)); }
    void flip     (KartTeam team)   { state ^= (1 << (team + 1));  }
    void intersect(KartTeamSet rhs) { state &= rhs.state;          }
};

/** Handicap per player. */
const int HANDICAP_COUNT = 49;

class NetworkPlayerProfile;

class RemoteKartInfo
{
        std::string         m_kart_name;
        irr::core::stringw  m_user_name;
        int                 m_local_player_id;
        int                 m_global_player_id;
        uint32_t            m_host_id;
        KartTeam            m_kart_team;
        bool                m_network_player;
        uint8_t       m_handicap;
        unsigned            m_starting_tyre;
        float               m_default_kart_color;
        uint32_t            m_online_id;
        std::string         m_country_code;
        std::weak_ptr<NetworkPlayerProfile> m_profile;
        KartData            m_kart_data;
public:
     RemoteKartInfo(int player_id, const std::string& kart_name,
                    const irr::core::stringw& user_name, uint32_t host_id,
                    bool network)
                  : m_kart_name(kart_name), m_user_name(user_name),
                    m_local_player_id(player_id), m_global_player_id(-1),
                    m_host_id(host_id), m_kart_team(KART_TEAM_NONE),
                    m_network_player(network),
                    m_handicap(0),
                    m_starting_tyre(2),
                    m_default_kart_color(0.0f), m_online_id(0)
     {}
     RemoteKartInfo(const std::string& kart_name) : m_kart_name(kart_name),
                    m_user_name(""), m_local_player_id(-1),
                    m_global_player_id(-1),
                    m_host_id(std::numeric_limits<uint32_t>::max()),
                    m_kart_team(KART_TEAM_NONE), m_network_player(false),
                    m_handicap(0),
                    m_starting_tyre(2),
                    m_default_kart_color(0.0f), m_online_id(0)
     {}
     RemoteKartInfo() : m_kart_name(""), m_user_name(""),
                    m_local_player_id(-1), m_global_player_id(-1),
                    m_host_id(std::numeric_limits<uint32_t>::max()),
                    m_kart_team(KART_TEAM_NONE), m_network_player(false),
                    m_handicap(0),
                    m_starting_tyre(2),
                    m_default_kart_color(0.0f), m_online_id(0)
     {}
    void setKartName(const std::string& n)   { m_kart_name = n;           }
    void setPlayerName(const irr::core::stringw& u) { m_user_name = u;    }
    void setHostId(uint32_t id)              { m_host_id = id;            }
    void setLocalPlayerId(int id)            { m_local_player_id = id;    }
    void setGlobalPlayerId(int id)           { m_global_player_id = id;   }
    void setKartTeam(KartTeam team)      { m_kart_team = team;      }
    void setNetworkPlayer(bool value)        { m_network_player = value;  }
    void setDefaultKartColor(float value) { m_default_kart_color = value; }
    void setHandicap(uint8_t value)    { m_handicap = value;        }
    void setStartingTyre(unsigned value)    { m_starting_tyre = value; }
    void setOnlineId(uint32_t id)            { m_online_id = id;          }
    uint32_t getHostId() const               { return m_host_id;          }
    int  getLocalPlayerId() const            { return m_local_player_id;  }
    int  getGlobalPlayerId() const           { return m_global_player_id; }
    bool  isNetworkPlayer() const            { return m_network_player;   }
    const std::string& getKartName() const   { return m_kart_name;        }
    const irr::core::stringw& getPlayerName() const { return m_user_name; }
    KartTeam getKartTeam() const               { return m_kart_team;      }
    uint8_t getHandicap() const        { return m_handicap;         }
    unsigned getStartingTyre() const        { return m_starting_tyre;         }
    float getDefaultKartColor() const      { return m_default_kart_color; }
    uint32_t getOnlineId() const           { return m_online_id;          }
    void setCountryCode(const std::string& id) { m_country_code = id;     }
    const std::string& getCountryCode() const { return m_country_code;    }
    void setKartData(const KartData& data)          { m_kart_data = data; }
    const KartData& getKartData() const             { return m_kart_data; }
    void setNetworkPlayerProfile(
        std::weak_ptr<NetworkPlayerProfile> npp)       { m_profile = npp; }
    std::weak_ptr<NetworkPlayerProfile> getNetworkPlayerProfile() const
                                                      { return m_profile; }
    bool disconnected() const               { return m_profile.expired(); }
    bool isReserved() const
              { return m_host_id == std::numeric_limits<uint32_t>::max(); }
    void makeReserved()
    {
        m_host_id = std::numeric_limits<uint32_t>::max();
        m_profile.reset();
    }
    void copyFrom(std::shared_ptr<NetworkPlayerProfile> p,
                  unsigned local_id);
    bool operator<(const RemoteKartInfo& other) const
    {
        return ((m_host_id<other.m_host_id) ||
                (m_host_id==other.m_host_id && m_local_player_id<other.m_local_player_id));
    }
};   // RemoteKartInfo

typedef std::vector<RemoteKartInfo> RemoteKartInfoList;

#endif
