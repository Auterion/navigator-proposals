#pragma once
#include <cstdint>
#include <unistd.h>
#include <VehicleState.hpp>

namespace util {
	// source: https://gist.github.com/ruby0x1/81308642d0325fd386237cfa3b44785c
	constexpr uint32_t val_32_const = 0x811c9dc5;
	constexpr uint32_t prime_32_const = 0x1000193;
	constexpr uint64_t val_64_const = 0xcbf29ce484222325;
	constexpr uint64_t prime_64_const = 0x100000001b3;
	inline constexpr uint32_t hash_32_fnv1a_const(const char *const str, const uint32_t value = val_32_const) noexcept
	{
		return (str[0] == '\0') ? value : hash_32_fnv1a_const(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
	}
};



// forward defined non-existent setpoint class
class Setpoints {
    /*
     This class should contain
     position, velocity and acceleration setpoints.
     These setpoints then get sent to the controllers
     that execute on those
     */

};

/**
 * A flight mode is a vehicle specific (multicopter, fixed-wing, or other) specific
 * class that implements a certain vehicle behaviour. This could be something like
 * Taking off, or flying an orbit. FlightModes get instantiated by the vehicle class
 * as a reaction to commander state / user actions or as part of a mission item.
 */
class FlightMode {
public:
	using IdType = uint32_t;

    /**
     * Each FlightMode is supposed to have its own specific ID. 
     * This function is provided such that flight modes can generate
     * a compile-time integer ID from their name
     */
	template<size_t N>
	static constexpr uint32_t ID(const char (&name)[N])
	{
		return (0xffffff & util::hash_32_fnv1a_const(name))
	}

    /**
     * Each FlightMode should have a MODE_ID static public field. This function
     * should get overridden to report back this field. 
     * @return IdType 
     */
	virtual IdType getModeId() const = 0;

    /**
     * Function that gets called during execution of the flight mode
     * to generate the set points. It received the current vehicle state,
     * as ell as the time that has elapsed since the last time this function
     * was executed. The vehicle state also contains stick positions, so the flight mode
     * can also react to stick movements. The function is supposed to use some sort
     * of trajectory planning and to return set points for the controllers. 
     * @param vehicle_state 
     * @param delta_time 
     * @return Setpoints 
     */
    virtual Setpoints generateSetpoints(const VehicleState &vehicle_state, float delta_time) = 0; 

};


