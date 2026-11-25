//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006-2015 SuperTuxKart-Team
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

#include "karts/cached_characteristic.hpp"

#include "utils/interpolation_array.hpp"

CachedCharacteristic::CachedCharacteristic(const AbstractCharacteristic *origin) :
    m_values(CHARACTERISTIC_COUNT),
    m_origin(origin)
{
    updateSource();
}

// ----------------------------------------------------------------------------
/** Deletes all allocated values. */
CachedCharacteristic::~CachedCharacteristic()
{
    // Delete all not-null values
    for (int i = 0; i < CHARACTERISTIC_COUNT; i++)
    {
        SaveValue &v = m_values[i];
        if (v.content)
        {
            switch (getType(static_cast<CharacteristicType>(i)))
            {
            case TYPE_FLOAT:
                delete static_cast<float*>(v.content);
                break;
            case TYPE_FLOAT_VECTOR:
                delete static_cast<std::vector<float>*>(v.content);
                break;
            case TYPE_INTERPOLATION_ARRAY:
                delete static_cast<InterpolationArray*>(v.content);
                break;
            case TYPE_BOOL:
                delete static_cast<bool*>(v.content);
                break;
            case TYPE_STRING:
                delete static_cast<std::string*>(v.content);
                break;
            }
            v.content = nullptr;
        }
    }
}   // ~CachedCharacteristic

// ----------------------------------------------------------------------------
/** Recompute the values of all characteristics based on the list of
 *  source-characteristics.
 */
void CachedCharacteristic::updateSource()
{
    for (int i = 0; i < CHARACTERISTIC_COUNT; i++)
    {
        SaveValue &v = m_values[i];
        
        bool is_set = false;
        switch (getType(static_cast<CharacteristicType>(i)))
        {
        case TYPE_FLOAT:
        {
            float value;
            float *ptr = static_cast<float*>(v.content);
            m_origin->process(static_cast<CharacteristicType>(i), &value, &is_set);
            if (is_set)
            {
                if (!ptr)
                {
                    float *newPtr = new float();
                    v.content = newPtr;
                    ptr = newPtr;
                }
                *ptr = value;
            }
            else
            {
                if (ptr)
                {
                    delete ptr;
                    v.content = nullptr;
                }
            }
            break;
        }
        case TYPE_FLOAT_VECTOR:
        {
            std::vector<float> value;
            std::vector<float> *ptr = static_cast<std::vector<float>*>(v.content);
            m_origin->process(static_cast<CharacteristicType>(i), &value, &is_set);
            if (is_set)
            {
                if (!ptr)
                {
                    std::vector<float> *newPtr = new std::vector<float>();
                    v.content = newPtr;
                    ptr = newPtr;
                }
                *ptr = value;
            }
            else
            {
                if (ptr)
                {
                    delete ptr;
                    v.content = nullptr;
                }
            }
            break;
        }
        case TYPE_INTERPOLATION_ARRAY:
        {
            InterpolationArray value;
            InterpolationArray *ptr = static_cast<InterpolationArray*>(v.content);
            m_origin->process(static_cast<CharacteristicType>(i), &value, &is_set);
            if (is_set)
            {
                if (!ptr)
                {
                    InterpolationArray *newPtr = new InterpolationArray();
                    v.content = newPtr;
                    ptr = newPtr;
                }
                *ptr = value;
            }
            else
            {
                if (ptr)
                {
                    delete ptr;
                    v.content = nullptr;
                }
            }
            break;
        }
        case TYPE_BOOL:
        {
            bool value;
            bool *ptr = static_cast<bool*>(v.content);
            m_origin->process(static_cast<CharacteristicType>(i), &value, &is_set);
            if (is_set)
            {
                if (!ptr)
                {
                    bool *newPtr = new bool();
                    v.content = newPtr;
                    ptr = newPtr;
                }
                *ptr = value;
            }
            else
            {
                if (ptr)
                {
                    delete ptr;
                    v.content = nullptr;
                }
            }
            break;
        }
        case TYPE_STRING:
        {
            std::string value;
            std::string *ptr = static_cast<std::string*>(v.content);
            m_origin->process(static_cast<CharacteristicType>(i), &value, &is_set);
            if (is_set)
            {
                if (!ptr)
                {
                    std::string *newPtr = new std::string();
                    v.content = newPtr;
                    ptr = newPtr;
                }
                *ptr = value;
            }
            else
            {
                if (ptr)
                {
                    delete ptr;
                    v.content = nullptr;
                }
            }
            break;
        }
        }   // switch (type)
    }   // foreach characteristic
}   // updateSource

// ----------------------------------------------------------------------------
/** Returns the stored value. */
void CachedCharacteristic::process(CharacteristicType type, Value value,
                                   bool *is_set) const
{
    void *v = m_values[type].content;
    if (v)
    {
        switch (getType(type))
        {
        case TYPE_FLOAT:
            *value.f = *static_cast<float*>(v);
            break;
        case TYPE_FLOAT_VECTOR:
            *value.fv = *static_cast<std::vector<float>*>(v);
            break;
        case TYPE_INTERPOLATION_ARRAY:
            *value.ia = *static_cast<InterpolationArray*>(v);
            break;
        case TYPE_BOOL:
            *value.b = *static_cast<bool*>(v);
            break;
        case TYPE_STRING:
            *value.str = *static_cast<std::string*>(v);
            break;
        }
        *is_set = true;
    }
}   // process

void CachedCharacteristic::saveState(BareNetworkString *buffer) const {
    /* <characteristics-start ccnetworksave> */
    buffer->addFloat(*(static_cast<float *>(m_values[0].content))); // SuspensionStiffness
    buffer->addFloat(*(static_cast<float *>(m_values[1].content))); // SuspensionRest
    buffer->addFloat(*(static_cast<float *>(m_values[2].content))); // SuspensionTravel
    buffer->addUInt8(*(static_cast<bool *>(m_values[3].content))); // SuspensionExpSpringResponse
    buffer->addFloat(*(static_cast<float *>(m_values[4].content))); // SuspensionMaxForce
    buffer->addFloat(*(static_cast<float *>(m_values[5].content))); // StabilityRollInfluence
    buffer->addFloat(*(static_cast<float *>(m_values[6].content))); // StabilityChassisLinearDamping
    buffer->addFloat(*(static_cast<float *>(m_values[7].content))); // StabilityChassisAngularDamping
    buffer->addFloat(*(static_cast<float *>(m_values[8].content))); // StabilityDownwardImpulseFactor
    buffer->addFloat(*(static_cast<float *>(m_values[9].content))); // StabilityTrackConnectionAccel
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[10].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[10].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[10].content))[i]); // StabilityAngularFactor
    }
    buffer->addFloat(*(static_cast<float *>(m_values[11].content))); // StabilitySmoothFlyingImpulse
    buffer->encodeInterpolationArray(*(static_cast<InterpolationArray *>(m_values[12].content))); // TurnRadius
    buffer->encodeInterpolationArray(*(static_cast<InterpolationArray *>(m_values[13].content))); // TurnTimeFullSteer
    buffer->addFloat(*(static_cast<float *>(m_values[14].content))); // TurnTimeResetSteer
    buffer->addFloat(*(static_cast<float *>(m_values[15].content))); // TurnBrakeMultiplier
    buffer->addFloat(*(static_cast<float *>(m_values[16].content))); // EnginePower
    buffer->addFloat(*(static_cast<float *>(m_values[17].content))); // EngineMaxSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[18].content))); // EngineGenericMaxSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[19].content))); // EngineBrakeFactor
    buffer->addFloat(*(static_cast<float *>(m_values[20].content))); // EngineTimeFullBrake
    buffer->addFloat(*(static_cast<float *>(m_values[21].content))); // EngineMaxSpeedReverseRatio
    buffer->addFloat(*(static_cast<float *>(m_values[22].content))); // EngineRearForceFraction
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[23].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[23].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[23].content))[i]); // GearSwitchRatio
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[24].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[24].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[24].content))[i]); // GearPowerIncrease
    }
    buffer->addFloat(*(static_cast<float *>(m_values[25].content))); // Mass
    buffer->addFloat(*(static_cast<float *>(m_values[26].content))); // TrackZipperFactor
    buffer->addFloat(*(static_cast<float *>(m_values[27].content))); // VirtualMass
    buffer->addFloat(*(static_cast<float *>(m_values[28].content))); // FuelMassReal
    buffer->addFloat(*(static_cast<float *>(m_values[29].content))); // FuelMassVirtual
    buffer->addFloat(*(static_cast<float *>(m_values[30].content))); // FuelConsumption
    buffer->addFloat(*(static_cast<float *>(m_values[31].content))); // FuelCapacity
    buffer->addFloat(*(static_cast<float *>(m_values[32].content))); // FuelStopRate
    buffer->addFloat(*(static_cast<float *>(m_values[33].content))); // FuelMaxSpeedDecrease
    buffer->addFloat(*(static_cast<float *>(m_values[34].content))); // FuelTurnRadiusIncrease
    buffer->addFloat(*(static_cast<float *>(m_values[35].content))); // FuelLiftAndCoastFactor
    buffer->addFloat(*(static_cast<float *>(m_values[36].content))); // WheelsDampingRelaxation
    buffer->addFloat(*(static_cast<float *>(m_values[37].content))); // WheelsDampingCompression
    buffer->addFloat(*(static_cast<float *>(m_values[38].content))); // JumpAnimationTime
    buffer->addFloat(*(static_cast<float *>(m_values[39].content))); // LeanMax
    buffer->addFloat(*(static_cast<float *>(m_values[40].content))); // LeanSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[41].content))); // AnvilDuration
    buffer->addFloat(*(static_cast<float *>(m_values[42].content))); // AnvilWeight
    buffer->addFloat(*(static_cast<float *>(m_values[43].content))); // AnvilSpeedFactor
    buffer->addFloat(*(static_cast<float *>(m_values[44].content))); // ParachuteFriction
    buffer->addFloat(*(static_cast<float *>(m_values[45].content))); // ParachuteDuration
    buffer->addFloat(*(static_cast<float *>(m_values[46].content))); // ParachuteDurationOther
    buffer->addFloat(*(static_cast<float *>(m_values[47].content))); // ParachuteDurationRankMult
    buffer->addFloat(*(static_cast<float *>(m_values[48].content))); // ParachuteDurationSpeedMult
    buffer->addFloat(*(static_cast<float *>(m_values[49].content))); // ParachuteLboundFraction
    buffer->addFloat(*(static_cast<float *>(m_values[50].content))); // ParachuteUboundFraction
    buffer->addFloat(*(static_cast<float *>(m_values[51].content))); // ParachuteMaxSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[52].content))); // FrictionKartFriction
    buffer->addFloat(*(static_cast<float *>(m_values[53].content))); // FrictionSlowdownFactor
    buffer->addFloat(*(static_cast<float *>(m_values[54].content))); // FrictionDragCoefficient
    buffer->addFloat(*(static_cast<float *>(m_values[55].content))); // FrictionDragExponent
    buffer->addUInt8(*(static_cast<bool *>(m_values[56].content))); // FrictionGameEngineSpeedUncap
    buffer->addFloat(*(static_cast<float *>(m_values[57].content))); // FrictionWallPenaltyTime
    buffer->addFloat(*(static_cast<float *>(m_values[58].content))); // FrictionWallPenaltyFadeInTime
    buffer->addFloat(*(static_cast<float *>(m_values[59].content))); // FrictionWallPenaltySlowdownFraction
    buffer->addFloat(*(static_cast<float *>(m_values[60].content))); // FrictionWallPenaltyEngineMultiplier
    buffer->addFloat(*(static_cast<float *>(m_values[61].content))); // BubblegumDuration
    buffer->addFloat(*(static_cast<float *>(m_values[62].content))); // BubblegumSpeedFraction
    buffer->addFloat(*(static_cast<float *>(m_values[63].content))); // BubblegumTorque
    buffer->addFloat(*(static_cast<float *>(m_values[64].content))); // BubblegumFadeInTime
    buffer->addFloat(*(static_cast<float *>(m_values[65].content))); // BubblegumShieldDuration
    buffer->addFloat(*(static_cast<float *>(m_values[66].content))); // BubblegumMiniBoostEngineForce
    buffer->addFloat(*(static_cast<float *>(m_values[67].content))); // BubblegumMiniBoostAddedSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[68].content))); // BubblegumMiniBoostMaxSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[69].content))); // BubblegumMiniBoostDuration
    buffer->addFloat(*(static_cast<float *>(m_values[70].content))); // BubblegumMiniBoostFadeOutTime
    buffer->addFloat(*(static_cast<float *>(m_values[71].content))); // BubblegumMiniCollectionDurationMultiplier
    buffer->addFloat(*(static_cast<float *>(m_values[72].content))); // BubblegumBoostEngineForce
    buffer->addFloat(*(static_cast<float *>(m_values[73].content))); // BubblegumBoostAddedSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[74].content))); // BubblegumBoostMaxSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[75].content))); // BubblegumBoostDuration
    buffer->addFloat(*(static_cast<float *>(m_values[76].content))); // BubblegumBoostFadeOutTime
    buffer->addFloat(*(static_cast<float *>(m_values[77].content))); // BubblegumCollectionDurationMultiplier
    buffer->addFloat(*(static_cast<float *>(m_values[78].content))); // ZipperDuration
    buffer->addFloat(*(static_cast<float *>(m_values[79].content))); // ZipperForce
    buffer->addFloat(*(static_cast<float *>(m_values[80].content))); // ZipperSpeedGain
    buffer->addFloat(*(static_cast<float *>(m_values[81].content))); // ZipperMaxSpeedIncrease
    buffer->addFloat(*(static_cast<float *>(m_values[82].content))); // ZipperFadeOutTime
    buffer->addFloat(*(static_cast<float *>(m_values[83].content))); // SwatterDuration
    buffer->addFloat(*(static_cast<float *>(m_values[84].content))); // SwatterDistance
    buffer->addFloat(*(static_cast<float *>(m_values[85].content))); // SwatterSquashDuration
    buffer->addFloat(*(static_cast<float *>(m_values[86].content))); // SwatterSquashSlowdown
    buffer->addFloat(*(static_cast<float *>(m_values[87].content))); // PlungerBandMaxLength
    buffer->addFloat(*(static_cast<float *>(m_values[88].content))); // PlungerBandForce
    buffer->addFloat(*(static_cast<float *>(m_values[89].content))); // PlungerBandDuration
    buffer->addFloat(*(static_cast<float *>(m_values[90].content))); // PlungerBandSpeedIncrease
    buffer->addFloat(*(static_cast<float *>(m_values[91].content))); // PlungerBandFadeOutTime
    buffer->addFloat(*(static_cast<float *>(m_values[92].content))); // NitroHackDuration
    buffer->addFloat(*(static_cast<float *>(m_values[93].content))); // NitroHackFactor
    buffer->addFloat(*(static_cast<float *>(m_values[94].content))); // ElectroDuration
    buffer->addFloat(*(static_cast<float *>(m_values[95].content))); // ElectroEngineMult
    buffer->addFloat(*(static_cast<float *>(m_values[96].content))); // ElectroMaxSpeedIncrease
    buffer->addFloat(*(static_cast<float *>(m_values[97].content))); // ElectroFadeOutTime
    buffer->addFloat(*(static_cast<float *>(m_values[98].content))); // TyresPitSpeedFraction
    buffer->addFloat(*(static_cast<float *>(m_values[99].content))); // TyresPitTimeMultiplier
    buffer->encodeString32L(*(static_cast<std::string *>(m_values[100].content))); // TyresChangeKartMap
    buffer->encodeString32L(*(static_cast<std::string *>(m_values[101].content))); // TyresNamesLong
    buffer->encodeString32L(*(static_cast<std::string *>(m_values[102].content))); // TyresNamesShort
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[103].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[103].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[103].content))[i]); // TyresMaxLifeTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[104].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[104].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[104].content))[i]); // TyresMaxLifeTraction
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[105].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[105].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[105].content))[i]); // TyresMinLifeTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[106].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[106].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[106].content))[i]); // TyresMinLifeTraction
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[107].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[107].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[107].content))[i]); // TyresMinLifeTurningGui
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[108].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[108].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[108].content))[i]); // TyresMinLifeTractionGui
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[109].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[109].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[109].content))[i]); // TyresRegularTransferTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[110].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[110].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[110].content))[i]); // TyresRegularTransferTraction
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[111].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[111].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[111].content))[i]); // TyresLimitingTransferTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[112].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[112].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[112].content))[i]); // TyresLimitingTransferTraction
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[113].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[113].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[113].content))[i]); // TyresInitialBonusAddTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[114].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[114].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[114].content))[i]); // TyresInitialBonusMultTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[115].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[115].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[115].content))[i]); // TyresInitialBonusAddTraction
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[116].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[116].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[116].content))[i]); // TyresInitialBonusMultTraction
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[117].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[117].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[117].content))[i]); // TyresInitialBonusAddTopspeed
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[118].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[118].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[118].content))[i]); // TyresInitialBonusMultTopspeed
    }
    buffer->encodeString32L(*(static_cast<std::string *>(m_values[119].content))); // TyresResponseCurveTurning
    buffer->encodeString32L(*(static_cast<std::string *>(m_values[120].content))); // TyresResponseCurveTraction
    buffer->encodeString32L(*(static_cast<std::string *>(m_values[121].content))); // TyresResponseCurveTopspeed
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[122].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[122].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[122].content))[i]); // TyresDoGripBasedTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[123].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[123].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[123].content))[i]); // TyresDoSubstractiveTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[124].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[124].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[124].content))[i]); // TyresDoSubstractiveTraction
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[125].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[125].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[125].content))[i]); // TyresDoSubstractiveTopspeed
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[126].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[126].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[126].content))[i]); // TyresTractionConstant
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[127].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[127].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[127].content))[i]); // TyresTurningConstant
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[128].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[128].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[128].content))[i]); // TyresTopspeedConstant
    }
    buffer->addFloat(*(static_cast<float *>(m_values[129].content))); // TyresCompoundNumber
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[130].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[130].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[130].content))[i]); // TyresLowGripTopspeedMult
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[131].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[131].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[131].content))[i]); // TyresLowGripEngineforceMult
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[132].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[132].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[132].content))[i]); // TyresLowGripTurningMult
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[133].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[133].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[133].content))[i]); // TyresOffroadFactor
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[134].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[134].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[134].content))[i]); // TyresRollingResistance
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[135].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[135].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[135].content))[i]); // TyresSkidFactorPartial
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[136].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[136].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[136].content))[i]); // TyresSkidFactorFull
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[137].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[137].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[137].content))[i]); // TyresUsageMultiplierTurning
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[138].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[138].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[138].content))[i]); // TyresUsageMultiplierTraction
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[139].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[139].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[139].content))[i]); // TyresReferenceSpeedMult
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[140].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[140].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[140].content))[i]); // TyresBrakeThreshold
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[141].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[141].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[141].content))[i]); // TyresCrashPenalty
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[142].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[142].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[142].content))[i]); // TyresDefaultColor
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[143].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[143].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[143].content))[i]); // StartupTime
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[144].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[144].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[144].content))[i]); // StartupBoost
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[145].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[145].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[145].content))[i]); // StartupEngineForce
    }
    buffer->addFloat(*(static_cast<float *>(m_values[146].content))); // StartupDuration
    buffer->addFloat(*(static_cast<float *>(m_values[147].content))); // StartupFadeOutTime
    buffer->addFloat(*(static_cast<float *>(m_values[148].content))); // RescueDuration
    buffer->addFloat(*(static_cast<float *>(m_values[149].content))); // RescueVertOffset
    buffer->addFloat(*(static_cast<float *>(m_values[150].content))); // RescueHeight
    buffer->addFloat(*(static_cast<float *>(m_values[151].content))); // ExplosionDuration
    buffer->addFloat(*(static_cast<float *>(m_values[152].content))); // ExplosionRadius
    buffer->addFloat(*(static_cast<float *>(m_values[153].content))); // ExplosionInvulnerabilityTime
    buffer->addFloat(*(static_cast<float *>(m_values[154].content))); // NitroDuration
    buffer->addFloat(*(static_cast<float *>(m_values[155].content))); // NitroEngineForce
    buffer->addFloat(*(static_cast<float *>(m_values[156].content))); // NitroEngineMult
    buffer->addFloat(*(static_cast<float *>(m_values[157].content))); // NitroConsumption
    buffer->addFloat(*(static_cast<float *>(m_values[158].content))); // NitroSmallContainer
    buffer->addFloat(*(static_cast<float *>(m_values[159].content))); // NitroBigContainer
    buffer->addFloat(*(static_cast<float *>(m_values[160].content))); // NitroMaxSpeedIncrease
    buffer->addFloat(*(static_cast<float *>(m_values[161].content))); // NitroMinBurst
    buffer->addFloat(*(static_cast<float *>(m_values[162].content))); // NitroFadeOutTime
    buffer->addFloat(*(static_cast<float *>(m_values[163].content))); // NitroMax
    buffer->addFloat(*(static_cast<float *>(m_values[164].content))); // SlipstreamDurationFactor
    buffer->addFloat(*(static_cast<float *>(m_values[165].content))); // SlipstreamBaseSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[166].content))); // SlipstreamLength
    buffer->addFloat(*(static_cast<float *>(m_values[167].content))); // SlipstreamWidth
    buffer->addFloat(*(static_cast<float *>(m_values[168].content))); // SlipstreamInnerFactor
    buffer->addFloat(*(static_cast<float *>(m_values[169].content))); // SlipstreamMinCollectTime
    buffer->addFloat(*(static_cast<float *>(m_values[170].content))); // SlipstreamMaxCollectTime
    buffer->addFloat(*(static_cast<float *>(m_values[171].content))); // SlipstreamAddPower
    buffer->addFloat(*(static_cast<float *>(m_values[172].content))); // SlipstreamMinSpeed
    buffer->addFloat(*(static_cast<float *>(m_values[173].content))); // SlipstreamMaxSpeedIncrease
    buffer->addFloat(*(static_cast<float *>(m_values[174].content))); // SlipstreamFadeOutTime
    buffer->addUInt8(*(static_cast<bool *>(m_values[175].content))); // SkidEnabled
    buffer->encodeString32L(*(static_cast<std::string *>(m_values[176].content))); // SkidMode
    buffer->addFloat(*(static_cast<float *>(m_values[177].content))); // SkidIncrease
    buffer->addFloat(*(static_cast<float *>(m_values[178].content))); // SkidDecrease
    buffer->addFloat(*(static_cast<float *>(m_values[179].content))); // SkidMax
    buffer->addFloat(*(static_cast<float *>(m_values[180].content))); // SkidTimeTillMax
    buffer->addFloat(*(static_cast<float *>(m_values[181].content))); // SkidSlowdown
    buffer->addFloat(*(static_cast<float *>(m_values[182].content))); // SkidFadeIn
    buffer->addFloat(*(static_cast<float *>(m_values[183].content))); // SkidFadeOut
    buffer->addFloat(*(static_cast<float *>(m_values[184].content))); // SkidVisual
    buffer->addFloat(*(static_cast<float *>(m_values[185].content))); // SkidVisualTime
    buffer->addFloat(*(static_cast<float *>(m_values[186].content))); // SkidRevertVisualTime
    buffer->addFloat(*(static_cast<float *>(m_values[187].content))); // SkidMinSpeed
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[188].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[188].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[188].content))[i]); // SkidTimeTillBonus
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[189].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[189].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[189].content))[i]); // SkidBonusSpeed
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[190].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[190].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[190].content))[i]); // SkidBonusTime
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[191].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[191].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[191].content))[i]); // SkidFadeOutTime
    }
buffer->addUInt32((static_cast<std::vector<float> *>(m_values[192].content))->size());
    for (unsigned i = 0; i < (static_cast<std::vector<float> *>(m_values[192].content))->size(); i++) {
        buffer->addFloat((*static_cast<std::vector<float> *>(m_values[192].content))[i]); // SkidBonusForce
    }
    buffer->addFloat(*(static_cast<float *>(m_values[193].content))); // SkidPhysicalJumpTime
    buffer->addFloat(*(static_cast<float *>(m_values[194].content))); // SkidGraphicalJumpTime
    buffer->addFloat(*(static_cast<float *>(m_values[195].content))); // SkidPostSkidRotateFactor
    buffer->addFloat(*(static_cast<float *>(m_values[196].content))); // SkidReduceTurnMin
    buffer->addFloat(*(static_cast<float *>(m_values[197].content))); // SkidReduceTurnMax
    buffer->addFloat(*(static_cast<float *>(m_values[198].content))); // ItemBonusBoxCap

    /* <characteristics-end ccnetworksave> */
}

void CachedCharacteristic::restoreState(BareNetworkString *buffer) {
    /* <characteristics-start ccnetworkrestore> */
    uint32_t size;
    *(static_cast<float *>(m_values[0].content)) = buffer->getFloat(); // SuspensionStiffness
    *(static_cast<float *>(m_values[1].content)) = buffer->getFloat(); // SuspensionRest
    *(static_cast<float *>(m_values[2].content)) = buffer->getFloat(); // SuspensionTravel
    *(static_cast<bool *>(m_values[3].content)) = buffer->getUInt8(); // SuspensionExpSpringResponse
    *(static_cast<float *>(m_values[4].content)) = buffer->getFloat(); // SuspensionMaxForce
    *(static_cast<float *>(m_values[5].content)) = buffer->getFloat(); // StabilityRollInfluence
    *(static_cast<float *>(m_values[6].content)) = buffer->getFloat(); // StabilityChassisLinearDamping
    *(static_cast<float *>(m_values[7].content)) = buffer->getFloat(); // StabilityChassisAngularDamping
    *(static_cast<float *>(m_values[8].content)) = buffer->getFloat(); // StabilityDownwardImpulseFactor
    *(static_cast<float *>(m_values[9].content)) = buffer->getFloat(); // StabilityTrackConnectionAccel

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[10].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[10].content))->push_back(buffer->getFloat()); // StabilityAngularFactor
    }
    *(static_cast<float *>(m_values[11].content)) = buffer->getFloat(); // StabilitySmoothFlyingImpulse
    buffer->decodeInterpolationArray((static_cast<InterpolationArray *>(m_values[12].content))); // TurnRadius
    buffer->decodeInterpolationArray((static_cast<InterpolationArray *>(m_values[13].content))); // TurnTimeFullSteer
    *(static_cast<float *>(m_values[14].content)) = buffer->getFloat(); // TurnTimeResetSteer
    *(static_cast<float *>(m_values[15].content)) = buffer->getFloat(); // TurnBrakeMultiplier
    *(static_cast<float *>(m_values[16].content)) = buffer->getFloat(); // EnginePower
    *(static_cast<float *>(m_values[17].content)) = buffer->getFloat(); // EngineMaxSpeed
    *(static_cast<float *>(m_values[18].content)) = buffer->getFloat(); // EngineGenericMaxSpeed
    *(static_cast<float *>(m_values[19].content)) = buffer->getFloat(); // EngineBrakeFactor
    *(static_cast<float *>(m_values[20].content)) = buffer->getFloat(); // EngineTimeFullBrake
    *(static_cast<float *>(m_values[21].content)) = buffer->getFloat(); // EngineMaxSpeedReverseRatio
    *(static_cast<float *>(m_values[22].content)) = buffer->getFloat(); // EngineRearForceFraction

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[23].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[23].content))->push_back(buffer->getFloat()); // GearSwitchRatio
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[24].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[24].content))->push_back(buffer->getFloat()); // GearPowerIncrease
    }
    *(static_cast<float *>(m_values[25].content)) = buffer->getFloat(); // Mass
    *(static_cast<float *>(m_values[26].content)) = buffer->getFloat(); // TrackZipperFactor
    *(static_cast<float *>(m_values[27].content)) = buffer->getFloat(); // VirtualMass
    *(static_cast<float *>(m_values[28].content)) = buffer->getFloat(); // FuelMassReal
    *(static_cast<float *>(m_values[29].content)) = buffer->getFloat(); // FuelMassVirtual
    *(static_cast<float *>(m_values[30].content)) = buffer->getFloat(); // FuelConsumption
    *(static_cast<float *>(m_values[31].content)) = buffer->getFloat(); // FuelCapacity
    *(static_cast<float *>(m_values[32].content)) = buffer->getFloat(); // FuelStopRate
    *(static_cast<float *>(m_values[33].content)) = buffer->getFloat(); // FuelMaxSpeedDecrease
    *(static_cast<float *>(m_values[34].content)) = buffer->getFloat(); // FuelTurnRadiusIncrease
    *(static_cast<float *>(m_values[35].content)) = buffer->getFloat(); // FuelLiftAndCoastFactor
    *(static_cast<float *>(m_values[36].content)) = buffer->getFloat(); // WheelsDampingRelaxation
    *(static_cast<float *>(m_values[37].content)) = buffer->getFloat(); // WheelsDampingCompression
    *(static_cast<float *>(m_values[38].content)) = buffer->getFloat(); // JumpAnimationTime
    *(static_cast<float *>(m_values[39].content)) = buffer->getFloat(); // LeanMax
    *(static_cast<float *>(m_values[40].content)) = buffer->getFloat(); // LeanSpeed
    *(static_cast<float *>(m_values[41].content)) = buffer->getFloat(); // AnvilDuration
    *(static_cast<float *>(m_values[42].content)) = buffer->getFloat(); // AnvilWeight
    *(static_cast<float *>(m_values[43].content)) = buffer->getFloat(); // AnvilSpeedFactor
    *(static_cast<float *>(m_values[44].content)) = buffer->getFloat(); // ParachuteFriction
    *(static_cast<float *>(m_values[45].content)) = buffer->getFloat(); // ParachuteDuration
    *(static_cast<float *>(m_values[46].content)) = buffer->getFloat(); // ParachuteDurationOther
    *(static_cast<float *>(m_values[47].content)) = buffer->getFloat(); // ParachuteDurationRankMult
    *(static_cast<float *>(m_values[48].content)) = buffer->getFloat(); // ParachuteDurationSpeedMult
    *(static_cast<float *>(m_values[49].content)) = buffer->getFloat(); // ParachuteLboundFraction
    *(static_cast<float *>(m_values[50].content)) = buffer->getFloat(); // ParachuteUboundFraction
    *(static_cast<float *>(m_values[51].content)) = buffer->getFloat(); // ParachuteMaxSpeed
    *(static_cast<float *>(m_values[52].content)) = buffer->getFloat(); // FrictionKartFriction
    *(static_cast<float *>(m_values[53].content)) = buffer->getFloat(); // FrictionSlowdownFactor
    *(static_cast<float *>(m_values[54].content)) = buffer->getFloat(); // FrictionDragCoefficient
    *(static_cast<float *>(m_values[55].content)) = buffer->getFloat(); // FrictionDragExponent
    *(static_cast<bool *>(m_values[56].content)) = buffer->getUInt8(); // FrictionGameEngineSpeedUncap
    *(static_cast<float *>(m_values[57].content)) = buffer->getFloat(); // FrictionWallPenaltyTime
    *(static_cast<float *>(m_values[58].content)) = buffer->getFloat(); // FrictionWallPenaltyFadeInTime
    *(static_cast<float *>(m_values[59].content)) = buffer->getFloat(); // FrictionWallPenaltySlowdownFraction
    *(static_cast<float *>(m_values[60].content)) = buffer->getFloat(); // FrictionWallPenaltyEngineMultiplier
    *(static_cast<float *>(m_values[61].content)) = buffer->getFloat(); // BubblegumDuration
    *(static_cast<float *>(m_values[62].content)) = buffer->getFloat(); // BubblegumSpeedFraction
    *(static_cast<float *>(m_values[63].content)) = buffer->getFloat(); // BubblegumTorque
    *(static_cast<float *>(m_values[64].content)) = buffer->getFloat(); // BubblegumFadeInTime
    *(static_cast<float *>(m_values[65].content)) = buffer->getFloat(); // BubblegumShieldDuration
    *(static_cast<float *>(m_values[66].content)) = buffer->getFloat(); // BubblegumMiniBoostEngineForce
    *(static_cast<float *>(m_values[67].content)) = buffer->getFloat(); // BubblegumMiniBoostAddedSpeed
    *(static_cast<float *>(m_values[68].content)) = buffer->getFloat(); // BubblegumMiniBoostMaxSpeed
    *(static_cast<float *>(m_values[69].content)) = buffer->getFloat(); // BubblegumMiniBoostDuration
    *(static_cast<float *>(m_values[70].content)) = buffer->getFloat(); // BubblegumMiniBoostFadeOutTime
    *(static_cast<float *>(m_values[71].content)) = buffer->getFloat(); // BubblegumMiniCollectionDurationMultiplier
    *(static_cast<float *>(m_values[72].content)) = buffer->getFloat(); // BubblegumBoostEngineForce
    *(static_cast<float *>(m_values[73].content)) = buffer->getFloat(); // BubblegumBoostAddedSpeed
    *(static_cast<float *>(m_values[74].content)) = buffer->getFloat(); // BubblegumBoostMaxSpeed
    *(static_cast<float *>(m_values[75].content)) = buffer->getFloat(); // BubblegumBoostDuration
    *(static_cast<float *>(m_values[76].content)) = buffer->getFloat(); // BubblegumBoostFadeOutTime
    *(static_cast<float *>(m_values[77].content)) = buffer->getFloat(); // BubblegumCollectionDurationMultiplier
    *(static_cast<float *>(m_values[78].content)) = buffer->getFloat(); // ZipperDuration
    *(static_cast<float *>(m_values[79].content)) = buffer->getFloat(); // ZipperForce
    *(static_cast<float *>(m_values[80].content)) = buffer->getFloat(); // ZipperSpeedGain
    *(static_cast<float *>(m_values[81].content)) = buffer->getFloat(); // ZipperMaxSpeedIncrease
    *(static_cast<float *>(m_values[82].content)) = buffer->getFloat(); // ZipperFadeOutTime
    *(static_cast<float *>(m_values[83].content)) = buffer->getFloat(); // SwatterDuration
    *(static_cast<float *>(m_values[84].content)) = buffer->getFloat(); // SwatterDistance
    *(static_cast<float *>(m_values[85].content)) = buffer->getFloat(); // SwatterSquashDuration
    *(static_cast<float *>(m_values[86].content)) = buffer->getFloat(); // SwatterSquashSlowdown
    *(static_cast<float *>(m_values[87].content)) = buffer->getFloat(); // PlungerBandMaxLength
    *(static_cast<float *>(m_values[88].content)) = buffer->getFloat(); // PlungerBandForce
    *(static_cast<float *>(m_values[89].content)) = buffer->getFloat(); // PlungerBandDuration
    *(static_cast<float *>(m_values[90].content)) = buffer->getFloat(); // PlungerBandSpeedIncrease
    *(static_cast<float *>(m_values[91].content)) = buffer->getFloat(); // PlungerBandFadeOutTime
    *(static_cast<float *>(m_values[92].content)) = buffer->getFloat(); // NitroHackDuration
    *(static_cast<float *>(m_values[93].content)) = buffer->getFloat(); // NitroHackFactor
    *(static_cast<float *>(m_values[94].content)) = buffer->getFloat(); // ElectroDuration
    *(static_cast<float *>(m_values[95].content)) = buffer->getFloat(); // ElectroEngineMult
    *(static_cast<float *>(m_values[96].content)) = buffer->getFloat(); // ElectroMaxSpeedIncrease
    *(static_cast<float *>(m_values[97].content)) = buffer->getFloat(); // ElectroFadeOutTime
    *(static_cast<float *>(m_values[98].content)) = buffer->getFloat(); // TyresPitSpeedFraction
    *(static_cast<float *>(m_values[99].content)) = buffer->getFloat(); // TyresPitTimeMultiplier
    buffer->decodeString32L((static_cast<std::string *>(m_values[100].content))); // TyresChangeKartMap
    buffer->decodeString32L((static_cast<std::string *>(m_values[101].content))); // TyresNamesLong
    buffer->decodeString32L((static_cast<std::string *>(m_values[102].content))); // TyresNamesShort

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[103].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[103].content))->push_back(buffer->getFloat()); // TyresMaxLifeTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[104].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[104].content))->push_back(buffer->getFloat()); // TyresMaxLifeTraction
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[105].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[105].content))->push_back(buffer->getFloat()); // TyresMinLifeTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[106].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[106].content))->push_back(buffer->getFloat()); // TyresMinLifeTraction
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[107].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[107].content))->push_back(buffer->getFloat()); // TyresMinLifeTurningGui
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[108].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[108].content))->push_back(buffer->getFloat()); // TyresMinLifeTractionGui
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[109].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[109].content))->push_back(buffer->getFloat()); // TyresRegularTransferTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[110].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[110].content))->push_back(buffer->getFloat()); // TyresRegularTransferTraction
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[111].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[111].content))->push_back(buffer->getFloat()); // TyresLimitingTransferTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[112].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[112].content))->push_back(buffer->getFloat()); // TyresLimitingTransferTraction
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[113].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[113].content))->push_back(buffer->getFloat()); // TyresInitialBonusAddTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[114].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[114].content))->push_back(buffer->getFloat()); // TyresInitialBonusMultTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[115].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[115].content))->push_back(buffer->getFloat()); // TyresInitialBonusAddTraction
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[116].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[116].content))->push_back(buffer->getFloat()); // TyresInitialBonusMultTraction
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[117].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[117].content))->push_back(buffer->getFloat()); // TyresInitialBonusAddTopspeed
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[118].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[118].content))->push_back(buffer->getFloat()); // TyresInitialBonusMultTopspeed
    }
    buffer->decodeString32L((static_cast<std::string *>(m_values[119].content))); // TyresResponseCurveTurning
    buffer->decodeString32L((static_cast<std::string *>(m_values[120].content))); // TyresResponseCurveTraction
    buffer->decodeString32L((static_cast<std::string *>(m_values[121].content))); // TyresResponseCurveTopspeed

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[122].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[122].content))->push_back(buffer->getFloat()); // TyresDoGripBasedTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[123].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[123].content))->push_back(buffer->getFloat()); // TyresDoSubstractiveTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[124].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[124].content))->push_back(buffer->getFloat()); // TyresDoSubstractiveTraction
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[125].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[125].content))->push_back(buffer->getFloat()); // TyresDoSubstractiveTopspeed
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[126].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[126].content))->push_back(buffer->getFloat()); // TyresTractionConstant
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[127].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[127].content))->push_back(buffer->getFloat()); // TyresTurningConstant
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[128].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[128].content))->push_back(buffer->getFloat()); // TyresTopspeedConstant
    }
    *(static_cast<float *>(m_values[129].content)) = buffer->getFloat(); // TyresCompoundNumber

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[130].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[130].content))->push_back(buffer->getFloat()); // TyresLowGripTopspeedMult
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[131].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[131].content))->push_back(buffer->getFloat()); // TyresLowGripEngineforceMult
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[132].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[132].content))->push_back(buffer->getFloat()); // TyresLowGripTurningMult
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[133].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[133].content))->push_back(buffer->getFloat()); // TyresOffroadFactor
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[134].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[134].content))->push_back(buffer->getFloat()); // TyresRollingResistance
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[135].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[135].content))->push_back(buffer->getFloat()); // TyresSkidFactorPartial
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[136].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[136].content))->push_back(buffer->getFloat()); // TyresSkidFactorFull
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[137].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[137].content))->push_back(buffer->getFloat()); // TyresUsageMultiplierTurning
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[138].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[138].content))->push_back(buffer->getFloat()); // TyresUsageMultiplierTraction
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[139].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[139].content))->push_back(buffer->getFloat()); // TyresReferenceSpeedMult
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[140].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[140].content))->push_back(buffer->getFloat()); // TyresBrakeThreshold
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[141].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[141].content))->push_back(buffer->getFloat()); // TyresCrashPenalty
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[142].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[142].content))->push_back(buffer->getFloat()); // TyresDefaultColor
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[143].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[143].content))->push_back(buffer->getFloat()); // StartupTime
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[144].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[144].content))->push_back(buffer->getFloat()); // StartupBoost
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[145].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[145].content))->push_back(buffer->getFloat()); // StartupEngineForce
    }
    *(static_cast<float *>(m_values[146].content)) = buffer->getFloat(); // StartupDuration
    *(static_cast<float *>(m_values[147].content)) = buffer->getFloat(); // StartupFadeOutTime
    *(static_cast<float *>(m_values[148].content)) = buffer->getFloat(); // RescueDuration
    *(static_cast<float *>(m_values[149].content)) = buffer->getFloat(); // RescueVertOffset
    *(static_cast<float *>(m_values[150].content)) = buffer->getFloat(); // RescueHeight
    *(static_cast<float *>(m_values[151].content)) = buffer->getFloat(); // ExplosionDuration
    *(static_cast<float *>(m_values[152].content)) = buffer->getFloat(); // ExplosionRadius
    *(static_cast<float *>(m_values[153].content)) = buffer->getFloat(); // ExplosionInvulnerabilityTime
    *(static_cast<float *>(m_values[154].content)) = buffer->getFloat(); // NitroDuration
    *(static_cast<float *>(m_values[155].content)) = buffer->getFloat(); // NitroEngineForce
    *(static_cast<float *>(m_values[156].content)) = buffer->getFloat(); // NitroEngineMult
    *(static_cast<float *>(m_values[157].content)) = buffer->getFloat(); // NitroConsumption
    *(static_cast<float *>(m_values[158].content)) = buffer->getFloat(); // NitroSmallContainer
    *(static_cast<float *>(m_values[159].content)) = buffer->getFloat(); // NitroBigContainer
    *(static_cast<float *>(m_values[160].content)) = buffer->getFloat(); // NitroMaxSpeedIncrease
    *(static_cast<float *>(m_values[161].content)) = buffer->getFloat(); // NitroMinBurst
    *(static_cast<float *>(m_values[162].content)) = buffer->getFloat(); // NitroFadeOutTime
    *(static_cast<float *>(m_values[163].content)) = buffer->getFloat(); // NitroMax
    *(static_cast<float *>(m_values[164].content)) = buffer->getFloat(); // SlipstreamDurationFactor
    *(static_cast<float *>(m_values[165].content)) = buffer->getFloat(); // SlipstreamBaseSpeed
    *(static_cast<float *>(m_values[166].content)) = buffer->getFloat(); // SlipstreamLength
    *(static_cast<float *>(m_values[167].content)) = buffer->getFloat(); // SlipstreamWidth
    *(static_cast<float *>(m_values[168].content)) = buffer->getFloat(); // SlipstreamInnerFactor
    *(static_cast<float *>(m_values[169].content)) = buffer->getFloat(); // SlipstreamMinCollectTime
    *(static_cast<float *>(m_values[170].content)) = buffer->getFloat(); // SlipstreamMaxCollectTime
    *(static_cast<float *>(m_values[171].content)) = buffer->getFloat(); // SlipstreamAddPower
    *(static_cast<float *>(m_values[172].content)) = buffer->getFloat(); // SlipstreamMinSpeed
    *(static_cast<float *>(m_values[173].content)) = buffer->getFloat(); // SlipstreamMaxSpeedIncrease
    *(static_cast<float *>(m_values[174].content)) = buffer->getFloat(); // SlipstreamFadeOutTime
    *(static_cast<bool *>(m_values[175].content)) = buffer->getUInt8(); // SkidEnabled
    buffer->decodeString32L((static_cast<std::string *>(m_values[176].content))); // SkidMode
    *(static_cast<float *>(m_values[177].content)) = buffer->getFloat(); // SkidIncrease
    *(static_cast<float *>(m_values[178].content)) = buffer->getFloat(); // SkidDecrease
    *(static_cast<float *>(m_values[179].content)) = buffer->getFloat(); // SkidMax
    *(static_cast<float *>(m_values[180].content)) = buffer->getFloat(); // SkidTimeTillMax
    *(static_cast<float *>(m_values[181].content)) = buffer->getFloat(); // SkidSlowdown
    *(static_cast<float *>(m_values[182].content)) = buffer->getFloat(); // SkidFadeIn
    *(static_cast<float *>(m_values[183].content)) = buffer->getFloat(); // SkidFadeOut
    *(static_cast<float *>(m_values[184].content)) = buffer->getFloat(); // SkidVisual
    *(static_cast<float *>(m_values[185].content)) = buffer->getFloat(); // SkidVisualTime
    *(static_cast<float *>(m_values[186].content)) = buffer->getFloat(); // SkidRevertVisualTime
    *(static_cast<float *>(m_values[187].content)) = buffer->getFloat(); // SkidMinSpeed

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[188].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[188].content))->push_back(buffer->getFloat()); // SkidTimeTillBonus
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[189].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[189].content))->push_back(buffer->getFloat()); // SkidBonusSpeed
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[190].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[190].content))->push_back(buffer->getFloat()); // SkidBonusTime
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[191].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[191].content))->push_back(buffer->getFloat()); // SkidFadeOutTime
    }

    size = buffer->getUInt32();
    (static_cast<std::vector<float> *>(m_values[192].content))->clear();
    for (unsigned i = 0; i < size; i++) {
        (static_cast<std::vector<float> *>(m_values[192].content))->push_back(buffer->getFloat()); // SkidBonusForce
    }
    *(static_cast<float *>(m_values[193].content)) = buffer->getFloat(); // SkidPhysicalJumpTime
    *(static_cast<float *>(m_values[194].content)) = buffer->getFloat(); // SkidGraphicalJumpTime
    *(static_cast<float *>(m_values[195].content)) = buffer->getFloat(); // SkidPostSkidRotateFactor
    *(static_cast<float *>(m_values[196].content)) = buffer->getFloat(); // SkidReduceTurnMin
    *(static_cast<float *>(m_values[197].content)) = buffer->getFloat(); // SkidReduceTurnMax
    *(static_cast<float *>(m_values[198].content)) = buffer->getFloat(); // ItemBonusBoxCap

    /* <characteristics-end ccnetworkrestore> */
}
