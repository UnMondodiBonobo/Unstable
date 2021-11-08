// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Car.generated.h"


class UPhysicsConstraintComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UUserWidget;

UCLASS()
class MACHINEPROJECT_API ACar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Copmponents", meta = (AllowPrivateAccess = true));
	USceneComponent* Root;

	//Car Body

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UStaticMeshComponent* Body;

	//Springarm and camera

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UCameraComponent* Camera;



	//Suspensions

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UPhysicsConstraintComponent* WheelAxis1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UPhysicsConstraintComponent* WheelAxis2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UPhysicsConstraintComponent* WheelAxis3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UPhysicsConstraintComponent* WheelAxis4;

	//Car axis

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UPhysicsConstraintComponent* WheelSteer1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UPhysicsConstraintComponent* WheelSteer2;

	//Wheels Meshes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UStaticMeshComponent* Wheel1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UStaticMeshComponent* Wheel2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UStaticMeshComponent* Wheel3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UStaticMeshComponent* Wheel4;

	//Axis Meshes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UStaticMeshComponent* WheelSteerMesh1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true));
	UStaticMeshComponent* WheelSteerMesh2;

	//Max Forward Torque
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TORQUE")
	float MaxForwardTorque;

	//Max Steer Torque
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TORQUE")
	float MaxSteerTorque;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPEED")
	float MaxWheelsSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "TurboParameters")
	bool bIsTurbo;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bIsMovingForward;

	UPROPERTY()
	bool bIsSteering; 

	UFUNCTION()
	void LookUp(float AxisValue);

	UFUNCTION()
	void LookRight(float AxisValue);

	UFUNCTION()
	void TurboOn();

	UFUNCTION()
	void TurboOff();

	UFUNCTION()
	void ToggleTurbo();

	UFUNCTION()
	void MoveForward(float ForwardInput);

	UFUNCTION()
	void Steer(float SteerInput);

	UFUNCTION()
	void BreakStart();

	UFUNCTION()
	void BreakEnd();

	UFUNCTION()
	void ResetPosition();

	UFUNCTION()
	void PauseMenu();

	//New Instances needed to respawn on reset
	UPROPERTY(EditDefaultsOnly)
	ACar* NewCar;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACar> BP_Car;

	//Pause menu widgets
	UPROPERTY(EditDefaultsOnly)
	UUserWidget* PauseMenuW;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BP_PauseMenuW;

	//PlayerController
	UPROPERTY()
	APlayerController* PC;

	//Movement Variables
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FTransform Wheel1Transform;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FTransform Wheel2Transform;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FQuat Wheel1Rot;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector W1RotationVector;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FQuat Wheel2Rot;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector W2RotationVector;

	//Steer Variables
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FTransform WheelSteer1Transform;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FTransform WheelSteer2Transform;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FQuat WheelSteer1Rot;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector W1SteerVector;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FQuat WheelSteer2Rot;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector W2SteerVector;

};
