#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "interface.h"


extern mavlink_set_position_target_local_ned_t initial_position;
extern mavlink_set_position_target_local_ned_t ip;

extern Mavlink_Messages current_messages;

extern char arm_status;
extern char control_status;

void operation (float timer);
void operation_extended (float timer);
void square_operation (float timer);
void circle_operation (float timer);
void automatic_takeoff(float timer, time_t *begin);
void flight_control_sequence(float timer);

// Flight Functions
void arm_sequence (void);
void offboard_control_sequence(void);
void disable_offboard_control_sequence(void);
void disarm_sequence (void);
void program_counter_sequence(float timer, time_t* begin);

// Function helpers
void autopilot_write_helper(void);

#endif /* COMMANDS_H_ */