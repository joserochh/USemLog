// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Individuals/Type/SLBaseIndividual.h"
#include "SLLightIndividual.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = SL)
class USEMLOG_API USLLightIndividual : public USLBaseIndividual
{
	GENERATED_BODY()

public:
    // Ctor
    USLLightIndividual();

    // Called before destroying the object.
    virtual void BeginDestroy() override;

    // Init asset references (bForced forces re-initialization)
    virtual bool Init(bool bReset);

    // Load semantic data (bForced forces re-loading)
    virtual bool Load(bool bReset, bool bTryImport);

    // Get the type name as string
    virtual FString GetTypeName() const override { return FString("LightIndividual"); };

protected:
    // Get class name, virtual since each invidiual type will have different name
    virtual FString CalcDefaultClassValue() override;


private:
    // Set dependencies
    bool InitImpl();

    // Set data
    bool LoadImpl(bool bTryImport);

    // Clear all values of the individual
    void InitReset();

    // Clear all data of the individual
    void LoadReset();
};
