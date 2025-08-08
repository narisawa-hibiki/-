#include <stdio.h>

//自動接続テスト

#include "ASerial_lib_Controller_Win.h"
#include "WindowsSerial/WindowsSerial.h"

ASerial_lib_Controller_Win test(0x03, 0x01);
WindowsSerial Serial;


int main(void) {
    /*int st = Serial.OpenPort(7);

    printf("%d\n", st);

    Serial.write("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    Sleep(1000);
    Serial.write("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    Serial.ClosePort();*/

    test.SetInterfacePt(&Serial);

    int st = test.AutoConnectDevice();

    if (st == -1) {
        printf("No Connection COM...");
    }
    else {
        printf("Connection COM %d", st);

        Sleep(100);
        test.DisConnectDevice();
    }

    return 0;
}