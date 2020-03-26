#include "Gear.h"

class SampleScene : public Gear::Scene
{
public:
	SampleScene(const std::string& name)
		: Gear::Scene(name)
	{

	}

	virtual void Update(Gear::Timestep ts) override;

};