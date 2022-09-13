#include "DropObject.h"
#include "Setting.h"

ADropObject::ADropObject()
: m_pObjectItem(nullptr), m_szName("none")
{
	m_pMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("drop_object"));
	m_pMeshComp->SetupAttachment(RootComponent);
}

void ADropObject::SetObjectItem(FName name, ObjectItem* pObjectItem)
{
	m_pMeshComp->SetStaticMesh(pObjectItem->staticmesh);
	m_pObjectItem = pObjectItem;
	m_szName = name;
	SetRenderCustomDepthState(true);
}

void ADropObject::SetRenderCustomDepthState(bool bShowState)
{
	if (bShowState)
	{
		m_pMeshComp->SetRenderCustomDepth(true);
		m_pMeshComp->SetCustomDepthStencilValue(2);
	}
	else
		m_pMeshComp->SetRenderCustomDepth(false);
}
