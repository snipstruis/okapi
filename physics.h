#pragma once

#include "transform.h"

uint16_t jet_rotate_left  = GLFW_KEY_Q,
		 jet_rotate_right = GLFW_KEY_E,
		 jet_rotate_up 	  = GLFW_KEY_S,
		 jet_rotate_down  = GLFW_KEY_W,
		 jet_rotate_cw 	  = GLFW_KEY_D,
		 jet_rotate_ccw   = GLFW_KEY_A,
		 jet_accelerate	  = GLFW_KEY_R,
		 jet_decelerate	  = GLFW_KEY_F;

struct jet_physics{
	quat  velocity_direction;
	float speed;
	float const 
		jerkback  = 0.1, // ammount the orientation vector is changed by velocity vector
		stability = 0.1, // ammount the velocity vector is changed by orientation vector
		pitch_up_speed   = 0.05f,
		pitch_down_speed = 0.02f,
		yaw_speed        = 0.01f,
		roll_speed       = 0.05f;
};

void jet_input(GLFWwindow* window, transform &trans, jet_physics &phy){
	// adjust orientation according to keyboard input
	if(glfwGetKey(window,jet_rotate_down)) 	
		trans.orientation = glm::rotate(trans.orientation, phy.pitch_down_speed,vec3(1,0,0));
	if(glfwGetKey(window,jet_rotate_up))	
		trans.orientation = glm::rotate(trans.orientation,-phy.pitch_up_speed,vec3(1,0,0));
	if(glfwGetKey(window,jet_rotate_right))
		trans.orientation = glm::rotate(trans.orientation, phy.yaw_speed,vec3(0,1,0));
	if(glfwGetKey(window,jet_rotate_left)) 	
		trans.orientation = glm::rotate(trans.orientation,-phy.yaw_speed,vec3(0,1,0));
	if(glfwGetKey(window,jet_rotate_cw)) 
		trans.orientation = glm::rotate(trans.orientation, phy.roll_speed,vec3(0,0,1));
	if(glfwGetKey(window,jet_rotate_ccw))
		trans.orientation = glm::rotate(trans.orientation,-phy.roll_speed,vec3(0,0,1));

	// average out velocity vector and orientation
	//phy.velocity_direction = glm::lerp(phy.velocity_direction, trans.orientation, phy.stability);
	//trans.orientation = glm::lerp(trans.orientation, phy.velocity_direction, phy.jerkback);

	// move along velocity vector
	trans.position = trans.position + (trans.orientation * vec3(0,0,(1.f/60.f)*phy.speed));
}

