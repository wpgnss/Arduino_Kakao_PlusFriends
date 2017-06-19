#include "kakao.h"

kakao::kakao(){
//KAKAO PLUS FRIEND DEFAULT GPS
    strcpy(_curr_gps.lat, "37.3786173");
    strcpy(_curr_gps.lon, "127.1108427");

}

kakao::~kakao(){
  Serial.println("destroy kakao");
}

int kakao::HTTPReturnOK(EthernetClient* client_to_User, char* data){
  
    client_to_User->println("HTTP/1.1 200 OK");
    
    if(data != NULL){
        client_to_User->println(data);
    }
    client_to_User->println();
    return 1;
}

int kakao::HTTPReturnJSON(EthernetClient* client_to_User, char* data){
  
    client_to_User->println("HTTP/1.1 200 OK");
    
    if(data != NULL){
        client_to_User->println("Content-Type: application/json; charset=utf-8\r\n");
        client_to_User->println(data);
    }
    client_to_User->println();
    return 1;
}


int kakao::CodeParser(char* org_str,char* header, char* tail, char* code){
  
    char* ptr_header;
    char* ptr_header_end;
    char* ptr_end;

    if(header != NULL)  { ptr_header = strstr(org_str, header); ptr_header_end = ptr_header + strlen(header); }
    else                { ptr_header = org_str; ptr_header_end = org_str; }

    if(tail != NULL)    ptr_end = strstr(ptr_header_end, tail);
    else                ptr_end = org_str + strlen(org_str);

    if( ptr_header == NULL || ptr_end == NULL )
      return 0;

    strncpy(code, ptr_header_end, ptr_end - ptr_header_end);
    return strlen(code);
}

int kakao::keyboard(EthernetClient* client_to_User, const String body){
    char txbuf[300] = {0, };
        
    kakao_keyboard_t keyboard;
    keyboard.cnt = 4;
    keyboard.buttons[0] = BTN_MAP;
    keyboard.buttons[1] = BTN_ROAD;
    keyboard.buttons[2] = BTN_ALERT;
    keyboard.buttons[3] = BTN_CONF;

    //KEYBOARD_TXT(txbuf);
    KEYBOARD_BTN(txbuf, keyboard);
    //Serial.println(txbuf);
    HTTPReturnJSON(client_to_User, txbuf);
}


int kakao::get_message(EthernetClient* client_to_User, const String body){
    char txbuf[MAX_TXBUF_LEN] = {0, };
    int length_userkey = 0;
    int length_content = 0;
    char userkey[15] = {0,};
    char content[100] = {0,};
    
    length_userkey = CodeParser(body.c_str(),"\"user_key\":\"","\"", userkey);
    length_content = CodeParser(body.c_str(),"\"content\":\"","\"", content);
    //Serial.println(_curr_gps.lat);
    //Serial.println(_curr_gps.lon);
    
    if( strcmp(content, BTN_MAP) == 0 ){
          Serial.println("MAP");
          kakao_message_t message;
          message.text = "아이의 현재 위치입니다.";
          sprintf(message.photo_url, URL_KAKAO_PF_PURL, NAVER_CLIENT_ID, NAVER_REDIRECT_URL, _curr_gps.lon, _curr_gps.lat, _curr_gps.lon, _curr_gps.lat);
          message.button.label = "자세히 보기";
          sprintf(message.button.url, URL_KAKAO_PF_MURL, _curr_gps.lat, _curr_gps.lon);

          kakao_keyboard_t keyboard;
          keyboard.cnt = 4;
          keyboard.buttons[0] = BTN_MAP;
          keyboard.buttons[1] = BTN_ROAD;
          keyboard.buttons[2] = BTN_ALERT;
          keyboard.buttons[3] = BTN_CONF;
       
          MESSAGE(txbuf, message, keyboard);
    }
    else if( strcmp(content, BTN_ROAD) == 0 ){
          Serial.println("ROAD");
          kakao_message_t message;
          message.text = "아이의 주변 모습입니다.";
          sprintf(message.photo_url, URL_KAKAO_PF_PURL, NAVER_CLIENT_ID, NAVER_REDIRECT_URL, _curr_gps.lon, _curr_gps.lat, _curr_gps.lon, _curr_gps.lat);
          message.button.label = "자세히 보기";
          sprintf(message.button.url, URL_KAKAO_PF_RURL, _curr_gps.lat, _curr_gps.lon);

          kakao_keyboard_t keyboard;
          keyboard.cnt = 4;
          keyboard.buttons[0] = BTN_MAP;
          keyboard.buttons[1] = BTN_ROAD;
          keyboard.buttons[2] = BTN_ALERT;
          keyboard.buttons[3] = BTN_CONF;
       
          MESSAGE(txbuf, message, keyboard);
    }
    
    else if( strcmp(content, BTN_ALERT) == 0 ){
          Serial.println("ALERT");
          kakao_message_t message;
          message.text = "아이에게 알림을 보냈습니다.";
          sprintf(message.photo_url, "https://cdn.pixabay.com/photo/2012/04/24/13/19/exclamation-40026_960_720.png");
          message.button.label = "자세히 보기";
          sprintf(message.button.url, URL_KAKAO_PF_MURL, _curr_gps.lat, _curr_gps.lon);
    
          kakao_keyboard_t keyboard;
          keyboard.cnt = 4;
          keyboard.buttons[0] = BTN_MAP;
          keyboard.buttons[1] = BTN_ROAD;
          keyboard.buttons[2] = BTN_ALERT;
          keyboard.buttons[3] = BTN_CONF;
        
          MESSAGE(txbuf, message, keyboard);
    }
    else if( strcmp(content, BTN_CONF) == 0 ){
      Serial.println("CONFIG");
      
    }

    HTTPReturnJSON(client_to_User, txbuf);
}

int kakao::tp_subscription(EthernetClient* client_to_User, const String body){
  
    int length_tp_rxdata = 0;
    char tp_rxdata[70] = {0,};
    char tBuf[35] = {0,};
    char intBuf[5] = {0,};
    char decBuf[10] = {0,};
    int length_tBuf = 0;
   
    HTTPReturnOK(client_to_User);
    
    length_tp_rxdata = CodeParser(body.c_str(),"<con>","</con>", tp_rxdata);
    //Serial.println(tp_rxdata); //FF123AA45BB678AA90FE
    
    if( length_tp_rxdata > 0 ) {
      gps_data_t gps;
      CodeParser(tp_rxdata,DATA_H, DATA_D, tBuf); //123AA45
      CodeParser(tBuf, NULL, DATA_P, intBuf);
      CodeParser(tBuf, DATA_P, NULL, decBuf);
      sprintf(gps.lat, "%s.%s", intBuf, decBuf); //123.45
      
      memset(tBuf, 0, sizeof(tBuf));
      memset(intBuf, 0, sizeof(intBuf));
      memset(decBuf, 0, sizeof(decBuf));
      
      CodeParser(tp_rxdata,DATA_D, DATA_T, tBuf); //678AA90
      CodeParser(tBuf, NULL, DATA_P, intBuf);
      CodeParser(tBuf, DATA_P, NULL, decBuf);
      sprintf(gps.lon, "%s.%s", intBuf, decBuf); //678.90

      _curr_gps = gps;
    }
}


int kakao::tp_dummydata(EthernetClient* client_to_User, const String body){
    int length_tp_rxdata = 0;
    char tp_rxdata[70] = {0,};
    char tBuf[35] = {0,};
    char intBuf[5] = {0,};
    char decBuf[10] = {0,};
    int length_tBuf = 0;

    //HTTPReturnOK(client_to_User);
    
    length_tp_rxdata = CodeParser(body.c_str(),"?", " ", tp_rxdata);
//    Serial.println(tp_rxdata); //FF123AA45BB678AA90FE
    
    if( length_tp_rxdata > 0 ) {
      gps_data_t gps;
      CodeParser(tp_rxdata,DATA_H, DATA_D, tBuf); //123AA45
      CodeParser(tBuf, NULL, DATA_P, intBuf);
      CodeParser(tBuf, DATA_P, NULL, decBuf);
      sprintf(gps.lat, "%s.%s", intBuf, decBuf);
      
      memset(tBuf, 0, sizeof(tBuf));
      memset(intBuf, 0, sizeof(intBuf));
      memset(decBuf, 0, sizeof(decBuf));
      
      CodeParser(tp_rxdata,DATA_D, DATA_T, tBuf); //678AA90
      CodeParser(tBuf, NULL, DATA_P, intBuf);
      CodeParser(tBuf, DATA_P, NULL, decBuf);
      sprintf(gps.lon, "%s.%s", intBuf, decBuf);

      _curr_gps = gps;

       memset(tp_rxdata, 0, sizeof(tp_rxdata));
       sprintf(tp_rxdata, "{\"result\": \"ok\", \"gps_data\": {\"longitude\": %s, \"latitude\": %s }}", _curr_gps.lon, _curr_gps.lat);
       HTTPReturnJSON(client_to_User, tp_rxdata);
              
       return 1;
    }
}
