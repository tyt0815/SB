#pragma once

#include "CoreMinimal.h"
#include "BuildSystemDefines.h"

/**
 * 
 */
class SB_API BuildSystem
{
public:
	BuildSystem();
	~BuildSystem();

	static FVector SnapLocationXY(FVector WorldLocation);
	
	static FVector CalculateBoxExtent(int X, int Y, int Z);

public:
	FORCEINLINE static FVector CalculateBoxExtent(FIntVector V)
	{
		return CalculateBoxExtent(V.X, V.Y, V.Z);
	}
};
