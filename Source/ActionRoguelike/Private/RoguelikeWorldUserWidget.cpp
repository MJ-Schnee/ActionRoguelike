// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void URoguelikeWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor is no longer valid. Removing health widget."));
		return;
	}

	FVector2D ScreenPosition;
	bool bIsVisibleOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(),
	                                                                 AttachedActor->GetActorLocation() + WorldOffset,
	                                                                 ScreenPosition);
	if (bIsVisibleOnScreen)
	{
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= ViewportScale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsVisibleOnScreen
			                             ? ESlateVisibility::HitTestInvisible
			                             : ESlateVisibility::Collapsed);
	}
}
