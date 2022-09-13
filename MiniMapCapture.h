#pragma once

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "MiniMapCapture.generated.h"


UCLASS(Blueprintable)
class AMiniMapCapture : public ASceneCapture2D
{
	GENERATED_BODY()
	AMiniMapCapture();

	virtual void BeginPlay();
 
	 
public:
	void UpdateWorldBounds();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MiniMap, meta=(ClampMin = "0", ClampMax="1024"))
	int32 MiniMapWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MiniMap, meta=(ClampMin = "0", ClampMax="1024"))
	int32 MiniMapHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MiniMap)
	FVector2D MapPosLeftTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MiniMap)
	FVector2D MapPosRightDown;

	FBox m_WorldBounds;

protected:
	UPROPERTY()
	UTextureRenderTarget2D* MiniMapView;

	FVector CachedLocation;
	float CachedFOV;

};
