#pragma once

#include <string>

class aiScene;

enum ExportFileType : short
{
	FBX
};

/**
 * @interface IExporter
 * Base interface for any mesh exporter
 */
class IExporter
{
public:
	IExporter(ExportFileType inExportType);
	~IExporter();

	/**
	 * Handles exporting the mesh
	 * 
	 * Call IExporter::Export to actually export the mesh in your
	 * overridden function.
	 */
	virtual bool Export(const std::string& outPath);

protected:
	aiScene* exporterScene;

private:
	ExportFileType exportType;
};