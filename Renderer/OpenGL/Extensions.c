/**********************************************************************
 *
 * PROJECT:		Mylly GUI - OpenGL Renderer
 * FILE:		Extensions.c
 * LICENCE:		See Licence.txt
 * PURPOSE:		An OpenGL reference renderer for Mylly GUI.
 *				Code to load OpenGL extensions.
 *
 *				(c) Tuomo Jauhiainen 2013
 *
 **********************************************************************/

#include "Extensions.h"
#include <string.h>

#if defined( _WIN32 )
	#define myllyGetProcAddress(x) wglGetProcAddress((const char*)x)
#elif defined( __APPLE__ )
	#define myllyGetProcAddress(x) NSGLGetProcAddress(x)
#else
	#define myllyGetProcAddress(x) glXGetProcAddress(x)
#endif

// --------------------------------------------------

bool MYLLY_EXT_framebuffer_object = false;
bool MYLLY_GL_2_0 = false;

// --------------------------------------------------

glGenFramebuffersEXT_t __glGenFramebuffersEXT = NULL;
glBindFramebufferEXT_t __glBindFramebufferEXT = NULL;
glDeleteFramebuffersEXT_t __glDeleteFramebuffersEXT = NULL;
glFramebufferTextureEXT_t __glFramebufferTextureEXT = NULL;
glCheckFramebufferStatusEXT_t __glCheckFramebufferStatusEXT = NULL;
glDrawBuffers_t __glDrawBuffers = NULL;

// --------------------------------------------------

static void mgui_opengl_load_framebuffer_object_EXT( void );
static void mgui_opengl_load_gl_2_0( void );
static size_t mgui_opengl_substr_len( const char* str, char c );
static bool mgui_opengl_get_extension( const char* name );

// --------------------------------------------------

void mgui_opengl_initialize_extensions( void )
{
	mgui_opengl_load_gl_2_0();

	if ( mgui_opengl_get_extension( "GL_EXT_framebuffer_object" ) )
		mgui_opengl_load_framebuffer_object_EXT();
}

static void mgui_opengl_load_framebuffer_object_EXT( void )
{
	bool result = true;

	result = ( ( __glGenFramebuffersEXT = (glGenFramebuffersEXT_t)myllyGetProcAddress( (const GLubyte*)"glGenFramebuffersEXT" ) ) != NULL ) && result;
	result = ( ( __glBindFramebufferEXT = (glBindFramebufferEXT_t)myllyGetProcAddress( (const GLubyte*)"glBindFramebufferEXT" ) ) != NULL ) && result;
	result = ( ( __glDeleteFramebuffersEXT = (glDeleteFramebuffersEXT_t)myllyGetProcAddress( (const GLubyte*)"glDeleteFramebuffersEXT" ) ) != NULL ) && result;
	result = ( ( __glFramebufferTextureEXT = (glFramebufferTextureEXT_t)myllyGetProcAddress( (const GLubyte*)"glFramebufferTextureEXT" ) ) != NULL ) && result;
	result = ( ( __glCheckFramebufferStatusEXT = (glCheckFramebufferStatusEXT_t)myllyGetProcAddress( (const GLubyte*)"glCheckFramebufferStatusEXT" ) ) != NULL ) && result;

	MYLLY_EXT_framebuffer_object = result;
}

static void mgui_opengl_load_gl_2_0( void )
{
	bool result = true;

	result = ( ( __glDrawBuffers = (glDrawBuffers_t)myllyGetProcAddress( (const GLubyte*)"glDrawBuffers" ) ) != NULL ) && result;

	MYLLY_GL_2_0 = result;
}

static bool mgui_opengl_get_extension( const char* name )
{    
	register const char* s = (const char*)glGetString( GL_EXTENSIONS );
	size_t n, len = strlen( name );

	if ( s == NULL ) return false;

	while ( *s )
	{
		n = mgui_opengl_substr_len( s, ' ' );

		if ( n == len && strncmp( name, s, n ) )
			return true;

		s += n+1;
	}

	return false;
}

static size_t mgui_opengl_substr_len( const char* str, char c )
{
	size_t n = 0;
	register const char* s = str;

	if ( s == NULL ) return 0;

	while ( *s != '\0' && *s != c ) { s++; n++; }
	return *s == c ? n : 0;
}
