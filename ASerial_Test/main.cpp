// main.cpp - ASerialライブラリ 最小構成テストコード

#include <iostream>
#include <Windows.h> // Sleep関数を使用するために必要
#include <conio.h>   // キーボード入力を検知するために必要 (_kbhit)
#include <cstdint>   // ★★★ int32_t を使用するためにこのヘッダを追加 ★★★

// ライブラリのヘッダファイルをインクルード
#include "ASerial_lib_Controller_Win.h"
#include "WindowsSerial/WindowsSerial.h"
#include "ASerialCore/ASerial_packet.h"

// グローバルスコープでインスタンスを生成
// 【設定箇所】接続したいデバイスのIDとバージョンをここに記述
ASerial_lib_Controller_Win ASerialController(0x10, 0x01); // ID: 0x10 (16), Ver: 0x01
WindowsSerial SerialInterface;

int main()
{
    // 1. ライブラリに通信インターフェースを設定
    ASerialController.SetInterfacePt(&SerialInterface);

    // 2. デバイスとの自動接続を試みる
    std::cout << "Searching for device (ID: 0x10)..." << std::endl;
    // ★★★ int32 を int32_t に修正 ★★★
    const int32_t ConnectedComPort = ASerialController.AutoConnectDevice();

    // 3. 接続結果を判定
    if (ConnectedComPort == -1)
    {
        // 接続失敗
        std::cout << "Connection failed. Could not find device." << std::endl;
        std::cout << "Please check the device ID and physical connection." << std::endl;
        system("pause"); // 何かキーを押すまでウィンドウを閉じない
        return -1;
    }

    // 接続成功
    std::cout << "Connection successful on COM " << ConnectedComPort << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Press 's' to send command 0x20 to the device." << std::endl;
    std::cout << "Press 'q' to quit." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // 4. メインループ ( 'q' が押されるまで無限に繰り返す)
    while (true)
    {
        // --- データ受信処理 ---
        static ASerialDataStruct::ASerialData receivedData;
        // ★★★ int32 を int32_t に修正 ★★★
        const int32_t ReadStatus = ASerialController.ReadDataProcess(&receivedData);

        // パケット受信が完了した場合
        if (ReadStatus == 1)
        {
            std::cout << "Packet Received! Command: 0x" << std::hex << (int)receivedData.command
                << ", Data count: " << std::dec << (int)receivedData.data_num << std::endl;
        }

        // --- データ送信処理 ---
        if (_kbhit())
        {
            char key = _getch(); // 押されたキーを取得

            if (key == 's')
            {
                ASerialController.WriteData(0x20);
                std::cout << "--> Sent command 0x20 to device." << std::endl;
            }
            else if (key == 'q')
            {
                break;
            }
        }

        Sleep(10);
    }

    // 5. アプリケーション終了時にデバイスとの接続を切断
    std::cout << "Disconnecting device..." << std::endl;
    ASerialController.DisConnectDevice();

    return 0;
}