//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009-2015 Marianne Gagnon
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

#include "config/user_config.hpp"
#include "guiengine/engine.hpp"
#include "guiengine/widgets/kart_stats_widget.hpp"
#include "karts/abstract_characteristic.hpp"
#include "karts/kart_properties.hpp"
#include "karts/kart_properties_manager.hpp"
#include "utils/log.hpp"
#include "utils/string_utils.hpp"
#include "utils/translation.hpp"
#include "io/file_manager.hpp"

#include <IGUIEnvironment.h>
#include <IGUIElement.h>
#include <IGUIButton.h>
#include <string>

using namespace GUIEngine;
using namespace irr::core;
using namespace irr;

// -----------------------------------------------------------------------------
KartStatsWidget::KartStatsWidget(core::recti area, const int player_id,
                                 std::string kart_group, bool multiplayer,
                                 bool display_icons) : Widget(WTYPE_DIV)
{
    m_title_font = !multiplayer;
    m_player_id = player_id;

    const std::string default_kart = UserConfigParams::m_default_kart;
    const KartProperties* props =
        kart_properties_manager->getKart(default_kart);
    if(!props)
    {
        // If the default kart can't be found (e.g. previously a addon
        // kart was used, but the addon package was removed), use the
        // first kart as a default. This way we don't have to hardcode
        // any kart names.
        int id = kart_properties_manager->getKartByGroup(kart_group, 0);
        if (id == -1)
            props = kart_properties_manager->getKartById(0);
        else
            props = kart_properties_manager->getKartById(id);

        if(!props)
            Log::fatal("KartSelectionScreen", "Can't find default "
                       "kart '%s' nor any other kart.",
                       default_kart.c_str());
    }
                                                                                                                                    
    for (int i = 0; i < SKILL_COUNT; ++i)
    {
        irr::core::recti skillArea(0, 0, 1, 1);

        SkillLevelWidget* skill_bar = NULL;

        skill_bar = new SkillLevelWidget(skillArea, m_player_id, multiplayer, display_icons);       

        m_skills.push_back(skill_bar);
        m_children.push_back(skill_bar);
    }

    setValues(props, 0);

    move(area.UpperLeftCorner.X, area.UpperLeftCorner.Y,
         area.getWidth(), area.getHeight());
}   // KartStatsWidget

// -----------------------------------------------------------------------------
void KartStatsWidget::setValues(const KartProperties* props, uint8_t h)
{
    // If props is NULL (e.g., when kart selection is "?" (random)),
    // skip this function to avoid errors.
    if( props == NULL ) return;
	
    // Use kart properties computed for best difficulty to show the user, so
    // that properties don't change according to the the last used difficulty
    RaceManager::Difficulty previous_difficulty = RaceManager::get()->getDifficulty();
    RaceManager::get()->setDifficulty(RaceManager::DIFFICULTY_BEST);
    KartProperties kp_computed;
    kp_computed.copyForPlayer(props, h);
    for (SkillLevelWidget* skills : m_skills)
        skills->setVisible(true);

    // Scale the values so they look better
    // A value of 100 takes the whole bar width, including borders.
    // So values should be in the 0-99 range

    // The base mass is of 350 ; 250/2.78 ~= 90
    setSkillValues(SKILL_MASS,
                   (kp_computed.getCombinedCharacteristic()->getMass() - 100.0f)/2.78f,
                   "mass.png", "mass", _("Mass"));
    
    // The base speed is of 27.75
    // Speed is the characteristics most affected by handicap, but it is also
    // important to display the base differences between classes as significant,
    // as small differences matter a lot.
    // Therefore, a non-linear formula is used.
    float speed_power_four = kp_computed.getCombinedCharacteristic()->getEngineMaxSpeed();
    speed_power_four *= speed_power_four; // squaring
    speed_power_four *= speed_power_four; // squaring again
    speed_power_four *= 0.001f; // divide by 1000 to improve readbility of the formula
    setSkillValues(SKILL_SPEED,
                   (speed_power_four - 100.0f) * 0.1924f,
                   "speed.png", "speed", _("Maximum speed"));
    
    // The acceleration depend on power and mass, and it changes depending on speed
    // We call a function which gives us a single number to represent it
    // power/mass gives numbers in the 1-11 range, so we multiply it by 9.
    setSkillValues(SKILL_ACCELERATION,
                   kp_computed.getAccelerationEfficiency()*9.0f,
                   "power.png", "acceleration", _("Acceleration"));

    // The base nitro consumption is 1, higher for heavier karts.
    // The base max speed increase is 5, higher for lighter karts
    // Nitro efficiency is hence computed as 18 * max_speed_increase / nitro_consumption
    setSkillValues(SKILL_NITRO_EFFICIENCY,
                    18.0f * kp_computed.getCombinedCharacteristic()->getNitroMaxSpeedIncrease()
                          / kp_computed.getCombinedCharacteristic()->getNitroConsumption(),
                   "nitro.png", "nitro", _("Nitro efficiency"));

    // The base time for the skidding bonuses is 1, 2.7 and 4
    // The lower, the better. We add 4 times level 1, 2 times level 2 and 1 time level 3
    // to obtain an aggregate value.
    // We proceed similarly for the skid bonuses (4.0, 6.0, 8.0 by default)
    float aggregate_skid_time = 4 * kp_computed.getCombinedCharacteristic()->getSkidTimeTillBonus()[0] +
                                2 * kp_computed.getCombinedCharacteristic()->getSkidTimeTillBonus()[1] +
                                    kp_computed.getCombinedCharacteristic()->getSkidTimeTillBonus()[2];

    float aggregate_bonus_speed = 4 * kp_computed.getCombinedCharacteristic()->getSkidBonusSpeed()[0] +
                                  2 * kp_computed.getCombinedCharacteristic()->getSkidBonusSpeed()[1] +
                                      kp_computed.getCombinedCharacteristic()->getSkidBonusSpeed()[2];
    setSkillValues(SKILL_SKIDDING,
                    (56.0f * aggregate_bonus_speed / aggregate_skid_time) - 90.0f,
                   "android/drift.png", "skidding", _("Drifting bonuses"));
    
    RaceManager::get()->setDifficulty(previous_difficulty);
}   // setValues

// -----------------------------------------------------------------------------
void KartStatsWidget::setSkillValues(Stats skill_type, float value, const std::string icon_name,
                                     const std::string skillbar_propID, const irr::core::stringw icon_tooltip)
{
    m_skills[skill_type]->setValue(value);
    m_skills[skill_type]->setIcon(irr::core::stringc(
            file_manager->getAsset(FileManager::GUI_ICON, icon_name).c_str()));    
    m_skills[skill_type]->m_properties[PROP_ID] = StringUtils::insertValues("@p%i_"+skillbar_propID, m_player_id);
    m_skills[skill_type]->m_iconbutton->setTooltip(icon_tooltip);
}   // setSkillValues

// -----------------------------------------------------------------------------
void KartStatsWidget::hideAll()
{
    for (SkillLevelWidget* skills : m_skills)
        skills->setVisible(false);
}   // hideAll

// -----------------------------------------------------------------------------
void KartStatsWidget::add()
{
    for (int i = 0; i < SKILL_COUNT; ++i)
    {
        m_skills[i]->add();
    }
}   // add

// -----------------------------------------------------------------------------
void KartStatsWidget::move(int x, int y, int w, int h)
{
    Widget::move(x,y,w,h);
    setSize(m_x, m_y, m_w, m_h);
    int margin = m_h / SKILL_COUNT * 0.6f;
    int offset = m_h / 5;
    for (int i = 0; i < SKILL_COUNT; ++i)
    {
        m_skills[i]->move(m_skill_bar_x,
                          m_y + offset + margin * i,
                          m_skill_bar_w,
                          m_skill_bar_h);
    }
}   // move

// -----------------------------------------------------------------------------
/** Set value for given type
 */
void KartStatsWidget::setValue(Stats type, float value)
{
    m_skills[type]->setValue(value);
}   // setValue

// -----------------------------------------------------------------------------
/** Get value for given type
 */
float KartStatsWidget::getValue(Stats type)
{
    return m_skills[type]->getValue();
}   // getValue

// -----------------------------------------------------------------------------
/** Set size for widgets inside KartStatsWidget
 */
void KartStatsWidget::setSize(const int x, const int y, const int w, const int h)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;

    // -- sizes
    m_skill_bar_w = w - m_w / 16; // make sure the bars can't be out of screen 
    m_skill_bar_h = m_h / SKILL_COUNT / 4;

    m_skill_bar_x = x;
}   // setSize

// -----------------------------------------------------------------------------
void KartStatsWidget::setDisplayIcons(bool display_icons)
{
    for (int i = 0; i < SKILL_COUNT; ++i)
    {
        m_skills[i]->setDisplayIcon(display_icons);
    }
}   // setDisplayIcons
