/**
 * @file
 * Command Class Association Group Information
 * @remarks This file is auto generated
 * @copyright 2022 Silicon Laboratories Inc.
 */

#include <cc_agi_config_api.h>
#include <Assert.h>
#include <SizeOf.h>

static const char GROUP_NAME_ENDPOINT_0_GROUP_2[] = "Water Alarm Set";
static const ccc_pair_t COMMANDS_ENDPOINT_0_GROUP_2[] = {
  {
    .cmdClass = COMMAND_CLASS_BASIC,
    .cmd = BASIC_SET
  },
};
static const char GROUP_NAME_ENDPOINT_0_GROUP_3[] = "Weather Alarm Set";
static const ccc_pair_t COMMANDS_ENDPOINT_0_GROUP_3[] = {
  {
    .cmdClass = COMMAND_CLASS_BASIC,
    .cmd = BASIC_SET
  },
};
static const char GROUP_NAME_ENDPOINT_0_GROUP_4[] = "Home Monitoring Set";
static const ccc_pair_t COMMANDS_ENDPOINT_0_GROUP_4[] = {
  {
    .cmdClass = COMMAND_CLASS_BASIC,
    .cmd = BASIC_SET
  },
};

static const cc_agi_group_t ROOT_DEVICE_GROUPS[] =
{
  {
    .name = GROUP_NAME_ENDPOINT_0_GROUP_2,
    .name_length = sizeof(GROUP_NAME_ENDPOINT_0_GROUP_2),
    .profile = {
      .profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION,
      .profile_LS = NOTIFICATION_REPORT_WATER_V8
    },
    .ccc_pairs = COMMANDS_ENDPOINT_0_GROUP_2,
    .ccc_pair_count = sizeof_array(COMMANDS_ENDPOINT_0_GROUP_2),
  },
  {
    .name = GROUP_NAME_ENDPOINT_0_GROUP_3,
    .name_length = sizeof(GROUP_NAME_ENDPOINT_0_GROUP_3),
    .profile = {
      .profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION,
      .profile_LS = NOTIFICATION_REPORT_WEATHER_ALARM_V8
    },
    .ccc_pairs = COMMANDS_ENDPOINT_0_GROUP_3,
    .ccc_pair_count = sizeof_array(COMMANDS_ENDPOINT_0_GROUP_3),
  },
  {
    .name = GROUP_NAME_ENDPOINT_0_GROUP_4,
    .name_length = sizeof(GROUP_NAME_ENDPOINT_0_GROUP_4),
    .profile = {
      .profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION,
      .profile_LS = NOTIFICATION_REPORT_HOME_MONITORING_V8
    },
    .ccc_pairs = COMMANDS_ENDPOINT_0_GROUP_4,
    .ccc_pair_count = sizeof_array(COMMANDS_ENDPOINT_0_GROUP_4),
  },
};

static const cc_agi_config_t config[] = {
  {
    .groups = ROOT_DEVICE_GROUPS,
    .group_count = sizeof_array(ROOT_DEVICE_GROUPS)
  },
};

const cc_agi_config_t * cc_agi_get_config(void)
{
  return config;
}

uint8_t cc_agi_config_get_group_count_by_endpoint(uint8_t endpoint)
{
  if (endpoint > sizeof_array(config)) {
    return 0;
  }
  return config[endpoint].group_count;
}

const cc_agi_group_t * cc_agi_get_rootdevice_groups(void)
{
  return ROOT_DEVICE_GROUPS;
}
