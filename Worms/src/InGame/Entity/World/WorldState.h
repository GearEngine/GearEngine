#pragma once

namespace InGame {

	namespace WorldState {

		enum : unsigned int
		{
			OnStart,
			OnRunning,
			OnWormsDying,
			OnPrepareRun,
			OnWindowSelect,
			OnItemWindow,
			OnQuitWindow,
			OnPrepareNextPhase,
			OnWaiting,
			InGameStart
		};

	}

	namespace WorldInfo {

		enum : unsigned int
		{
			CurrnetTeam,
			CurrentWorm,
			CurrentTeamColor,

			WaitingTimeBorderOffset,
			TeamInfo,
			TeamInfoBlink,
			DyeInfo
		};
	}

	namespace WorldStatusHandleType	{

		enum : unsigned int
		{
			DisplayWaitingCount,
			DisplayTeamInfo,
			DisplayMassage
		};

	}

}