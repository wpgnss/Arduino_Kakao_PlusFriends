# Arduino_Kakao_PlusFriends

이 어플리케이션은 SKT의 LoRa 디바이스로부터 약속된 포맷의 GPS 데이터를 수신해 KAKAOTALK의 플러스친구로 위치정보를 알려주는 어플리케이션이다.
![](https://github.com/wpgnss/Arduino_Kakao_PlusFriends/blob/master/img/01_intro.PNG)


## 서버 생성하기
아두이노 Web Server 예제를 기반으로 서버를 생성한다. 이 서버는 아래 그림과 같이 ThingPlug에서 Push 해주는 데이터 수신과 KakaoTalk에서 데이터를 요청하는 데이터 전송 기능을 담당하는 서버 역할을 한다.
이 예제는 사설 IP로 구현되었기 때문에 공유기 설정에서 Port Forwading 설정까지 해주어야 한다. 아니면 공인 IP를 사용해야 한다.

![](https://github.com/wpgnss/Arduino_Kakao_PlusFriends/blob/master/img/02_flow.PNG)

## GPS 데이터 수신하기
SKT의 LoRa + GPS 디바이스에서 주기적으로 GPS 데이터를 생성&전달 해준다.
이 데이터는 SKT의 IoT 플랫폼인 ThingPlug에 저장되며, 이 데이터를 Subscription 기능을 사용해 아두이노 서버에서 수신하게 된다.

ThingPlug Subscription에 대한 간단한 설명: http://1byte.tistory.com/25
![](https://github.com/wpgnss/Arduino_Kakao_PlusFriends/blob/master/img/03_subscription.PNG)


## 카카오톡 플러스 친구 개발하기
카카오톡에서 위치정보를 확인하기 위해서 카카오톡의 '플러스친구'를 사용했다.

카카오톡 플러스친구: https://center-pf.kakao.com
카카오톡 플러스친구 API: https://github.com/plusfriend/auto_reply

아두이노에서 플러스 친구를 사용하기 위해서는 '자동응답' 설정을 'API형 자동응답'으로 설정해야 한다.

그 후 아래 두가지에 대한 내용을 개발해야 한다.
1. keyboard
	keyboard는 사용자에게 제공할 입력 수단이다. 직접 text 입력을 받을 수도 있고, 정해진 버튼을 만들어 사용자가 선택하도록 할 수도 있다.
    ex) GET url:port/keyboard HTTP/1.1
2. message
	사용자가 keyboard를 통해 입력&선택하게 되면 아두이노 서버에 message 로 요청이 오게 된다.
    ex) POST url:port/message HTTP/1.1

위 두 기능을 구현하면 아래와 같이 카카오톡 플러스친구를 통해 아두이노 서버와 통신할 수 있다.
![](https://github.com/wpgnss/Arduino_Kakao_PlusFriends/blob/master/img/04_kakaopf_api.PNG)

## 카카오톡에서 위치 정보 확인
Location, Roadview 기능이 구현되어 있다.
* Location을 누르면, 자동응답으로 가장 최신의 GPS 데이터를 기반으로 지도 정보를 보내준다. 메세지에는 간략한 지도정보 이미지를 보여주고, 자세히 보기를 누르면 다음 지도로 연결된다.
* Roadview를 누르면, 자세히 보기 클릭시 GPS 위치의 Roadview를 보여주게 된다.
* Alert은 아직 추가되지 않았지만, Alert을 누르면 아이에게 진동이나 알림음으로 알려주는 기능을 추가할 예정이다.

![](https://github.com/wpgnss/Arduino_Kakao_PlusFriends/blob/master/img/05_on_the_phone.PNG)
