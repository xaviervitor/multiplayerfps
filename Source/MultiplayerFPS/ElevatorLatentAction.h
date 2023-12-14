#pragma once

#include "LatentActions.h"
#include "Math/Transform.h"
#include "Kismet/KismetMathLibrary.h"
#include "Elevator.h"

class ElevatorLatentAction : public FPendingLatentAction {
private:
    AElevator* ElevatorActor;
    FTransform InitialTransform;
    FTransform TargetTransform;
    FTransform CurrentTransform;
    float& DeltaTime;
    float ElevatorSpeed;
    float InterpolationTime;
    float ElapsedTime;

    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget; 
public:
    ElevatorLatentAction(AElevator* ElevatorActor, FTransform InitialTransform, FTransform TargetTransform, float& DeltaTime, const FLatentActionInfo& LatentInfo) : 
            ElevatorActor(ElevatorActor),
            InitialTransform(InitialTransform),
            TargetTransform(TargetTransform),
            CurrentTransform(InitialTransform),
            DeltaTime(DeltaTime),
            ExecutionFunction(LatentInfo.ExecutionFunction),
            OutputLink(LatentInfo.Linkage),
            CallbackTarget(LatentInfo.CallbackTarget) {
        ElevatorSpeed = ElevatorActor->ElevatorSpeed;
        InterpolationTime = FVector::Distance(TargetTransform.GetLocation(), CurrentTransform.GetLocation()) / ElevatorSpeed;
    }

    virtual void UpdateOperation(FLatentResponse& Response) override {
	    // GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::MakeRandomColor(), "in elevator");
        
        // CurrentTransform = UKismetMathLibrary::TInterpTo(CurrentTransform, TargetTransform, DeltaTime, ElevatorSpeed); 
        
        ElapsedTime += DeltaTime;
        CurrentTransform = UKismetMathLibrary::TLerp(CurrentTransform, TargetTransform, ElapsedTime / InterpolationTime, ELerpInterpolationMode::DualQuatInterp);

        ElevatorActor->SetActorTransform(CurrentTransform);
        
        Response.FinishAndTriggerIf(CurrentTransform.EqualsNoScale(TargetTransform, 0.01f), ExecutionFunction, OutputLink, CallbackTarget);
    }
};
