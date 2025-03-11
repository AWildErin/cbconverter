#include "cbconverter/export_rmesh.h"

#include "cbconverter/utils.h"

#include <spdlog/spdlog.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ExportRMesh::ExportRMesh(rmesh::RMesh& inRMesh, ExportFileType inExportType)
	: IExporter(inExportType),
	rmeshFile(inRMesh)
{

}

ExportRMesh::~ExportRMesh()
{
	// aiScene will clear the materials for us as long as they are referenced, which they are
	materialMap.clear();
}

bool ExportRMesh::Export(const std::string& outPath)
{
	AddMaterials();
	AddDrawnMesh();
	AddCollisionMesh();
	AddEntities();

	return IExporter::Export(outPath);
}

void ExportRMesh::AddMaterials()
{
	int materialIndex = 0;

	for (int surfIdx = 0; surfIdx < rmeshFile.drawnMesh.surfaces.size(); surfIdx++)
	{
		auto& surface = rmeshFile.drawnMesh.surfaces[surfIdx];

		rmesh::Texture& lightMapTexture = surface.textures[0];
		if (AddMaterial(lightMapTexture.textureName, materialIndex, surfIdx))
		{
			materialIndex++;
		}

		rmesh::Texture& mapTexture = surface.textures[1];
		if (AddMaterial(mapTexture.textureName, materialIndex, surfIdx))
		{
			materialIndex++;
		}
	}

	exporterScene->mMaterials = new aiMaterial * [materialMap.size()];
	exporterScene->mNumMaterials = materialMap.size();

	for (auto& mapPair : materialMap)
	{
		int index = mapPair.second.second;
		aiMaterial* mat = mapPair.second.first;
		exporterScene->mMaterials[index] = mat;
	}
}

bool ExportRMesh::AddMaterial(const std::string& texturePath, int materialIndex, int surfaceIndex)
{
	if (texturePath.empty())
	{
		spdlog::debug("{} (MatIndex: {} SurfIndex: {}) was empty!", texturePath, materialIndex, surfaceIndex);
		return false;
	}

	if (materialMap.contains(texturePath))
	{
		spdlog::debug("{} (MatIndex: {} SurfIndex: {}) already exists!", texturePath, materialIndex, surfaceIndex);
		return false;
	}

	auto* mat = new aiMaterial();
	aiString aiTexturePath = CbConverterUtils::ToString(texturePath);
	mat->AddProperty(&aiTexturePath, AI_MATKEY_NAME);

	std::pair<aiMaterial*, int> pair{ mat, materialIndex };
	materialMap[texturePath] = pair;

	spdlog::debug("{} (MatIndex: {} SurfIndex: {}) added to material list!", texturePath, materialIndex, surfaceIndex);
	return true;
}

void ExportRMesh::AddDrawnMesh()
{
	int surfaceCount = rmeshFile.drawnMesh.surfaces.size();

	exporterScene->mMeshes = new aiMesh * [surfaceCount];
	exporterScene->mNumMeshes = surfaceCount;

	exporterScene->mRootNode->mMeshes = new unsigned int[surfaceCount];
	exporterScene->mRootNode->mNumMeshes = surfaceCount;
	
	for (int i = 0; i < surfaceCount; i++)
	{
		auto& surface = rmeshFile.drawnMesh.surfaces[i];
		AddSurface(surface, surface.textures[1].textureName, i);
	}
}

void ExportRMesh::AddSurface(rmesh::Surface& surface, std::string meshName, int surfaceIndex)
{
	auto mesh = new aiMesh();
	mesh->mName = CbConverterUtils::ToString(meshName);

	// Setup materials
	std::pair<aiMaterial*, int> material = materialMap[surface.textures[1].textureName];
	if (material.first)
	{
		mesh->mMaterialIndex = material.second;
	}

	// Add Vertices
	mesh->mNumVertices = surface.vertices.size();
	mesh->mVertices = new aiVector3D[mesh->mNumVertices];
	mesh->mNormals = new aiVector3D[mesh->mNumVertices];

	mesh->mTextureCoords[0] = new aiVector3D[mesh->mNumVertices];
	mesh->mNumUVComponents[0] = 2;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		rmesh::Vertex& vert = surface.vertices[i];

		// Copy the vector so we can invert the z channel
		cbtypes::Vector3 vertex = vert.vertex;
		vertex.z = -vertex.z;

		mesh->mVertices[i] = CbConverterUtils::ToVector3D(vertex);

		mesh->mTextureCoords[0][i] = CbConverterUtils::ToVector3D(vert.uv);
	}

	// Add Faces
	mesh->mNumFaces = surface.triangles.size();
	mesh->mFaces = new aiFace[mesh->mNumFaces];

	for (int j = 0; j < mesh->mNumFaces; j++)
	{
		auto& tri = surface.triangles[j];

		aiFace face;
		face.mNumIndices = 3;
		face.mIndices = new unsigned int[3];

		face.mIndices[0] = tri.index3;
		face.mIndices[1] = tri.index2;
		face.mIndices[2] = tri.index1;
		mesh->mFaces[j] = face;
	}

	exporterScene->mMeshes[surfaceIndex] = mesh;
	exporterScene->mRootNode->mMeshes[surfaceIndex] = surfaceIndex;
}

void ExportRMesh::AddCollisionMesh()
{

}

void ExportRMesh::AddEntities()
{

}
