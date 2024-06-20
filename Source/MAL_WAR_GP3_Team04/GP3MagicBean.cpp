#include "GP3MagicBean.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


AMagicBean::AMagicBean()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	BeanCol = CreateDefaultSubobject<USphereComponent>("Bean Collision");
	BeanCol->SetupAttachment(Root);
	//BeanCol->OnComponentBeginOverlap.AddDynamic(this, &AMagicBean::HandleBeginOverlap);
	//BeanCol->OnComponentEndOverlap.AddDynamic(this, &AMagicBean::HandleEndOverlap);
	
}

void AMagicBean::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicBean::BeginPlay()
{
	Super::BeginPlay();
	

	AGP3CharacterMovement* charMov = Cast<AGP3CharacterMovement>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	

	if (charMov != nullptr)
	{
		NormalSpeed = charMov->GetCharacterMovement()->MaxWalkSpeed;;
		NormalJumpVel = charMov->GetCharacterMovement()->JumpZVelocity;
		NormalPushForce = charMov->GetCharacterMovement()->PushForceFactor;
		NormalMass = charMov->GetCharacterMovement()->Mass;
		NormalCamLength = charMov->GetComponentByClass<USpringArmComponent>()->TargetArmLength;
		charMov->CamLengthNormal = NormalCamLength;
		charMov->CamLengthChanged = 580;
	
	}
	
}

void AMagicBean::UpdateSizeStates(AGP3CharacterMovement* charMov, ESizeState updateSize, FVector& returnValue)
{
	

	if (updateSize != charMov->SizeState)
	{

	GEngine->AddOnScreenDebugMessage(-1, 5.f,
		FColor::Orange,
		FString::Printf(TEXT("My state: %s"),
			*UEnum::GetValueAsString(updateSize)));
	GEngine->AddOnScreenDebugMessage(-1, 5.f,
		FColor::Orange,
		FString::Printf(TEXT("My state: %s"),
			*UEnum::GetValueAsString(charMov->SizeState)));

	switch (updateSize)
	{
	
	case ESizeState::Small:
		GEngine->AddOnScreenDebugMessage(-1, 5.f,
			FColor::Orange,
			FString::Printf(TEXT("Success")));
		returnValue = CharaSize / SizeMultiplier;
			//Character->SetActorScale3D(returnValue);

		if (charMov->SizeState == ESizeState::Big)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f,
				FColor::Orange,
				FString::Printf(TEXT("Success Big")));
			charMov->SizeState = ESizeState::Normal;
			NormalValues(charMov);
		}
		else if (charMov->SizeState == ESizeState::Normal)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f,
				FColor::Orange,
				FString::Printf(TEXT("Success Normal")));
			charMov->SizeState = ESizeState::Small;
			BeanValues(charMov);
		}

		break;
	case ESizeState::Normal:
		charMov->SizeState = ESizeState::Normal;
		break;
	case ESizeState::Big:
		//state som inte byter när du är redan i staten
		GEngine->AddOnScreenDebugMessage(-1, 5.f,
			FColor::Orange,
			FString::Printf(TEXT("Success")));
		returnValue = CharaSize * SizeMultiplier;
		//Character->SetActorScale3D(returnValue);
	
		if (charMov->SizeState == ESizeState::Small)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f,
				FColor::Orange,
				FString::Printf(TEXT("Success Small")));
			charMov->SizeState = ESizeState::Normal;
			NormalValues(charMov);
		}
		else if (charMov->SizeState == ESizeState::Normal)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f,
				FColor::Orange,
				FString::Printf(TEXT("Success Normal")));
			charMov->SizeState = ESizeState::Big;
			BeanValues(charMov);
		}
		break;
	default:
		break;
	}
	
	}
	
}

void AMagicBean::NormalValues(AGP3CharacterMovement* charMov)
{
	charMov->GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	charMov->GetCharacterMovement()->JumpZVelocity = NormalJumpVel;
	charMov->GetCharacterMovement()->PushForceFactor = NormalPushForce;
	charMov->GetCharacterMovement()->Mass = NormalMass;
	charMov->GetComponentByClass<USpringArmComponent>()->TargetArmLength = NormalCamLength;
}

void AMagicBean::BeanValues(AGP3CharacterMovement* charMov)
{
	charMov->GetCharacterMovement()->MaxWalkSpeed = BeanSpeed;
	charMov->GetCharacterMovement()->JumpZVelocity = BeanJump;
	charMov->GetCharacterMovement()->PushForceFactor = PushForce;
	charMov->GetCharacterMovement()->Mass = ChangedMass;
	charMov->GetComponentByClass<USpringArmComponent>()->TargetArmLength = ChangedLengthCam;
}