#pragma once

#include "FWOrbitFlightMode.hpp"
#include "MCPosControlFlightMode.hpp"
#include "MCAltControlFlightMode.hpp"
#include "Vehicle.hpp"


enum class CommanderState {
	MANUAL,
	ALTCTL,
	POSCTL,
	AUTO_MISSION,
	AUTO_LOITER,
	AUTO_RTL,
	ACRO,
	OFFBOARD,
	STAB,
	AUTO_TAKEOFF,
	AUTO_LAND,
	AUTO_FOLLOW_TARGET,
	AUTO_PRECLAND,
	ORBIT,
	MAX
};

enum class NavCommand {
	IDLE,
	WAYPOINT,
	LOITER_UNLIMITED,
	LOITER_TIME_LIMIT,
	RETURN_TO_LAUNCH,
	LAND,
	TAKEOFF,
	LOITER_TO_ALT,
	DO_FOLLOW_REPOSITION,
	VTOL_TAKEOFF,
	VTOL_LAND,
	DELAY,
	DO_JUMP,
	DO_CHANGE_SPEED,
	DO_SET_HOME,
	DO_SET_SERVO,
	DO_LAND_START,
	DO_SET_ROI_LOCATION,
	DO_SET_ROI_WPNEXT_OFFSET,
	DO_SET_ROI_NONE,
	DO_CONTROL_VIDEO,
	DO_SET_ROI,
	DO_DIGICAM_CONTROL,
	DO_MOUNT_CONFIGURE,
	DO_MOUNT_CONTROL,
	DO_SET_CAM_TRIGG_INTERVAL,
	DO_SET_CAM_TRIGG_DIST,
	OBLIQUE_SURVEY,
	SET_CAMERA_MODE,
	SET_CAMERA_ZOOM,
	SET_CAMERA_FOCUS,
	DO_GIMBAL_MANAGER_PITCHYAW,
	DO_GIMBAL_MANAGER_CONFIGURE,
	IMAGE_START_CAPTURE,
	IMAGE_STOP_CAPTURE,
	DO_TRIGGER_CONTROL,
	VIDEO_START_CAPTURE,
	VIDEO_STOP_CAPTURE,
	DO_VTOL_TRANSITION,
	FENCE_RETURN_POINT,
	FENCE_POLYGON_VERTEX_INCLUSION,
	FENCE_POLYGON_VERTEX_EXCLUSION,
	FENCE_CIRCLE_INCLUSION,
	FENCE_CIRCLE_EXCLUSION,
	CONDITION_GATE
};




union FlightModeUnion {
	FWOrbitFlightMode fw_orbit_flight_mode;
	MCPosControlFlightMode mc_pos_control_flight_mode;
	MCAltControlFlightMode mc_alt_control_flight_mode;
};


class FlightModeManager {

private:
	// State
	Vehicle& _vehicle;

	FlightModeUnion _current_flight_mode_memory;
	FlightMode* _current_flight_mode{reinterpret_cast<FlightMode*>(&_current_flight_mode_memory)};
	FlightModeSwitcher _flight_mode_switcher;

	FlightModeManager() : _flight_mode_switcher(&_current_flight_mode_memory) {};

	/**
	 * @brief Function that gets executed ~20Hz.
	 * Checks current FlightMode and assigns a new FlightMode
	 * in case of changes
	 *
	 * @param commander_state
	 */
	void _navigatorTick(const CommanderState &commander_state) {
		switch (commander_state) {
			case CommanderState::AUTO_LOITER: {
				auto result = _vehicle.modeForCommanderLoiter(*_current_flight_mode, _flight_mode_switcher);
                // depending on result, inform commander (switch to other mode), navigator(advance mission), or user (fail safes)
                break;
			}
			case CommanderState::ALTCTL: {
				auto result = _vehicle.modeForCommanderLoiter(*_current_flight_mode, _flight_mode_switcher);
                // depending on result, inform commander (switch to other mode), navigator(advance mission), or user (fail safes)				break;
			}
            //...
		}

	}
};

