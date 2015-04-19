#define PI 3.14159265358979323846264338327950288419716939937510

#include <iostream>
#include <vector>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "util.h"
#include "file.h"
#include "obj.h"
#include "mesh.h"
#include "shader.h"
#include "transform.h"
#include "physics.h"

using namespace std;
using glm::vec3;
using glm::mat4;

GLFWwindow* init(int width, int height){
	if (!glfwInit()){
		cout<<FAIL"GLFW failed to initialize\n";
		exit(EXIT_FAILURE);
	}
	else cout<<OK"GLFW initialized\n";

	GLFWwindow* window;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
	
	window = glfwCreateWindow(width, height, "float", NULL, NULL);
	// check if window has been created
	if (!window){
		cout<<FAIL"GLFW failed to create a window\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else cout<<OK"GLFW window created"<<endl;
	// assign context to current thread
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	// initialise GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		cerr<<FAIL"GLEW "<<glewGetErrorString(err)<<endl;
		exit(EXIT_FAILURE);
	} else cout<<OK"GLEW initialized\n";
	
	// set GL states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glCullFace(GL_FRONT);

	return window;
}

char* read_file_or_die(const char* const filename){
	char* ret = read_file(filename);
	if(!ret){
		cout<<FAIL"could not read "<<filename<<endl;
		exit(EXIT_FAILURE);
	}
	return ret;
}

GLuint prog_from_file(char const* const vertfile, char const * const fragfile){
	char* vert = read_file_or_die(vertfile);
	char* frag = read_file_or_die(fragfile);
	GLuint program = make_shader(vert, frag);
	free(vert);
	free(frag);
	return program;
}

mesh mesh_from_file(char const * const filename){
	auto file = read_file_or_die(filename);
	//cout<<"FILE:\n"<<file<<endl;
	auto vertices = get_vertices(file);
	//for(auto& v:vertices) cout<<v<<" "; cout<<endl;
	auto indices = get_indices(file);
	//for(auto& i:indices) cout<<i<<" ";  cout<<endl;
	auto ret  = create_mesh(vertices,indices);
	free(file);
	return ret;
}

GLint uniform_or_die(GLuint program, char const * const name){
	GLint ret = glGetUniformLocation(program,name);
	if(ret==-1){
		std::cout<<FAIL
		"failed to find uniform \""<<name<<"\" in shader program\n";
		exit(EXIT_FAILURE);
	}else cout<<OK"found uniform \""<<name<<"\"\n";
	return ret;
}

void uniform_set(GLint loc, mat4 data){
	float array[16] = {
		data[0][0],data[0][1],data[0][2],data[0][3],
		data[1][0],data[1][1],data[1][2],data[1][3],
		data[2][0],data[2][1],data[2][2],data[2][3],
		data[3][0],data[3][1],data[3][2],data[3][3]
	};
	glUniformMatrix4fv(loc,1,false,array);
}

void uniform_set(GLint loc,vec3 data){
	glUniform3f(loc,data.x,data.y,data.z);
}

unsigned const jet_id = 0;

struct flat_shaded_renderable{
	transform t;
	mesh      m;
	vec3      c;
};


vector<flat_shaded_renderable> gen_cubes(int width, int height, int depth){
	vector<flat_shaded_renderable> ret;
	for(int x=-width;  x<width;  x++)
	for(int y=-height; y<height; y++)
	for(int z=-width;  z<depth;  z++){
		flat_shaded_renderable r;
		r.c = vec3(float(rand())/float(RAND_MAX),
				   float(rand())/float(RAND_MAX),
				   float(rand())/float(RAND_MAX));
		r.m = mesh_from_file("cube.obj");
		r.t.position = vec3(float(x),float(y),float(z));
		r.t.orientation = quat(1,0,0,0);
		r.t.scale = vec3(0.1,0.1,0.1);
		ret.push_back(r);
	}
	return ret;
}

flat_shaded_renderable gen_jet(){
	flat_shaded_renderable r;
	r.c = vec3(float(rand())/float(RAND_MAX),
			   float(rand())/float(RAND_MAX),
			   float(rand())/float(RAND_MAX));
	r.m = mesh_from_file("jet.obj");
	r.t.position    = vec3(0,0,-5);
	r.t.orientation = glm::rotate(quat(),float(float(1/4.f)*PI),vec3(0,0,1));
	r.t.scale       = vec3(0.2,0.2,0.2);
	return r;
}


int main(){
	auto window = init(800,600);

	auto prog = prog_from_file("flat.vert","flat.frag");
	glUseProgram(prog);

	auto color_uniform      = uniform_or_die(prog,"color");
	auto modelview_uniform  = uniform_or_die(prog,"modelview");
	auto projection_uniform = uniform_or_die(prog,"projection");

	uniform_set(projection_uniform, glm::perspective(1.f,4.f/3.f,0.1f,1000.f));

	vector<flat_shaded_renderable> renderables;
	renderables.push_back(gen_jet());
	{	auto cubes = gen_cubes(5,5,5);
		renderables.insert(renderables.end(), cubes.begin(), cubes.end());
	}
	
	transform camera;
	camera.position    = vec3(0,0,-5);
	camera.scale       = vec3(1,1,1);

	jet_physics phy;
	phy.velocity_direction = quat(1,0,0,0);
	phy.speed = 10;

	while(true){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		jet_input(window, renderables[jet_id].t, phy);

		//camera.orientation = -renderables[jet_id].t.orientation;
		camera.origin      = -renderables[jet_id].t.position;

		for(auto& r: renderables){
			uniform_set(color_uniform, r.c);
			uniform_set(modelview_uniform,
						camera.get_transform() * r.t.get_transform());
			draw_mesh(r.m);
		}
		glfwSwapBuffers(window);
	}

	for(auto& r: renderables)
		destroy_mesh(r.m);
	glUseProgram(0);
	glDeleteProgram(prog);
}

