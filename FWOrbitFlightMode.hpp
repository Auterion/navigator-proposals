#pragma once
#include "FlightMode.hpp"

class FWOrbitFlightMode : public FlightMode {
public:
	static constexpr IdType MODE_ID = FlightMode::ID("FWOrbitFlightMode");
	virtual IdType getModeId() const {
		return MODE_ID;
	}

	static bool canRun(const VehicleState& state) {
		return false;
	}


    virtual Setpoints generateSetpoints(const VehicleState &vehicle_state, float delta_time) {
        return Setpoints{};
    }

};
