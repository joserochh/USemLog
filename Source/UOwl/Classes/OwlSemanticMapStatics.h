// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "EngineMinimal.h"
#include "OwlSemanticMap.h"

/**
* Helper functions for generating semantic maps
*/
struct UOWL_API FOwlSemanticMapStatics
{
	/* Semantic map template creation */
	// Create Default semantic map
	static TSharedPtr<FOwlSemanticMap> CreateDefaultSemanticMap(
		const FString& InMapId,
		const FString& InMapPrefix = "ue-def",
		const FString& InMapOntologyName = "UE-DefaultMap");

	// Create IAI Kitchen semantic map
	static TSharedPtr<FOwlSemanticMap> CreateIAIKitchenSemanticMap(
		const FString& InMapId,
		const FString& InMapPrefix = "ue-iai-kitchen",
		const FString& InMapOntologyName = "UE-IAI-Kitchen");

	// Create IAI Supermarket semantic map
	static TSharedPtr<FOwlSemanticMap> CreateIAISupermarketSemanticMap(
		const FString& InMapId,
		const FString& InMapPrefix = "ue-iai-supermarket",
		const FString& InMapOntologyName = "UE-IAI-Supermarket");
		
		
	/* Owl individuals / definitions creation */
	// Create an object individual
	static FOwlNode CreateObjectIndividual(
		const FString& InMapPrefix, 
		const FString& Id, 
		const FString& Class);

	// Create a pose individual
	static FOwlNode CreatePoseIndividual(
		const FString& InMapPrefix, 
		const FString& InId,
		const FVector& InLoc,
		const FQuat& InQuat);

	// Create a constraint individual
	static FOwlNode CreateConstraintIndividual(
		const FString& InMapPrefix, 
		const FString& InId,
		const FString& ParentId,
		const FString& ChildId);

	// Create linear constraint properties individual
	static FOwlNode CreateLinearConstraintProperties(
		const FString& InMapPrefix, 
		const FString& InId,
		uint8 XMotion,
		uint8 YMotion,
		uint8 ZMotion,
		float Limit,
		bool bSoftConsraint,
		float Stiffness,
		float Damping);

	// Create angular constraint properties individual
	static FOwlNode CreateAngularConstraintProperties(
		const FString& InMapPrefix,
		const FString& InId,
		uint8 Swing1Motion,
		uint8 Swing2Motion,
		uint8 TwistMotion,
		float Swing1Limit,
		float Swing2Limit,
		float TwistLimit,
		bool bSoftSwingConstraint,
		float SwingStiffness,
		float SwingDamping,
		bool bSoftTwistConstraint,
		float TwistStiffness,
		float TwistDamping);

	// Create a constraint individual
	static FOwlNode CreateClassDefinition(const FString& Class);

	// Create Tags individual
	static FOwlNode CreateTagsIndividual(
		const FString& InMapPrefix,
		const FString& Id,
		const FString& Class);

	
	/* Owl properties creation */
	// Create generic property
	static FOwlNode CreateGenericProperty(const FOwlPrefixName& InPrefixName,
		const FOwlAttributeValue& InAttributeValue);
	
	// Create class property
	static FOwlNode CreateClassProperty(const FString& InClass);

	// Create describedInMap property
	static FOwlNode CreateDescribedInMapProperty(
		const FString& InMapPrefix, const FString& InMapId);

	// Create pathToCadModel property
	static FOwlNode CreatePathToCadModelProperty(const FString& InClass);

	// Create tagsData property
	static FOwlNode CreateTagsDataProperty(const TArray<FName>& InTags);

	// Create subClassOf property
	static FOwlNode CreateSubClassOfProperty(const FString& InSubClassOf);

	// Create skeletal bone property
	static FOwlNode CreateSkeletalBoneProperty(const FString& InBone);

	// Create subclass - depth property
	static FOwlNode CreateDepthProperty(float Value);

	// Create subclass - height property
	static FOwlNode CreateHeightProperty(float Value);

	// Create subclass - width property
	static FOwlNode CreateWidthProperty(float Value);

	// Create owl:onProperty meta property
	static FOwlNode CreateOnProperty(const FString& InProperty);

	// Create a property with a bool value
	static FOwlNode CreateBoolValueProperty(const FOwlPrefixName& InPrefixName, bool bValue);

	// Create a property with a int value
	static FOwlNode CreateIntValueProperty(const FOwlPrefixName& InPrefixName, int32 Value);

	// Create a property with a float value
	static FOwlNode CreateFloatValueProperty(const FOwlPrefixName& InPrefixName, float Value);

	// Create a property with a string value
	static FOwlNode CreateStringValueProperty(const FOwlPrefixName& InPrefixName, const FString& InValue);

	// Create pose property
	static FOwlNode CreatePoseProperty(const FString& InMapPrefix, const FString& InId);

	// Create linear constraint property
	static FOwlNode CreateLinearConstraintProperty(const FString& InMapPrefix, const FString& InId);

	// Create angular constraint property
	static FOwlNode CreateAngularConstraintProperty(const FString& InMapPrefix, const FString& InId);

	// Create child property
	static FOwlNode CreateChildProperty(const FString& InMapPrefix, const FString& InId);

	// Create parent property
	static FOwlNode CreateParentProperty(const FString& InMapPrefix, const FString& InId);

	// Create a location property
	static FOwlNode CreateLocationProperty(const FVector& InLoc);

	// Create a quaternion property
	static FOwlNode CreateQuaternionProperty(const FQuat& InQuat);
};
