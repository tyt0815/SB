// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/BuildSystem.h"

BuildSystem::BuildSystem()
{
}

BuildSystem::~BuildSystem()
{
}

FVector BuildSystem::SnapLocationXY(FVector WorldLocation)
{
	int XIndex = WorldLocation.X / CELL_SIZE;
	int YIndex = WorldLocation.Y / CELL_SIZE;
	FVector SnappedLocation = WorldLocation;
	SnappedLocation.X = CELL_SIZE * XIndex;
	SnappedLocation.Y = CELL_SIZE * YIndex;
	if (WorldLocation.X >= 0)
	{
		SnappedLocation.X += CELL_SIZE * 0.5f;
	}
	else
	{
		SnappedLocation.X -= CELL_SIZE * 0.5f;
	}
	if (WorldLocation.Y >= 0)
	{
		SnappedLocation.Y += CELL_SIZE * 0.5f;
	}
	else
	{
		SnappedLocation.Y -= CELL_SIZE * 0.5f;
	}
	return SnappedLocation;
}

FVector BuildSystem::CalculateBoxExtent(int X, int Y, int Z)
{
	int Offset = (CELL_SIZE / 2);
	FVector Extent;
	Extent.X = CELL_SIZE * X - Offset;
	Extent.Y = CELL_SIZE * Y - Offset;
	Extent.Z = CELL_SIZE * Z - Offset;
	return Extent;
}
