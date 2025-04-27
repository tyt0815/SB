#pragma once

struct FItemData
{
	TSubclassOf<class AItem> ItemClass = nullptr;
	UTexture2D* Thumbnail = nullptr;
	FName Name;
	int Quantity = 0;
	bool bStackable = false;
};