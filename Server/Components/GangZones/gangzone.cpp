#include "gangzone.hpp"

struct GangZonesComponent final : public IGangZonesComponent {
	ICore * core;
	MarkedPoolStorage<GangZone, IGangZone, IGangZonesComponent::Cnt> storage;
	DefaultEventDispatcher<GangZoneEventHandler> eventDispatcher;

	StringView componentName() override {
		return "GangZones";
	}

	void onLoad(ICore * core) override {
		this->core = core;
	}

	IGangZone* create(GangZonePos pos) override {
		int freeIdx = storage.findFreeIndex();
		if (freeIdx == -1) {
			// No free index
			return nullptr;
		}

		int pid = storage.claim(freeIdx);
		if (pid == -1) {
			// No free index
			return nullptr;
		}

		GangZone& gangZone = storage.get(pid);
		gangZone.pos = pos;
		return &gangZone;
	}

	void free() override {
		delete this;
	}

	int findFreeIndex() override {
		return storage.findFreeIndex();
	}

	int claim() override {
		int res = storage.claim();
		return res;
	}

	int claim(int hint) override {
		int res = storage.claim(hint);
		return res;
	}

	bool valid(int index) const override {
		return storage.valid(index);
	}

	IGangZone& get(int index) override {
		return storage.get(index);
	}

	void release(int index) override {
		storage.release(index, false);
	}

	void lock(int index) override {
		storage.lock(index);
	}

	void unlock(int index) override {
		storage.unlock(index);
	}

	IEventDispatcher<GangZoneEventHandler> & getEventDispatcher() override {
		return eventDispatcher;
	}

	/// Get a set of all the available labels
	const FlatPtrHashSet<IGangZone>& entries() override {
		return storage.entries();
	}
};

COMPONENT_ENTRY_POINT() {
	return new GangZonesComponent();
}