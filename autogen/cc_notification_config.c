/**
 * @file
 * Command Class Notification Configuration
 * @remarks This file is auto generated
 * @copyright 2022 Silicon Laboratories Inc.
 */

#include <cc_notification_config_api.h>
#include <CC_Notification.h>
#include "cc_notification_events.h"
#include <SizeOf.h>
#include <Assert.h>

static const notification_event_state STATES_EVENTS_EP0_NOTIFICATION_TYPE_WATER_ALARM[] = {
  NOTIFICATION_EVENT_WATER_ALARM_SENSOR_STATUS_LEAK_DETECTED,
  NOTIFICATION_EVENT_WATER_ALARM_WATER_FLOW_ALARM_STATUS,
};
static const notification_event_state STATES_EVENTS_EP0_NOTIFICATION_TYPE_WEATHER_ALARM[] = {
  NOTIFICATION_EVENT_WEATHER_ALARM_FREEZE_ALARM,
};
static const notification_event_state STATES_EVENTS_EP0_NOTIFICATION_TYPE_HOME_MONITORING[] = {
  NOTIFICATION_EVENT_HOME_MONITORING_HOME_OCCUPIED,
};

static cc_notification_t notifications[] = {
  {
    .type = NOTIFICATION_TYPE_WATER_ALARM,
    .endpoint = 0,
    .agi_profile.profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL,
    .agi_profile.profile_LS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE,
    .events = STATES_EVENTS_EP0_NOTIFICATION_TYPE_WATER_ALARM,
    .event_count = sizeof_array(STATES_EVENTS_EP0_NOTIFICATION_TYPE_WATER_ALARM),
    .current_event = NOTIFICATION_EVENT_NO_EVENT,
    .status        = NOTIFICATION_STATUS_UNSOLICIT_ACTIVATED,
  },
  {
    .type = NOTIFICATION_TYPE_WEATHER_ALARM,
    .endpoint = 0,
    .agi_profile.profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL,
    .agi_profile.profile_LS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE,
    .events = STATES_EVENTS_EP0_NOTIFICATION_TYPE_WEATHER_ALARM,
    .event_count = sizeof_array(STATES_EVENTS_EP0_NOTIFICATION_TYPE_WEATHER_ALARM),
    .current_event = NOTIFICATION_EVENT_NO_EVENT,
    .status        = NOTIFICATION_STATUS_UNSOLICIT_ACTIVATED,
  },
  {
    .type = NOTIFICATION_TYPE_HOME_MONITORING,
    .endpoint = 0,
    .agi_profile.profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL,
    .agi_profile.profile_LS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE,
    .events = STATES_EVENTS_EP0_NOTIFICATION_TYPE_HOME_MONITORING,
    .event_count = sizeof_array(STATES_EVENTS_EP0_NOTIFICATION_TYPE_HOME_MONITORING),
    .current_event = NOTIFICATION_EVENT_NO_EVENT,
    .status        = NOTIFICATION_STATUS_UNSOLICIT_ACTIVATED,
  },
};

STATIC_ASSERT((sizeof_array(notifications) > 0), STATIC_ASSERT_FAILED_size_mismatch);

cc_notification_t * cc_notification_get_config(void) {
  return notifications;
}

uint8_t cc_notification_get_config_length(void) {
  return sizeof_array(notifications);
}

cc_notification_t* cc_notification_get(uint8_t index) {
  if(index >= sizeof_array(notifications)) {
    return NULL;
  }
  return &notifications[index];
}

int8_t cc_notification_get_index_by_type_and_endpoint(notification_type_t type, uint8_t endpoint) {
  for (uint8_t i = 0; i < sizeof_array(notifications); i++) {
    if (type == notifications[i].type && endpoint == notifications[i].endpoint) {
      return (int8_t)i;
    }
  }
  return -1;
}

notification_type_t cc_notification_get_type(uint8_t index) {
  return notifications[index].type;
}

notification_event_state cc_notification_get_current_event(uint8_t index) {
  return notifications[index].current_event;
}

agi_profile_t cc_notification_get_agi_profile(uint8_t index) {
  return notifications[index].agi_profile;
}

uint8_t cc_notification_get_endpoint(uint8_t index) {
  return notifications[index].endpoint;
}

NOTIFICATION_STATUS cc_notification_get_status(uint8_t index) {
  return notifications[index].status;
}

bool cc_notification_is_type_supported(notification_type_t type) {
  for (uint8_t i = 0; i < sizeof_array(notifications); i++) {
    if (type == notifications[i].type) {
      return true;
    }
  }
  return false;
}

int8_t cc_notification_find_event(uint8_t event, uint8_t index)
{
  for (uint8_t i = 0; i < notifications[index].event_count; i++) {
    if (event == notifications[index].events[i]) {
      return (int8_t)i;
    }
  }
  return -1;
}

uint8_t cc_notification_config_get_number_of_events(uint8_t index)
{
  ASSERT(index < sizeof_array(notifications));
  return notifications[index].event_count;
}

ZAF_CC_REGISTER_CONFIG(COMMAND_CLASS_NOTIFICATION_V8, &notifications[0], 0);
ZAF_CC_REGISTER_CONFIG(COMMAND_CLASS_NOTIFICATION_V8, &notifications[1], 1);
ZAF_CC_REGISTER_CONFIG(COMMAND_CLASS_NOTIFICATION_V8, &notifications[2], 2);
