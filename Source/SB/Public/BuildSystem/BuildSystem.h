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
};
