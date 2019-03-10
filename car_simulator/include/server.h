#ifndef __SERVER__
#define __SERVER__
#include <iostream>
#include <string>

constexpr char STR_BAT_VOLT[] = "ATRV"; /* Get battery voltage from interface. */
constexpr char STR_PROT_NAME[] = "ATDP"; /* Get OBD protocol name from interface. */
constexpr char STR_VIN_NUM[] = "09 02"; /* Get VIN number. */
constexpr char STR_ECU_NAME[] = "09 0A"; /* Get ECU Name. */
constexpr char STR_DTC_CNT[] = "01 01"; /*Get DTC count*/
constexpr char STR_SUPP_PID_1[] = "01 00"; /* Get supported PIDs 1 - 32 for MODE 1. */
constexpr char STR_SUPP_PID_9[] = "09 00"; /* Get supported PIDs 1 - 32 for MODE 9. */
constexpr char STR_DTC[] = "03";      /* Get DTCs that are set. */
constexpr char STR_ENGINE_RPM[] = "01 0C"; /* Engine RPM */
constexpr char STR_VEHICLE_SPEED[] = "01 0D"; /* Vehicle Speed */
constexpr char STR_FUEL_PRESS[] = "01 0A"; /* Fuel Pressure */
constexpr char STR_MAP_PRESS[] = "01 0B"; /* MAP Pressure */
constexpr char STR_FUEL_FLOW_RATE[] = "01 5E"; /* Fuel Flow Rate */  
constexpr char STR_COOLANT_TEMP[]  = "01 05"; /* Coolant Temperature */
constexpr char STR_FUEL_TANK_LEV[] = "01 2F"; /* Fuel Tank Level */
constexpr char STR_INTAKE_AIR_TMP[] = "01 0F"; /* Intake Air Temperature */
constexpr char STR_OIL_TMP[] = "01 5C"; /* Oil Temperature */    
constexpr char STR_FREEZE_FRAME_DTC[] = "02 02"; /* Freeze Frame DTC. */

#endif