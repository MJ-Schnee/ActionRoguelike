// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAnimInstance.h"

#include "GameplayTagContainer.h"
#include "RoguelikeActionComponent.h"

void URoguelikeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<URoguelikeActionComponent>(OwningActor->GetComponentByClass(URoguelikeActionComponent::StaticClass()));
	}
}

void URoguelikeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
