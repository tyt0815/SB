#pragma once
#include "Kismet/KismetStringLibrary.h"

// String Log
#define SCREEN_DEBUG_MESSAGE(Key, TimeToDisplay, Color, DebugMessage)\
	if(GEngine) \
	{\
		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, GetName() +": " + DebugMessage);\
	}

#define SCREEN_LOG(Key, DebugMessage)\
	switch (Key)\
	{ \
		case 0: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Red, DebugMessage); break;\
		case 1: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Green, DebugMessage); break;\
		case 2: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Blue, DebugMessage); break;\
		case 3: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Yellow, DebugMessage); break; \
		case 4: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Cyan, DebugMessage); break; \
		case 5: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Magenta, DebugMessage); break;\
		default: FColor::Purple; break;\
	}\
	

#define SCREEN_LOG_COLOR(Key, DisplayColor, DebugMessage) SCREEN_DEBUG_MESSAGE(Key, 5, DisplayColor, DebugMessage);
#define SCREEN_LOG_SINGLE_FRAME(DebugMessage) SCREEN_DEBUG_MESSAGE(INDEX_NONE, 0, FColor::Purple, DebugMessage);
#define SCREEN_LOG_NONE_KEY(DebugMessage) SCREEN_DEBUG_MESSAGE(INDEX_NONE, 5, FColor::Orange, DebugMessage);

// Debug Draw Shape
#define DRAW_SPHERE(Location) if(GetWorld())DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Cyan, false, 5.0f);
#define DRAW_SPHERE_COLOR(Location, Color) if(GetWorld())DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.0f);
#define DRAW_SPHERE_SINGLE_FRAME(Location) if(GetWorld())DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Cyan, false, -1.f);
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_COLOR(StartLocation, EndLocation, Color) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, true, -1.f, 0, 1.f);
#define DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_LINE_SINGLE_FRAME_COLOR(StartLocation, EndLocation, Color) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Green, true);
#define DRAW_POINT_SINGLE_FRAME(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Green, false, -1.f);
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld())\
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);\
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Green, true);\
}
#define DRAW_VECTOR_SINGLE_FRAME(StartLocation, EndLocation) if (GetWorld())\
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);\
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Green, false, -1.f);\
}