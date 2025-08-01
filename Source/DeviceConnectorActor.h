#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASerial/WindowsSerial.h"
#include "ASerial/ASerial_lib_Controller_Win.h"
#include "DeviceConnectorActor.generated.h"

// 前方宣言
class ASerial_lib_Controller_Win;
class WindowsSerial;

UCLASS()
class SWEETPROJECT_API ADeviceConnectorActor : public AActor
{
    GENERATED_BODY()

public:
    ADeviceConnectorActor();

protected:
    // ゲーム開始時に呼ばれる関数
    virtual void BeginPlay() override;

    // ゲーム終了時に呼ばれる関数
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    // 毎フレーム呼ばれる関数（メインループ）
    virtual void Tick(float DeltaTime) override;

private:
    // ASerialライブラリのインスタンスを保持するポインタ
    TUniquePtr<ASerial_lib_Controller_Win> ASerialController;

    // Windowsシリアル通信機能のインスタンスを保持するポインタ
    TUniquePtr<WindowsSerial> SerialInterface;

    // デバイスとの接続状態を保持するフラグ
    bool bIsDeviceConnected = false;

public:
    // ブループリントから接続状態を確認するための変数
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASerial|Status")
    FString ConnectionStatus;

    // ブループリントに公開するデータ用の変数
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASerial|Data")
    FQuat DeviceRotation;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASerial|Data")
    FVector DeviceAcceleration;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASerial|Data")
    bool bIsActionButtonPressed;
};