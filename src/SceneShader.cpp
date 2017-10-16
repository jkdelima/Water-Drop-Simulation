/*
 * SceneShader.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: acarocha
 *
 *  Edited by: Jeremy Kyle Delima
 *
 */

#include "SceneShader.h"
#include "lodepng.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

static float PI = 3.14159265359;

//GLUquadricObj *quadratic = NULL;

SceneShader::SceneShader(): Shader()
{
	_programMesh = 0;
	_programSphere = 0;
	_programCube = 0;
	_mvUniform = -1;
	_projUniform = -1;
	_zTranslation = 1.0;
	_aspectRatio = 1.0;
	_xRot = 0.0;
	_yRot = 0.0;
	lightPosition = glm::vec3(0.5, 0.5, 0.5);
	refraction = 0.5;
}

void SceneShader::readMesh( std::string filename )
{
	_mesh = trimesh::TriMesh::read(filename);

	_mesh->need_bbox();
	_mesh->need_faces();
	_mesh->need_normals();
	_mesh->need_bsphere();

	for(unsigned int i = 0; i < _mesh->faces.size(); i++)
	{
   	  _triangleIndices.push_back(_mesh->faces[i][0]);
	  _triangleIndices.push_back(_mesh->faces[i][1]);
	  _triangleIndices.push_back(_mesh->faces[i][2]);
	}
}

void SceneShader::readSphere( std::string filename )
{
	_sphere = trimesh::TriMesh::read(filename);

	_sphere->need_bbox();
	_sphere->need_faces();
	_sphere->need_normals();
	_sphere->need_bsphere();

	for(unsigned int i = 0; i < _sphere->faces.size(); i++)
	{
   	  _triangleIndices2.push_back(_sphere->faces[i][0]);
	  _triangleIndices2.push_back(_sphere->faces[i][1]);
	  _triangleIndices2.push_back(_sphere->faces[i][2]);
	}
}

void SceneShader::readCube( std::string filename )
{
	_cube = trimesh::TriMesh::read(filename);

	_cube->need_bbox();
	_cube->need_faces();
	_cube->need_normals();
	_cube->need_bsphere();

	for(unsigned int i = 0; i < _cube->faces.size(); i++)
	{
   	  _triangleIndices3.push_back(_cube->faces[i][0]);
	  _triangleIndices3.push_back(_cube->faces[i][1]);
	  _triangleIndices3.push_back(_cube->faces[i][2]);
	}
}


void SceneShader::createVertexBuffer()
{
	//creating texture 2D
	std::string imageFilename("textures/image.png");

	//reading image
	unsigned int error =lodepng::decode(_image, _imageWidth, _imageHeight, imageFilename.c_str());

	if(error){
		std::cout << "reading error " << error << ":" << lodepng_error_text(error) << std::endl;
	}

	_textureImageID = _texture.create2DTexture(_image, _imageWidth, _imageHeight);

	//read and create mesh geometry
	readMesh("./models/vase.obj"); //normalized vertices coordinates

	//triangle mesh
	glGenVertexArrays(1, &_meshVertexArray);
	glBindVertexArray(_meshVertexArray);

	glGenBuffers(1, &_meshVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  _mesh->vertices.size() * sizeof (trimesh::point), _mesh->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_meshNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _mesh->normals.size() * sizeof(trimesh::vec), _mesh->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &_meshIndicesBuffer );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshIndicesBuffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangleIndices.size()*sizeof(unsigned int), _triangleIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	//read and create mesh geometry
	readSphere("./models/sphere.obj"); //normalized vertices coordinates

	//triangle mesh
	glGenVertexArrays(1, &_sphereVertexArray);
	glBindVertexArray(_sphereVertexArray);

	glGenBuffers(1, &_sphereVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _sphereVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  _sphere->vertices.size() * sizeof (trimesh::point), _sphere->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_sphereNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _sphereNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _sphere->normals.size() * sizeof(trimesh::vec), _sphere->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &_sphereIndicesBuffer );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sphereIndicesBuffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangleIndices2.size()*sizeof(unsigned int), _triangleIndices2.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	//read and create mesh geometry
	readCube("./models/cube.obj"); //normalized vertices coordinates

	//triangle mesh
	glGenVertexArrays(1, &_cubeVertexArray);
	glBindVertexArray(_cubeVertexArray);

	glGenBuffers(1, &_cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  _cube->vertices.size() * sizeof (trimesh::point), _cube->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_cubeNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _cubeNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _cube->normals.size() * sizeof(trimesh::vec), _cube->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &_cubeIndicesBuffer );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _cubeIndicesBuffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangleIndices3.size()*sizeof(unsigned int), _triangleIndices3.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	
}

void SceneShader::startup()
{
	_programMesh = compile_shaders("./shaders/mesh.vert", "./shaders/mesh.frag");

	_programSphere = compile_shaders("./shaders/sphere.vert", "./shaders/sphere.frag");

	_programCube = compile_shaders("./shaders/cube.vert", "./shaders/cube.frag");

	_programLight = compile_shaders("./shaders/light.vert", "./shaders/light.frag");

	createVertexBuffer();
}

void SceneShader::renderMesh()
{
	glBindVertexArray(_meshVertexArray);

	glUseProgram(_programMesh);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programMesh, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glDrawElements( GL_TRIANGLES, _mesh->faces.size()*3, GL_UNSIGNED_INT, 0 );

	glBindVertexArray(0);
}

void SceneShader::renderSphere()
{
	glBindVertexArray(_sphereVertexArray);

	glUseProgram(_programSphere);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_texture.bind2DTexture( _programSphere, _textureImageID, std::string("image"));

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programSphere, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programSphere, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programSphere, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glUniform1f(glGetUniformLocation(_programSphere, "refraction"), refraction);

	glDrawElements( GL_TRIANGLES, _sphere->faces.size()*3, GL_UNSIGNED_INT, 0 );

	_texture.unbind2DTexture();

	glDisable(GL_BLEND);

	glBindVertexArray(0);
}

void SceneShader::renderCube()
{
	glBindVertexArray(_cubeVertexArray);

	glUseProgram(_programCube);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_texture.bind2DTexture( _programCube, _textureImageID, std::string("image"));

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programCube, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programCube, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programCube, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glDrawElements( GL_TRIANGLES, _cube->faces.size()*3, GL_UNSIGNED_INT, 0 );

	_texture.unbind2DTexture();

	glDisable(GL_BLEND);

	glBindVertexArray(0);
}

void SceneShader::renderLight()
{
	glUseProgram(_programLight);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programLight, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glPointSize(30.0f);
	glDrawArrays( GL_POINTS, 0, 1);

}

void SceneShader::render()
{
	renderMesh();
	renderSphere();
	renderLight();
	//renderCube();
}

void SceneShader::setZTranslation(float z)
{
	_zTranslation = z;
}

void SceneShader::setAspectRatio(float ratio)
{
	_aspectRatio = ratio;
}

void SceneShader::setRotationX( float x )
{
	_xRot = x;
}

void SceneShader::setRotationY( float y )
{
	_yRot = y;
}

void SceneShader::shutdown()
{
	glDeleteBuffers(1, &_meshVertexBuffer);
	glDeleteBuffers(1, &_meshNormalBuffer);
	glDeleteBuffers(1, &_meshIndicesBuffer);
	glDeleteBuffers(1, &_sphereVertexBuffer);
	glDeleteBuffers(1, &_sphereNormalBuffer);
	glDeleteBuffers(1, &_sphereIndicesBuffer);
	glDeleteBuffers(1, &_cubeVertexBuffer);
	glDeleteBuffers(1, &_cubeNormalBuffer);
	glDeleteBuffers(1, &_cubeIndicesBuffer);
	glDeleteVertexArrays(1, &_meshVertexArray);
	glDeleteVertexArrays(1, &_sphereVertexArray);
	glDeleteVertexArrays(1, &_cubeVertexArray);
}

void SceneShader::updateLightPositionX(float x)
{
	lightPosition.x += x;
}

void SceneShader::updateLightPositionY(float y)
{
	lightPosition.y += y;
}

void SceneShader::updateLightPositionZ(float z)
{
	lightPosition.z += z;
}

void SceneShader::updateRefraction(float x)
{
	if (refraction > 1 && x > 0) { 
		refraction = 1;
	} else if (refraction < 0 && x < 0) {
		refraction = 0;
	} else {
		refraction += x;
	}
}

SceneShader::~SceneShader()
{
	shutdown();
}
