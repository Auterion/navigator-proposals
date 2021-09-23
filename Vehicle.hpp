#pragma once

#include "FlightModeSwitcher.hpp"

class Vehicle {
private:
	VehicleState _state{};


protected:
	const VehicleState& getState() const {
		return _state;
	}

public:

	virtual FlightModeSelectionResult checkVehicleFailsafe(
		const FlightMode &current_flight_mode,
		FlightModeSwitcher &switcher
	) = 0;


	virtual FlightModeSelectionResult modeForCommanderLoiter(
		const FlightMode &current_flight_mode,
		FlightModeSwitcher &switcher
	) = 0;

	virtual FlightModeSelectionResult modeForCommanderAltCtl(
		const FlightMode& currentMode,
		FlightModeSwitcher &switcher
	) = 0;

	// more overwritable functions, one for each possible commander state / action + mission item
};



