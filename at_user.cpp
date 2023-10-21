#include "at_user.h"

#include "HardwareSerial.h"
#include <ESP8266WiFi.h>

#include <string.h>
using namespace std;

/*============================================================
    AT
============================================================*/

At_Err_t at_user_AT_NULL(At_Param_t param);
At_Err_t at_user_AT(At_Param_t param);
At_Err_t at_user_AT_List(At_Param_t param);
At_Err_t at_user_AT_Reboot(At_Param_t param);
At_Err_t at_user_AT_WiFi_Connect(At_Param_t param);
At_Err_t at_user_AT_WiFi_Get_IP(At_Param_t param);

struct At_State atTable[] = {
  { "AT", AT_TYPE_CMD, at_user_AT },
  { "AT+LS", AT_TYPE_CMD, at_user_AT_List },
  { "AT+REBOOT", AT_TYPE_CMD, at_user_AT_Reboot },
  { "AT+WIFI", AT_TYPE_CMD, at_user_AT_WiFi_Connect },
  { "AT+IP", AT_TYPE_CMD, at_user_AT_WiFi_Get_IP },
  { AT_LABLE_TAIL, AT_TYPE_NULL, at_user_AT_NULL },
};

At at(atTable, Serial, Serial);

At_Err_t at_user_AT_NULL(At_Param_t param) {
  return AT_EOK;
}

At_Err_t at_user_AT(At_Param_t param) {
  at.sendInfor(AT_USER_OK);
  return AT_EOK;
}

/**
 * @brief 打印命令列表，同时另外发送指定位置(argv[i])的命令
 */
At_Err_t at_user_AT_List(At_Param_t param) {
  at.printSet("AT");
  at.sendInfor(AT_USER_OK);
  return AT_EOK;
}

#include <EspSaveCrash.h>  // 重启
At_Err_t at_user_AT_Reboot(At_Param_t param) {
  at.sendInfor(AT_USER_OK);
  ESP.reset();
  return AT_EOK;
}

At_Err_t at_user_AT_WiFi_Connect(At_Param_t param) {
  String infor = "";
  if (param->argc != 2) {
    infor += "AT+WIFI\r\n";
    infor += "\tSSID PSK\r\n";
    at.sendInfor(infor);
    return AT_ERROR;
  }
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(param->argv[0], param->argv[1]);
  uint8_t err_time = 0;
  int ret = 0;
  while (WiFi.status() != WL_CONNECTED) {
    err_time++;
    if (err_time >= 100) { ret = -1; break; }
    delay(200);
    Serial.print(".");
  }
  if (ret) {
    infor += AT_USER_ERROR_FUNCTION;
    at.sendInfor(infor);
    return AT_ERROR;
  } else {
    Serial.println();
    Serial.println("[Wi-Fi STA] WiFi connected!");
    Serial.print("[Wi-Fi STA] IP: ");
    Serial.println(WiFi.localIP());
  }

  infor += AT_USER_OK;
  at.sendInfor(infor);

  return AT_EOK;
}

At_Err_t at_user_AT_WiFi_Get_IP(At_Param_t param) {
  String infor = "";
  infor = WiFi.localIP().toString();
  at.sendInfor(infor);

  return AT_EOK;
}
