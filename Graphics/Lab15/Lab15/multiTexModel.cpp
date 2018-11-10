//FileName:		multiTexModel.cpp
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		Defines the methods for the MultiTexModel class. Specifically, it follows example 6.14 in the
//OpenGL Programming Guide (8th edition) to send the texture information to the shaders in uniform variables.

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "multiTexModel.h"
#include "texture.h"
#include <iostream>
#include <string>
using namespace std;

MultiTexModel::MultiTexModel()
{
	GLSLprogram = 0;
	tex1_uniform_Loc = 0;
	tex2_uniform_Loc = 0;
}

void MultiTexModel::buildMipMaps()
{
	if (initialized)
	{
		glBindTexture(GL_TEXTURE_2D, texIDs[0]);
		glEnable(GL_TEXTURE_2D);  //This Web page (http://www.opengl.org/wiki/Common_Mistakes) reports that some
								  //ATI drivers require that texturing be enabled before calling glGenerateMipMap
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glBindTexture(GL_TEXTURE_2D, texIDs[1]);
		glEnable(GL_TEXTURE_2D);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		cout << "TexModel objects must be initialized before calling buildMipMaps." << endl;
	}
}

//init() does all of the OpenGL initialization for your model(s)
bool MultiTexModel::init(string texFileName1, string texFileName2, GLuint program, const GLfloat *c)
{
	GLSLprogram = program;

	glGenVertexArrays(1, &VAO);  //Create one vertex array object
	glBindVertexArray(VAO);

	defineVertexPositions(); //Call the method inherited from Quad to set up the vertex positions
	defineTexCoords(); //Call the method inherited from TexModel to set up the texture coordinates

	glGenBuffers(2, Buffers); //Create two buffer objects, one for vertex positions and one for texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);  //Buffers[0] will be the position for each vertex
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);  //Buffers[1] will be the texture coordinates
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(4);

	defineVerticeColor(c); //Call the method inherited from Quad to set up the vertices' color

	//Now we need to load our textures
	initTextures(texFileName1, texFileName2);

	initialized = true;
	return true;  //Everything got initialized
}

void MultiTexModel::initTextures(string texFileName1, string texFileName2)
{
	glGenTextures(2, texIDs);

	Texture t1(texFileName1, true); cout << endl;  
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texIDs[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, t1.width(), t1.height());
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, t1.width(), t1.height(), GL_RGB, GL_UNSIGNED_BYTE, t1.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Now, tell the shader how to find this texture
	tex1_uniform_Loc = glGetUniformLocation(GLSLprogram, "tex_image1");

	Texture t2(texFileName2, true); cout << endl;
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texIDs[1]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, t2.width(), t2.height());
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, t2.width(), t2.height(), GL_RGB, GL_UNSIGNED_BYTE, t2.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Now, tell the shader how to find this texture
	tex2_uniform_Loc = glGetUniformLocation(GLSLprogram, "tex_image2");
}

//draw() explains how to render your model
void MultiTexModel::draw()
{
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a MultiTexModel object before it has been initialized." << endl;
		return;
	}

	glBindVertexArray(VAO);

	//Do the shader plumbing for the textures.
	glUniform1i(tex1_uniform_Loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texIDs[0]);

	glUniform1i(tex2_uniform_Loc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texIDs[1]);

	glVertexAttrib4f(1, color[0], color[1], color[2], color[3]);  //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 0.0, 1.0);  //normal is attribute location 2
	glVertexAttrib1f(3, 2.0); //shininess is attribute location 3

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}