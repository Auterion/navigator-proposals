#pragma once

#include "FlightModeSwitcher.hpp"

class Vehicle {
private:
	VehicleState _state{};


protected:

    /**
     * returns the vehicles current state. This object should include
     * all information to decide whether a FlightMode can execute or not, 
     * and to actually generate setpoints for executing it. 
     */
	const VehicleState& getState() const {
		return _state;
	}

public:
    /*
     * For each commander state / action as well as for each item in a mission, there
     * is an overwritable function in the vehicle. The function takes the current state
     * of the vehicle, the intention from user or mission, and selects an appropriate 
     * flight mode to execute that. A flight mode should only ever be chosen if it can
     * run. It is also expected that these functions / flight modes check geofence, 
     * object avoidance themselves as part of their canRun functions.
     */


	virtual FlightModeSelectionResult modeForCommanderLoiter(
		const FlightMode &current_flight_mode,
		FlightModeSwitcher &switcher
	) = 0;

	virtual FlightModeSelectionResult modeForCommanderAltCtl(
		const FlightMode& currentMode,
		FlightModeSwitcher &switcher
	) = 0;


    /*
     * After selection of a flight mode using the above functions,
     * This function is executed to check if the vehicle itself
     * has any fail safe states, such as battery emergencies. 
     * If this is the case, this function shall assign a flight
     * mode that reacts to that. 
     */
 	virtual FlightModeSelectionResult checkVehicleFailsafe(
		const FlightMode &current_flight_mode,
		FlightModeSwitcher &switcher
	) = 0;

    /*
     * Update the vehicle state by reading from the uORB messages 
     */
    void updateState() {
        _state.update();
    }

	// more overwritable functions, one for each possible commander state / action + mission item
};



