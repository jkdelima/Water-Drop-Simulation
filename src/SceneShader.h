/*
 * SceneShader.h
 *
 *  Created on: Nov 17, 2015
 *      Author: acarocha
 *
 *  Edited by: Jeremy Kyle Delima
 *
 */

#ifndef SCENESHADER_H_
#define SCENESHADER_H_

#include "Shader.h"
#include <vector>

#define GLM_FORCE_RADIANS

#include "TriMesh.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"

class SceneShader : public Shader
{
public:

	SceneShader();
	~SceneShader();

	void startup ();
	void shutdown ();
	void render();

	void renderMesh();
	void renderLight();
	void renderSphere();
	void renderCube();

	void setAspectRatio( float ratio );
	void setZTranslation(float z);
	void setRotationX( float x );
	void setRotationY( float y );

	void updateLightPositionX( float x );
	void updateLightPositionY( float y );
	void updateLightPositionZ( float z );

	void updateRefraction( float x );

private:

	/*methods*/

	void readMesh( std::string filename );
	void readSphere( std::string filename );
	void readCube( std::string filename );
	void createVertexBuffer();

	//image
	std::vector<unsigned char> _image;
	unsigned int _imageWidth;
	unsigned int _imageHeight;

	/*variables*/
	GLuint _programLight;
	GLuint _programMesh;
	GLuint _programSphere;
	GLuint _programCube;

	GLuint _meshVertexArray;
	GLuint _meshVertexBuffer;
	GLuint _meshNormalBuffer;
	GLuint _meshIndicesBuffer;

	GLuint _sphereVertexArray;
	GLuint _sphereVertexBuffer;
	GLuint _sphereNormalBuffer;
	GLuint _sphereIndicesBuffer;

	GLuint _cubeVertexArray;
	GLuint _cubeVertexBuffer;
	GLuint _cubeNormalBuffer;
	GLuint _cubeIndicesBuffer;

    GLint _mvUniform, _projUniform;

	/* Matrices */
	glm::mat4 _modelview;
	glm::mat4 _projection;

	float _zTranslation;
	float _xRot;
	float _yRot;
	float _aspectRatio;

	/* Textures */
	GLuint _textureID;
	GLuint _textureImageID;
	Texture _texture;

	/*vertices*/
	std::vector<float> _vertices;
	std::vector<float> _vertices2;
	std::vector<float> _vertices3;

	trimesh::TriMesh* _mesh;
	trimesh::TriMesh* _sphere;
	trimesh::TriMesh* _cube;
	std::vector<unsigned int> _triangleIndices;
	std::vector<unsigned int> _triangleIndices2;
	std::vector<unsigned int> _triangleIndices3;

	glm::vec3 lightPosition;

	float refraction;
	
};

#endif /* SCENESHADER_H_ */
