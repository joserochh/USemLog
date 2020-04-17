// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "Data/SLIndividualComponent.h"
#include "Data/SLIndividual.h"
#include "Data/SLSkeletalIndividual.h"
#include "Data/SLVisualIndividual.h"

// Sets default values for this component's properties
USLIndividualComponent::USLIndividualComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bOverwriteEditChanges = false;
	bSaveToTagButton = false;
	bLoadFromTagButton = false;
	bToggleVisualMaskMaterial = false;
	bToggleSemanticText = false;
}

// Called before destroying the object.
void USLIndividualComponent::BeginDestroy()
{
	Super::BeginDestroy();
	if (SemanticIndividual)
	{
		SemanticIndividual->ConditionalBeginDestroy();
	}
}

// Called after the C++ constructor and after the properties have been initialized, including those loaded from config.
//void USLIndividualComponent::PostInitProperties()
//{
//	Super::PostInitProperties();
//}

#if WITH_EDITOR
// Called when a property is changed in the editor
void USLIndividualComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Get the changed property name
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Convert datatype
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USLIndividualComponent, ConvertTo))
	{
		FSLIndividualUtils::ConvertIndividualObject(SemanticIndividual, ConvertTo);
	}

	/* Button workaround triggers */
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(USLIndividualComponent, bSaveToTagButton))
	{
		bSaveToTagButton = false;
		ExportToTag(bOverwriteEditChanges);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(USLIndividualComponent, bLoadFromTagButton))
	{
		bLoadFromTagButton = false;
		ImportFromTag(bOverwriteEditChanges);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(USLIndividualComponent, bToggleVisualMaskMaterial))
	{
		bToggleVisualMaskMaterial = false;
		ToggleVisualMaskVisibility();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(USLIndividualComponent, bToggleSemanticText))
	{
		bToggleSemanticText = false;
		ToggleSemanticTextVisibility();
	}
}
#endif // WITH_EDITOR

// Called when a component is created(not loaded).This can happen in the editor or during gameplay
void USLIndividualComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	AActor* Owner = GetOwner();

	// Check if actor already has a semantic data component
	for (const auto AC : Owner->GetComponentsByClass(USLIndividualComponent::StaticClass()))
	{
		if (AC != this)
		{
			UE_LOG(LogTemp, Error, TEXT("%s::%d %s already has a semantic data component (%s), self-destruction commenced.."),
				*FString(__FUNCTION__), __LINE__, *GetOwner()->GetName(), *AC->GetName());
			//DestroyComponent();
			ConditionalBeginDestroy();
			return;
		}
	}

	// Set semantic individual type depending on owner
	if (UClass* IndividualClass = FSLIndividualUtils::CreateIndividualObject(this, Owner, SemanticIndividual))
	{
		// Cache the current individual class type
		ConvertTo = IndividualClass;
	}
	else
	{
		// Unknown individual type, destroy self
		ConditionalBeginDestroy();
		return;
	}
}

// Called when the game starts
void USLIndividualComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Save data to owners tag
void USLIndividualComponent::ExportToTag(bool bOverwrite)
{
	if (SemanticIndividual)
	{
		SemanticIndividual->ExportToTag(bOverwrite);
	}
}

// Load data from owners tag
void USLIndividualComponent::ImportFromTag(bool bOverwrite)
{
	if (SemanticIndividual)
	{
		SemanticIndividual->ImportFromTag(bOverwrite);
	}
}

// Reload the individual data
bool USLIndividualComponent::LoadIndividual()
{
	if (SemanticIndividual->IsValidLowLevel())
	{
		SemanticIndividual->Load();
	}
	return false;
}

// Toggle between original and mask material is possible
bool USLIndividualComponent::ToggleVisualMaskVisibility()
{
	if (USLVisualIndividual* SI = GetCastedIndividualObject<USLVisualIndividual>())
	{
		return SI->ToggleMaterials();
	}
	return false;
}

// Toggle between showing the semantic data in text form
bool USLIndividualComponent::ToggleSemanticTextVisibility()
{
	UE_LOG(LogTemp, Error, TEXT("%s::%d TEXT FORM"), *FString(__func__), __LINE__);
	return false;
}
