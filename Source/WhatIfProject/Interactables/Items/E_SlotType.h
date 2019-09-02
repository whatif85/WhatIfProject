// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "E_SlotType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum E_SlotType
{
	ITEM_EQUIP_HandRight		UMETA(DisplayName = "Right Hand Equip"),	// Equipped Object Right Hand
	ITEM_EQUIP_HandLeft			UMETA(DisplayName = "Left Hand Equip"),		// Equipped Object Left Hand
	
	ITEM_CLOTHING_Forehead		UMETA(DisplayName = "Forehead"),			// Headbands, Tiaras
	ITEM_CLOTHING_Head			UMETA(DisplayName = "Head"),				// Hats, Caps
	ITEM_CLOTHING_HeadAcc1		UMETA(DisplayName = "Head Accessory 1"),	// Head/Hair Accessory 1
	ITEM_CLOTHING_HeadAcc2		UMETA(DisplayName = "Head Accessory 2"),	// Head/Hair Accessory 2
	ITEM_CLOTHING_Neck1			UMETA(DisplayName = "Neck 1"),				// Necklaces, Tags
	ITEM_CLOTHING_Neck2			UMETA(DisplayName = "Neck 2"),				// Ties, Collars
	ITEM_CLOTHING_Torso1		UMETA(DisplayName = "Torso Layer 1"),		// T-shirts, Tank Tops
	ITEM_CLOTHING_Torso2		UMETA(DisplayName = "Torso Layer 2"),		// Button Shirts
	ITEM_CLOTHING_Torso3		UMETA(DisplayName = "Torso Layer 3"),		// Vests, Sweaters
	ITEM_CLOTHING_Torso4		UMETA(DisplayName = "Torso Layer 4"),		// Jackets, Coats
	ITEM_CLOTHING_Shoulders		UMETA(DisplayName = "Shoulders"),			// Capes
	ITEM_CLOTHING_ArmRight		UMETA(DisplayName = "Right Arm"),			// Right Arm Accessories
	ITEM_CLOTHING_ArmLeft		UMETA(DisplayName = "Left Arm"),			// Left Arm Accessories
	ITEM_CLOTHING_Wrists1		UMETA(DisplayName = "Wrists 1"),			// Wrists 1 (Quick Slot)
	ITEM_CLOTHING_Wrists2		UMETA(DisplayName = "Wrists 2"),			// Wrists 2 (Quick Slot)
	ITEM_CLOTHING_Hands			UMETA(DisplayName = "Hands"),				// Gloves
	ITEM_CLOTHING_Waist1		UMETA(DisplayName = "Waist 1"),				// Sashes and Belts (Quick Slot)
	ITEM_CLOTHING_Waist2		UMETA(DisplayName = "Waist 2"),				// Sashes and Belts (Quick Slot)
	ITEM_CLOTHING_Waist3		UMETA(DisplayName = "Waist 3"),				// Sashes and Belts (Quick Slot)
	ITEM_CLOTHING_Legs			UMETA(DisplayName = "Legs"),				// Pants
	ITEM_CLOTHING_Thighs1		UMETA(DisplayName = "Thighs 1"),			// Thighs (Quick Slot)
	ITEM_CLOTHING_Thighs2		UMETA(DisplayName = "Thighs 2"),			// Thighs (Quick Slot)
	ITEM_CLOTHING_Feet			UMETA(DisplayName = "Feet"),				// Shoes, Boots

	ITEM_ARMOR_Head				UMETA(DisplayName = "Head Armor"),			// Helmets, Visors
	ITEM_ARMOR_Torso			UMETA(DisplayName = "Power Armor"),			// Torso Power Armors
	ITEM_ARMOR_Shields			UMETA(DisplayName = "Shield Projectors"),	// Shield Projectors
	ITEM_ARMOR_ArmRight			UMETA(DisplayName = "Right Arm Armor"),		// Right Arm Power Armor
	ITEM_ARMOR_ArmLeft			UMETA(DisplayName = "Left Arm Armor"),		// Left Arm Power Armor
	ITEM_ARMOR_Hands			UMETA(DisplayName = "Hands Armor"),			// Power Gloves
	ITEM_ARMOR_Waist1			UMETA(DisplayName = "Waist Armor 1"),		// Power Belt (Quick Slot)
	ITEM_ARMOR_Waist2			UMETA(DisplayName = "Waist Armor 2"),		// Power Belt (Quick Slot)
	ITEM_ARMOR_Waist3			UMETA(DisplayName = "Waist Armor 3"),		// Power Belt (Quick Slot)
	ITEM_ARMOR_Legs				UMETA(DisplayName = "Legs Armor"),			// Legs Armor
	ITEM_ARMOR_Thighs1			UMETA(DisplayName = "Thighs Armor 1"),		// Thighs (Quick Slot)
	ITEM_ARMOR_Thighs2			UMETA(DisplayName = "Thighs Armor 2"),		// Thighs (Quick Slot)
	ITEM_ARMOR_Feet				UMETA(DisplayName = "Feet Armor"),			// Power/Mag Boots

	ITEM_IMPLANTS_Brain			UMETA(DisplayName = "Brain Implants"),		// Brain Implants
	ITEM_IMPLANTS_Eyes			UMETA(DisplayName = "Eye Implants"),		// Eye Implants
	ITEM_IMPLANTS_Skin			UMETA(DisplayName = "Skin Augmentation"),	// Skin Augmentation
	ITEM_IMPLANTS_Heart			UMETA(DisplayName = "Heart Implants"),		// Heart Implants
	ITEM_IMPLANTS_ArmRight		UMETA(DisplayName = "Right Bionic Arm"),	// Right Bionic Arm
	ITEM_IMPLANTS_ArmLeft		UMETA(DisplayName = "Left Bionic Arm"),		// Left Bionic Arm
	ITEM_IMPLANTS_LegRight		UMETA(DisplayName = "Right Bionic Leg"),	// Right Bionic Leg
	ITEM_IMPLANTS_LegLeft		UMETA(DisplayName = "Left Bionic Leg")		// Left Bionic Leg
};
