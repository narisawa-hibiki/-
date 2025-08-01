#include "narisawaBranch/DeviceConnectorActor.h"
#include "ASerial/ASerial_packet.h"
#include "HAL/UnrealMemory.h"

ADeviceConnectorActor::ADeviceConnectorActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

// アクターがレベルに配置され、ゲームが開始された時に一度だけ呼ばれる
void ADeviceConnectorActor::BeginPlay()
{
    Super::BeginPlay();

    // ライブラリのインスタンスを生成
    // 引数でデバイスID(20)とバージョン(1)を指定
    ASerialController = MakeUnique<ASerial_lib_Controller_Win>(0x10, 0x01);
    SerialInterface = MakeUnique<WindowsSerial>();

    // 作成したインスタンスをライブラリに設定
    ASerialController->SetInterfacePt(SerialInterface.Get());

    // デバイスとの自動接続を試みる
    const int32 ConnectedComPort = ASerialController->AutoConnectDevice();

    // 接続結果を判定する
    if (ConnectedComPort != -1)
    {
        // 接続に成功した場合
        bIsDeviceConnected = true;
        ConnectionStatus = FString::Printf(TEXT("Connection successful on COM %d"), ConnectedComPort);
        UE_LOG(LogTemp, Log, TEXT("%s"), *ConnectionStatus);
    }
    else
    {
        // 接続に失敗した場合
        bIsDeviceConnected = false;
        ConnectionStatus = TEXT("Connection failed. Could not find device.");
        UE_LOG(LogTemp, Warning, TEXT("%s"), *ConnectionStatus);
    }
}

// 毎フレーム呼び出されるメインループ処理。
void ADeviceConnectorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // デバイスに接続されている場合のみ以下の処理を実行
    if (bIsDeviceConnected && ASerialController.IsValid())
    {
        // 受信データを格納するための構造体を準備
        static ASerialDataStruct::ASerialData receivedData;

        // デバイスからのデータ受信を試みる
        // この関数が、ご提示の「毎ループごとに呼び出す」処理に相当します。
        const int32 ReadStatus = ASerialController->ReadDataProcess(&receivedData);

        // ReadDataProcessの戻り値が1なら、パケット受信が完了
        if (ReadStatus == 1)
        {
            // --- パケット受信後の処理 ---
            // コマンドチャンネル0x20で、長さが17バイト以上のデータパケットが届いたら、
            //そのパケットの先頭16バイトを回転データ、17番目のバイトをボタンの状態として扱う
            if (receivedData.command == 0x20 && receivedData.data_num >= 17)
            {
                FMemory::Memcpy(&DeviceRotation, receivedData.data, sizeof(float) * 4);
                bIsActionButtonPressed = (receivedData.data[16] == 1);
            }

            // 指示通り、パケット受信が完了したらコマンド0x20をデバイスに送信
            ASerialController->WriteData(0x20);
        }
    }
}

// アクターが破棄されるか、ゲームが終了する時に呼ばれる
void ADeviceConnectorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // アプリケーション終了時にデバイスとの接続を安全に切断。
    if (ASerialController.IsValid() && bIsDeviceConnected)
    {
        ASerialController->DisConnectDevice();
        UE_LOG(LogTemp, Log, TEXT("Device disconnected."));
    }

    Super::EndPlay(EndPlayReason);
}