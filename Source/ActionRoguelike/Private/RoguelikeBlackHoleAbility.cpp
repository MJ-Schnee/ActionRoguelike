// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeBlackHoleAbility.h"

#include "PhysicsEngine/RadialForceComponent.h"

ARoguelikeBlackHoleAbility::ARoguelikeBlackHoleAbility()
{
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(RootComponent);
}
