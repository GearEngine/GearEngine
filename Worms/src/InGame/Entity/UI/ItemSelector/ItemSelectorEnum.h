#pragma once

namespace InGame {

	namespace ItemSelectorInfo 
	{
		enum State : unsigned int 
		{
			OnNotActivate,

			OnUpdate,

			OnEmergy,
			OnSelect,
			OnSelectOnNotTurn,
			OnSink,

		};

		enum Stat : unsigned int
		{
			ItemList,
			CurrentTeam,
		};

		enum Size : unsigned int
		{
			_2, _3, _4, _5, _6, _7, _8,
			_9, _10, _11, _12, _13
		};
	}

}