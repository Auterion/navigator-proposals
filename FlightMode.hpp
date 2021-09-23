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
class Setpoints {};


class FlightMode {
public:
	using IdType = uint32_t;
	template<size_t N>
	static constexpr uint32_t ID(const char (&name)[N])
	{
		return (0xffffff & util::hash_32_fnv1a_const(name))
	}


	virtual IdType getModeId() const = 0;

    virtual Setpoints generateSetpoints(const VehicleState &vehicle_state, float delta_time) = 0; 

};


