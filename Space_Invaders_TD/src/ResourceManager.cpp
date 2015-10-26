#include "../includes/ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <FreeImagePlus.h>

// Reference: http://www.learnopengl.com/#!In-Practice/2D-Game/Setting-up

// instantiate static variables
std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, std::string> ResourceManager::paths;
std::map<std::string, font> ResourceManager::fonts;

Shader ResourceManager::loadShader( const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name ) {
	if ( shaders.count( name ) == 0 ) {
		shaders[name] = loadShaderFromFile( vShaderFile, fShaderFile, gShaderFile );
	}
	return shaders[name];
}

Shader ResourceManager::getShader( std::string name ) {
	assert( shaders.count( name ) != 0 );
	return shaders[name];
}

Texture ResourceManager::loadTexture( const GLchar* file, GLboolean alpha, std::string name ) {
	if ( textures.count( name ) == 0 ) {
		textures[name] = loadTextureFromFile( file, alpha );
	}
	return textures[name];
}

Texture ResourceManager::getTexture( std::string name ) {
	if ( textures.count( name ) == 0 ) {
		std::cout << "texture " << name << " not found." << std::endl;
	}
	return textures[name];
}

font ResourceManager::loadFont( const GLchar* file, std::string name, GLuint size ) {
	if ( fonts.count( name ) == 0 ) {
		fonts[name] = loadFontFromFile( file, size );
	}
	return fonts[name];
}

font ResourceManager::getFont( std::string name ) {
	assert( fonts.count( name ) != 0 );
	return fonts[name];
}


void ResourceManager::setPath( std::string name, std::string path ) {
	paths[name] = path;
}

void ResourceManager::setShaderPath( std::string path ) {
	paths["shaders"] = path;
}

void ResourceManager::setTexturePath( std::string path ) {
	paths["textures"] = path;
}

std::string	ResourceManager::getPath( std::string name ) {
	return paths[name];
}

void ResourceManager::clear() {
	for ( auto iter : shaders )
		glDeleteProgram( iter.second.ID );
	for ( auto iter : textures )
		glDeleteTextures( 1, &iter.second.ID );
}

Shader ResourceManager::loadShaderFromFile( const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile ) {
	std::string vertexCode, fragmentCode, geometryCode;

	try {
		// open the files
		std::ifstream vertexShaderFile( ( paths["shaders"] + vShaderFile ).c_str() );
		std::ifstream fragmentShaderFile( ( paths["shaders"] + fShaderFile ).c_str() );
		std::stringstream vShaderStream, fShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// convert stream to string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// if geometry path was passed in, load the geometry shader
		if ( gShaderFile != nullptr ) {
			std::ifstream geoShaderFile( ( paths["shaders"] + gShaderFile ).c_str() );
			std::stringstream gShaderStream;
			gShaderStream << geoShaderFile.rdbuf();
			geoShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	} catch ( std::exception e ) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();

	// create the shader object from source code
	Shader shader;
	shader.compile( vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr );
	return shader;
}

Texture ResourceManager::loadTextureFromFile( const GLchar* file, GLboolean alpha ) {
	// create a texture object
	Texture texture;
	if ( alpha ) {
		texture.internalFormat = GL_RGBA;
		texture.imageFormat = GL_BGRA;
	}

	// load the texture
	fipImage image;
	if ( image.load( ( paths["textures"] + file ).c_str() ) ) {
		if ( image.getImageType() == FIT_UNKNOWN ) {
			std::cerr << "Unknown image type for texture: " << file << std::endl;
		}

		if ( alpha ) {
			image.convertTo32Bits();
		}
		image.flipVertical();

		texture.generate( image.getWidth(), image.getHeight(), image.accessPixels() );
	} else {
		std::cout << "Failed to load texture " << file << std::endl;
	}
	return texture;
}

font ResourceManager::loadFontFromFile( const GLchar* file, GLuint fontSize ) {
	std::map<GLchar, TextChar> characters;

	// initialize and load the freetype library
	FT_Library ft;
	if ( FT_Init_FreeType( &ft ) ) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	// load font as face
	FT_Face face;
	if ( FT_New_Face( ft, ( ResourceManager::getPath( "fonts" ) + file ).c_str(), 0, &face ) ) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
	// set size of glyph
	FT_Set_Pixel_Sizes( face, 0, fontSize );
	// disable byte-alignment restriction
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	// for the first 128 ASCII character, pre-load/compile their characters and store them
	for ( GLubyte c = 0; c < 128; c++ ) {
		// load character glyph
		if ( FT_Load_Char( face, c, FT_LOAD_RENDER ) ) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		GLuint texture;
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );
		// set texture options
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		// store the character for later use
		TextChar character = {
			texture,
			glm::ivec2( face->glyph->bitmap.width, face->glyph->bitmap.rows ),
			glm::ivec2( face->glyph->bitmap_left, face->glyph->bitmap_top ),
			face->glyph->advance.x
		};
		characters.insert( std::pair<GLchar, TextChar>( c, character ) );
	}
	glBindTexture( GL_TEXTURE_2D, 0 );
	// destroy freetype
	FT_Done_Face( face );
	FT_Done_FreeType( ft );

	return characters;
}