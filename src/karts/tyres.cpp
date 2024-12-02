//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2024 Nomagno
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

#include "tyres.hpp"

#include "karts/kart_properties.hpp"
#include "karts/kart.hpp"
#include "tracks/track.hpp"
#include "race/race_manager.hpp"
#include "network/network_string.hpp"
#include "network/rewind_manager.hpp"
#include <iostream>
#include <algorithm>

Tyres::Tyres(Kart *kart) {
    m_kart = kart;
    m_current_compound = 1; // Placeholder value
    m_current_fuel = 1; // Placeholder value
    m_lap_count = 0;
    m_reset_compound = false;
    m_reset_fuel = false;
    m_current_fuel = m_c_fuel;

    m_speed_fetching_period = 0.3f;
    m_speed_accumulation_limit = 6;

    m_current_life_traction = m_kart->getKartProperties()->getTyresMaxLifeTraction()[m_current_compound-1];
    m_current_life_turning = m_kart->getKartProperties()->getTyresMaxLifeTurning()[m_current_compound-1];
    m_heat_cycle_count = 0.0f;
    m_current_temp = m_kart->getKartProperties()->getTyresIdealTemp()[m_current_compound-1];
    m_center_of_gravity_x = 0.0f;
    m_center_of_gravity_y = 0.0f;
    m_previous_speeds.clear();
    m_acceleration = 0.0f;
    m_time_elapsed = 0.0f;
    m_debug_cycles = 0;


    m_c_fuel = (std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[0];
    m_c_fuel_regen = (std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[1];
    m_c_fuel_stop = (std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[2];
    m_c_fuel_weight = ((std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[3])/100.0f;
    m_c_fuel_rate = (std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[4];

    m_kart->m_tyres_queue = std::get<2>(RaceManager::get()->getFuelAndQueueInfo());

    irr::core::clamp(m_c_fuel, 1.0f, 1000.0f);
    irr::core::clamp(m_c_fuel_stop, 0.0f, 1000.0f);
    irr::core::clamp(m_c_fuel_rate, 0.0f, 1000.0f);
    irr::core::clamp(m_c_fuel_regen, 0.0f, 100.0f);
    irr::core::clamp(m_c_fuel_weight, 0.0f, 1.0f);

    m_c_hardness_multiplier = m_kart->getKartProperties()->getTyresHardnessMultiplier()[m_current_compound-1];
    m_c_heat_cycle_hardness_curve = m_kart->getKartProperties()->getTyresHeatCycleHardnessCurve();
    m_c_hardness_penalty_curve = m_kart->getKartProperties()->getTyresHardnessPenaltyCurve();

    m_c_mass = m_kart->getKartProperties()->getMass();
    m_c_ideal_temp = m_kart->getKartProperties()->getTyresIdealTemp()[m_current_compound-1];
    m_c_max_life_traction = m_kart->getKartProperties()->getTyresMaxLifeTraction()[m_current_compound-1];
    m_c_max_life_turning = m_kart->getKartProperties()->getTyresMaxLifeTurning()[m_current_compound-1];
    m_c_min_life_traction = m_kart->getKartProperties()->getTyresMinLifeTraction()[m_current_compound-1];
    m_c_min_life_turning = m_kart->getKartProperties()->getTyresMinLifeTurning()[m_current_compound-1];
    m_c_limiting_transfer_traction = m_kart->getKartProperties()->getTyresLimitingTransferTraction()[m_current_compound-1];
    m_c_regular_transfer_traction = m_kart->getKartProperties()->getTyresRegularTransferTraction()[m_current_compound-1];
    m_c_limiting_transfer_turning = m_kart->getKartProperties()->getTyresLimitingTransferTurning()[m_current_compound-1];
    m_c_regular_transfer_turning = m_kart->getKartProperties()->getTyresRegularTransferTurning()[m_current_compound-1];

    m_c_do_substractive_traction = m_kart->getKartProperties()->getTyresDoSubstractiveTraction()[m_current_compound-1] > 0.5f;
    m_c_do_grip_based_turning  = m_kart->getKartProperties()->getTyresDoGripBasedTurning()[m_current_compound-1] > 0.5f;
    m_c_do_substractive_turning  = m_kart->getKartProperties()->getTyresDoSubstractiveTurning()[m_current_compound-1] > 0.5f;
    m_c_do_substractive_topspeed  = m_kart->getKartProperties()->getTyresDoSubstractiveTopspeed()[m_current_compound-1] > 0.5f;

    m_c_response_curve_traction = m_kart->getKartProperties()->getTyresResponseCurveTraction();
    m_c_response_curve_turning = m_kart->getKartProperties()->getTyresResponseCurveTurning();
    m_c_response_curve_topspeed = m_kart->getKartProperties()->getTyresResponseCurveTopspeed();
    m_c_initial_bonus_mult_traction = m_kart->getKartProperties()->getTyresInitialBonusMultTraction()[m_current_compound-1];
    m_c_initial_bonus_add_traction = m_kart->getKartProperties()->getTyresInitialBonusAddTraction()[m_current_compound-1];
    m_c_initial_bonus_mult_turning = m_kart->getKartProperties()->getTyresInitialBonusMultTurning()[m_current_compound-1];
    m_c_initial_bonus_add_turning = m_kart->getKartProperties()->getTyresInitialBonusAddTurning()[m_current_compound-1];
    m_c_initial_bonus_mult_topspeed = m_kart->getKartProperties()->getTyresInitialBonusMultTopspeed()[m_current_compound-1];
    m_c_initial_bonus_add_topspeed = m_kart->getKartProperties()->getTyresInitialBonusAddTopspeed()[m_current_compound-1];
    m_c_traction_constant = m_kart->getKartProperties()->getTyresTractionConstant()[m_current_compound-1];
    m_c_turning_constant = m_kart->getKartProperties()->getTyresTurningConstant()[m_current_compound-1];
    m_c_topspeed_constant = m_kart->getKartProperties()->getTyresTopspeedConstant()[m_current_compound-1];

    m_c_offroad_factor = m_kart->getKartProperties()->getTyresOffroadFactor()[m_current_compound-1];
    m_c_skid_factor = m_kart->getKartProperties()->getTyresSkidFactor()[m_current_compound-1];
    m_c_brake_threshold = m_kart->getKartProperties()->getTyresBrakeThreshold()[m_current_compound-1];
    m_c_crash_penalty = m_kart->getKartProperties()->getTyresCrashPenalty()[m_current_compound-1];
}

float Tyres::correct(float f) {
    return (100.0f*(float)(m_current_compound-1)+(float)(m_current_compound-1))+f;
}

void Tyres::computeDegradation(float dt, bool is_on_ground, bool is_skidding, bool is_using_zipper, float slowdown, float brake_amount, float steer_amount, float throttle_amount) {
    m_debug_cycles += 1;
    m_time_elapsed += dt;
    float speed = m_kart->getSpeed();
    if (fmod(m_time_elapsed, m_speed_fetching_period) < dt) {
        m_previous_speeds.push_back(speed);
        if (m_previous_speeds.size() > m_speed_accumulation_limit ) {
            m_previous_speeds.pop_front();
        }
        if (m_previous_speeds.size() >= 2) {
            m_acceleration = (speed-m_previous_speeds[m_previous_speeds.size()-2])/dt;
            for (unsigned i = 0; i < m_previous_speeds.size()-2; i++) {
                if (std::abs(speed-m_previous_speeds[i])/dt < std::abs(m_acceleration) && std::abs(speed-m_previous_speeds[i])/dt < 2300.0f) { //Empirical, above this generally indicates a crash, so we throw it out.
                    m_acceleration = (speed-m_previous_speeds[i])/dt;
                }
            }
//          printf("Smallest acceleration: %f. Queue:\n", std::abs(m_acceleration));
//          for (float n : m_previous_speeds)
//              printf("%f ", n);
//          printf("\n\n");
        }
        if (slowdown < 0.5f && !is_using_zipper) {
            m_acceleration = 0.0f; //No fair traction simulation can be achieved with such materials
        }
    }
    
    float turn_radius = 1.0f/steer_amount; // not really the "turn radius" but proportional
    float current_hardness = m_c_hardness_multiplier*m_c_heat_cycle_hardness_curve.get((m_heat_cycle_count));
    float deg_tur = 0.0f;
    float deg_tra = 0.0f;
    float deg_tur_percent = 0.0f;
    float deg_tra_percent = 0.0f;
    float regen_amount = 0.0f;

    m_center_of_gravity_x = m_acceleration*m_kart->getMass();
    m_center_of_gravity_y = ((speed*speed)/turn_radius)*m_kart->getMass();

    //Doesn't make much sense to degrade the tyres midair or in reverse or at ridiculously low speeds, now does it?
    if (!is_on_ground || speed < 1.0f) goto LOG_ZONE;

    if (throttle_amount > 0.3f) {
        m_current_fuel -= std::abs(speed)*dt*m_c_fuel_rate*(1.0f/1000.0f); /*1 meter -> 0.005 units of fuel, 200 meters -> 1 unit of fuel*/
    } else {
        m_current_fuel -= 0.5f*std::abs(speed)*dt*m_c_fuel_rate*(1.0f/1000.0f); /*1 meter -> 0.005 units of fuel, 200 meters -> 1 unit of fuel*/
    }

    if(m_center_of_gravity_x < 0.0f && throttle_amount < 0.3f) {
        regen_amount += std::abs(m_center_of_gravity_x)*0.00000001f*dt*m_c_fuel_regen;
    }

    
    deg_tra = dt*std::abs(m_center_of_gravity_x)*current_hardness/100000.0f;
    deg_tra += dt*std::abs(speed)/50.0f;

    if (brake_amount > m_c_brake_threshold) {
        deg_tra *= brake_amount*(1.0f/m_c_brake_threshold);
        regen_amount *= 2;
    }
    if (slowdown < 0.98f && !is_using_zipper) {
        deg_tra *= m_c_offroad_factor;
    }

    deg_tur = dt*std::abs(m_center_of_gravity_y)*current_hardness/10000.0f;

    if (is_skidding) {
        deg_tur *= m_c_skid_factor;
        regen_amount *= 2;
    }

    m_current_fuel += regen_amount;
    if (m_current_fuel > 1000.0f) m_current_fuel = 1000.0f;
    if (m_current_fuel < 0.0f) m_current_fuel = 0.0f;


    deg_tra_percent = deg_tra/m_c_max_life_traction;
    deg_tur_percent = deg_tur/m_c_max_life_turning;

    if(m_current_life_traction < m_current_life_turning) {
        m_current_life_turning -= deg_tra_percent*m_c_limiting_transfer_traction*m_c_max_life_turning;
        m_current_life_traction -= deg_tur_percent*m_c_regular_transfer_turning*m_c_max_life_traction;
    } else {
        m_current_life_turning -= deg_tra_percent*m_c_regular_transfer_traction*m_c_max_life_turning;
        m_current_life_traction -= deg_tur_percent*m_c_limiting_transfer_turning*m_c_max_life_traction;
    }

    m_current_life_traction -= deg_tra;
    m_current_life_turning -= deg_tur;
    if (m_current_life_traction < 0.0f) m_current_life_traction = 0.0f;
    if (m_current_life_turning < 0.0f) m_current_life_turning = 0.0f;

    LOG_ZONE:
    ;

    //printf("Cycle %20lu || K %s || C %u\n\ttrac: %f%% ||| turn: %f%%\n", m_debug_cycles, m_kart->getIdent().c_str(),
    //m_current_compound, 100.0f*(m_current_life_traction)/m_c_max_life_traction, 100.0f*(m_current_life_turning)/m_c_max_life_turning);
    //printf("\tCenter of gravity: (%f, %f)\n\tTurn: %f || Speed:%f || Brake: %f\n", m_center_of_gravity_x,
    //m_center_of_gravity_y, turn_radius, speed, brake_amount);
    //printf("\tFuel: %f || Weight: %f || TrackLength: %f\n", m_current_fuel, m_kart->getMass(), Track::getCurrentTrack()->getTrackLength());
}

void Tyres::applyCrashPenalty(void) {
    m_current_life_traction -= (m_c_crash_penalty/100.0f)*m_c_max_life_traction;
    m_current_life_turning -= (m_c_crash_penalty/100.0f)*m_c_max_life_turning;
}

float Tyres::degEngineForce(float initial_force) {
    float current_hardness = m_c_hardness_multiplier*m_c_heat_cycle_hardness_curve.get((m_heat_cycle_count));
    float hardness_deviation = (current_hardness - m_c_hardness_multiplier) / m_c_hardness_multiplier;
    float hardness_penalty = current_hardness *  m_c_hardness_penalty_curve.get((hardness_deviation*100));
    float percent = m_current_life_traction/m_c_max_life_traction;
    float factor = m_c_response_curve_traction.get(correct(percent*100.0f))*m_c_traction_constant;
    float bonus_traction = (initial_force+m_c_initial_bonus_add_traction)*m_c_initial_bonus_mult_traction;
    if (m_c_do_substractive_traction) {
        return bonus_traction - hardness_penalty*factor;
    } else {
        return bonus_traction*hardness_penalty*factor;
    }
}

float Tyres::degTurnRadius(float initial_radius) {
    float current_hardness = m_c_hardness_multiplier*m_c_heat_cycle_hardness_curve.get((m_heat_cycle_count));
    float hardness_deviation = (current_hardness - m_c_hardness_multiplier) / m_c_hardness_multiplier;
    float hardness_penalty = current_hardness *  m_c_hardness_penalty_curve.get((hardness_deviation*100));
    float percent = m_current_life_turning/m_c_max_life_turning;
    float factor = m_c_response_curve_turning.get(correct(percent*100.0f))*m_c_turning_constant;
    float bonus_turning = (initial_radius+m_c_initial_bonus_add_turning)*m_c_initial_bonus_mult_turning;
    if (m_c_do_substractive_turning) {
        return bonus_turning - hardness_penalty*factor;
    } else {
        return bonus_turning*hardness_penalty*factor;
    }
}

float Tyres::degTopSpeed(float initial_topspeed) {
    float current_hardness = m_c_hardness_multiplier*m_c_heat_cycle_hardness_curve.get((m_heat_cycle_count));
    float hardness_deviation = (current_hardness - m_c_hardness_multiplier) / m_c_hardness_multiplier;
    float hardness_penalty = current_hardness *  m_c_hardness_penalty_curve.get((hardness_deviation*100));
    float percent = m_current_life_traction/m_c_max_life_traction;
    float factor = m_c_response_curve_topspeed.get(correct(percent*100.0f))*m_c_topspeed_constant;
    float bonus_topspeed = (initial_topspeed+m_c_initial_bonus_add_topspeed)*m_c_initial_bonus_mult_topspeed;
    if (m_c_do_substractive_topspeed && m_current_fuel > 0.1f) {
        return bonus_topspeed - hardness_penalty*factor;
    } else if (m_current_fuel > 0.1f) {
        return bonus_topspeed*hardness_penalty*factor;
    } else {
        return 5;
    }
}


void Tyres::reset() {

    if (m_reset_compound) {
        const float kart_hue = RaceManager::get()->getKartColor(m_kart->getWorldKartId()) * 100.0f;
        if (kart_hue < 0.5f) { /*Color 0 -> random kart color*/
            m_current_compound = ((int)rand() % 3) + 2; /*Should be modulo the compound number, but at the moment some compounds are not finished*/
        } else {
            m_current_compound = ((int)kart_hue % (int)m_kart->getKartProperties()->getTyresCompoundNumber()) + 1;
        }
        system((std::string("tools/runrecord.sh ") + RaceManager::get()->getTrackName().c_str() + " S " + std::to_string(m_current_compound).c_str() + " " + m_kart->getIdent().c_str()).c_str());
        printf("S;%s;%s;%s\n", m_kart->getIdent().c_str(), RaceManager::get()->getTrackName().c_str(), std::to_string(m_current_compound).c_str());
    } else if (m_kart->getKartProperties()->getTyresDefaultColor()[m_current_compound-1] > -0.5f) {
        const float kart_hue = m_kart->getKartProperties()->getTyresDefaultColor()[m_current_compound-1]/100.0f;
        m_kart->setKartColor(kart_hue);
        printf("Setting color to %f\n", m_kart->getKartProperties()->getTyresDefaultColor()[m_current_compound-1]);
    }
    if (m_reset_fuel) {
        m_kart->m_tyres_queue = std::get<2>(RaceManager::get()->getFuelAndQueueInfo());
        m_current_fuel = m_c_fuel;
    }

    m_lap_count = 0;

    m_current_life_traction = m_kart->getKartProperties()->getTyresMaxLifeTraction()[m_current_compound-1];
    m_current_life_turning = m_kart->getKartProperties()->getTyresMaxLifeTurning()[m_current_compound-1];
    m_heat_cycle_count = 0.0f;
    m_current_temp = m_kart->getKartProperties()->getTyresIdealTemp()[m_current_compound-1];
    m_center_of_gravity_x = 0.0f;
    m_center_of_gravity_y = 0.0f;
    m_previous_speeds.clear();
    m_acceleration = 0.0f;
    m_time_elapsed = 0.0f;
    m_debug_cycles = 0;

    m_c_fuel = (std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[0];
    m_c_fuel_regen = (std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[1];
    m_c_fuel_stop = (std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[2];
    m_c_fuel_weight = ((std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[3])/100.0f;
    m_c_fuel_rate = (std::get<0>(RaceManager::get()->getFuelAndQueueInfo()))[4];
    irr::core::clamp(m_c_fuel, 1.0f, 1000.0f);
    irr::core::clamp(m_c_fuel_stop, 0.0f, 1000.0f);
    irr::core::clamp(m_c_fuel_rate, 0.0f, 1000.0f);
    irr::core::clamp(m_c_fuel_regen, 0.0f, 100.0f);
    irr::core::clamp(m_c_fuel_weight, 0.0f, 1.0f);

    m_c_hardness_multiplier = m_kart->getKartProperties()->getTyresHardnessMultiplier()[m_current_compound-1];
    m_c_heat_cycle_hardness_curve = m_kart->getKartProperties()->getTyresHeatCycleHardnessCurve();
    m_c_hardness_penalty_curve = m_kart->getKartProperties()->getTyresHardnessPenaltyCurve();

    m_c_mass = m_kart->getKartProperties()->getMass();
    m_c_ideal_temp = m_kart->getKartProperties()->getTyresIdealTemp()[m_current_compound-1];
    m_c_max_life_traction = m_kart->getKartProperties()->getTyresMaxLifeTraction()[m_current_compound-1];
    m_c_max_life_turning = m_kart->getKartProperties()->getTyresMaxLifeTurning()[m_current_compound-1];
    m_c_min_life_traction = m_kart->getKartProperties()->getTyresMinLifeTraction()[m_current_compound-1];
    m_c_min_life_turning = m_kart->getKartProperties()->getTyresMinLifeTurning()[m_current_compound-1];
    m_c_limiting_transfer_traction = m_kart->getKartProperties()->getTyresLimitingTransferTraction()[m_current_compound-1];
    m_c_regular_transfer_traction = m_kart->getKartProperties()->getTyresRegularTransferTraction()[m_current_compound-1];
    m_c_limiting_transfer_turning = m_kart->getKartProperties()->getTyresLimitingTransferTurning()[m_current_compound-1];
    m_c_regular_transfer_turning = m_kart->getKartProperties()->getTyresRegularTransferTurning()[m_current_compound-1];

    m_c_do_substractive_traction = m_kart->getKartProperties()->getTyresDoSubstractiveTraction()[m_current_compound-1] > 0.5f;
    m_c_do_grip_based_turning  = m_kart->getKartProperties()->getTyresDoGripBasedTurning()[m_current_compound-1] > 0.5f;
    m_c_do_substractive_turning  = m_kart->getKartProperties()->getTyresDoSubstractiveTurning()[m_current_compound-1] > 0.5f;
    m_c_do_substractive_topspeed  = m_kart->getKartProperties()->getTyresDoSubstractiveTopspeed()[m_current_compound-1] > 0.5f;

    m_c_response_curve_traction = m_kart->getKartProperties()->getTyresResponseCurveTraction();
    m_c_response_curve_turning = m_kart->getKartProperties()->getTyresResponseCurveTurning();
    m_c_response_curve_topspeed = m_kart->getKartProperties()->getTyresResponseCurveTopspeed();
    m_c_initial_bonus_mult_traction = m_kart->getKartProperties()->getTyresInitialBonusMultTraction()[m_current_compound-1];
    m_c_initial_bonus_add_traction = m_kart->getKartProperties()->getTyresInitialBonusAddTraction()[m_current_compound-1];
    m_c_initial_bonus_mult_turning = m_kart->getKartProperties()->getTyresInitialBonusMultTurning()[m_current_compound-1];
    m_c_initial_bonus_add_turning = m_kart->getKartProperties()->getTyresInitialBonusAddTurning()[m_current_compound-1];
    m_c_initial_bonus_mult_topspeed = m_kart->getKartProperties()->getTyresInitialBonusMultTopspeed()[m_current_compound-1];
    m_c_initial_bonus_add_topspeed = m_kart->getKartProperties()->getTyresInitialBonusAddTopspeed()[m_current_compound-1];
    m_c_traction_constant = m_kart->getKartProperties()->getTyresTractionConstant()[m_current_compound-1];
    m_c_turning_constant = m_kart->getKartProperties()->getTyresTurningConstant()[m_current_compound-1];
    m_c_topspeed_constant = m_kart->getKartProperties()->getTyresTopspeedConstant()[m_current_compound-1];

    m_c_offroad_factor = m_kart->getKartProperties()->getTyresOffroadFactor()[m_current_compound-1];
    m_c_skid_factor = m_kart->getKartProperties()->getTyresSkidFactor()[m_current_compound-1];
    m_c_brake_threshold = m_kart->getKartProperties()->getTyresBrakeThreshold()[m_current_compound-1];
    m_c_crash_penalty = m_kart->getKartProperties()->getTyresCrashPenalty()[m_current_compound-1];

}


void Tyres::saveState(BareNetworkString *buffer)
{
    buffer->addFloat(m_current_life_traction);
    buffer->addFloat(m_current_life_turning);
    buffer->addFloat(m_current_temp);
    buffer->addFloat(m_heat_cycle_count);
    buffer->addFloat(m_current_fuel);
    buffer->addFloat(m_kart->m_target_refuel);
    buffer->addUInt8(m_current_compound);
    buffer->addUInt8(m_lap_count);
    buffer->addUInt8(m_kart->m_tyres_queue.size());
    for (long unsigned i = 0; i < m_kart->m_tyres_queue.size(); i++) {
        buffer->addUInt8(m_kart->m_tyres_queue[i]+1);
    }
//  printf("Saved compound %u, kart: %s\n", m_current_compound, m_kart->getIdent().c_str());
}

void Tyres::rewindTo(BareNetworkString *buffer)
{
    m_current_life_traction = buffer->getFloat();
    m_current_life_turning = buffer->getFloat();
    m_current_temp = buffer->getFloat();
    m_heat_cycle_count = buffer->getFloat();
    m_current_fuel = buffer->getFloat();
    m_kart->m_target_refuel = buffer->getFloat();
    m_current_compound = buffer->getUInt8();
    m_lap_count = buffer->getUInt8();
    unsigned queue_size = buffer->getUInt8();
    std::vector<int> tmpvec;
    for (unsigned i = 0; i < queue_size; i++) {
        int tmpint = buffer->getUInt8()-1;
        tmpvec.push_back(tmpint);
    }
    m_kart->m_tyres_queue = tmpvec;
//  printf("Rewinded compound %u, kart: %s\n", m_current_compound, m_kart->getIdent().c_str());
}
