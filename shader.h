#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "util.h"

// wrappers around stupid functions that modify pointers instead of returning values

static bool compile_status(GLuint shader){
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	return bool(compiled);
}

static void shader_source(GLuint shader, std::string src){
	const GLint len = GLint(src.size());
	const char *cstr = src.c_str();
	glShaderSource(shader, 1, &cstr, &len);	
}

static std::string get_compile_error(GLuint shader){
	GLint log_len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

	std::string log_txt(log_len,0);

	glGetShaderInfoLog(shader, log_len, &log_len, &log_txt[0]);
	
	return log_txt;
}

GLuint make_shader(std::string vert_src, std::string frag_src){
	//// STEP 1: compiling the vertex shader
	// create empty vertex shader, according to documentation, must be nonzero
	// so we'll return zero to indicate error
	GLuint vert_o = glCreateShader(GL_VERTEX_SHADER);
	
	// fill shader with source code
	shader_source(vert_o, vert_src);

	// compile shader
	glCompileShader(vert_o);
	
	// check for compilation errors
	if(!compile_status(vert_o)){
		std::cout<<FAIL"vertex shader compilation failed\n\n"
				 <<vert_src
				 <<"\n\nwith following error:\n"
				 <<get_compile_error(vert_o)<<"\n";
		// cleanup
		glDeleteShader(vert_o);
		return 0;
	}
	std::cout<<OK"vertex shader compilation successful\n";


	//// STEP 2: compiling fragment shader 	
	// create empty fragment shader
	GLuint frag_o = glCreateShader(GL_FRAGMENT_SHADER);

	// fill shader with source code
	shader_source(frag_o, frag_src);

	// compile shader
	glCompileShader(frag_o);
	
	// check for compilation errors
	if(!compile_status(frag_o)){
		std::cout<<FAIL"fragment shader compilation failed\n\n"
				 <<frag_src
				 <<"\n\nwith following error:\n"
				 <<get_compile_error(frag_o)<<"\n";
		// cleanup
		glDeleteShader(vert_o);
		glDeleteShader(frag_o);
		return 0;
	}
	std::cout<<OK"fragment shader compilation successful\n";


	//// STEP 3: linking the shaders together into a program	
	GLuint prog = glCreateProgram();
	glAttachShader(prog,vert_o);
	glAttachShader(prog,frag_o);
	glLinkProgram(prog);

	// strip off the subshaders
	glDetachShader(prog,vert_o);
	glDeleteShader(vert_o);
	glDetachShader(prog,frag_o);
	glDeleteShader(frag_o);

	return prog;
}
