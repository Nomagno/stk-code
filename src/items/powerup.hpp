//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006-2015 Joerg Henrichs
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

#ifndef HEADER_POWERUP_HPP
#define HEADER_POWERUP_HPP

#define MAX_POWERUPS 5

#include "items/powerup_manager.hpp"  // needed for powerup_type
#include "utils/no_copy.hpp"

class Kart;
class BareNetworkString;
class ItemState;
class SFXBase;

/**
  * \ingroup items
  */
class Powerup : public NoCopy
{
private:
    /** The powerup type. */
    PowerupManager::PowerupType m_type;

    /** The mini-state (used only by the mini-wish powerup) */
    PowerupManager::MiniState m_mini_state;

    /** Number of collected powerups. */
    int                         m_number;

    /** The owner (kart) of this powerup. */
    Kart*                       m_kart;

    /** Returns an integer in the 0-32767 range.*/
    int simplePRNG(const int seed, const int time, const int item_id, const int position);
    int useBubblegum(bool mini = false);
    int useNitroHack();

public:
                    Powerup      (Kart* kart_);
    void            set          (PowerupManager::PowerupType _type, int n = 1);
    void            setNum       (int n = 1);
    void            reset        ();
    Material*       getIcon      (bool wide=false) const;
    void            use          ();
    void            remove          ();
    void            hitBonusBox (const ItemState &item);
    void            saveState(BareNetworkString *buffer) const;
    void            rewindTo(BareNetworkString *buffer);
    void            update(int ticks);

    /** Returns the number of powerups. */
    int             getNum                 () const {return m_number;}
    // ------------------------------------------------------------------------
    /** Returns the type of this powerup. */
    PowerupManager::PowerupType getType    () const {return m_type;  }
    // ------------------------------------------------------------------------
    PowerupManager::MiniState getMiniState () const { return m_mini_state; }
    // ------------------------------------------------------------------------
    void setMiniState (PowerupManager::MiniState new_mini_state)
            { m_mini_state = new_mini_state; }
    // ------------------------------------------------------------------------
    bool            hasWideIcon            () const {return m_type == PowerupManager::POWERUP_MINI; }
};

#endif
