#include "BoxRenderer.h"
#include "ResourceManager.h"

BoxRenderer::BoxRenderer()
{
	shader = ResourceManager::loadShader( "box.vs", "box.fs", nullptr, "box" );
	initRenderData();
}


BoxRenderer::~BoxRenderer()
{
	glDeleteVertexArrays( 1, &VAO );
}

void BoxRenderer::initRenderData() {
	GLuint VBO;
	GLfloat vertices[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );

	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glBindVertexArray( VAO );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof( GLfloat ), (GLvoid*)0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray( 0 );
}

void BoxRenderer::renderBox( glm::vec2 pos, glm::vec2 size, glm::vec3 color, GLfloat rot ) {
	shader.use();
	glm::mat4 model;

	// translate (transformation order: scale, rotate, translate but in reversed order)
	model = glm::translate( model, glm::vec3( pos, 0.0f ) );

	// need to translate to center of quad before rotating
	model = glm::translate( model, glm::vec3( 0.5f * size.x, 0.5f * size.y, 0.0f ) );
	// rotate the sprite along the z axis
	model = glm::rotate( model, rot, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	// move the origin back
	model = glm::translate( model, glm::vec3( -0.5f * size.x, -0.5f * size.y, 0.0f ) );

	// scale the model
	model = glm::scale( model, glm::vec3( size, 1.0f ) );

	// render the sprite
	shader.setMatrix4( "model", model );
	shader.setVector3f( "boxColor", color );

	glBindVertexArray( VAO );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
}

