#include "../includes/TextRenderer.h"

// Reference: http://www.learnopengl.com/#!In-Practice/2D-Game/Render-text

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include "../includes/ResourceManager.h"

TextRenderer::TextRenderer( GLuint width, GLuint height ) {
	// load and configure shader
	shader = ResourceManager::loadShader( "text.vs", "text.fs", nullptr, "text" );
	shader.use().setMatrix4( "projection", glm::ortho( 0.0f, static_cast<GLfloat>( width ), static_cast<GLfloat>( height ), 0.0f ) );
	shader.setInteger( "text", 0 );
	// configure VAO/VBO for texture quads
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 6 * 4, NULL, GL_DYNAMIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( GLfloat ), 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray( 0 );
}

TextRenderer::~TextRenderer() {

}

void TextRenderer::renderText( const font& characters, std::string text, glm::vec2 pos, GLfloat scale, glm::vec3 color,
	HOR_ALIGNMENT horAlign, VERT_ALIGNMENT vertAlign ) {
	// activate corresponding render state
	shader.use();
	shader.setVector3f( "textColor", color );
	glActiveTexture( GL_TEXTURE0 );
	glBindVertexArray( VAO );

	if( horAlign == RIGHT_ALIGNED || horAlign == HOR_CENTERED) {
		// offset x with size of text
		GLint offsetX = 0;
		for ( int i = 0; i < text.length(); ++i ) {
			offsetX += ( characters.at( text[i] ).advance >> 6 ) * scale;
		}
		pos.x -= horAlign == RIGHT_ALIGNED ?  offsetX : offsetX / 2.0f;
	}
	if ( vertAlign == VERT_CENTERED ) {
		pos.y -= characters.at( 'H' ).bearing.y / 2.0f * scale;
	} else if ( vertAlign == ABOVE ) {
		pos.y -= characters.at( 'H' ).bearing.y * scale;
	}

	// iterate through all characters
	std::string::const_iterator c;
	for ( int c = 0; c < text.length(); ++c ) {
		TextChar ch = characters.at(text[c]);

		GLfloat xpos = pos.x + ch.bearing.x * scale;
		GLfloat ypos = pos.y + ( characters.at('H').bearing.y - ch.bearing.y ) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		// update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 0.0 },
			{ xpos, ypos, 0.0, 0.0 },

			{ xpos, ypos + h, 0.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 1.0 },
			{ xpos + w, ypos, 1.0, 0.0 }
		};
		// render glyph texture over quad
		glBindTexture( GL_TEXTURE_2D, ch.textureID );
		// update content of VBO memory
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		// now advance cursors for next glyph
		pos.x += ( ch.advance >> 6 ) * scale;
	}
	glBindVertexArray( 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
}
