#pragma once


namespace Gear {

	// °üÂûÀÚ
	class Observer
	{
	public:
		virtual ~Observer() = default;

		virtual void Update(Observable& observable, const void* dataKey) = 0;
	};

	class Observable
	{
	public:
		virtual ~Observable() = 0;

		void Attach(Observer& observer)
		{
			observerSet_.insert(&observer);
		}

		void Detach(Observer& observer)
		{
			observerSet_.erase(&observer);
		}

		void Notify(const void* dataKey)
		{
			for (auto& observer : observerSet_)
			{
				observer->Update(*this, dataKey);
			}
		}

	private:
		std::set<Observer*> observerSet_;
	};

	

}