//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2022 SuperTuxKart-Team
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

#ifndef HEADER_MEM_UTILS_HPP
#define HEADER_MEM_UTILS_HPP

namespace MemUtils {
    template<typename callback>
    class deref {
        callback cb;
    bool chg;
    public:
    deref(callback _c) : cb(std::move(_c)), chg(true) {}
    deref(const deref &) = delete;
    deref& operator=(const deref &) = delete;
    deref(deref &&other) : 
            cb(other.cb),
        chg(other.chg)
    {
            other.chg = false;
    }
    ~deref()
    {
            if (chg)
               cb();
    }
    };
}

#endif
