#include "cbconverter/export_rmesh.h"

#include <spdlog/spdlog.h>
#include <argparse/argparse.hpp>

#include <string>
#include <filesystem>

/**
* @todo Handle other export types, like gltf or similar?
*/
int main(int argc, char** argv)
{
	// Setup Logging
	{
		// Remove the time from spdlog
		spdlog::set_pattern("[%^%l%$] %v");

#ifdef _DEBUG
		spdlog::set_level(spdlog::level::debug);
#endif
	}


	// Setup arg parse
	argparse::ArgumentParser program("cbconverter");
	{
		program.add_description("Takes a file that is supported and converts the mesh to a fbx file.\n"
			"Supported formats:\n"
			"  - Blitz3D (.b3d)\n"
			"  - Room Mesh (.rmesh)"
		);

		program.add_argument("<path>")
			.help("Path to a supported model file")
			.required();

		program.add_argument("-o", "--output")
			.help("Directory where the fbx will be output.");
	}

	try
	{
		program.parse_args(argc, argv);

		std::string path = program.get("<path>");
		std::string outputPath;
		if (program.present("-o"))
		{
			outputPath = program.get("-o");
		}
		else
		{
			std::filesystem::path filePath(path);
			outputPath = filePath.replace_extension("fbx").string();
		}

		spdlog::info("Converting asset...");
		spdlog::info("In: {}", path);
		spdlog::info("Out: {}", outputPath);

		rmesh::RMesh rmesh;
		rmesh.Read(path);

		ExportRMesh exporter(rmesh, ExportFileType::FBX);
		if (!exporter.Export(outputPath))
		{
			spdlog::error("Failed to export {}!", path);
			return 1;
		}
		else
		{
			spdlog::error("Exported {} to {}", path, outputPath);
		}
	}
	catch (const std::exception& e)
	{
		spdlog::error("Encountered exception! {}", e.what());
		return 1;
	}

	return 0;
}