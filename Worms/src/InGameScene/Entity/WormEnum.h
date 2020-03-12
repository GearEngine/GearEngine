#pragma once

namespace WormState {

	enum : unsigned int
	{
		OnMove,
		OnDye,
		OnIdle,
		OnReadyItemUse,
		OnUseItem,
		OnAir,
		OnReadyJump,
		OnAirItemUse,
		OnTurnOver,
		OnDamaged
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
		ChangeWorm
	};
}
