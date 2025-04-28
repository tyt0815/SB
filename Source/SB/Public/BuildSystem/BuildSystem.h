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
	FORCEINLINE static FVector GetRelativeLocation(int32 GridX, int32 GridY, int32 GridZ)
	{
		return FVector(CELL_SIZE * GridX, CELL_SIZE * GridY, CELL_SIZE * GridZ);
	}
	FORCEINLINE static FVector GetRelativeLocation(FIntVector GridCoord)
	{
		return GetRelativeLocation(GridCoord.X, GridCoord.Y, GridCoord.Z);
	}
};
