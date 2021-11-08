// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include <iostream>

#define Check(b) b ? TEXT("True") : TEXT("False")

// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Body);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	WheelAxis1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WheelAxis1"));
	WheelAxis1->SetupAttachment(Root);
	WheelAxis2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WheelAxis2"));
	WheelAxis2->SetupAttachment(Root);
	WheelAxis3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WheelAxis3"));
	WheelAxis3->SetupAttachment(Root);
	WheelAxis4 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WheelAxis4"));
	WheelAxis4->SetupAttachment(Root);

	Wheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel1"));
	Wheel1->SetupAttachment(Root);
	Wheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel2"));
	Wheel2->SetupAttachment(Root);
	Wheel3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel3"));
	Wheel3->SetupAttachment(Root);
	Wheel4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel4"));
	Wheel4->SetupAttachment(Root);

	WheelSteer1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WheelSteer1"));
	WheelSteer1->SetupAttachment(Root);
	WheelSteer2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WheelSteer2"));
	WheelSteer2->SetupAttachment(Root);

	WheelSteerMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelSteerMesh1"));
	WheelSteerMesh1->SetupAttachment(Root);
	WheelSteerMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelSteerMesh2"));
	WheelSteerMesh2->SetupAttachment(Root);

	//Sets spring-arm values
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 10.f;
	SpringArm->CameraRotationLagSpeed = 10.f;

	//Set turbo values and movement values
	bIsTurbo = false;
	bIsMovingForward = false;

	//Set torques (can be changed in edit panel)
	MaxForwardTorque = 5000.f;
	MaxSteerTorque = 5000.f;
	bIsSteering = false;
	MaxWheelsSpeed = 1000.f;

}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ToggleTurbo();

}

// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACar::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ACar::LookRight);
	PlayerInputComponent->BindAction(TEXT("Turbo"),IE_Pressed, this, &ACar::TurboOn);
	PlayerInputComponent->BindAction(TEXT("Turbo"),IE_Released, this, &ACar::TurboOff);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACar::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Steer"), this, &ACar::Steer);
	PlayerInputComponent->BindAction(TEXT("Break"), IE_Pressed, this, &ACar::BreakStart);
	PlayerInputComponent->BindAction(TEXT("Break"), IE_Released, this, &ACar::BreakEnd);
	PlayerInputComponent->BindAction(TEXT("ResetPosition"), IE_Pressed, this, &ACar::ResetPosition);
	PlayerInputComponent->BindAction(TEXT("PauseMenu"), IE_Pressed, this, &ACar::PauseMenu);

}

void ACar::LookUp(float AxisValue) 
{
	
	SpringArm->AddRelativeRotation(FRotator(AxisValue, 0.f, 0.f ), false);
}

void ACar::LookRight(float AxisValue) 
{

	SpringArm->AddRelativeRotation(FRotator( 0.f, AxisValue, 0.f ), false);
}

void ACar::TurboOn() 
{
	bIsTurbo = true;

	//Turn on Tick to check and activate turbo
	SetActorTickEnabled(true);
}

void ACar::TurboOff() 
{
	bIsTurbo = false;

	//Turn off Tick to improve performance since we don't need to check turbo
	SetActorTickEnabled(false);
}

void ACar::ToggleTurbo() 
{
	if(bIsTurbo == true) 
	{
		Body->AddForce((Body->GetForwardVector()) * 2000.f, NAME_None, true);
	}
}

void ACar::MoveForward(float ForwardInput) 
{
	int Size = sizeof(this);
	UE_LOG(LogTemp, Warning, TEXT("%i"), Size);


	//Check if input is 0, if not set bIsMovingForward to true and viceversa
	ForwardInput == 0.f ? bIsMovingForward = false : bIsMovingForward = true;

	//GetTransform from the front wheels
	Wheel1Transform = Wheel1->GetComponentTransform();
	Wheel2Transform = Wheel2->GetComponentTransform();

	//Get quaternion and project up vector, which is NOT WHEELS Z AXIS because the wheels are rotated of 90°
	Wheel1Rot = Wheel1Transform.Rotator().Quaternion();
	W1RotationVector = Wheel1Rot.GetUpVector();
	Wheel2Rot = Wheel2Transform.Rotator().Quaternion();
	W2RotationVector = Wheel2Rot.GetUpVector();

	//Draw a line that represents the vector added to the torque
	DrawDebugLine(GetWorld(), Wheel1Transform.GetLocation(), ((Wheel1Transform.GetLocation())+(W1RotationVector*MaxForwardTorque)), FColor::Blue, false, 0.04f, 0, 10.f);
	DrawDebugLine(GetWorld(), Wheel2Transform.GetLocation(), ((Wheel2Transform.GetLocation())+(W2RotationVector*MaxForwardTorque)), FColor::Blue, false, 0.04f, 0, 10.f);

	//Multiply RotationVector with (MaxForwardTorque * ForwardValue) to pass to torque as direction and magnitude of torque
	W1RotationVector *= (ForwardInput * MaxForwardTorque);
	W2RotationVector *= (ForwardInput * MaxForwardTorque);
		
	//Add Torque with previous vectors
	Wheel1->AddTorqueInDegrees(W1RotationVector, NAME_None, true);
	Wheel2->AddTorqueInDegrees(W2RotationVector, NAME_None, true);

}

void ACar::Steer(float SteerInput) 
{
	bIsSteering = SteerInput != 0.0f;//Check Steer input from keyboard and set to true and false

	if(!bIsSteering)//If it's not steering, lock the limit
	{
		WheelSteer1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
		WheelSteer2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	}

	else //if it's steering set the limit to 20 than steer
	{
		WheelSteer1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 20.f);
		WheelSteer2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 20.f);

		WheelSteer1Transform = WheelSteerMesh1->GetComponentTransform();
		WheelSteer2Transform = WheelSteerMesh2->GetComponentTransform();

		WheelSteer1Rot = WheelSteer1Transform.Rotator().Quaternion();
		WheelSteer2Rot = WheelSteer2Transform.Rotator().Quaternion();

		W1SteerVector = WheelSteer1Rot.GetUpVector();
		W2SteerVector = WheelSteer2Rot.GetUpVector();

		DrawDebugLine(GetWorld(), WheelSteer1Transform.GetLocation(), ((WheelSteer1Transform.GetLocation())+(W1SteerVector*MaxForwardTorque)), FColor::Blue, false, 0.04f, 0, 10.f);
		DrawDebugLine(GetWorld(), WheelSteer2Transform.GetLocation(), ((WheelSteer2Transform.GetLocation())+(W2SteerVector*MaxForwardTorque)), FColor::Blue, false, 0.04f, 0, 10.f);

		W1SteerVector *= (SteerInput * MaxSteerTorque);
		W2SteerVector *= (SteerInput * MaxSteerTorque);

		Wheel1->AddTorqueInDegrees(W1SteerVector, NAME_None, true);
		Wheel2->AddTorqueInDegrees(W2SteerVector, NAME_None, true);

	}
}

//Break Logic, sets angular speed limit to 0 to simulate break
void ACar::BreakStart() 
{
	Wheel3->SetPhysicsMaxAngularVelocity(0.f);
	Wheel4->SetPhysicsMaxAngularVelocity(0.f);
}

//Reset angular speed limit
void ACar::BreakEnd() 
{
	Wheel3->SetPhysicsMaxAngularVelocity(MaxWheelsSpeed);
	Wheel4->SetPhysicsMaxAngularVelocity(MaxWheelsSpeed);
}

void ACar::ResetPosition() 
{
	//Get last location and default rotation
	FVector ResetLoc = Body->GetComponentLocation();
	FRotator ResetRot = GetActorRotation();
	//Set location a bit higher than before to prevent collisions
	ResetLoc.Z += 200.f;
	//Spawn new car, destory the previous one and possess the new one
	NewCar = GetWorld()->SpawnActor<ACar>(BP_Car, ResetLoc, ResetRot);
	DetachFromControllerPendingDestroy();
	Destroy();
	
	if(PC) {PC->Possess(NewCar);}

}

void ACar::PauseMenu() 
{
	//Create a PauseMenuWidget if it's not been created already
	if(!PauseMenuW && BP_PauseMenuW) 
	{
		PauseMenuW = CreateWidget(GetWorld(), BP_PauseMenuW);
	}

	PauseMenuW->AddToViewport(0);
	
	//Set input mode to be Game and UI and show mouse on display
	FInputModeGameAndUI InputMode;
	if(PC) {PC->SetInputMode(InputMode);}
	PC->bShowMouseCursor = true;
	
}

