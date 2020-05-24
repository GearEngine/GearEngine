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
			OnVictory,
			OnAfterUseItem,
			OnUseShotgun,
			OnUseWindowPickItem,

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

			OnLeftFlatGrenadeReady, OnRightFlatGrenadeReady,
			OnLeftUpGrenadeReady, OnRightUpGrenadeReady,
			OnLeftDownGrenadeReady, OnRightDownGrenadeReady,

			OnLeftFlatGrenadeWithdraw, OnRightFlatGrenadeWithdraw,
			OnLeftUpGrenadeWithdraw, OnRightUpGrenadeWithdraw,
			OnLeftDownGrenadeWithdraw, OnRightDownGrenadeWithdraw,

			OnLeftFlatGrenadeOn, OnRightFlatGrenadeOn,
			OnLeftUpGrenadeOn, OnRightUpGrenadeOn,
			OnLeftDownGrenadeOn, OnRightDownGrenadeOn,

			OnLeftFlatBananaReady, OnRightFlatBananaReady,
			OnLeftUpBananaReady, OnRightUpBananaReady,
			OnLeftDownBananaReady, OnRightDownBananaReady,

			OnLeftFlatBananaWithdraw, OnRightFlatBananaWithdraw,
			OnLeftUpBananaWithdraw, OnRightUpBananaWithdraw,
			OnLeftDownBananaWithdraw, OnRightDownBananaWithdraw,

			OnLeftFlatBananaOn, OnRightFlatBananaOn,
			OnLeftUpBananaOn, OnRightUpBananaOn,
			OnLeftDownBananaOn, OnRightDownBananaOn,

			OnLeftFlatSkipGoOn, OnRightFlatSkipGoOn,
			OnLeftUpSkipGoOn, OnRightUpSkipGoOn,
			OnLeftDownSkipGoOn, OnRightDownSkipGoOn,

			OnLeftFlatSkipGoReady, OnRightFlatSkipGoReady,
			OnLeftUpSkipGoReady, OnRightUpSkipGoReady,
			OnLeftDownSkipGoReady, OnRightDownSkipGoReady,

			OnLeftFlatSurrenderOn, OnRightFlatSurrenderOn,
			OnLeftUpSurrenderOn, OnRightUpSurrenderOn,
			OnLeftDownSurrenderOn, OnRightDownSurrenderOn,

			OnLeftFlatSurrenderReady, OnRightFlatSurrenderReady,
			OnLeftUpSurrenderReady, OnRightUpSurrenderReady,
			OnLeftDownSurrenderReady, OnRightDownSurrenderReady,

			OnLeftFlatTeleportOn, OnRightFlatTeleportOn,
			OnLeftUpTeleportOn, OnRightUpTeleportOn,
			OnLeftDownTeleportOn, OnRightDownTeleportOn,

			OnLeftFlatTeleportWithraw, OnRightFlatTeleportWithraw,
			OnLeftUpTeleportWithraw, OnRightUpTeleportWithraw,
			OnLeftDownTeleportWithraw, OnRightDownTeleportWithraw,

			OnLeftFlatTeleportReady, OnRightFlatTeleportReady,
			OnLeftUpTeleportReady, OnRightUpTeleportReady,
			OnLeftDownTeleportReady, OnRightDownTeleportReady,

			OnLeftFlatTeleportUse, OnRightFlatTeleportUse,
			OnLeftUpTeleportUse, OnRightUpTeleportUse,
			OnLeftDownTeleportUse, OnRightDownTeleportUse,

			OnLeftFlatTeleportAfterUse, OnRightFlatTeleportAfterUse,
			OnLeftUpTeleportAfterUse, OnRightUpTeleportAfterUse,
			OnLeftDownTeleportAfterUse, OnRightDownTeleportAfterUse,

			OnLeftFlatShotgunOn1, OnRightFlatShotgunOn1,
			OnLeftUpShotgunOn1, OnRightUpShotgunOn1,
			OnLeftDownShotgunOn1, OnRightDownShotgunOn1,

			OnLeftFlatShotgunOn2, OnRightFlatShotgunOn2,
			OnLeftUpShotgunOn2, OnRightUpShotgunOn2,
			OnLeftDownShotgunOn2, OnRightDownShotgunOn2,

			OnLeftFlatShotgunReady, OnRightFlatShotgunReady,
			OnLeftUpShotgunReady, OnRightUpShotgunReady,
			OnLeftDownShotgunReady, OnRightDownShotgunReady,

			OnLeftFlatShotgunWithraw, OnRightFlatShotgunWithraw,
			OnLeftUpShotgunWithraw, OnRightUpShotgunWithraw,
			OnLeftDownShotgunWithraw, OnRightDownShotgunWithraw,

			OnLeftFlatShotgunShot, OnRightFlatShotgunShot,
			OnLeftUpShotgunShot, OnRightUpShotgunShot,
			OnLeftDownShotgunShot, OnRightDownShotgunShot,
			
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
			AfterUseItem,
			SkipGo,
			Surrender,
			Teleport
		};
	}

}

