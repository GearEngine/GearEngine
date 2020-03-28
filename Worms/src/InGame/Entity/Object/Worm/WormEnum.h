#pragma once

namespace InGame {

	namespace WormState {

		enum : unsigned int
		{
			OnNotMyTurn,
			OnReady,
			OnStart,
			OnMove,
			OnIdle,
			OnReadyItemUse,
			OnUseItem,
			OnAir,
			OnReadyJump,
			OnAirItemUse,
			OnDamaged,
			OnDye,
			OnTurnOver
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
			DisplayName,
			DisplayHp,
			Damaged,
			FireAngle,
			FirePower
		};
	}

}

