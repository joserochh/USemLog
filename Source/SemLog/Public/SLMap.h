// Copyright 2017, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SLOwl.h"
#include "SLMap.generated.h"

/**
 * 
 */
UCLASS()
class SEMLOG_API USLMap : public UObject
{
	GENERATED_BODY()

public:
	// Constructor
	USLMap();

	// Destructor
	~USLMap();

	// Check if semantic map already exists
	UFUNCTION(BlueprintCallable)
	bool Exists();

	// Generate the semantic map
	UFUNCTION(BlueprintCallable)
	bool Generate(UWorld* World);

	// Write document to file
	UFUNCTION(BlueprintCallable)
	bool WriteToFile(bool bOverwrite = false);

	// Filename
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SL)
	FString Filename;

	// Semantic logging directory path
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SL)
	FString LogDirectoryPath;

	// Semantic map object // TODO see which version to use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SL)
	FOwlObject OwlObject;

	// Class name of the semantic map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SL)
	FString Class;

	// Unique ID of the semantic map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SL)
	FString Id;

	// Namespace of the semantic map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SL)
	FString Ns;

	// Semantic map document as owl representation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SL)
	FOwlDocument OwlDocument;

	// Shows if the map already exists
	uint8 bExists : 1;

	// Shows if the default values of the owl document have been set
	uint8 bOwlDefaultValuesSet : 1;

private:
	// Full name of the semantic map, Class + Id 
	FString Name;

	// Full name of the semantic map, Ns + Name
	FString FullName;

	// Set document default values
	void SetDefaultValues();

	// Remove document default values
	void RemoveDefaultValues();

	// Insert individual to the map with its 3D transform
	bool InsertIndividual(const TPair<AActor*, TMap<FString, FString>>& ActorWithProperties);
};