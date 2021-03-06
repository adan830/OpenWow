#pragma once

#include "WDL_LowResTile.h"
#include "ADT_WMO_Instance.h"

// FORWARD BEGIN
class MapController;
// FORWARD END

class WDL
{
public:
	WDL(MapController* _mapController);
	virtual ~WDL();

	void CreateInsances(MapController* _parent);
	void Load();

	SharedTexturePtr getMinimap() { return m_Minimap; }

private:
	SharedTexturePtr					m_Minimap;
	uint32								m_MAREOffsets[C_TilesInMap][C_TilesInMap];
	vector<std::shared_ptr<CWDL_LowResTile>>	m_LowResilutionTiles;
	vector<string>						m_LowResolutionWMOsNames;
	vector<ADT_MODF>					m_LowResolutionWMOsPlacementInfo;
	vector<std::shared_ptr<ADT_WMO_Instance>>	m_LowResolutionWMOs;

private: // PARENT
	const MapController*				m_MapController;
};