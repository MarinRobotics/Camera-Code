#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Vision front_vision_sensor(16);
pros::Vision back_vision_sensor(15);


pros::vision_signature_s_t ring_signature =
front_vision_sensor.signature_from_utility(1, -205, 863, 329, 7521, 8655, 8088, 3.000, 0);
pros::vision_signature_s_t red_mogus_signature =
front_vision_sensor.signature_from_utility(2, 5105, 6761, 5933, 699, 1629, 1164, 3.000, 0);
pros::vision_signature_s_t blue_mogus_signature =
front_vision_sensor.signature_from_utility(3, -3049, -2407, -2728, 9121, 10519, 9820, 3.000, 0);
pros::vision_signature_s_t nuetral_mogus_signature =
front_vision_sensor.signature_from_utility(1, 1455, 1783, 1619, -3579, -3329, -3454, 3.000, 0);



pros::Motor front_left (8, MOTOR_GEARSET_18);
pros::Motor front_right (9, MOTOR_GEARSET_18, true);

pros::Motor back_left (18, MOTOR_GEARSET_18);
pros::Motor back_right (20, MOTOR_GEARSET_18, true);

pros::Motor lift_motor (19, MOTOR_GEARSET_36);

pros::Motor varuns_foot (10, MOTOR_GEARSET_36);

pros::Motor ring_bow (15, MOTOR_GEARSET_36);


/*pros::ADIAnalogIn limit_switch1 (6);*/

int right_front_output;
int left_front_output;
int right_back_output;
int left_back_output;

pros::vision_object_s_t red_mobile_goals[2];
pros::vision_object_s_t blue_mobile_goals[2];
pros::vision_object_s_t nutral_mogii[3];

bool complado = false;

int goal_area = 1000;

int speed;
/*
front_vision_sensor.read_by_sig(0, nuetral_mogus_signature.id, 2, nutral_mogii);
front_vision_sensor.read_by_sig(0, red_mogus_signature.id, 2, red_mobile_goals);
*/

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

void stop () {
	front_right = 0;
	back_right = 0;
	front_left = 0;
	back_left = 0;
}


void RedLeft() {
	while (true) {
		front_vision_sensor.read_by_sig(0, nuetral_mogus_signature.id, 2, nutral_mogii);
		int n_mogus_area1 = nutral_mogii[0].height * nutral_mogii[0].width;

		pros::lcd::print(0, "objet count: %d", front_vision_sensor.get_object_count());
		pros::lcd::print(1, "mogus objct 0: (%u, %u, %d)", nutral_mogii[0].x_middle_coord, nutral_mogii[0].y_middle_coord);
		pros::lcd::print(2, "mogus object 1: (%u, %u)", nutral_mogii[1].x_middle_coord, nutral_mogii[1].y_middle_coord);
		pros::lcd::print(3, "Area: %u", n_mogus_area1);

		if (n_mogus_area1 > 100 || n_mogus_area1 < 1000 ) {
			if (190 < nutral_mogii[0].x_middle_coord && nutral_mogii[0].x_middle_coord < 150) {
				right_front_output = 127;
				left_front_output = 127;
				right_back_output = 127;
				left_back_output = 127;
				pros::lcd::print(6, "foward");
			} else if (nutral_mogii[0].x_middle_coord > 140) {
				right_front_output = 70;
				left_front_output = 127;
				right_back_output = 70;
				left_back_output = 127;
				pros::lcd::print(6, "left");
			} else {
				right_front_output = 127;
				left_front_output = 70;
				right_back_output = 127;
				left_back_output = 70;
				pros::lcd::print(6, "right");
			}

		} else if (n_mogus_area1 <= 100) {
			right_front_output = 25;
			left_front_output = 25;
			right_back_output = 25;
			left_back_output = 25;
			pros::lcd::print(6, "searching");
		} else {
			right_front_output = 0;
			left_front_output = 0;
			right_back_output = 0;
			left_back_output = 0;
			pros::lcd::print(6, "stop");
			pros::delay(1000);
			break;

		}

		if (front_right.get_position() > 300) {
			right_front_output = 0;
			left_front_output = 0;
			right_back_output = 0;
			left_back_output = 0;
			break;
		}



		front_right = right_front_output;
		front_left = left_front_output;

		back_right = right_back_output;
		back_left = left_back_output;

		pros::delay(20);
		}
	for (int i=0;i<=1;i++) {
		right_front_output = 127;
		left_front_output = 127;
		right_back_output = 127;
		left_back_output = 127;
		pros::lcd::print(6, "grabbing goal");
		if (front_right.get_position() > 400) {
			right_front_output = 0;
			left_front_output = 0;
			right_back_output = 0;
			left_back_output = 0;
			break;
		}
		pros::delay(20);
		}
	for (int i=0;i<=3;i++) {
		right_front_output = -127;
		left_front_output = -127;
		right_back_output = -127;
		left_back_output = -127;
		pros::delay(20);
		}

/*
	while (limit_switch1.get_value() >= 15) {
		if (speed <=127) {
			speed += 50;
		}
		lift_motor = speed;
		pros::delay(20);
		}
		*/
		right_front_output = 0;
		left_front_output = 0;
		right_back_output = 0;
		left_back_output = 0;

	}


void RedRightStart() {
	int Position_lift = 0;


	front_right = 75;
	front_left = 75;
	back_right = 75;
	back_left = 75;

	lift_motor = 127;
	pros::delay(2500);

	front_right = 0;
	front_left = 0;
	back_right = 0;
	back_left = 0;

	pros::delay(2500);

	lift_motor = 0;

	front_right = -100;
	front_left = -100;
	back_right = -100;
	back_left = -100;

	pros::delay(3000);

	front_right = 0;
	front_left = 0;
	back_right = 0;
	back_left = 0;

}

void newLeft () {

	int step_1_lift_down = 8000;
	int step_1_foward = 500;

	int step_2_lift = 4000;
	int step_2_back = 1000;
	int step_2_rotate = 2000;

	int step_3_Foward = 0;
	int step_3_backward = 0;


	varuns_foot = 127;
	pros::delay(300);
	varuns_foot = 0;


	/*
	lift_motor = 127;
	pros::delay(5000);
	lift_motor = 0; */


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



	/* 2nd step
	pros::delay(500);
	lift_motor = -127;
	full(true);
	pros::delay(step_2_back);
	stop();
	rotateOnPoint();
	pros::delay(step_2_rotate);
	stop();
	pros::delay((step_2_lift - step_2_back) - step_2_rotate);
	lift_motor = 0; */



	/* New 3rd step
	front_right = 100;
	front_left = 100;
	back_right = 100;
	back_left = 100;
	pros::delay(step_3_Foward);
	front_right = -50;
	front_left = -50;
	back_right = -50;
	back_left = -50;
	*/



	/* 3rd step *old*
	lift_motor = 127;
	pros::delay(step_3_lift_down - step_3_back);
	full();
	pros::delay(step_3_back);
	stop();
	lift_motor = 0; */


	/* 4th step *old*
	full(true);
	pros::delay(step_4_foward);
	stop();

	rotateOnPoint();
	pros::delay(step_4_rotate);
	stop(); */


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

/*vision sensor stuff */
	front_vision_sensor.set_signature(ring_signature.id, &ring_signature);
	front_vision_sensor.set_signature(red_mogus_signature.id, &red_mogus_signature);
	front_vision_sensor.set_signature(blue_mogus_signature.id, &blue_mogus_signature);
	front_vision_sensor.set_signature(nuetral_mogus_signature.id, &nuetral_mogus_signature);

	back_vision_sensor.set_signature(ring_signature.id, &ring_signature);
	back_vision_sensor.set_signature(red_mogus_signature.id, &red_mogus_signature);
	back_vision_sensor.set_signature(blue_mogus_signature.id, &blue_mogus_signature);
	back_vision_sensor.set_signature(nuetral_mogus_signature.id, &nuetral_mogus_signature);

	varuns_foot.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	lift_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}
void disabled() {}
void competition_initialize() {}
void autonomous() {
	pros::lcd::initialize();
	newLeft();
}

/**
 * Runs the operato control code. This function will be started in its own task
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
while (true) {
int left_y = master.get_analog(ANALOG_LEFT_Y);
int right_y = master.get_analog(ANALOG_RIGHT_Y);

int left_x = master.get_analog(ANALOG_LEFT_X);
int right_x = master.get_analog(ANALOG_RIGHT_X);

int right_front_output = 0;
int left_front_output = 0;

int right_back_output = 0;
int left_back_output = 0;

bool right_front_bumper = master.get_digital(DIGITAL_R1);
bool right_back_bumper = master.get_digital(DIGITAL_R2);

bool left_front_bumper = master.get_digital(DIGITAL_L1);
bool left_back_bumper = master.get_digital(DIGITAL_L2);



/* Drive Creation */
right_front_output = left_y + left_x;
left_front_output = left_y - left_x;

right_back_output = left_y - left_x;
left_back_output = left_y + left_x;

/*hey calvin! you could condense this into an if then else staement*/
/*and achieve the exact same result!*/

if (abs(right_x) > 5) {
	right_front_output -= right_x;
	right_back_output -= right_x;

	left_front_output += right_x;
	left_back_output += right_x;
}

if (master.get_digital(DIGITAL_RIGHT)) {
	right_front_output = -75;
	left_front_output = 75;

	right_back_output = 75;
	left_back_output = -75;
}

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


pros::lcd::print(1, "Left Y: %d  Let X: %d", left_y, left_x);
pros::lcd::print(2, "Right Y: %d  Right X: %d", right_y, right_x);

pros::lcd::print(4, "%d    %d", left_front_output, right_front_output);
pros::lcd::print(5, "%d    %d", left_back_output, right_back_output);

pros::lcd::print(6, "%d", motor_position);
/* pros::lcd::print(6, "Limit swith value: %d", limit_switch1.get_value()); */

/*
	if (limit_switch1.get_value() >= 15) {
		if (right_front_bumper) {
			lift_motor = 127;
		} else if (right_back_bumper) {
			lift_motor = -127;
		} else {
			lift_motor = 0;
			}
	} else {
		if (right_front_bumper) {
			lift_motor = 127;
		} else {
			lift_motor = 0;
			}
	}

*/
	if (right_front_bumper) {
		lift_motor = 127;
	} else if (right_back_bumper) {
		lift_motor = -127;
	} else {
		lift_motor = 0;
		}

	if (abs(right_y) <= 5) {
		varuns_foot.move_velocity(0);
	} else {
		varuns_foot = -right_y;
	}




pros::delay(20);

}
}
