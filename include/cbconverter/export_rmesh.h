#pragma once

#include "cbconverter/exporter.h"	

#include <cbreader/rmesh.h>

#include <map>

class aiMaterial;

/**
 * @class ExportRMesh
 * 
 * @todo Add checks where needed
 * @todo Allow for collision meshes to be exported, requires altering the scene meshes count
 * @todo Add lightmaps
 * @todo Add entity exporting
 */
class ExportRMesh : public IExporter
{
public:
	ExportRMesh(rmesh::RMesh& inRMesh, ExportFileType inExportType);
	~ExportRMesh();

	bool Export(const std::string& outPath) override;

private:
	rmesh::RMesh rmeshFile;

	void AddMaterials();
	bool AddMaterial(const std::string& texturePath, int materialIndex, int surfaceIndex);

	void AddDrawnMesh();
	void AddSurface(rmesh::Surface& surface, std::string meshName, int surfaceIndex);

	void AddCollisionMesh();
	void AddEntities();

	/** <name, <index, material>> */
	std::map<std::string, std::pair<aiMaterial*, int>> materialMap;
};