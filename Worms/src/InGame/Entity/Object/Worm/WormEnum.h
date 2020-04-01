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
			OnReadyJump,
			OnReadyBackJump,
			OnJump,
			OnLand,
			OnAirItemUse,
			OnDamaged,
			OnDye,
			OnTurnOver,

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
			FireAngle,
			FirePower
		};
	}

}

