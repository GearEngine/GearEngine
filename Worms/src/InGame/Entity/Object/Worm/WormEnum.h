#pragma once

namespace InGame {

	namespace WormState {

		enum : unsigned int
		{
			OnNotMyTurn,
			OnReady,
			OnWaiting,
			OnStart,
			OnMove,
			OnBreath,
			OnReadyItemUse,
			OnUseItem,
			OnAir,
			OnJump,
			OnReadyJump,
			OnReadyBackJump,
			OnReadyLand,
			OnLand,
			OnFallen,
			OnReadyFallen,
			OnSliding,
			OnStuck,
			OnStandUp,
			OnAirItemUse,
			OnDamaged,
			OnDye,
			OnTurnOver,
			OnUnderWater,
			OnAfterDamaged,
			OnNothing,

			OnLeftFlatMove, OnRightFlatMove,
			OnLeftUpMove, OnRightUpMove,
			OnLeftDownMove, OnRightDownMove,

			OnLeftFlatBreath, OnRightFlatBreath,
			OnLeftUpBreath, OnRightUpBreath,
			OnLeftDownBreath, OnRightDownBreath,

			OnLeftJump, OnRightJump,
			OnRightBackJump, OnLeftBackJump,
			OnRightBackFlip, OnLeftBackFlip,

			OnLeftFlatJumpReady, OnRightFlatJumpReady,
			OnLeftUpJumpReady, OnRightUpJumpReady,
			OnLeftDownJumpReady, OnRightDownJumpReady,

			OnLeftFlatLand, OnRightFlatLand,
			OnLeftUpLand, OnRightUpLand,
			OnLeftDownLand, OnRightDownLand,

			OnLeftFlatSliding, OnRightFlatSliding,
			OnLeftUpSliding, OnRightUpSliding,
			OnLeftDownSliding, OnRightDownSliding,

			OnLeftFlatAfterSliding, OnRightFlatAfterSliding,
			OnLeftUpAfterSliding, OnRightUpAfterSliding,
			OnLeftDownAfterSliding, OnRightDownAfterSliding,

			OnLeftFlatAfterDamagedSliding, OnRightFlatAfterDamagedSliding,
			OnLeftUpAfterDamagedSliding, OnRightUpAfterDamagedSliding,
			OnLeftDownAfterDamagedSliding, OnRightDownAfterDamagedSliding,

			OnLeftFlatStuck, OnRightFlatStuck,
			OnLeftUpStuck, OnRightUpStuck,
			OnLeftDownStuck, OnRightDownStuck
		};
	}

	namespace WormCommand {

		enum : unsigned int
		{
			BackJump,
			Jump,
			Left,
			Right,
			Up,
			Down,
			UseItem,
			SetTimer1,
			SetTimer2,
			SetTimer3,
			SetTimer4,
			SetTimer5,
			ChangeWorm,
		};
	}

	namespace WormStatusHandleType {

		enum : unsigned int
		{
			Display,
			WaitingDisplay,
			DisplayPosChange,
			Damaged,
			DisplayDamage,
			FireAngle,
			FirePower,
		};
	}

}

