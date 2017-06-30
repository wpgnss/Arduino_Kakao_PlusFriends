#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <Ethernet.h>

#define DBG(x) Serial.print("[DBG ");Serial.print(#x); Serial.print("] "); Serial.println(x); 
#define DBG_K(x) Serial.print("[KAKAO] ");Serial.println(x);

/*
 *  ThingPlug Define 
 */
#define ONEM2M_CON_H "<con>"
#define ONEM2M_CON_T "</con>"
#define DATA_H "ff"  // data header
#define DATA_T "fe"  // data tail
#define DATA_D "bb"  // data delimiter
#define DATA_P "a"  // point

/*
 *  PLUS FRIENDS DEFINE
 */
#define NAVER_CLIENT_ID "Client Id"
#define NAVER_REDIRECT_URL "http://Server Url"
#define URL_KAKAO_PF_PURL "https://openapi.naver.com/v1/map/staticmap.bin?clientId=%s&url=%s&crs=EPSG:4326&center=%s,%s&level=10&w=320&h=240&baselayer=default&markers=%s,%s"  // 126,37
#define URL_KAKAO_PF_MURL "http://map.daum.net/link/map/우리아이,%s,%s" // 37,126
#define URL_KAKAO_PF_RURL "http://map.daum.net/link/roadview/%s,%s"


#define KEYBOARD_TXT(buf) sprintf(buf, "{\"type\": \"text\"}");
#define KEYBOARD_BTN(buf, btn_obj)      char b[btn_obj.cnt*20] = {0,};    \
                                        char t[20] = {0,};      \
                                        for(int i=0; i<btn_obj.cnt; i++){ \
                                            if(i == btn_obj.cnt-1) sprintf(t, "\"%s\"", btn_obj.buttons[i]); \
                                            else         sprintf(t, "\"%s\", ", btn_obj.buttons[i]); \
                                            strcat(b, t);                                  \
                                         }\
                                        sprintf(buf, "{\"type\":\"buttons\",\"buttons\":[%s]}", b);

#define MESSAGE_TXT(buf, msg_obj) sprintf(buf, "{\"text\":%s }", msg_obj.text);
#define MESSAGE_IMGBT(buf, msg_obj) sprintf(buf, "{\"text\":\"%s\", \"photo\":{\"url\":\"%s\", \"width\": 640, \"height\": 480},\"message_button\": {\"label\": \"%s\",\"url\": \"%s\"}}", msg_obj.text, msg_obj.photo_url, msg_obj.button.label, msg_obj.button.url);
#define MESSAGE(buf, msg_obj, btn_obj)     char tbuf1[MAX_TXBUF_LEN/2] = {0,};  \
                                           char tbuf2[MAX_TXBUF_LEN/2] = {0,};  \
                                           MESSAGE_IMGBT(tbuf1, msg_obj); \
                                           KEYBOARD_BTN(tbuf2, btn_obj);  \
                                           sprintf(buf, "{\"message\":%s, \"keyboard\":%s}", tbuf1, tbuf2);
                                           
#define BTN_MAP "Location"
#define BTN_ROAD "Roadview"
#define BTN_ALERT "Alert"
#define BTN_CONF "Config"

#define BTN_CNF_DEV_LIST "Device List"
#define BTN_BACK "Back"

#define MAX_TXBUF_LEN 1200

typedef struct {
  const char* label;
  char url[80];
} kakao_msg_button_t;

typedef struct {
  const char* text;
  char photo_url[250];
  kakao_msg_button_t button;
} kakao_message_t;

typedef struct {
  int cnt;
  const char* buttons[10];
} kakao_keyboard_t;

typedef struct {
  char lat[30];
  char lon[30];
} gps_data_t;

class kakao{
public:
  kakao();
  ~kakao();

  //PLUS FRIENDS
  int keyboard(EthernetClient* client_to_User, const String body);
  int get_message(EthernetClient* client_to_User, const String body);
  
   //thingplug subscription
  int tp_subscription(EthernetClient* client_to_User, const String body);
  int tp_dummydata(EthernetClient* client_to_User, const String body);

private:      
  int CodeParser(char* org_str,char* header, char* tail, char* code );
  int HTTPReturnOK(EthernetClient* client_to_User, char* data = NULL);
  int HTTPReturnJSON(EthernetClient* client_to_User, char* data = NULL);
  gps_data_t _curr_gps;
    
};
