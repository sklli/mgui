/**********************************************************************
 *
 * PROJECT:		Mylly GUI
 * FILE:		Renderer.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		An interface for the GUI renderer.
 *
 *				(c) Tuomo Jauhiainen 2012-13
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_GUI_RENDERER_H
#define __MYLLY_GUI_RENDERER_H

#include "MGUI/MGUI.h"
#include "Math/Matrix4.h"
#include "Math/Vector3.h"

typedef enum {
	DRAWING_INVALID,
	DRAWING_2D,			// Draw 2D entities on top of everything else
	DRAWING_2D_DEPTH,	// Draw 2D entities, depth test enabled
	DRAWING_3D,			// Draw 3D entities (use with set_draw_transform)
} DRAW_MODE;

enum {
	TFLAG_NONE			= 0,
	TFLAG_TAGS			= 1 << 0,	// Text has format tag support
	TFLAG_SHADOW		= 1 << 1,	// Text has a shadow
	TFLAG_BOLD			= 1 << 2,	// Font used by the text is bold
	TFLAG_ITALIC		= 1 << 3,	// Font used by the text is cursive
};

enum {
	TAG_NONE			= 0,		// No special characteristics
	TAG_COLOUR			= 1 << 0,	// Tag specifies a custom colour
	TAG_COLOUR_END		= 1 << 1,	// Tag returns to default colour
	TAG_UNDERLINE		= 1 << 2,	// Enable underlining
	TAG_UNDERLINE_END	= 1 << 3,	// Enable underlining
};

typedef struct {
	uint16		index;		// Text buffer start index
	uint16		flags;		// Tag flags (see enum above)
	colour_t	colour;		// The colour that should be used with this tag
} MGuiFormatTag;

typedef struct {
	uint32	width;		// Width of the target texture
	uint32	height;		// Height of the target texture
} MGuiCache;

struct MGuiRenderer
{
	// --------------------------------------------------
	// Scene begin/end
	// --------------------------------------------------
	void			( *begin )					( void );
	void			( *end )					( void );
	void			( *resize )					( uint32 w, uint32 h );

	// --------------------------------------------------
	// Draw modes
	// --------------------------------------------------
	DRAW_MODE		( *set_draw_mode )			( DRAW_MODE mode );
	void			( *set_draw_colour )		( const colour_t* col );
	void			( *set_draw_depth )			( float z_depth );
	void			( *set_draw_transform )		( const matrix4_t* mat );	
	void			( *reset_draw_transform )	( void );

	void			( *start_clip )				( int32 x, int32 y, uint32 w, uint32 h );
	void			( *end_clip )				( void );

	// --------------------------------------------------
	// Primitive rendering
	// --------------------------------------------------
	void			( *draw_rect )				( int32 x, int32 y, uint32 w, uint32 h );
	void			( *draw_triangle )			( int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3 );
	void			( *draw_pixel )				( int32 x, int32 y );

	// --------------------------------------------------
	// Textures and textured primitive rendering
	// --------------------------------------------------
	void*			( *load_texture )			( const char_t* path, uint32* width, uint32* height );
	void			( *destroy_texture )		( void* texture );
	void			( *draw_textured_rect )		( const void* texture, int32 x, int32 y, uint32 w, uint32 h, const float uv[] );

	// --------------------------------------------------
	// Text rendering and fonts
	// --------------------------------------------------
	void*			( *load_font )				( const char_t* font, uint32 size, uint32 flags, uint32 charset,
												  uint32 firstc, uint32 lastc );

	void			( *destroy_font )			( void* font );

	void			( *draw_text )				( const void* font, const char_t* text, int32 x, int32 y,
												  uint32 flags, const MGuiFormatTag tags[], uint32 ntags );

	void			( *measure_text )			( const void* font, const char_t* text, uint32* w, uint32* h );

	// --------------------------------------------------
	// Cached rendering
	// --------------------------------------------------
	MGuiCache*		( *create_render_target )	( uint32 width, uint32 height );
	void			( *destroy_render_target )	( MGuiCache* target );
	void			( *draw_render_target )		( const MGuiCache* target, int32 x, int32 y, uint32 w, uint32 h );
	void			( *enable_render_target )	( const MGuiCache* target, int32 x, int32 y );
	void			( *disable_render_target )	( const MGuiCache* target );

	// --------------------------------------------------
	// Utility functions
	// --------------------------------------------------
	void			( *screen_pos_to_world )	( const vector3_t* src, vector3_t* dst );
	void			( *world_pos_to_screen )	( const vector3_t* src, vector3_t* dst );
};

#endif /* __MYLLY_GUI_RENDERER_H */
