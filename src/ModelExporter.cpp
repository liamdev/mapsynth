#include "ModelExporter.hpp"

void ModelExporter::addBuilding(const Building& building){
	buildings.push_back(building);
};

void ModelExporter::addPath(const Path& path){
	paths.push_back(path);	
};

void ModelExporter::exportModel(const std::string& filePath){

	// Open output file.
	std::ofstream modelFile(filePath.c_str());
	if(modelFile.is_open()){
		// File header.
		modelFile << "# Generated city OBJ file\n";
		modelFile << "# Map Synthesiser by Liam de Valmency, 2013\n";

		int totalVerts = 0;

		//Output each building.
		for(size_t i = 0; i < buildings.size(); ++i){
			const std::vector<float>& verts = buildings[i].getVertices();;

			modelFile << "o bldng." << (i + 1) << "\n";

			for(size_t j = 0; j < verts.size(); j += 3){
				modelFile << "v " << verts[j] << " " << verts[j + 1] << " " << verts[j + 2] << "\n";
			}

			modelFile << "s off";

			for(size_t j = 0; j < verts.size() / 3; ++j){
				if(j % 3 == 0)
					modelFile << "\nf";
				modelFile << " " << (totalVerts + j + 1);
			}

			totalVerts += verts.size() / 3;
			
			modelFile << "\n";
		}

		//Output each path.
		for(size_t i = 0; i < paths.size(); ++i){
			const std::vector<float>& verts = paths[i].getVertices();;

			modelFile << "o road." << (i + 1) << "\n";

			for(size_t j = 0; j < verts.size(); j += 3){
				modelFile << "v " << verts[j] << " " << verts[j + 1] << " " << verts[j + 2] << "\n";
			}

			modelFile << "s off";

			for(size_t j = 0; j < verts.size() / 3; ++j){
				if(j % 3 == 0)
					modelFile << "\nf";
				modelFile << " " << (totalVerts + j + 1);
			}

			totalVerts += verts.size() / 3;
			
			modelFile << "\n";
		}
	}

	modelFile.close();
};
