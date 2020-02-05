// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "World/SLGazeDataHandler.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SLEntitiesManager.h"

#if SL_WITH_EYE_TRACKING
#include "SLGazeProxy.h"
#endif // SL_WITH_EYE_TRACKING

// Default ctor
FSLGazeDataHandler::FSLGazeDataHandler()
{
	bIsInit = false;
	bIsStarted = false;
	bIsFinished = false;
	
	World = nullptr;
	PlayerCameraRef = nullptr;
}

// Setup the eye tracking software
void FSLGazeDataHandler::Init(UWorld* InWorld)
{
	if(!bIsInit)
	{
#if SL_WITH_EYE_TRACKING
		World = InWorld;

		for (TActorIterator<ASLGazeProxy> GazeItr(InWorld); GazeItr; ++GazeItr)
		{
			GazeProxy = *GazeItr;
			break;
		}
		
		if (!GazeProxy)
		{
			UE_LOG(LogTemp, Error, TEXT("%s::%d No ASLGazeProxy found in the world, eye tracking will be disabled.."), *FString(__func__), __LINE__);
			return;
		}

		if (GazeProxy->Start())
		{
			UE_LOG(LogTemp, Error, TEXT("%s::%d Eye tracking framework successfully started.."), *FString(__func__), __LINE__);
			bIsInit = true;
		}	
#endif // SL_WITH_EYE_TRACKING
	}
}

	// Setup the active camera
void FSLGazeDataHandler::Start()
{
	if(!bIsStarted && bIsInit)
	{
#if SL_WITH_EYE_TRACKING
		if(World)
		{
			if(UGameplayStatics::GetPlayerController(World, 0))
			{
				PlayerCameraRef = UGameplayStatics::GetPlayerController(World, 0)->PlayerCameraManager;
				if(PlayerCameraRef)
				{
					bIsStarted = true;
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s::%d calling GetPlayerController failed, this should be called after BeginPlay!"), *FString(__func__), __LINE__);
			}
		}
#endif // SL_WITH_EYE_TRACKING
	}
}

// Stop the eye tracking framework
void FSLGazeDataHandler::Finish()
{
	if (!bIsFinished && (bIsStarted || bIsInit))
	{
#if SL_WITH_EYE_TRACKING
		GazeProxy->Stop();		
		bIsStarted = false;
		bIsInit = false;
		bIsFinished = true;
	#endif // SL_WITH_EYE_TRACKING
	}
}

// Get the current gaze data, true if a raytrace hit occurs
bool FSLGazeDataHandler::GetData(FSLGazeData& OutData)
{
	if(!bIsStarted)
	{
		return false;
	}

#if SL_WITH_EYE_TRACKING
	FVector RelativeGazeDirection;
	if (GazeProxy->GetRelativeGazeDirection(RelativeGazeDirection))
	{
		const FVector RaycastOrigin = PlayerCameraRef->GetCameraLocation();
		const FVector RaycastTarget =  PlayerCameraRef->GetCameraRotation().RotateVector(RaycastOrigin + RelativeGazeDirection * RayLength);

		FCollisionQueryParams TraceParam = FCollisionQueryParams(FName("EyeTraceParam"), true, PlayerCameraRef);
		FHitResult HitResult;

		// Line trace
		if(RayRadius == 0.f)
		{
			if(World->LineTraceSingleByChannel(HitResult, RaycastOrigin, RaycastTarget, ECC_Pawn, TraceParam))
			{
				FSLEntity Entity;
				if(FSLEntitiesManager::GetInstance()->GetEntity(HitResult.Actor.Get(),Entity))
				{
					OutData.SetData(RaycastOrigin, HitResult.ImpactPoint, Entity);
					DrawDebugLine(World, RaycastOrigin, RaycastTarget, FColor::Green);
					DrawDebugSphere(World, HitResult.ImpactPoint, 2.f, 32, FColor::Red);
					return true;
				}
				else
				{
					DrawDebugLine(World, RaycastOrigin, RaycastTarget, FColor::Emerald);
					return false;
				}
			}
		}
		else
		{
			FCollisionShape Sphere;
			Sphere.SetSphere(RayRadius);
			if(World->SweepSingleByChannel(HitResult,RaycastOrigin, RaycastTarget, FQuat::Identity, 
				ECC_Pawn, Sphere, TraceParam))
			{
				FSLEntity Entity;
				if(FSLEntitiesManager::GetInstance()->GetEntity(HitResult.Actor.Get(),Entity))
				{
					OutData.SetData(RaycastOrigin, HitResult.ImpactPoint, Entity);
					DrawDebugLine(World, RaycastOrigin, RaycastTarget, FColor::Green);
					DrawDebugSphere(World, HitResult.ImpactPoint, RayRadius, 32, FColor::Red);
					return true;
				}
				else
				{
					DrawDebugLine(World, RaycastOrigin, RaycastTarget, FColor::Emerald);
					return false;
				}
			}
		}
	}
#endif // SL_WITH_EYE_TRACKING	
return false;
}

