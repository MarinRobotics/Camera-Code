#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

//vision sensor stuff
pros::Vision front_vision_sensor(6);
pros::Vision back_vision_sensor(10);

pros::vision_signature_s_t ring_signature =
front_vision_sensor.signature_from_utility(1, -205, 863, 329, 7521, 8655, 8088, 3.000, 0);
pros::vision_signature_s_t red_mogus_signature =
front_vision_sensor.signature_from_utility(2, 5105, 6761, 5933, 699, 1629, 1164, 3.000, 0);
pros::vision_signature_s_t blue_mogus_signature =
front_vision_sensor.signature_from_utility(3, -3049, -2407, -2728, 9121, 10519, 9820, 3.000, 0);
pros::vision_signature_s_t nuetral_mogus_signature =
front_vision_sensor.signature_from_utility(1, 2427, 3407, 2917, -3847, -3463, -3655, 3.000, 0);

//Pnuematic
 pros::ADIDigitalOut claw (1, 0);



//define motors
pros::Motor front_left (20, MOTOR_GEARSET_18);
pros::Motor front_right (18, MOTOR_GEARSET_18, true);

pros::Motor back_left (9, MOTOR_GEARSET_18);
pros::Motor back_right (1, MOTOR_GEARSET_18, true);

pros::Motor lift_motor (13, MOTOR_GEARSET_36);

pros::Motor forebar_right (7, MOTOR_GEARSET_18);
pros::Motor forebar_left (10, MOTOR_GEARSET_18,true);





void moveRight(int speed) {
	front_right = speed;
	back_right = speed;
}

void moveLeft (int speed) {
	front_left = speed;
	back_right = speed;
}

// TODO: fix all this weird auton code CALVIN!!!!!!
// TODO: Use PID controller for autonomous

//define motor speed variables
int right_front_output;
int left_front_output;
int right_back_output;
int left_back_output;

//define vision sensor variables
pros::vision_object_s_t red_mobile_goals[2];
pros::vision_object_s_t blue_mobile_goals[2];
pros::vision_object_s_t nutral_mogii[3];

bool complado = false;
int goal_area = 1000;
int speed;

//go full speed on all motors
void full (bool reversed = false) {
	front_right = 127;
	front_left = 127;
	back_right = 127;
	back_left = 127;

	if (reversed) {
		front_right = -127;
		front_left = -127;
		back_right = -127;
		back_left = -127;
	}
}

//rotate on a point
void rotateOnPoint (bool right = true) {
	if (right) {
		front_right = -127;
		back_right = -127;
		front_left = 127;
		back_left = 127;
	} else {
		front_right = 127;
		back_right = 127;
		front_left = -127;
		back_left = -127;
	}
}

//stop all motors
void stop () {
	front_right = 0;
	back_right = 0;
	front_left = 0;
	back_left = 0;
}

// TODO: Remove this if never used
void RedLeft() {
	int target_area = 1000;
	pros::lcd::initialize();
	front_vision_sensor.set_exposure(10);

	while(true) {
		front_vision_sensor.read_by_sig(0, nuetral_mogus_signature.id, 2, nutral_mogii);
		int n_mogus_area1 = nutral_mogii[0].height * nutral_mogii[0].width;

		pros::lcd::print(0, "object count: %d", front_vision_sensor.get_object_count());
		pros::lcd::print(1, "mogus objct 0: (%u, %u, %d)", nutral_mogii[0].x_middle_coord, nutral_mogii[0].y_middle_coord);
		pros::lcd::print(2, "mogus object 1: (%u, %u)", nutral_mogii[1].x_middle_coord, nutral_mogii[1].y_middle_coord);
		pros::lcd::print(3, "Area: %u", n_mogus_area1);

		while ((n_mogus_area1 < target_area) && (n_mogus_area1 > 250)) {
			front_vision_sensor.read_by_sig(0, nuetral_mogus_signature.id, 2, nutral_mogii);
			int n_mogus_area1 = nutral_mogii[0].height * nutral_mogii[0].width;

			pros::lcd::print(0, "object count: %d", front_vision_sensor.get_object_count());
			pros::lcd::print(1, "mogus objct 0: (%u, %u, %d)", nutral_mogii[0].x_middle_coord, nutral_mogii[0].y_middle_coord);
			pros::lcd::print(2, "mogus object 1: (%u, %u)", nutral_mogii[1].x_middle_coord, nutral_mogii[1].y_middle_coord);
			pros::lcd::print(3, "Area: %u", n_mogus_area1);

			if (nutral_mogii[0].x_middle_coord < 300) {
				moveRight(-50);
				moveLeft(-50+(50/300)*nutral_mogii[0].x_middle_coord);
			} else if ((nutral_mogii[0].x_middle_coord > 340)) {
				moveRight(-50+(50/300)*nutral_mogii[0].x_middle_coord);
				moveLeft(-50);
			} else {
				moveRight(50);
				moveLeft(50);
			}
			pros::delay(20);
		}
		stop();
		pros::delay(20);

}

}
 void vision_test () {

	pros::lcd::initialize();
	while (true) {
	pros::lcd::clear();
	front_vision_sensor.read_by_sig(0, nuetral_mogus_signature.id, 2, nutral_mogii);
	pros::screen::print(TEXT_MEDIUM, 3, "mogus objct 0: (%u, %u, %d)", nutral_mogii[0].x_middle_coord, nutral_mogii[0].y_middle_coord);
	pros::screen::print(TEXT_MEDIUM, 4, "mogus object 1: (%u, %u)", nutral_mogii[1].x_middle_coord, nutral_mogii[1].y_middle_coord);
	pros::screen::print(TEXT_MEDIUM, 5, "object count: %d", front_vision_sensor.get_object_count());

	pros::screen::set_pen(COLOR_YELLOW);
	int x_0 = round((nutral_mogii[0].x_middle_coord - (nutral_mogii[0].width*0.5)) * (480/640));
	int y_0 = round(200 - (nutral_mogii[0].top_coord * 0.5));
	pros::screen::print(TEXT_MEDIUM, 6, "top right coord: (%d, %d)", x_0,y_0);


	int x_1 = round((nutral_mogii[0].x_middle_coord - (nutral_mogii[0].width*0.5)) + nutral_mogii[0].width) * (240/640);
	int y_1 = round(200 - ((nutral_mogii[0].top_coord + nutral_mogii[0].height) * 0.5));
	pros::screen::print(TEXT_MEDIUM, 7, "bottom left coord: (%d, %d)", x_1, y_1);

	pros::screen::print(TEXT_MEDIUM, 7,"x middle coord %d", nutral_mogii[0].x_middle_coord);


	pros::screen::fill_rect(1, y_0, 100, y_1);
	pros::delay(20);
	}

 }

// TODO: Remove if never used
void newLeft () {

	int step_1_lift_down = 8000;
	int step_1_foward = 500;

	int step_2_lift = 4000;
	int step_2_back = 1000;
	int step_2_rotate = 2000;

	int step_3_Foward = 0;
	int step_3_backward = 0;




	/* 1st step */
	lift_motor = 127;
	full();
	pros::delay(500);
	lift_motor = 0;
	pros::delay(step_1_foward);
	stop();

	/* Old second step */
	pros::delay(500);
	lift_motor = -127;
	full(true);
	pros::delay(1000);
	stop();
	pros::delay(4000);
	lift_motor = 0;

}

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	/* pros::lcd::set_text(1, "Hello PROS User!"); */

	pros::lcd::register_btn1_cb(on_center_button);

//vision sensor stuff
	front_vision_sensor.set_signature(ring_signature.id, &ring_signature);
	front_vision_sensor.set_signature(red_mogus_signature.id, &red_mogus_signature);
	front_vision_sensor.set_signature(blue_mogus_signature.id, &blue_mogus_signature);
	front_vision_sensor.set_signature(nuetral_mogus_signature.id, &nuetral_mogus_signature);

	back_vision_sensor.set_signature(ring_signature.id, &ring_signature);
	back_vision_sensor.set_signature(red_mogus_signature.id, &red_mogus_signature);
	back_vision_sensor.set_signature(blue_mogus_signature.id, &blue_mogus_signature);
	back_vision_sensor.set_signature(nuetral_mogus_signature.id, &nuetral_mogus_signature);

	lift_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	forebar_left.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	forebar_right.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);


}
void disabled() {}
void competition_initialize() {}
void autonomous() {
	vision_test();
	/*RedLeft(); */

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */


void opcontrol() {
	int left_y_out = 0;
	int right_y_out = 0;

	int left_x_out = 0;
	int right_x_out = 0;

	int differnce = 10;
while (true) {
int left_y = master.get_analog(ANALOG_LEFT_Y);
int right_y = master.get_analog(ANALOG_RIGHT_Y);

int left_x = master.get_analog(ANALOG_LEFT_X);
int right_x = master.get_analog(ANALOG_RIGHT_X);



if (left_y > left_y_out) {
	left_y_out += differnce;
}

if (left_y < left_y_out) {
	left_y_out -= differnce;
}

if (left_x > left_x_out) {
	left_x_out += differnce;
}

if (left_x < left_x_out) {
	left_x_out -= differnce;
}



int right_front_output = 0;
int left_front_output = 0;

int right_back_output = 0;
int left_back_output = 0;

bool right_front_bumper = master.get_digital(DIGITAL_R1);
bool right_back_bumper = master.get_digital(DIGITAL_R2);

bool left_front_bumper = master.get_digital(DIGITAL_L1);
bool left_back_bumper = master.get_digital(DIGITAL_L2);

bool a_button = master.get_digital(DIGITAL_A);
bool b_button = master.get_digital(DIGITAL_B);



/* Drive Creation */
right_front_output = left_y_out + left_x_out;
left_front_output = left_y_out - left_x_out;

right_back_output = left_y_out - left_x_out;
left_back_output = left_y_out + left_x_out;

/*hey calvin! you could condense this into an if then else staement*/
/*and achieve the exact same result!*/

if (abs(right_x) > 5) {
	right_front_output -= right_x;
	right_back_output -= right_x;

	left_front_output += right_x;
	left_back_output += right_x;
}

/*
if (right_front_output > 5) {
	right_front_output -= 5;
}

if (right_front_output < -5) {
	right_front_output += 5;
}


if (left_front_output > 5) {
	left_front_output -= 5;
}

if (left_front_output < -5) {
	left_front_output += 5;
}

*/

if (master.get_digital(DIGITAL_RIGHT)) {
	right_front_output = 75;
	left_front_output = -75;

	right_back_output = -75;
	left_back_output = 75;
}

if (master.get_digital(DIGITAL_LEFT)) {

	right_front_output = -75;
	left_front_output = 75;

	right_back_output = 75;
	left_back_output = -75;
}


int motor_position = lift_motor.get_position();


pros::lcd::print(1, "Left Y: %d  Left X: %d", left_y, left_x);
pros::lcd::print(2, "Right Y: %d  Right X: %d", right_y, right_x);

pros::lcd::print(4, "%d    %d", left_front_output, right_front_output);
pros::lcd::print(5, "%d    %d", left_back_output, right_back_output);

pros::lcd::print(6, "%d", motor_position);


	if (right_front_bumper) {
		lift_motor = 127;
	} else if (right_back_bumper) {
		lift_motor = -127;
	} else {
		lift_motor = 0;
		}

	if (left_front_bumper) {
		forebar_left = 127;
		forebar_right = 127;
	} else if (left_back_bumper) {
		forebar_left = -127;
		forebar_right = -127;
	} else {
		forebar_left = 0;
		forebar_right = 0;
	}

	if (a_button) {
		claw.set_value(1);
	}

	if (b_button) {
		claw.set_value(0);
	}


//assign motor velocity
front_left.move_velocity(left_front_output*2);
front_right.move_velocity(right_front_output*2);
back_left.move_velocity(left_back_output*2);
back_right.move_velocity(right_back_output*2);




pros::delay(20);

}
}
