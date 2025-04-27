#include "Characters/Player/SBPlayer.h"
#include "BuildSystem/Building.h"
#include "BuildSystem/BuildCameraPawn.h"
#include "BuildSystem/BuildingCreater.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/InteractionComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Items/Weapon.h"
#include "SB/DebugMacro.h"
#include "PlayerController/SBPlayerController.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "HUDs/SBHUD.h"
#include "HUDs/SBPlayerOverlayWidget.h"

ASBPlayer::ASBPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ�ѷ��� ȸ���� ĳ���Ͱ� ȸ������ �ʵ��� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	ZoomState = ECharacterZoomState::ECZS_NoZoom;
	MovementSpeedScale = JogScale;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);	// ȸ�� �ӵ�
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->MaxAcceleration = 1200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.5f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;
	

	// ������ �� ������Ʈ ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, ZoomOutCameraLoaction.Y, ZoomOutCameraLoaction.Z));
	CameraBoom->TargetArmLength = ZoomOutCameraLoaction.X;
	CameraBoom->bUsePawnControlRotation = true; // ī�޶� ��Ʈ�ѷ��� ���� ȸ���ǵ���	����

	// ī�޶� ������Ʈ ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// ī�޶� ������ �Ͽ� �����ǵ��� ����. USpringArmComponent::SocketName�� "SpringEndPoint"�� ������ �� ���κп� �����ǵ��� ����
	FollowCamera->bUsePawnControlRotation = false;	// ī�޶� ��Ʈ�ѷ��� ���� ȸ������ �ʵ��� ����

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	// �迭 �ʱ�ȭ
	WeaponQuickslot.SetNum(3);
}

void ASBPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetBuildingCreaterLocation();

	if (ControllMode != ECharacterControllMode::ECCM_None)
	{
		TraceInteractionActors();
		SelectInteractionActor();
	}
	else
	{
		OverlayWidget->HideInteractionList();
		FocusedInteractable = nullptr;
		FocusedInteractionComponent = nullptr;
		FocusedInteractionOptionIndex = 0;
	}
}

void ASBPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::Look);
		EnhancedInputComponent->BindAction(MouseLInpuatAction, ETriggerEvent::Started, this, &ASBPlayer::MouseLStarted);
		EnhancedInputComponent->BindAction(MouseLInpuatAction, ETriggerEvent::Ongoing, this, &ASBPlayer::MouseLOngoing);
		EnhancedInputComponent->BindAction(MouseLInpuatAction, ETriggerEvent::Completed, this, &ASBPlayer::MouseLCompleted);
		EnhancedInputComponent->BindAction(MouseRInputAction, ETriggerEvent::Started, this, &ASBPlayer::MouseRStarted);
		EnhancedInputComponent->BindAction(MouseWheelInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::MouseWheelStarted);
		EnhancedInputComponent->BindAction(RInputAction, ETriggerEvent::Started, this, &ASBPlayer::RStarted);
		EnhancedInputComponent->BindAction(BInputAction, ETriggerEvent::Started, this, &ASBPlayer::BStarted);
		EnhancedInputComponent->BindAction(IInputAction, ETriggerEvent::Started, this, &ASBPlayer::IStarted);
		EnhancedInputComponent->BindAction(FInputAction, ETriggerEvent::Started, this, &ASBPlayer::FStarted);
		EnhancedInputComponent->BindAction(TabInputAction, ETriggerEvent::Started, this, &ASBPlayer::TabStarted);
		EnhancedInputComponent->BindAction(CapsLockInputAction, ETriggerEvent::Started, this, &ASBPlayer::CapsLockStarted);
		if (NumberInputActions.IsValidIndex(1) && NumberInputActions[1])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[1], ETriggerEvent::Started, this, &ASBPlayer::Number1Started);
		}
		if (NumberInputActions.IsValidIndex(2) && NumberInputActions[2])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[2], ETriggerEvent::Started, this, &ASBPlayer::Number2Started);
		}
		if (NumberInputActions.IsValidIndex(3) && NumberInputActions[3])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[3], ETriggerEvent::Started, this, &ASBPlayer::Number3Started);
		}
	}
}

void ASBPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayerController();

	// Spawn Weapon
	UWorld* World = GetWorld();
	if(World)
	{
		FActorSpawnParameters WeaponSpawnParameters;
		WeaponSpawnParameters.Owner = this;
		SpawnAndStockWeapon(1);
		SpawnAndStockWeapon(2);
		SpawnBuildCameraPawn();
	}

	SpawnBuildingCreater();

	// UI
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(GetController());
	if (PlayerController)
	{
		HUD = Cast<ASBHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			OverlayWidget = HUD->GetPlayerOverlay();
			if (OverlayWidget)
			{
				OverlayWidget->CloseInventoryWidget();
				Inventory->SetInventoryWidget(OverlayWidget->GetPlayerInventoryWidget());
			}
		}

		
	}
}

void ASBPlayer::StockWeaponInQuickSlot(AWeapon* Weapon, uint32 Index)
{
	if (Weapon)
	{
		WeaponQuickslot[Index] = Weapon;
		SetWeaponVisibility(Weapon, false, false);
	}
}

void ASBPlayer::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void ASBPlayer::StopMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.2f);
	}
}

void ASBPlayer::PlayFireMontage(AWeapon* Weapon)
{
	if (Weapon)
	{
		EWeaponType WeaponType = Weapon->GetWeaponType();
		UAnimMontage* Montage = WeaponMontages[WeaponType].FireMontage;
		if (Montage)
		{
			
			PlayMontage(Montage);
		}
	}
	
}

void ASBPlayer::PlayReloadMontage(AWeapon* Weapon)
{
	if (Weapon)
	{
		EWeaponType WeaponType = Weapon->GetWeaponType();
		UAnimMontage* Montage = WeaponMontages[WeaponType].ReloadMontage;
		if (Montage)
		{
			UpperBodyState = EUpperBodyState::EUBS_Reloading;
			PlayMontage(Montage);
		}
	}
}

void ASBPlayer::PlayEquipMontage(AWeapon* Weapon)
{
	if (Weapon)
	{
		EWeaponType WeaponType = Weapon->GetWeaponType();
		UAnimMontage* Montage = WeaponMontages[WeaponType].EquipMontage;
		if (Montage)
		{
			UpperBodyState = EUpperBodyState::EUBS_Equipping;
			PlayMontage(Montage);
		}
	}
}

bool ASBPlayer::PickUpItem(AItem* Item)
{
	return Inventory->AddItem(Item);
}

bool ASBPlayer::IsFireReady() const
{
	return !IsUnarmed() &&
		ZoomState == ECharacterZoomState::ECZS_Zooming &&
		GetUpperBodyState() == EUpperBodyState::EUBS_Idle && 
		ControllMode == ECharacterControllMode::ECCM_Combat
		;
}

TArray<TSubclassOf<ABuilding>> ASBPlayer::GetBuildingList()
{
	return BuildingClasses;
}

void ASBPlayer::OnPlayerPossessStarted_Implementation()
{
	ControllMode = ECharacterControllMode::ECCM_Build;
	InitializePlayerController();
}

bool ASBPlayer::IsUnarmed() const
{
	return bUnArmed;
}

// TODO: �̰� �� �ٲٱ� �ؾ��� �� ����.
FTransform ASBPlayer::GetLHIKTransform() const
{
	FTransform LHIKTransform;
	AWeapon* Weapon = GetCurrentWeapon();
	if (Weapon != nullptr)
	{
		FTransform Transform = Weapon->GetSkeletalMesh()->GetSocketTransform(FName("LHIK"));
		FVector Location;
		FRotator Rotator;
		GetMesh()->TransformToBoneSpace("hand_r", Transform.GetLocation(), Transform.GetRotation().Rotator(), Location, Rotator);
		LHIKTransform.SetLocation(Location);
		LHIKTransform.SetRotation(Rotator.Quaternion());
	}
	return LHIKTransform;
}

AWeapon* ASBPlayer::GetCurrentWeapon() const
{
	if (WeaponQuickslot.IsValidIndex(CurrentWeaponIndex))
	{
		return WeaponQuickslot[CurrentWeaponIndex];
	}
	else
	{
		return nullptr;
	}
}

void ASBPlayer::OnReloadEndNotify_Implementation()
{
	ReloadEnd();
}

void ASBPlayer::OnEquipEndNotify_Implementation()
{
	EquipEnd();
}

void ASBPlayer::ZoomIn_Implementation()
{
	ZoomState = ECharacterZoomState::ECZS_Zooming;
	MovementSpeedScale = WalkScale;
}

void ASBPlayer::ZoomOut_Implementation()
{
	ZoomState = ECharacterZoomState::ECZS_NoZoom;
	MovementSpeedScale = JogScale;
}


void ASBPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y * MovementSpeedScale);
		AddMovementInput(RightDirection, MovementVector.X * MovementSpeedScale);
	}
}

void ASBPlayer::Look(const FInputActionValue& Value)
{
	if (!bUseUI)
	{
		// input is a Vector2D
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void ASBPlayer::MouseLStarted()
{
	if (IsFireReady()) 
	{
		GetCurrentWeapon()->UseStart();
	}
	else if (ControllMode == ECharacterControllMode::ECCM_Build)
	{
		BuildingCreater->CreateBuilding();
	}
}

void ASBPlayer::MouseLOngoing()
{
	if (IsFireReady())
	{
		GetCurrentWeapon()->UseOngoing();
	}
}

void ASBPlayer::MouseLCompleted()
{
	if (IsFireReady())
	{
		GetCurrentWeapon()->UseStop();
	}
}

void ASBPlayer::MouseRStarted()
{
	if (IsUnarmed() || ZoomState == ECharacterZoomState::ECZS_Zooming)
	{
		ZoomOut();

	}
	else if (ZoomState == ECharacterZoomState::ECZS_NoZoom)
	{
		ZoomIn();
	}
	else
	{
		SCREEN_LOG_NONE_KEY("Undefined ZoomState: ASBPlayer::Zoom");
	}
}

void ASBPlayer::MouseWheelStarted(const FInputActionValue& Value)
{
	if (FocusedInteractionComponent)
	{
		FocusedInteractionOptionIndex = FMath::Clamp<int32>(
			FocusedInteractionOptionIndex - Value.Get<float>(),
			0,
			FocusedInteractionComponent->GetInteractionNum() - 1
		);
		OverlayWidget->FocusInteractionDescriptionAt(FocusedInteractionOptionIndex);
	}
}

void ASBPlayer::RStarted()
{
	if (!IsUnarmed() && GetUpperBodyState() == EUpperBodyState::EUBS_Idle)
	{
		GetCurrentWeapon()->SpecificUse1();
	}
}

void ASBPlayer::BStarted()
{
	if (!IsUnarmed() && GetUpperBodyState() == EUpperBodyState::EUBS_Idle)
	{
		GetCurrentWeapon()->SpecificUse2();
	}
}

void ASBPlayer::IStarted()
{
	if (OverlayWidget)
	{
		if (!bUseUI)
		{
			ConvertToUIUseMode(true);
			OverlayWidget->OpenInventoryWidget(Inventory);
		}
		else
		{
			ConvertToUIUseMode(false);
			OverlayWidget->CloseInventoryWidget();
		}
	}
}

void ASBPlayer::FStarted()
{
	if (FocusedInteractionComponent)
	{
		FocusedInteractionComponent->BroadcastInteraction(FocusedInteractionOptionIndex, this);
	}
}

void ASBPlayer::NumberKeysStarted(uint32 i)
{
	switch (ControllMode)
	{
	case ECharacterControllMode::ECCM_Combat:
		SwitchWeapon(i);
		break;

	case ECharacterControllMode::ECCM_Build:
		SwitchBuildPreviewMesh(i);
		break;
	}
}

void ASBPlayer::Number1Started()
{
	NumberKeysStarted(1);
}

void ASBPlayer::Number2Started()
{
	NumberKeysStarted(2);
}

void ASBPlayer::Number3Started()
{
	NumberKeysStarted(3);
}

void ASBPlayer::SwitchWeapon(uint32 Index)
{
	if (Index == 3)
	{
		SwitchToUnarmedState();
	}
	else if (CurrentWeaponIndex != Index)
	{
		UnequipWeapon();
		EquipWeapon(Index);
		PlayEquipMontage(GetCurrentWeapon());
	}
}

void ASBPlayer::SwitchToUnarmedState()
{
	UnequipWeapon();
	ZoomOut();
}

void ASBPlayer::SwitchBuildPreviewMesh(uint32 Index)
{
	// TODO
	if (BuildingCreater && BuildingClasses.IsValidIndex(Index) && BuildingClasses[Index])
	{
		BuildingCreater->SetPreviewBuilding(BuildingClasses[Index]);
	}
}

void ASBPlayer::EquipWeapon(uint32 Index)
{
	SetCurrentWeapon(Index);
	AttachWeapon(GetCurrentWeapon(), RightHandSocketName);
	SetWeaponVisibility(GetCurrentWeapon(), true, true);
}

void ASBPlayer::UnequipWeapon()
{
	bUnArmed = true;
	DettachWeapon(GetCurrentWeapon());
	SetWeaponVisibility(GetCurrentWeapon(), false, true);
	StopMontage();
	if (GetCurrentWeapon())
	{
		GetCurrentWeapon()->StopMontage();
	}
	CurrentWeaponIndex = 0;
}

void ASBPlayer::SetWeaponVisibility(AWeapon* Weapon, bool bVisibility, bool bEffect)
{
	if (Weapon)
	{
		if (bEffect)
		{
			Weapon->SetMeshVisibleWithEffect(bVisibility);
		}
		else
		{
			Weapon->GetSkeletalMesh()->SetVisibility(bVisibility);
		}
	}
}

void ASBPlayer::SetCurrentWeapon(uint32 Index)
{
	if (WeaponQuickslot.IsValidIndex(Index))
	{
		bUnArmed = false;
		CurrentWeaponIndex = Index;
	}
}

void ASBPlayer::AttachWeapon(AWeapon* Weapon, FName SocketName)
{
	if (Weapon)
	{
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		Weapon->GetSkeletalMesh()->AttachToComponent(GetMesh(), AttachmentTransformRules, SocketName);
	}
}

void ASBPlayer::DettachWeapon(AWeapon* Weapon)
{
	if (Weapon)
	{
		Weapon->GetSkeletalMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void ASBPlayer::ReloadEnd()
{
	UpperBodyState = EUpperBodyState::EUBS_Idle;
}

void ASBPlayer::EquipEnd()
{
	UpperBodyState = EUpperBodyState::EUBS_Idle;
}

void ASBPlayer::TabStarted()
{
	if (ControllMode == ECharacterControllMode::ECCM_Build)
	{
		BuildingCreater->CancelPreview();
		ControllMode = ECharacterControllMode::ECCM_Combat;
	}
	else
	{
		SwitchToUnarmedState();
		ControllMode = ECharacterControllMode::ECCM_Build;
	}
}

void ASBPlayer::CapsLockStarted()
{
	if (ControllMode == ECharacterControllMode::ECCM_Build)
	{
		if (BuildCameraPawn == nullptr)
		{
			SpawnBuildCameraPawn();
		}
		if (BuildCameraPawn)
		{
			ControllMode = ECharacterControllMode::ECCM_None;
			BuildingCreater->CancelPreview();
			TransferPlayerControllerPossessionToPawn(BuildCameraPawn);
			BuildCameraPawn->OnPlayerPossessStarted();
			GetCharacterMovement()->StopMovementImmediately();
		}
	}
}

bool ASBPlayer::IsPlayingMontage(UAnimMontage* Montage) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		return AnimInstance->Montage_IsPlaying(Montage);
	}
	return false;
}

bool ASBPlayer::IsPlayingFireMontage(AWeapon* Weapon) const
{
	if (Weapon)
	{
		UAnimMontage* Montage = WeaponMontages[Weapon->GetWeaponType()].FireMontage;
		return IsPlayingMontage(Montage);
	}
	return false;
}

bool ASBPlayer::IsPlayingEquipMontage(AWeapon* Weapon) const
{
	if (Weapon)
	{
		UAnimMontage* Montage = WeaponMontages[Weapon->GetWeaponType()].EquipMontage;
		return IsPlayingMontage(Montage);
	}
	return false;
}

bool ASBPlayer::IsPlayingReloadMontage(AWeapon* Weapon) const
{
	if (Weapon)
	{
		UAnimMontage* Montage = WeaponMontages[Weapon->GetWeaponType()].ReloadMontage;
		return IsPlayingMontage(Montage);
	}
	return false;
}

void ASBPlayer::InitializePlayerController()
{
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->SetMouseInterface(false);
		PlayerController->PlayerCameraManager->ViewPitchMax = 70.0f;
		PlayerController->PlayerCameraManager->ViewPitchMin = -70.0f;
		PlayerController->AddInputMappingContext(DefaultMappingContext);
	}
}

void ASBPlayer::TransferPlayerControllerPossessionToPawn(APawn* Pawn)
{
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->RemoveInputMappingContext(DefaultMappingContext);
		PlayerController->UnPossess();
		PlayerController->Possess(Pawn);
	}
}

void ASBPlayer::SpawnBuildCameraPawn()
{
	// Spawn BuildCameraPawn
	if (BuildCameraPawnClass)
	{
		FActorSpawnParameters BuildCameraSpawnParameters;
		BuildCameraSpawnParameters.Owner = this;
		BuildCameraPawn = GetWorld()->SpawnActor<ABuildCameraPawn>(BuildCameraPawnClass, BuildCameraSpawnParameters);
	}
	if (BuildCameraPawn == nullptr)
	{
		SCREEN_LOG_NONE_KEY("ABuildCameraPawn spawn failed.");
	}
}

void ASBPlayer::SpawnBuildingCreater()
{
	UWorld *World = GetWorld();
	if (World && BuildingCreaterClass)
	{
		BuildingCreater = World->SpawnActor<ABuildingCreater>(BuildingCreaterClass);
		if (!BuildingCreater)
		{
			BuildingCreater = World->SpawnActor<ABuildingCreater>();
		}
		BuildingCreater->SetGridVisibility(false);
	}
}

void ASBPlayer::SpawnAndStockWeapon(uint32 i)
{
	if (WeaponClasses.IsValidIndex(i) && WeaponClasses[i])
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		StockWeaponInQuickSlot(GetWorld()->SpawnActor<AWeapon>(WeaponClasses[i], SpawnParameters), i);
	}
}

void ASBPlayer::SetBuildingCreaterLocation()
{
	if (BuildingCreater)
	{
		FVector Start = FollowCamera->GetComponentLocation();
		FVector End = Start + FollowCamera->GetForwardVector() * 1000.0f;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult HitResult;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		UKismetSystemLibrary::LineTraceSingleForObjects(
			this,
			Start,
			End,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);

		FVector Location = GetActorLocation() - (FVector::ZAxisVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		if (!HitResult.GetActor())
		{
			FVector ToGround = End - FVector::ZAxisVector * (500 + End.Z - GetActorLocation().Z);
			UKismetSystemLibrary::LineTraceSingleForObjects(
				this,
				End,
				ToGround,
				ObjectTypes,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true
			);
		}
		if (HitResult.GetActor())
		{
			Location = HitResult.ImpactPoint;
		}
		BuildingCreater->SnapLocation(Location);
	}
}

void ASBPlayer::ConvertToUIUseMode(bool bUse)
{
	bUseUI = bUse;
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->SetMouseInterface(bUse);
	}
}

void ASBPlayer::TraceInteractionActors()
{
	InteractiveActorList.Empty();
	
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * InteractionRange;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	while (true)
	{
		UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,
			End,
			FVector(0.0f, GetCapsuleComponent()->GetScaledCapsuleRadius() * 2, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
			GetActorForwardVector().Rotation(),
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);
		AActor* Actor = HitResult.GetActor();
		if (!Actor)
		{
			break;
		}
		ActorsToIgnore.Add(Actor);
		UInteractionComponent* InteractionComponent = Actor->GetComponentByClass<UInteractionComponent>();
		if (InteractionComponent)
		{
			InteractiveActorList.AddUnique(Actor);
		}
	}
}

void ASBPlayer::SelectInteractionActor()
{
	// InteractiveActorList에 있는 액터중, Interaction의 갯수가 1개 이상이면서 현재 캐릭터의 forward방향과 캐릭터에서 
	// InteractionTarget 액터까지 이은 벡터의 각도가 가장 작은 액터를 FocusedInteractable로 선정한다.

	AActor* PrevFocusedInteractable = FocusedInteractable;
	FocusedInteractable = nullptr;
	FocusedInteractionComponent = nullptr;

	int i = 0;
	float AoF = 0;
	for (i = 0; i < InteractiveActorList.Num(); ++i)
	{
		AActor* NewActor = InteractiveActorList[i];
		UInteractionComponent* NewInterComp = NewActor->GetComponentByClass<UInteractionComponent>();
		if (NewInterComp->GetInteractionNum() > 0)
		{
			FocusedInteractable = NewActor;
			FocusedInteractionComponent = NewInterComp;
			AoF = ForwardVectorDot(UKismetMathLibrary::Normal(FocusedInteractable->GetActorLocation() - GetActorLocation()));
			break;
		}
	}
	for (; i < InteractiveActorList.Num(); ++i)
	{
		AActor* NewActor = InteractiveActorList[i];
		UInteractionComponent* NewInterComp = NewActor->GetComponentByClass<UInteractionComponent>();
		if (NewInterComp->GetInteractionNum() > 0)
		{
			float AoN = ForwardVectorDot(UKismetMathLibrary::Normal(NewActor->GetActorLocation() - GetActorLocation()));
			if (AoN > AoF)
			{
				AoF = AoN;
				FocusedInteractable = NewActor;
				FocusedInteractionComponent = NewInterComp;
			}
		}
	}
	if (FocusedInteractable == nullptr)
	{
		OverlayWidget->HideInteractionList();
	}
	if (FocusedInteractable != PrevFocusedInteractable)
	{
		OverlayWidget->ShowInteractionList(FocusedInteractionComponent);
		FocusedInteractionOptionIndex = 0;
	}
}

float ASBPlayer::ForwardVectorDot(FVector B)
{
	return GetActorForwardVector().Dot(B);
}
