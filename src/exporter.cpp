#include "cbconverter/exporter.h"

#include <spdlog/spdlog.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Exporter.hpp>

IExporter::IExporter(ExportFileType inExportType)
	: exportType(inExportType)
{
	exporterScene = new aiScene();
	exporterScene->mRootNode = new aiNode();
}

IExporter::~IExporter()
{
	delete exporterScene;
}

bool IExporter::Export(const std::string& outPath)
{
	std::string outType;
	switch (exportType)
	{
	case ExportFileType::FBX:
		outType = "fbx";
		break;
	default:
		spdlog::error("Failed to determine exportType!");
		return false;
	}

	// With assimp, 0 is success, 1 is failure
	Assimp::Exporter exporter;
	bool ret = !exporter.Export(exporterScene, outType, outPath, aiProcess_FlipUVs, nullptr);

	if (!ret)
	{
		spdlog::error("Assimp export error: {}", exporter.GetErrorString());
	}

	return ret;
}

