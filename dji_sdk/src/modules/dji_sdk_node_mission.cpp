#include <dji_sdk/dji_sdk_mission.h>

DJI::onboardSDK::HotPointData new_hotpoint = {0};
DJI::onboardSDK::FollowData new_follow = {0};

void DJISDKMission::mission_state_callback()
{
////
/*
	cmd_mission_common_data_t mission_state_data;
	dji_sdk::MissionPushInfo mission_state;
	////DJI_Pro_Get_Mission_State_Data(&mission_state_data);
	mission_state.type = mission_state_data.type;
	mission_state.data_1 = mission_state_data.data_1;
	mission_state.data_2 = mission_state_data.data_2;
	mission_state.data_3 = mission_state_data.data_3;
	mission_state.data_4 = mission_state_data.data_4;
	mission_state.data_5 = mission_state_data.data_5;
	mission_state_publisher.publish(mission_state);
*/
////
//TODO: [__CHRIS__b2]
ROS_INFO("Unsolved state callback.");
	//--------------------------------------------------;
}


void DJISDKMission::mission_event_callback()
{
////
/*
	cmd_mission_common_data_t mission_event_data;
	dji_sdk::MissionPushInfo mission_event;
	////DJI_Pro_Get_Mission_Event_Data(&mission_event_data);
	mission_event.data_1 = mission_event_data.data_1;
	mission_event.data_2 = mission_event_data.data_2;
	mission_event.data_3 = mission_event_data.data_3;
	mission_event.data_4 = mission_event_data.data_4;
	mission_event.data_5 = mission_event_data.data_5;
	mission_event_publisher.publish(mission_event);
*/
////
//TODO: [__CHRIS__b3]
ROS_INFO("Unsolve event callback.");
	//-------------------------------------------------;

}


bool DJISDKMission::mission_start_callback(dji_sdk::MissionStart::Request& request, dji_sdk::MissionStart::Response& response)
{
	switch(current_type)
	{	
		case MissionType::WAYPOINT:
			//send start command 
			rosAdapter->waypoint->start();

			break;
		
		case MissionType::HOTPOINT:
			//upload hp task info
			new_hotpoint.latitude = hotpoint_task.latitude;
			new_hotpoint.longitude = hotpoint_task.longitude;
			new_hotpoint.height = hotpoint_task.altitude;
			new_hotpoint.radius = hotpoint_task.radius;
			new_hotpoint.palstance = hotpoint_task.angular_speed;
			new_hotpoint.clockwise = hotpoint_task.is_clockwise;
			new_hotpoint.startPoint = hotpoint_task.start_point;
			new_hotpoint.yawMode = hotpoint_task.yaw_mode;
			rosAdapter->hotpoint->setData(new_hotpoint);
			rosAdapter->hotpoint->start();

			break;

		case MissionType::FOLLOWME:
			//upload fm task info
			new_follow.mode = followme_task.mode;
			new_follow.yaw = followme_task.yaw_mode;
			new_follow.target.latitude = followme_task.initial_latitude;
			new_follow.target.longitude = followme_task.initial_longitude;
			new_follow.target.height = followme_task.initial_altitude;
			new_follow.target.angle = 0; //unused param
			new_follow.sensitivity = followme_task.sensitivity;
			rosAdapter->followme->setData(new_follow);
			rosAdapter->followme->start();

			break;

		default:
			//empty
			return false;

	}
	return true;

}


bool DJISDKMission::mission_pause_callback(dji_sdk::MissionPause::Request& request, dji_sdk::MissionPause::Response& response)
{

	switch(current_type)
	{	
		//different cmd id
		case MissionType::WAYPOINT:
			rosAdapter->waypoint->pause(request.pause == 0 ? true : false);
			break;
		
		case MissionType::HOTPOINT:
			rosAdapter->hotpoint->pause(request.pause == 0 ? true : false);
			break;

		case MissionType::FOLLOWME:
			rosAdapter->followme->pause(request.pause == 0 ? true : false);

			break;
		default:
			return false;

	}

	return true;

}


bool DJISDKMission::mission_cancel_callback(dji_sdk::MissionCancel::Request& request, dji_sdk::MissionCancel::Response& response)
{

	switch(current_type)
	{	
		//different cmd id
		case MissionType::WAYPOINT:
			rosAdapter->waypoint->stop();
			break;
		
		case MissionType::HOTPOINT:
			rosAdapter->hotpoint->stop();
			break;

		case MissionType::FOLLOWME:
			rosAdapter->followme->stop();
			break;

		default:
			return false;

	}
	return true;
}


bool DJISDKMission::mission_download_callback(dji_sdk::MissionDownload::Request& request, dji_sdk::MissionDownload::Response& response)
{

	switch(current_type)
	{	
		//different callback
		case MissionType::WAYPOINT:
			////DJI_Pro_Mission_Waypoint_Download_Task();
			//TODO download (for each waypoint in task)
//TODO: [__CHRIS__b9]
//--------------------------------------------;
ROS_INFO("Unsovled problem of downloading");
			break;
		
		case MissionType::HOTPOINT:
			////DJI_Pro_Mission_Hotpoint_Download();
//-----------------------------------------------;
ROS_INFO("Unsovled problem of downloading");
			break;

		case MissionType::FOLLOWME:
			//No download for Followme
//---------------------------------------------------;
ROS_INFO("Unsovled problem of downloading");
			break;

		default:
			return false;
	}

	return true;

}


bool DJISDKMission::mission_wp_upload_callback(dji_sdk::MissionWpUpload::Request& request, dji_sdk::MissionWpUpload::Response& response)
{

	waypoint_task = request.waypoint_task;
	DJI::onboardSDK::WayPointInitData new_task = {0};
    DJI::onboardSDK::WayPointData new_waypoint = {0};

	new_task.indexNumber = waypoint_task.mission_waypoint.size();
	new_task.maxVelocity = waypoint_task.velocity_range;
	new_task.idleVelocity = waypoint_task.idle_velocity;
	new_task.finishAction = waypoint_task.action_on_finish;
	new_task.executiveTimes = waypoint_task.mission_exec_times;
	new_task.yawMode = waypoint_task.yaw_mode;
	new_task.traceMode = waypoint_task.trace_mode;
	new_task.RCLostAction = waypoint_task.action_on_rc_lost;
	new_task.gimbalPitch = waypoint_task.gimbal_pitch_mode;

	rosAdapter->waypoint->init(&new_task);
	printf("uploaded the task with %d waypoints\n", new_task.indexNumber);

	sleep(2);

	int i = 0;
	for (auto waypoint:waypoint_task.mission_waypoint) {
		new_waypoint.latitude = waypoint.latitude*C_PI/180;
		new_waypoint.longitude = waypoint.longitude*C_PI/180;
		new_waypoint.altitude = waypoint.altitude;
		new_waypoint.damping = waypoint.damping_distance;
		new_waypoint.yaw = waypoint.target_yaw;
		new_waypoint.gimbalPitch = waypoint.target_gimbal_pitch;
		new_waypoint.turnMode = waypoint.turn_mode;
		new_waypoint.hasAction = waypoint.has_action;
		new_waypoint.actionTimeLimit = waypoint.action_time_limit;

		new_waypoint.actionNumber = 15;
		new_waypoint.actionRepeat = waypoint.waypoint_action.action_repeat;
		std::copy(waypoint.waypoint_action.command_list.begin(), waypoint.waypoint_action.command_list.end(), new_waypoint.commandList);
		std::copy(waypoint.waypoint_action.command_parameter.begin(),waypoint.waypoint_action.command_parameter.end(), new_waypoint.commandParameter);

		new_waypoint.index = i;

		rosAdapter->waypoint->uploadIndexData(&new_waypoint);
		printf("uploaded the %dth waypoint\n", new_waypoint.index);
		i+=1;
		sleep(2);
	}
	
	current_type = MissionType::WAYPOINT;
	printf("current_type -> WP\n");

	return true;

}


bool DJISDKMission::mission_wp_get_speed_callback(dji_sdk::MissionWpGetSpeed::Request& request, dji_sdk::MissionWpGetSpeed::Response& response)
{
	if (current_type != MissionType::WAYPOINT)
	{
		printf("Not in Waypoint Mode!\n");
		return false;
	}
	rosAdapter->waypoint->readIdleVelocity();
	return true;

}


bool DJISDKMission::mission_wp_set_speed_callback(dji_sdk::MissionWpSetSpeed::Request& request, dji_sdk::MissionWpSetSpeed::Response& response)
{
	if (current_type != MissionType::WAYPOINT)
	{
		printf("Not in Waypoint Mode!\n");
		return false;
	}
	rosAdapter->waypoint->updateIdleVelocity(request.speed);
	return true;

}


bool DJISDKMission::mission_hp_upload_callback(dji_sdk::MissionHpUpload::Request& request, dji_sdk::MissionHpUpload::Response& response)
{
	hotpoint_task = request.hotpoint_task;
	hotpoint_task.latitude = hotpoint_task.latitude*C_PI/180;
	hotpoint_task.longitude = hotpoint_task.longitude*C_PI/180;

	current_type = MissionType::HOTPOINT;
	printf("current_type -> HP\n");
	return true;

}


bool DJISDKMission::mission_hp_set_speed_callback(dji_sdk::MissionHpSetSpeed::Request& request, dji_sdk::MissionHpSetSpeed::Response& response)
{
	if (current_type != MissionType::HOTPOINT)
	{
		printf("Not in Hotpoint Mode!\n");
		return false;
	}
	rosAdapter->hotpoint->updatePalstance(request.speed, request.direction);

	return true;

}


bool DJISDKMission::mission_hp_set_radius_callback(dji_sdk::MissionHpSetRadius::Request& request, dji_sdk::MissionHpSetRadius::Response& response)
{
	if (current_type != MissionType::HOTPOINT)
	{
		printf("Not in Hotpoint Mode!\n");
		return false;
	}
	rosAdapter->hotpoint->updateRadius(request.radius);

	return true;

}


bool DJISDKMission::mission_hp_reset_yaw_callback(dji_sdk::MissionHpResetYaw::Request& request, dji_sdk::MissionHpResetYaw::Response& response)
{
	if (current_type != MissionType::HOTPOINT)
	{
		printf("Not in Hotpoint Mode!\n");
		return false;
	}
	rosAdapter->hotpoint->resetYaw();

	return true;

}


bool DJISDKMission::mission_fm_upload_callback(dji_sdk::MissionFmUpload::Request& request, dji_sdk::MissionFmUpload::Response& response)
{
	followme_task = request.followme_task;
	followme_task.initial_latitude = followme_task.initial_latitude*C_PI/180;
	followme_task.initial_longitude = followme_task.initial_longitude*C_PI/180;

	current_type = MissionType::FOLLOWME;
	printf("current_type -> FM\n");
	return true;

}


bool DJISDKMission::mission_fm_set_target_callback(dji_sdk::MissionFmSetTarget::Request& request, dji_sdk::MissionFmSetTarget::Response& response)
{
	if (current_type != MissionType::FOLLOWME)
	{
		printf("Not in Followme Mode!\n");
		return false;
	}
	DJI::onboardSDK::FollowTarget target_info;
	target_info.latitude = request.followme_target.latitude;
	target_info.longitude = request.followme_target.longitude;
	target_info.height = request.followme_target.altitude;
	rosAdapter->followme->setTarget(target_info);

	return true;

}


DJISDKMission::DJISDKMission(ros::NodeHandle& nh)
{
	init_missions(nh);
//[__CHRIS__]
	////DJI_Pro_Register_Mission_State_Callback(std::bind(&DJISDKMission::mission_state_callback,this));
	////DJI_Pro_Register_Mission_Event_Callback(std::bind(&DJISDKMission::mission_event_callback,this));
}

