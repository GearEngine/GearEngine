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
			OnItemWithdraw,
			OnUseItem,
			OnAir,
			OnAttacked,
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

			OnLeftFlatBazukaReady, OnRightFlatBazukaReady,
			OnLeftUpBazukaReady, OnRightUpBazukaReady,
			OnLeftDownBazukaReady, OnRightDownBazukaReady,

			OnLeftFlatBazukaWithdraw, OnRightFlatBazukaWithdraw,
			OnLeftUpBazukaWithdraw, OnRightUpBazukaWithdraw,
			OnLeftDownBazukaWithdraw, OnRightDownBazukaWithdraw,

			OnLeftFlatBazukaOn, OnRightFlatBazukaOn,
			OnLeftUpBazukaOn, OnRightUpBazukaOn,
			OnLeftDownBazukaOn, OnRightDownBazukaOn,

			OnLeftFlatStuck, OnRightFlatStuck,
			OnLeftUpStuck, OnRightUpStuck,
			OnLeftDownStuck, OnRightDownStuck,

			OnLeftFly, OnRightFly,
			OnLeftDying, OnRightDying
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
			LeftMouseClick,
			RightMouseClick
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
			DisplayAim,
		};
	}

}

