#include "stdafx.h"

// General
#include "WMO.h"

struct WMO_GroupInfoDef
{
	WMOGroupFlags flags;
	CAaBox bounding_box;
	int32 nameoffset;                                   // name in MOGN chunk (-1 for no name)
};

WMO::WMO(cstring name) :
	m_FileName(name),

	m_TexturesNames(nullptr),
	m_GroupsNames(nullptr),
	m_Skybox_Filename(nullptr),
	m_M2Filenames(nullptr)

{}

WMO::~WMO()
{
	Log::Info("WMO[%s]: Unloading...", m_FileName.c_str());

	//

	if (m_TexturesNames) delete[] m_TexturesNames;
	if (m_GroupsNames) delete[] m_GroupsNames;
	if (m_Skybox_Filename) delete[] m_Skybox_Filename;
}

void WMO::CreateInsances(SceneNode* _parent)
{
	for (auto& it : m_Groups)
	{
		it->CreateInsances(_parent);
	}

	for (auto& it : m_M2PlacementInfos)
	{
		SmartPtr<M2> mdx = (M2*)GetManager<IM2Manager>()->Add(m_M2Filenames + it.flags.nameIndex);
		if (mdx)
		{
			new WMO_MODD_Instance(_parent, mdx, it);
		}
	}
}

bool WMO::Load()
{
	UniquePtr<IFile> f = GetManager<IFilesManager>()->Open(m_FileName);
	if (f == nullptr)
	{
		return false;
	}

	char fourcc[5];
	uint32 size;
	while (!f->IsEof())
	{
		memset(fourcc, 0, 4);
		f->ReadBytes(fourcc, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		size = 0;
		f->ReadBytes(&size, 4);
		if (size == 0) continue;
		uint32_t nextpos = f->GetPos() + size;

		if (strcmp(fourcc, "MVER") == 0)                    // Version
		{
			uint32 version;
			f->ReadBytes(&version, 4);
			assert1(version == 17);
		}
		else if (strcmp(fourcc, "MOHD") == 0)               // Header
		{
			f->ReadBytes(&m_Header, sizeof(WMO_HeaderDef));

			m_Bounds.set(m_Header.bounding_box.min, m_Header.bounding_box.max, true);
		}
		else if (strcmp(fourcc, "MOTX") == 0)               // List of m_DiffuseTextures (BLP Files) used in this map object.
		{
			m_TexturesNames = new char[size + 1];
			f->ReadBytes(m_TexturesNames, size);
			m_TexturesNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOMT") == 0)               // Materials used in this map object, 64 bytes per texture (BLP file), nMaterials entries.
		{
			uint32 materialsCount = size / sizeof(WMO_MaterialDef);
			WMO_MaterialDef* materials = (WMO_MaterialDef*)f->GetDataFromCurrent();
			assert1(materialsCount == m_Header.nTextures);
			for (uint32 i = 0; i < materialsCount; i++)
			{
				SmartPtr<WMO_Part_Material> _mat = new WMO_Part_Material(this, materials[i]);
				m_Materials.push_back(_mat);
			}
		}
		else if (strcmp(fourcc, "MOGN") == 0)              // List of group names for the m_Groups in this map object.
		{
			m_GroupsNames = new char[size + 1];
			f->ReadBytes(m_GroupsNames, size);
			m_GroupsNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOGI") == 0)
		{
			uint32 groupInfosCount = size / sizeof(WMO_GroupInfoDef);
			WMO_GroupInfoDef* groupInfos = (WMO_GroupInfoDef*)f->GetDataFromCurrent();
			assert1(groupInfosCount == m_Header.nGroups);
			for (uint32 i = 0; i < groupInfosCount; i++)
			{
				char temp[256];
				strcpy_s(temp, m_FileName.c_str());
				temp[m_FileName.length() - 4] = 0;

				char fname[256];
				sprintf_s(fname, "%s_%03d.wmo", temp, i);
				SmartPtr<IFile> groupFile = GetManager<IFilesManager>()->Open(fname);

				string groupName = groupFile->Name();
				if (groupInfos[i].nameoffset > 0)
				{
					groupName = string(m_GroupsNames + groupInfos[i].nameoffset);
				}

				WMO_Group* group = new WMO_Group(this, i, groupName, groupFile);
				m_Groups.push_back(group);
			}
		}
		else if (strcmp(fourcc, "MOSB") == 0) // Skybox. 
		{
			if (size > 4)
			{
				m_Skybox_Filename = new char[size + 1];
				f->ReadBytes(m_Skybox_Filename, size);
				m_Skybox_Filename[size] = 0x00;
				Log::Error("WMO[%s]: Skybox [%s]", m_FileName.c_str(), m_Skybox_Filename);
				//m_Skybox = _World->LoadMDX(m_Skybox_Filename);
			}
		}
		else if (strcmp(fourcc, "MOPV") == 0)
		{
			uint32 portalVertexesCount = size / sizeof(vec3);
			vec3* portalVertexes = (vec3*)f->GetDataFromCurrent();
			for (uint32 i = 0; i < portalVertexesCount; i++)
			{
				m_PortalVertices.push_back(portalVertexes[i].toXZmY());
			}

			m_PortalVB = _Render->r.createVertexBuffer(m_PortalVertices.size() * sizeof(vec3), m_PortalVertices.data(), false);
		}
		else if (strcmp(fourcc, "MOPT") == 0)
		{
			uint32 portalDefsCount = size / sizeof(WMO_PortalDef);
			WMO_PortalDef* portalDefs = (WMO_PortalDef*)f->GetDataFromCurrent();
			assert1(portalDefsCount == m_Header.nPortals);
			for (uint32 i = 0; i < portalDefsCount; i++)
			{
				m_PortalInformation.push_back(portalDefs[i]);

				CWMO_Part_Portal portal(this, portalDefs[i]);
				m_Portals.push_back(portal);
			}
		}
		else if (strcmp(fourcc, "MOPR") == 0)
		{
			uint32 portalReferencesCount = size / sizeof(WMO_PortalReferencesDef);
			WMO_PortalReferencesDef* portalReferences = (WMO_PortalReferencesDef*)f->GetDataFromCurrent();
			for (uint32 i = 0; i < portalReferencesCount; i++)
			{
				m_PortalReferences.push_back(portalReferences[i]);
			}
		}
		else if (strcmp(fourcc, "MOVV") == 0)
		{
			uint32 visibleVertexesCount = size / sizeof(vec3);
			vec3* visibleVertexes = (vec3*)f->GetDataFromCurrent();
			for (uint32 i = 0; i < visibleVertexesCount; i++)
			{
				m_VisibleBlockVertices.push_back(visibleVertexes[i].toXZmY());
			}
		}
		else if (strcmp(fourcc, "MOVB") == 0)
		{
			uint32 visibleBlockListsCount = size / sizeof(WMO_VisibleBlockListDef);
			WMO_VisibleBlockListDef* visibleBlockLists = (WMO_VisibleBlockListDef*)f->GetDataFromCurrent();
			for (uint32 i = 0; i < visibleBlockListsCount; i++)
			{
				m_VisibleBlockList.push_back(visibleBlockLists[i]);
			}
		}
		else if (strcmp(fourcc, "MOLT") == 0)
		{
			uint32 lightsCount = size / sizeof(WMO_LightDef);
			WMO_LightDef* lights = (WMO_LightDef*)f->GetDataFromCurrent();
			assert1(lightsCount == m_Header.nLights);
			for (uint32 i = 0; i < lightsCount; i++)
			{
				SmartPtr<WMO_Part_Light> _wmoLight = new WMO_Part_Light(lights[i]);
				m_Lights.push_back(_wmoLight);
			}
		}
		else if (strcmp(fourcc, "MODS") == 0)
		{
			uint32 doodadsSetsCount = size / sizeof(WMO_MODD_SetInfo);
			WMO_MODD_SetInfo* doodadsSets = (WMO_MODD_SetInfo*)f->GetDataFromCurrent();
			assert1(doodadsSetsCount == m_Header.nDoodadSets);
			for (uint32 i = 0; i < doodadsSetsCount; i++)
			{
				m_M2SetInfos.push_back(doodadsSets[i]);
			}
		}
		else if (strcmp(fourcc, "MODN") == 0) // List of filenames for M2 (mdx) models that appear in this WMO.
		{
			m_M2Filenames = (char*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MODD") == 0) // Information for doodad instances. 40 bytes per doodad instance, nDoodads entries.
		{
			uint32 doodadsPlacementsCount = size / sizeof(WMO_MODD_PlacementInfo);
			WMO_MODD_PlacementInfo* doodadsPlacements = (WMO_MODD_PlacementInfo*)f->GetDataFromCurrent();
			for (uint32 i = 0; i < doodadsPlacementsCount; i++)
			{
				m_M2PlacementInfos.push_back(doodadsPlacements[i]);
			}

			m_Header.nDoodadDefs = doodadsPlacementsCount; // HACK! INCORRECT SIZE
		}
		else if (strcmp(fourcc, "MFOG") == 0)
		{
			uint32 fogsCount = size / sizeof(WMO_FogDef);
			WMO_FogDef* fogs = (WMO_FogDef*)f->GetDataFromCurrent();
			for (uint32 i = 0; i < fogsCount; i++)
			{
				SmartPtr<WMO_Part_Fog> fog = new WMO_Part_Fog(fogs[i]);
				m_Fogs.push_back(fog);
			}
		}
		else if (strcmp(fourcc, "MCVP") == 0)
		{
			//Convex Volume Planes.Contains blocks of floating - point numbers.
			// C4Plane convex_volume_planes[];   // normal points out
		}
		else
		{
			Log::Fatal("WMO[%s]: No implement wmo chunk [%s] [%d].", m_FileName.c_str(), fourcc, size);
		}

		f->Seek(nextpos);
	}	

	// Init m_Groups
	for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
	{
		(*it)->Load();
	}

	return true;
}

//

bool WMO::Render(cmat4 _worldMatrix, uint32 _doodadSet)
{
	for (auto& it : m_Portals)
	{
		it.Render(_worldMatrix);
	}

	PERF_START(PERF_MAP_MODELS_WMOs_GEOMETRY);
	for (auto& it : m_Groups)
	{
		it->Render(_worldMatrix);
	}
	PERF_STOP(PERF_MAP_MODELS_WMOs_GEOMETRY);

	return true;
}

bool WMO::drawSkybox()
{
	if (m_Skybox == nullptr)
	{
		return false;
	}

	mat4 worldMatrix;
	worldMatrix.translate(_Camera->Position);
	worldMatrix.scale(2.0f);

	m_Skybox->Render(worldMatrix);

	//_World->EnvM()->m_HasSky = true;

	return true;
}