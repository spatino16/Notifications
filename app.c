/**
 * Z-Wave Certified Application Switch On/Off
 *
 * @copyright 2018 Silicon Laboratories Inc.
 */

#include <stdbool.h>
#include <stdint.h>
#include "Assert.h"
#include "MfgTokens.h"
#include "DebugPrintConfig.h"
#define DEBUGPRINT
#include "DebugPrint.h"
#include "ZW_system_startup_api.h"
#include "zaf_config_security.h"
#include "ZAF_Common_helper.h"
#include "ZAF_Common_interface.h"
#include "ZAF_network_learn.h"
#include "ZAF_network_management.h"
#include "events.h"
#include "zpal_watchdog.h"
#include "app_hw.h"
#include "board_indicator.h"
#include "ZAF_ApplicationEvents.h"
#include "CC_BinarySwitch.h"
#include "zaf_event_distributor_soc.h"
#include "zpal_misc.h"
#include "zw_build_no.h"
#include "zaf_protocol_config.h"
#include "CC_Notification.h"
#include "cc_notification_types.h"
#include "cc_notification_events.h"


#ifdef DEBUGPRINT
#include "ZAF_PrintAppInfo.h"
#endif

#ifdef DEBUGPRINT
static uint8_t m_aDebugPrintBuffer[96];
#endif

void ApplicationTask(SApplicationHandles* pAppHandles);

static uint8_t supportedEventsFreeze = NOTIFICATION_EVENT_WEATHER_ALARM_FREEZE_ALARM;
static uint8_t supportedEventsWaterLeak = NOTIFICATION_EVENT_WATER_ALARM_SENSOR_STATUS_LEAK_DETECTED;
static uint8_t supportedEventsWaterFlow = NOTIFICATION_EVENT_WATER_ALARM_WATER_FLOW_ALARM_STATUS;
static uint8_t supportedEventsHomeOccupied = NOTIFICATION_EVENT_HOME_MONITORING_HOME_OCCUPIED;


/**
 * @brief See description for function prototype in ZW_basis_api.h.
 */
ZW_APPLICATION_STATUS
ApplicationInit(__attribute__((unused)) EResetReason_t eResetReason)
{
  SRadioConfig_t* RadioConfig;

  DPRINT("Enabling watchdog\n");
  zpal_enable_watchdog(true);

#ifdef DEBUGPRINT
  zpal_debug_init();
  DebugPrintConfig(m_aDebugPrintBuffer, sizeof(m_aDebugPrintBuffer), zpal_debug_output);
  DebugPrintf("ApplicationInit eResetReason = %d\n", eResetReason);
#endif

  RadioConfig = zaf_get_radio_config();

  // Read Rf region from MFG_ZWAVE_COUNTRY_FREQ
  zpal_radio_region_t regionMfg;
  ZW_GetMfgTokenDataCountryFreq((void*) &regionMfg);
  if (isRfRegionValid(regionMfg)) {
    RadioConfig->eRegion = regionMfg;
  } else {
    ZW_SetMfgTokenDataCountryRegion((void*) &RadioConfig->eRegion);
  }
  DPRINTF("Rf region: %d\n", RadioConfig->eRegion);

  /*************************************************************************************
   * CREATE USER TASKS  -  ZW_ApplicationRegisterTask() and ZW_UserTask_CreateTask()
   *************************************************************************************
   * Register the main APP task function.
   *
   * ATTENTION: This function is the only task that can call ZAF API functions!!!
   * Failure to follow guidelines will result in undefined behavior.
   *
   * Furthermore, this function is the only way to register Event Notification
   * Bit Numbers for associating to given event handlers.
   *
   * ZW_UserTask_CreateTask() can be used to create additional tasks.
   * @see zwave_soc_sensor_pir example for more info.
   *************************************************************************************/
  bool bWasTaskCreated = ZW_ApplicationRegisterTask(
                                                    ApplicationTask,
                                                    EAPPLICATIONEVENT_ZWRX,
                                                    EAPPLICATIONEVENT_ZWCOMMANDSTATUS,
                                                    zaf_get_protocol_config()
                                                    );
  ASSERT(bWasTaskCreated);

  return(APPLICATION_RUNNING);
}


/**
 * A pointer to this function is passed to ZW_ApplicationRegisterTask() making it the FreeRTOS
 * application task.
 */
void
ApplicationTask(SApplicationHandles* pAppHandles)
{
  ZAF_Init(xTaskGetCurrentTaskHandle(), pAppHandles);

#ifdef DEBUGPRINT
  ZAF_PrintAppInfo();
#endif

  app_hw_init();

  /* Enter SmartStart*/
  /* Protocol will commence SmartStart only if the node is NOT already included in the network */
  ZAF_setNetworkLearnMode(E_NETWORK_LEARN_MODE_INCLUSION_SMARTSTART);

  // Wait for and process events
  DPRINT("SwitchOnOff Event processor Started\r\n");
  for (;;)
  {
    if (!zaf_event_distributor_distribute())
    {
      return;
    }
  }
}

/**
 * @brief The core state machine of this sample application.
 * @param event The event that triggered the call of zaf_event_distributor_app_event_manager.
 */
// #include "cc_notification_config_api.h"


void zaf_event_distributor_app_event_manager(const uint8_t event)
{
    DPRINTF("zaf_event_distributor_app_event_manager Ev: 0x%02x\r\n", event);

    int notification_index;

    switch (event)
    {
        case EVENT_APP_FREEZE_DATA:
            DPRINT("\r\n");
            DPRINT("\r\n      *!*!**!*!**!*!**!*!**!*!**!*!**!*!**!*!*");
            DPRINT("\r\n      *!*!*       FREEZE DATA ALARM      *!*!*");
            DPRINT("\r\n      *!*!**!*!**!*!**!*!**!*!**!*!**!*!**!*!*");
            DPRINT("\r\n");

            notification_index = cc_notification_get_index_by_type_and_endpoint(NOTIFICATION_TYPE_WEATHER_ALARM, 0);
            if (notification_index >= 0)
            {
                (void) CC_Notification_TriggerAndTransmit(notification_index,
                                                          supportedEventsFreeze,
                                                          NULL,
                                                          0,
                                                          NULL,
                                                          false);
            }
            else
            {
                DPRINT("Failed to find notification index for weather alarm.\n");
            }
            break;

        case EVENT_APP_WATER_LEAK_ALARM:
            DPRINT("\r\n");
            DPRINT("\r\n      *!*!**!*!**!*!**!*!**!*!**!*!**!*!**!*!*");
            DPRINT("\r\n      *!*!*      WATER LEAK ALARM       *!*!*");
            DPRINT("\r\n      *!*!**!*!**!*!**!*!**!*!**!*!**!*!**!*!*");
            DPRINT("\r\n");

            notification_index = cc_notification_get_index_by_type_and_endpoint(NOTIFICATION_TYPE_WATER_ALARM, 0);
            if (notification_index >= 0)
            {
                (void) CC_Notification_TriggerAndTransmit(notification_index,
                                                          supportedEventsWaterLeak,
                                                          NULL,
                                                          0,
                                                          NULL,
                                                          false);
            }
            else
            {
                DPRINT("Failed to find notification index for water alarm.\n");
            }
            break;

        case EVENT_APP_WATER_FLOW_ALARM:
            DPRINT("\r\n");
            DPRINT("\r\n      *!*!**!*!**!*!**!*!**!*!**!*!**!*!**!*!*");
            DPRINT("\r\n      *!*!*     WATER FLOW ALARM        *!*!*");
            DPRINT("\r\n      *!*!**!*!**!*!**!*!**!*!**!*!**!*!**!*!*");
            DPRINT("\r\n");

            notification_index = cc_notification_get_index_by_type_and_endpoint(NOTIFICATION_TYPE_WATER_ALARM, 0);
            if (notification_index >= 0)
            {
                (void) CC_Notification_TriggerAndTransmit(notification_index,
                                                          supportedEventsWaterFlow,
                                                          NULL,
                                                          0,
                                                          NULL,
                                                          false);
            }
            else
            {
                DPRINT("Failed to find notification index for water alarm.\n");
            }
            break;

        case EVENT_APP_HOME_OCCUPIED:
            DPRINT("\r\n");
            DPRINT("\r\n      *!*!**!*!**!*!**!*!**!*!**!*!**!*!**!*!*");
            DPRINT("\r\n      *!*!*     HOME OCCUPIED ALARM     *!*!*");
            DPRINT("\r\n      *!*!**!*!**!*!**!*!**!*!**!*!**!*!**!*!*");
            DPRINT("\r\n");

            notification_index = cc_notification_get_index_by_type_and_endpoint(NOTIFICATION_TYPE_HOME_MONITORING, 0);
            if (notification_index >= 0)
            {
                (void) CC_Notification_TriggerAndTransmit(notification_index,
                                                          supportedEventsHomeOccupied,
                                                          NULL,
                                                          0,
                                                          NULL,
                                                          false);
            }
            else
            {
                DPRINT("Failed to find notification index for home monitoring.\n");
            }
            break;

        default:
            DPRINT("Unknown event - no action taken.\n");
            break;
    }
}
