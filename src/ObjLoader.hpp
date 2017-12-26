#ifndef  _LOADOBJ_HPP
#define _LOADOBJ_HPP

#include <vector>
#include <string>
#include "../include/glm/glm.hpp"

#include "Reader.hpp"
#include "Utils.hpp"


using std::string;
using std::vector;

class ObjLoader
{
	public:
		
		vector<glm::vec3> vertices;
		vector<unsigned int> indices;
		vector<float> position;
	
		vector<string> rawData;

		void ReOrder()
		{
			for (int i = 0; i < indices.size(); ++i) {
				position.push_back(vertices[indices[i]].x);
				position.push_back(vertices[indices[i]].y);
				position.push_back(vertices[indices[i]].z);
			}
		}

		ObjLoader() {}

		ObjLoader(std::string filename)
		{
			using glm::vec3;
			using glm::mat4;

			rawData = Reader(filename).getLine();

			for (string buffer : rawData)
			{
				if (buffer[0] == 'v') {
					
					std::vector<std::string> v = Utils::split(buffer.substr(string("v ").size(), buffer.length() - string("v ").size()), ' ');
					
					auto pos = vec3(
						std::stof(v[0], nullptr),
						std::stof(v[1], nullptr),
						std::stof(v[2], nullptr)
					);
					
					vertices.push_back(pos);
				}
				else if (buffer[0] == 'f') {
					std::vector<std::string> f = Utils::split(buffer.substr(string("f ").size(), buffer.length() - string("f ").size()), ' ');

					unsigned int f0 = std::stod(f[0], nullptr) - 1;
					unsigned int f1 = std::stod(f[1], nullptr) - 1;
					unsigned int f2 = std::stod(f[2], nullptr) - 1;

					indices.push_back(f0);
					indices.push_back(f1);
					indices.push_back(f2);
					
					/*auto v1 = vertices[f0].positions;
					auto v2 = vertices[f1].positions;
					auto v3 = vertices[f2].positions;

					auto n = glm::normalize(glm::cross(v2 - v1, v3 - v1));

					vertices[f0].normals = n;
					vertices[f1].normals = n;
					vertices[f2].normals = n;*/
				}
			}

			ReOrder();
		}
};

#endif // ! _LOADOBJ_HPP