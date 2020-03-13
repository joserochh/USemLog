// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)


#include "SLEdUtils.h"
#include "EngineUtils.h"
#include "Editor/SLSemanticMapWriter.h"
#include "SLManager.h"

// Utils
#include "Utils/SLTagIO.h"
#include "Utils/SLUUid.h"

// Ctor
FSLEdUtils::FSLEdUtils() {}

/* Functionalities */
// Write the semantic map
void FSLEdUtils::WriteSemanticMap(UWorld* World, bool bOverwrite)
{
	FSLSemanticMapWriter SemMapWriter;
	FString TaskDir;

	for (TActorIterator<ASLManager> ActItr(World); ActItr; ++ActItr)
	{
		TaskDir = *ActItr->GetTaskId();
		break;
	}
	if(TaskDir.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("%s::%d Could not find the semantic manager to read the task id, set to default.."),
			*FString(__func__), __LINE__);
		TaskDir = "DefaultTaskId";
	}
	
	// Generate map and write to file
	SemMapWriter.WriteToFile(World, ESLOwlSemanticMapTemplate::IAIKitchen, TaskDir, TEXT("SemanticMap"), bOverwrite);
}

// Write unique IDs
void FSLEdUtils::WriteUniqueIds(UWorld* World, bool bOverwrite)
{
	static const FString TagType = TEXT("SemLog");
	static const FString TagKey = TEXT("Id");

	for (TActorIterator<AActor> ActItr(World); ActItr; ++ActItr)
	{
		if (ActItr->IsA(AStaticMeshActor::StaticClass()))
		{
			FSLTagIO::AddKVPair(*ActItr, TagType, TagKey, FSLUuid::NewGuidInBase64Url(), bOverwrite);
		}
	}
}