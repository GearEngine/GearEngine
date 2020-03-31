#include "WormEventHandle.h"

namespace InGame {

	class WormOnLeftMoveHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			/*static unsigned int prevState = -1;
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			int PCResult = physics->GetLastPixelCollisionResult();

			PCResult &= 0b11;
			unsigned int state = prevState;
			switch (PCResult)
			{
			case 0b11:
				state = WormState::OnLeftFlatMove;
				break;
			case 0b01:
				state = WormState::OnLeftUpMove;
				break;
			case 0b10:
				state = WormState::OnLeftDownMove;
				break;
			}
			if (prevState != state)
			{
				animator->PlayAnimation(state);
			}
			prevState = state;

			GR_TRACE("Left move state : {0}", state);

			if (cmd.KeyType == WormCommand::Left)
			{
				physics->SetExternalVector({ -1.0f, 0.0f });
				return state;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetExternalVector({ 1.0f, 0.0f });
				switch (state)
				{
				case WormState::OnLeftFlatMove:
					state = WormState::OnRightFlatMove;
					break;
				case WormState::OnLeftUpMove:
					state = WormState::OnRightDownMove;
					break;
				case WormState::OnLeftDownMove:
					state = WormState::OnRightUpMove;
					break;
				}
				prevState = -1;
				physics->ActivatePixelOffset("RightMove");
				return state;
			}
			state += 6;
			prevState = -1;
			physics->ActivatePixelOffset("Generall");
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });*/
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnRightMoveHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			/*static unsigned int prevState = -1;
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			int PCResult = physics->GetLastPixelCollisionResult();

			PCResult &= 0b11;
			unsigned int state = prevState;
			switch (PCResult)
			{
			case 0b11:
				state = WormState::OnRightFlatMove;
				break;
			case 0b10:
				state = WormState::OnRightUpMove;
				break;
			case 0b01:
				state = WormState::OnRightDownMove;
				break;
			}
			if (prevState != state)
			{
				animator->PlayAnimation(state);
			}
			prevState = state;

			if (cmd.KeyType == WormCommand::Left)
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ -1.0f, 0.0f });
				switch (state)
				{
				case WormState::OnRightFlatMove:
					state = WormState::OnLeftFlatMove;
					break;
				case WormState::OnRightUpMove:
					state = WormState::OnLeftDownMove;
					break;
				case WormState::OnRightDownMove:
					state = WormState::OnLeftUpMove;
					break;
				}
				prevState = -1;
				physics->ActivatePixelOffset("LeftMove");
				return state;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetExternalVector({ 1.0f, 0.0f });
				return state;
			}
			state += 6;
			prevState = -1;
			physics->ActivatePixelOffset("Generall");
			physics->SetExternalVector({ 0.0f, 0.0f });*/
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnDyeHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnDye;
		}
	};

	class WormOnAirHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnAir;
		}
	};

	class WormOnLeftBreathHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			/*static unsigned prevState = -1;
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			int PCResult = physics->GetLastPixelCollisionResult();

			PCResult &= 0b11;
			unsigned int state = prevState;
			state = WormState::OnLeftFlatBreath;
			switch (PCResult)
			{
			case 0b11:
				state = WormState::OnLeftFlatBreath;
				break;
			case 0b01:
				state = WormState::OnLeftUpBreath;
				break;
			case 0b10:
				state = WormState::OnLeftDownBreath;
				break;
			}
			if (prevState != state)
			{
				animator->PlayAnimation(state);
			}
			prevState = state;

			if (cmd.KeyType == WormCommand::Left)
			{
				state -= 6;
				physics->ActivatePixelOffset("LeftMove");
				prevState = -1;
				return state;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				switch (state)
				{
				case WormState::OnLeftFlatBreath:
					state = WormState::OnRightFlatMove;
					break;
				case WormState::OnLeftUpBreath:
					state = WormState::OnRightDownMove;
					break;
				case WormState::OnLeftDownBreath:
					state = WormState::OnRightUpMove;
					break;
				}
				physics->ActivatePixelOffset("RightMove");
				prevState = -1;
				return state;
			}
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });*/
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnRightBreathHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			/*static unsigned prevState = -1;
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			int PCResult = physics->GetLastPixelCollisionResult();

			PCResult &= 0b11;
			unsigned int state = WormState::OnRightFlatBreath;
			switch (PCResult)
			{
			case 0b11:
				state = WormState::OnRightFlatBreath;
				break;
			case 0b01:
				state = WormState::OnRightDownBreath;
				break;
			case 0b10:
				state = WormState::OnRightUpBreath;
				break;
			}
			if (prevState != state)
			{
				animator->PlayAnimation(state);
			}
			prevState = state;

			if (cmd.KeyType == WormCommand::Left)
			{
				switch (state)
				{
				case WormState::OnRightFlatBreath:
					state = WormState::OnLeftFlatMove;
					break;
				case WormState::OnRightUpBreath:
					state = WormState::OnLeftDownMove;
					break;
				case WormState::OnRightDownBreath:
					state = WormState::OnLeftUpMove;
					break;
				}
				physics->ActivatePixelOffset("LeftMove");
				prevState = -1;
				return state;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				state -= 6;
				physics->ActivatePixelOffset("RightMove");
				prevState = -1;
				return state;
			}
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });*/
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnReadyItemUseHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnUseItemHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnReadyJumpHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnAirItemUseHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnTurnOverHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			physics->SetExternalVector({ 0.0f, 0.0f });
			physics->SetPixelCollisionHandler("Generall");
			Gear::EntitySystem::GetStatus(entityID)->PopNeedHandleData(WormStatusHandleType::WaitingDisplay);

			return WormState::OnNotMyTurn;
		}
	};

	class WormOnDamagedHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnDamaged;
		}
	};

	class WormOnReadyHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			return WormState::OnReady;
		}
	};

	class WormOnWaitingHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			if (cmd.Keycode != 0xffffffb)
			{
				auto status = Gear::EntitySystem::GetStatus(entityID);
				status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(0.5f));
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::RunningStart, 0, entityID)));
			}
			if (cmd.KeyType == WormCommand::Left)
			{
				physics->SetPixelCollisionHandler("LeftMove");
				return WormState::OnLeftFlatMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetPixelCollisionHandler("LeftMove");
				return WormState::OnRightFlatMove;
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				return WormState::OnReadyJump;
			}
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			return WormState::OnWaiting;
		}
	};

	class WormOnNotMyTurnHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
			return WormState::OnNotMyTurn;
		}
	};

}
