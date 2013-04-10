#include "ModelExporter.hpp"

void ModelExporter::addBuilding(const Building& building){
	buildings.push_back(building);
};

void ModelExporter::addPath(const Path& path){
	paths.push_back(path);	
};

void ModelExporter::exportModel(const std::string& filePath){
	std::ofstream modelFile(filePath.c_str());
	if(modelFile.is_open()){
		modelFile << "# Generated city OBJ file\n";

		int totalVerts = 0;

		//Output buildings.
		for(size_t i = 0; i < buildings.size(); ++i){
			const std::vector<float>& verts = buildings[i].getVertices();;

			modelFile << "o city_obj." << (i + 1) << "\n";

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

		//Output paths
		for(size_t i = 0; i < paths.size(); ++i){
			const std::vector<float>& verts = paths[i].getVertices();;

			modelFile << "o city_obj." << (i + 1) << "\n";

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
