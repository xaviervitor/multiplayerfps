// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"

#define LOG(Value) UE_LOG(LogTemp, Warning, TEXT(Value))
#define PRINTC(Value) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, TEXT(Value))
#define PRINT(Value) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, Value)

#define ENUM_TO_INT32(Enum) static_cast<int32>(Enum)

static void PrintWeapons(TArray<AWeapon*> Weapons) {
	for (int32 i = 0 ; i < Weapons.Num() ; i++) {
		AWeapon* Weapon = Weapons[i];
		FString str;
		str.AppendInt(i);
		str.AppendChar({' '});
		str.Append(Weapon->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, str);
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, "");
}