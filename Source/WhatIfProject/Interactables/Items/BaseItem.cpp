// Copyright FastForward Studios. All Rights Reserved.


#include "BaseItem.h"
#include "Characters/Squad/PlayerCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


ABaseItem::ABaseItem(const FObjectInitializer& ObjectInitializer) : Super()
{
	// TODO: Redundancies should be changed
	ItemInfo.ItemType = ItemInfo.ItemType;
	ItemInfo.ItemName = ItemInfo.ItemName;
	ItemInfo.ItemImage = ItemInfo.ItemImage;
	ItemInfo.ItemClass = UGameplayStatics::GetObjectClass(this);
	ItemInfo.ItemIndex = ItemInfo.ItemIndex;
	ItemInfo.ItemSlot = ItemInfo.ItemSlot;
	ItemInfo.ItemValue = ItemInfo.ItemValue;
}

void ABaseItem::Interact(APlayerCharacter PlayerInstigator)
{
	// Const function (read-only, "pure" in BP)
	bool CallFunc_HasSpaceInInventory_ReturnValue{};

	//bpfv__CallFunc_HasSpaceInInventory__ReturnValue__pf = b1l__K2Node_Event_Instigator__pf->bpf__HasSpaceInInventoryx__pfzy();
	//CallFunc_HasSpaceInInventory_ReturnValue = PlayerInstigator->HasSpaceInInventory();

	if (CallFunc_HasSpaceInInventory_ReturnValue == true)
	{
		// Message from Interact Interface
		IInteractInterface::Execute_PickUp(PlayerInstigator.GetOwner(), ItemInfo);

		K2_DestroyActor();
	}
}
