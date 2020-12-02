#include "gameplay/plight/tilemap.h"
#include "core/system.h"

using namespace dagger;

class TilemapSystem : public System, public Subscriber<TilemapLoadRequest> {

public :
	void SpinUp() override;
	void WindDown() override;

	String SystemName() override {
		return "Plight Tilemap System";
	}

	void OnLoadAsset(TilemapLoadRequest request_);
};