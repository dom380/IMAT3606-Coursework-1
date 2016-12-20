#include "utils\ObjReader.h"

ObjReader::ObjReader() {

}

ObjReader::~ObjReader()
{
}

void ObjReader::readObj(char * filePath, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures, vector<unsigned short>&indices)
{
	unpackedVertices.clear();
	unpackedNormals.clear();
	unpackedTextures.clear();
	faceIndices.clear();
	indexVerticeMap.clear();
	std::fstream inStream;
	inStream.open(filePath);
	if (!inStream.is_open()) {
		std::cout << "Error opening file " << filePath << std::endl;
		exit(1);
	}

	std::string newLine;
	while (std::getline(inStream, newLine)) {
		std::string lineStart = newLine.substr(0, 2);
		if (lineStart.length() == 0) {
			continue;
		}
		std::istringstream stringStream(newLine.substr(2));
		if (lineStart == "v ") {
			float x, y, z;
			stringStream >> x; stringStream >> y; stringStream >> z;
			glm::vec4 vertex(x, y, z, 1.0f);
			unpackedVertices.push_back(vertex);
		}
		else if (lineStart == "vt") {
			float x, y;
			stringStream >> x; stringStream >> y;
			glm::vec2 texture(x, y);
			unpackedTextures.push_back(texture);
		}
		else if (lineStart == "vn") {
			float x, y, z;
			stringStream >> x; stringStream >> y; stringStream >> z;
			glm::vec3 normal(x, y, z);
			unpackedNormals.push_back(normal);
		}
		else if (lineStart == "f ") {
			float indexValue;
			glm::vec3 faceVertex;
			while (stringStream >> indexValue)
			{

				faceVertex.x = indexValue-1; //Position index
				int peek = stringStream.peek();
				if (peek == 47) //If position is followed by a /
				{
					stringStream.get(); //Read passed /
					if (stringStream.peek() == 47) //If a second /
					{
						stringStream.get();//read passed it again
						stringStream >> indexValue;
						faceVertex.z = indexValue -1; //Read in normals index
						faceVertex.y = -1; //There was no texture index supplied.
						faceIndices.push_back(faceVertex); //Finished reading this face
					}
					else //There was only 1 / after the position idex
					{
						stringStream >> indexValue; //Read in texture index
						faceVertex.y = indexValue -1;
						int peek = stringStream.peek();
						if (peek == 47) //If there is a / after the texture index
						{
							stringStream.get();
							stringStream >> indexValue; //Read in normal index
							faceVertex.z = indexValue -1;
							faceIndices.push_back(faceVertex); //Finished reading this face
						}
						else
						{
							faceVertex.z = -1; //No normal index supplied
							faceIndices.push_back(faceVertex); //Finished reading this face
						}
					}
				}
			}
		} //end if
	
	}
	unsigned short nextIndex = 0;
	int arraySize = faceIndices.size();
	for (int i = 0; i < arraySize; i += 3) {
		for (int j = 0; j < 3; j++)
		{
			glm::vec3 face = faceIndices.at(i + j);
			std::unordered_map<glm::vec3, unsigned short>::iterator it = indexVerticeMap.find(face);
			if (it != indexVerticeMap.end())
			{
				indices.push_back(it->second);
			}
			else
			{
				vertices.push_back(unpackedVertices.at(face.x));
				if (unpackedTextures.size() > 0) {
					textures.push_back(unpackedTextures.at(face.y));
				}
				if (unpackedNormals.size() > 0) {
					normals.push_back(unpackedNormals.at(face.z));
				}
				indices.push_back(nextIndex);
				indexVerticeMap.insert(std::pair<glm::vec3, unsigned short>(face, nextIndex));
				nextIndex++;
			}

		}
	}
}

//
//bool vec3LessThan(const glm::vec3 &v1, const glm::vec3 &v2) {
//	return v1.x < v2.x && v1.y < v2.y && v1.z < v2.z;
//}
//
//bool vec3Equals(const glm::vec3 &vecA, const glm::vec3 &vecB) {
//	const double epsilion = 0.0001;  // choose something apprpriate.
//
//	return    fabs(vecA[0] - vecB[0]) < epsilion
//		&& fabs(vecA[1] - vecB[1]) < epsilion
//		&& fabs(vecA[2] - vecB[2]) < epsilion;
//}