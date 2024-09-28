#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char** argv)
{
	argparse::ArgumentParser program("cbconverter");

	program.add_description("Takes a file that is supported and converts the mesh to a fbx file.\n"
		"Supported formats:\n"
		"  - Blitz3D (.b3d)\n"
		"  - Room Mesh (.rmesh)"
	);

	program.add_argument("<path>")
		.help("Path to a supported model file")
		.required();

	program.add_argument("-o", "--output")
		.help("Directory where the gltf will be output.");
	
	//try
	{
		program.parse_args(argc, argv);

		std::string path = program.get("<path>");
		std::string outputPath = program.get("-o");
		
        std::cout << path << std::endl;
	}
	//catch (const std::exception& e)
	//{
	//	if (argc > 1)
	//	{
	//		std::cerr << e.what() << '\n' << std::endl;
	//		std::cerr << program << std::endl;
	//	}
	//	else
	//	{
	//		std::cout << program << std::endl;
	//	}
	//}

	return 0;
}