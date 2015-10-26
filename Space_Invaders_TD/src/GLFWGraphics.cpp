#include "GLFWGraphics.h"
#include "ResourceManager.h"
#include "consts.h"

GLFWGraphics::GLFWGraphics( std::string windowName )
{
	// initialize glfw
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	GLFWvidmode* mode = (GLFWvidmode*)glfwGetVideoMode( glfwGetPrimaryMonitor() );
	glfwWindowHint( GLFW_RED_BITS, mode->redBits );
	glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
	glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
	glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );

	windowWidth = mode->width;
	windowHeight = mode->height;
	window = glfwCreateWindow( windowWidth, windowHeight, windowName.c_str(), glfwGetPrimaryMonitor(), nullptr );

	// perform common initialization
	init();
}

GLFWGraphics::GLFWGraphics( std::string windowName, GLuint width, GLuint height ) {
	// initialize glfw
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	windowWidth = width;
	windowHeight = height;
	window = glfwCreateWindow( width, height, windowName.c_str(), nullptr, nullptr );

	// perform common initialization
	init();
}

void GLFWGraphics::init(){
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 );

	// initialize glew
	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glViewport( 0, 0, windowWidth, windowHeight );
	glEnable( GL_CULL_FACE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// create sprite and text renderers
	spriteRenderer = new SpriteRenderer();
	glm::mat4 projection = glm::ortho( 0.0f, static_cast<GLfloat>( GAME_WIDTH ),
		static_cast<GLfloat>( GAME_HEIGHT ), 0.0f, -1.0f, 1.0f );
	ResourceManager::getShader( "sprite" ).use().setInteger( "image", 0 );
	ResourceManager::getShader( "sprite" ).setMatrix4( "projection", projection );

	boxRenderer = new BoxRenderer();
	ResourceManager::getShader( "box" ).use().setMatrix4( "projection", projection );

	textRenderer = new TextRenderer( GAME_WIDTH, GAME_HEIGHT );
}

GLFWGraphics::~GLFWGraphics()
{
	delete spriteRenderer;
	delete textRenderer;
	delete boxRenderer;
	glfwDestroyWindow( window );
	glfwTerminate();
}

void GLFWGraphics::setBackgroundColor( glm::vec3 color ) {
	glClearColor( color.x, color.y, color.z, 1.0f );
}

void GLFWGraphics::renderText( const font& characters, std::string text, glm::vec2 pos, GLfloat scale, glm::vec3 color,
	HOR_ALIGNMENT horAlign, VERT_ALIGNMENT vertAlign ){
	textRenderer->renderText( characters, text, pos, scale, color, horAlign, vertAlign );
}

void GLFWGraphics::draw2DTexture( const Texture& texture, glm::vec2 pos, glm::vec2 size, GLfloat rot, glm::vec4 color, GLboolean flipH ){
	spriteRenderer->drawSprite( texture, pos, size, rot, color, flipH );
}

void GLFWGraphics::draw2DBox( glm::vec2 pos, glm::vec2 size, glm::vec3 color, GLfloat rot )
{
	boxRenderer->renderBox( pos, size, color, rot );
}


GLboolean GLFWGraphics::windowIsValid() const {
	return window != nullptr && !glfwWindowShouldClose( window );
}

void GLFWGraphics::swapBuffers() {
	if ( windowIsValid() ){
		glfwSwapBuffers( window );
	}
}

GLFWwindow* GLFWGraphics::getWindow() const {
	return window;
}