#include "MiniMapCapture.h"
 
AMiniMapCapture::AMiniMapCapture()
{
	GetCaptureComponent2D()->bCaptureEveryFrame = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	MiniMapWidth = 256;
	MiniMapHeight = 256;
	MapPosLeftTop = FVector2D(3562, -464);
	MapPosRightDown = FVector2D(-573, 4059);
}

void AMiniMapCapture::BeginPlay()
{
	Super::BeginPlay();

	if (!GetCaptureComponent2D()->TextureTarget || MiniMapWidth != GetCaptureComponent2D()->TextureTarget->GetSurfaceWidth()
		|| MiniMapHeight != GetCaptureComponent2D()->TextureTarget->GetSurfaceHeight())
	{
		MiniMapView = NewObject<UTextureRenderTarget2D>();
		MiniMapView->InitAutoFormat(MiniMapWidth, MiniMapHeight);
		GetCaptureComponent2D()->TextureTarget = MiniMapView;
	}

	RootComponent->SetRelativeRotation(FVector(0, 0, -1).Rotation());
	FRotator OriginalRotation = RootComponent->GetComponentRotation();
	CachedFOV =  GetCaptureComponent2D()->FOVAngle;
	CachedLocation =  RootComponent->GetComponentLocation();
	UpdateWorldBounds();
}

void AMiniMapCapture::UpdateWorldBounds()
{
	TArray<FVector> Points;
	FVector const CamLocation = RootComponent->GetComponentLocation();
	float DistanceFromGround = CamLocation.Z;
	float Alpha = FMath::DegreesToRadians(GetCaptureComponent2D()->FOVAngle / 2);
	float MaxVisibleDistance = (DistanceFromGround / FMath::Cos(Alpha)) * FMath::Sin(Alpha);

	Points.Add(FVector(CamLocation.X + MaxVisibleDistance, CamLocation.Y + MaxVisibleDistance,0));
	Points.Add(FVector(CamLocation.X - MaxVisibleDistance, CamLocation.Y - MaxVisibleDistance,0));
	m_WorldBounds = FBox(Points);
	GetCaptureComponent2D()->UpdateContent();
}
