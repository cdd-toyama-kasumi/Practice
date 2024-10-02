#pragma once

inline FString Idle = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/idle_Anim.idle_Anim'");
inline FString Walk = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/walk_Anim.walk_Anim'");
inline FString JumpAnim = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/airi_Jump.airi_Jump'");
inline FString JumpLoopAnim = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/action_Anim.action_Anim'");
inline FString IdleActionAnim1 = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/idleAction_Anim1.idleAction_Anim1'");
inline FString IdleActionAnim2 = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/idleAction_Anim2.idleAction_Anim2'");
inline FString AnimInstance = TEXT("Class'/Game/Mine/Airi/BP_Anim_Airi.BP_Anim_Airi_C'");
inline FString Halo = TEXT("StaticMesh'/Game/Mine/Airi/StaticMesh/Airi_Halo.Airi_Halo'");
inline FString SkeletalMesh = TEXT("SkeletalMesh'/Game/Mine/Airi/Airi_Character.Airi_Character'");
inline FString EyeMouthMatIns = TEXT("MaterialInstanceConstant'/Game/Mine/Airi/Material/Airi_EyeMouth_Mat_Ins.Airi_EyeMouth_Mat_Ins'");


inline FName MouthInsParamUChannel = "UChannel";
inline FName MouthInsParamVChannel = "VChannel";


inline TMap<FString, FVector2D> MouthTypeMap =
{
	{"Mouth_0_0", FVector2D(0.0f, -3.0f)},
	{"Mouth_0_1", FVector2D(0.0f, -2.0f)},
	{"Mouth_0_2", FVector2D(0.0f, -1.0f)},
	{"Mouth_0_3", FVector2D(0.0f, 0.0f)},
	{"Mouth_0_4", FVector2D(0.0f, 1.0f)},
	{"Mouth_0_5", FVector2D(0.0f, 2.0f)},
	{"Mouth_0_6", FVector2D(0.0f, 3.0f)},
	{"Mouth_1_0", FVector2D(1.0f, -3.0f)},
	{"Mouth_1_1", FVector2D(1.0f, -2.0f)},
	{"Mouth_1_2", FVector2D(1.0f, -1.0f)},
	{"Mouth_1_3", FVector2D(1.0f, 0.0f)},
	{"Mouth_1_4", FVector2D(1.0f, 1.0f)},
	{"Mouth_1_5", FVector2D(1.0f, 2.0f)},
	{"Mouth_1_6", FVector2D(1.0f, 3.0f)},
	{"Mouth_2_0", FVector2D(2.0f, -3.0f)},
	{"Mouth_2_1", FVector2D(2.0f, -2.0f)},
	{"Mouth_2_2", FVector2D(2.0f, -1.0f)},
	{"Mouth_2_3", FVector2D(2.0f, 0.0f)},
	{"Mouth_2_4", FVector2D(2.0f, 1.0f)},
	{"Mouth_2_5", FVector2D(2.0f, 2.0f)},
	{"Mouth_2_6", FVector2D(2.0f, 3.0f)},
	{"Mouth_3_0", FVector2D(3.0f, -3.0f)},
	{"Mouth_3_1", FVector2D(3.0f, -2.0f)},
	{"Mouth_3_2", FVector2D(3.0f, -1.0f)},
	{"Mouth_3_3", FVector2D(3.0f, 0.0f)},
	{"Mouth_3_4", FVector2D(3.0f, 1.0f)},
	{"Mouth_3_5", FVector2D(3.0f, 2.0f)},
	{"Mouth_4_0", FVector2D(4.0f, -3.0f)},
	{"Mouth_4_1", FVector2D(4.0f, -2.0f)},
	{"Mouth_4_2", FVector2D(4.0f, -1.0f)},
	{"Mouth_4_3", FVector2D(4.0f, 0.0f)},
	{"Mouth_4_4", FVector2D(4.0f, 1.0f)},
	{"Mouth_4_5", FVector2D(4.0f, 2.0f)},
	{"Mouth_5_0", FVector2D(5.0f, -3.0f)},
	{"Mouth_5_1", FVector2D(5.0f, -2.0f)},
	{"Mouth_5_2", FVector2D(5.0f, -1.0f)},
	{"Mouth_5_3", FVector2D(5.0f, 0.0f)},
	{"Mouth_5_4", FVector2D(5.0f, 1.0f)},
	{"Mouth_5_5", FVector2D(5.0f, 2.0f)},
	{"Mouth_6_0", FVector2D(6.0f, -3.0f)},
	{"Mouth_6_1", FVector2D(6.0f, -2.0f)},
	{"Mouth_6_2", FVector2D(6.0f, -1.0f)},
	{"Mouth_6_3", FVector2D(6.0f, 0.0f)},
	{"Mouth_6_4", FVector2D(6.0f, 1.0f)},
	{"Mouth_6_5", FVector2D(6.0f, 2.0f)},
	{"Mouth_7_0", FVector2D(7.0f, -3.0f)},
	{"Mouth_7_1", FVector2D(7.0f, -2.0f)},
	{"Mouth_7_2", FVector2D(7.0f, -1.0f)},
	{"Mouth_7_3", FVector2D(7.0f, 0.0f)},
	{"Mouth_7_4", FVector2D(7.0f, 1.0f)},
	{"Mouth_7_5", FVector2D(7.0f, 2.0f)},
};