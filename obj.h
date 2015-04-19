#include <vector>
#include <cstdlib>
#include <GL/glew.h>

std::vector<GLfloat> get_vertices(char const * obj_string){
	std::vector<GLfloat> vertices;
	for(;*obj_string!='\0';obj_string++){
		// skip comments
		if(obj_string[0]=='#'){
			while(*obj_string!='\n'){
				if(*obj_string=='\0') return vertices;
				else obj_string++;
			}
		}
		// parse vertex
		if(obj_string[0]=='v'
		&& obj_string[1]==' '){
			obj_string += 2;
			auto size = vertices.size();
			vertices.resize(size+3);
			vertices[size  ] = strtof(obj_string,const_cast<char**>(&obj_string));
			vertices[size+1] = strtof(obj_string,const_cast<char**>(&obj_string));
			vertices[size+2] = strtof(obj_string,const_cast<char**>(&obj_string));
		}
	}
	return vertices;
}

std::vector<GLushort> get_indices(char const * obj_string){
	std::vector<GLushort> indices;
	for(;*obj_string!='\0';obj_string++){
		// skip comments
		if(obj_string[0]=='#'){
			while(*obj_string!='\n'){
				if(*obj_string=='\0') return indices;
				else obj_string++;
			}
		}
		// parse face
		if(obj_string[0]=='f'
		&& obj_string[1]==' '){
			obj_string += 2;
			auto size = indices.size();
			indices.resize(size+3);
			indices[size  ] = strtof(obj_string,const_cast<char**>(&obj_string)) -1;
			while(*obj_string!=' '){obj_string++;}// skip to first space
			indices[size+1] = strtof(obj_string,const_cast<char**>(&obj_string)) -1;
			while(*obj_string!=' '){obj_string++;}// skip to first space
			indices[size+2] = strtof(obj_string,const_cast<char**>(&obj_string)) -1;
		}
	}
	return indices;
}
