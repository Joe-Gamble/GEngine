#pragma once
#ifndef NET_COMPONENT
#define NET_COMPONENT

#include "Component.h"

namespace GEngine
{
	struct NetComponent : Component
	{
	public:
		inline bool HasPendingData() { return m_hasDataToBeSent; }
		virtual bool ForcePacketOnChange() { return false; }
		inline virtual void SetCycles(int cycles) { m_requiredCycles = cycles; }
		bool HasReachedCycles();

		void Update(double& dt) override;
	private:
		bool m_hasDataToBeSent = false;

		int m_requiredCycles = 1; // every 60 ticks
		int m_cyclesPassed = 0;
	};
}

#endif

