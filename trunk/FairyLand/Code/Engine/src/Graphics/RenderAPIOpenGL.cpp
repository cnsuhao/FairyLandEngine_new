//============================================================================
// RenderAPIOpenGL.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

#ifdef _OPENGL_RENDER

typedef GLvoid		(__stdcall * PFNGLBINDBUFFERARBPROC)				(GLenum target, GLuint buffer);
typedef GLvoid		(__stdcall * PFNGLDELETEBUFFERSARBPROC)			(GLsizei n, const GLuint *buffers);
typedef GLvoid		(__stdcall * PFNGLGENBUFFERSARBPROC)				(GLsizei n, GLuint *buffers);
typedef GLvoid*		(__stdcall * PFNGLMAPBUFFERARBPROC)				(GLenum target, GLenum access);
typedef GLboolean	(__stdcall * PFNGLUNMAPBUFFERARBPROC)			(GLenum target);
typedef GLvoid		(__stdcall * PFNGLBUFFERDATAARBPROC)				(GLenum target, int size, const GLvoid *data, GLenum usage);
typedef GLvoid		(__stdcall * PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)	(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
typedef GLvoid		(__stdcall * PFNGLSECONDARYCOLORPOINTEREXTPROC)	(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

// VBO Extension Function Pointers
PFNGLGENBUFFERSARBPROC				glGenBuffersARB				= 0;
PFNGLBINDBUFFERARBPROC				glBindBufferARB				= 0;
PFNGLBUFFERDATAARBPROC				glBufferDataARB				= 0;
PFNGLMAPBUFFERARBPROC				glMapBufferARB				= 0;
PFNGLUNMAPBUFFERARBPROC				glUnmapBufferARB			= 0;
PFNGLDELETEBUFFERSARBPROC			glDeleteBuffersARB			= 0;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC	glCompressedTexImage2DARB	= 0;
PFNGLSECONDARYCOLORPOINTEREXTPROC	glSecondaryColorPointerEXT	= 0;

//----------------------------------------------------------------------------
// RenderAPIOpenGL Implementation
//----------------------------------------------------------------------------

RenderAPIOpenGL::RenderAPIOpenGL( )
{
	mRGBAFormat			= _true;

	mWindowDC			= _null;
	mRenderContext		= _null;

	mVertexElementArray	= _null;

	mVertexSize			= 0;
	mVertexBuffer		= 0;

	mIndexSize			= 0;
	mIndexBuffer		= 0;

	mWorldTranform		= Matrix4::cIdentity;
}

_dword RenderAPIOpenGL::BeforeRender( )
{
	::glBindBufferARB( GL_ARRAY_BUFFER_ARB, mVertexBuffer );

	if ( mIndexBuffer != 0 )
		::glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexBuffer );

	_dword format = 0, offset = 0;

	for ( _dword i = 0; i < mVertexElementArray->Number( ); i ++ )
	{
		const VertexElement& element = (*mVertexElementArray)[i];

		switch ( element.mUsage )
		{
			case IVertexDeclaration::_USAGE_POSITION:
			{
				::glVertexPointer( 3, GL_FLOAT, mVertexSize, (_void*) offset );
				::glEnableClientState( GL_VERTEX_ARRAY );

				format |= Geometry::_FORMATFLAG_POSITION;
				offset += sizeof( _float ) * 3;

				break;
			}

			case IVertexDeclaration::_USAGE_NORMAL:
			{
				::glNormalPointer( GL_FLOAT, mVertexSize, (_void*) offset );
				::glEnableClientState( GL_NORMAL_ARRAY );

				format |= Geometry::_FORMATFLAG_NORMAL;
				offset += sizeof( _float ) * 3;

				break;
			}

			case IVertexDeclaration::_USAGE_DIFFUSE:
			{
				::glColorPointer( 4, GL_UNSIGNED_BYTE, mVertexSize, (_void*) offset );
				::glEnableClientState( GL_COLOR_ARRAY );

				format |= Geometry::_FORMATFLAG_DIFFUSE;
				offset += sizeof( _dword );

				break;
			}

			case IVertexDeclaration::_USAGE_SPECULAR:
			{
				//::glSecondaryColorPointerEXT( 4, GL_UNSIGNED_BYTE, mVertexSize, (_void*) offset );
				//::glEnableClientState( GL_SECONDARY_COLOR_ARRAY );

				format |= Geometry::_FORMATFLAG_DIFFUSE;
				offset += sizeof( _dword );

				break;
			}

			case IVertexDeclaration::_USAGE_TEXCOORD:
			{
				::glTexCoordPointer( 2, GL_FLOAT, mVertexSize, (_void*) offset );
				::glEnableClientState( GL_TEXTURE_COORD_ARRAY );

				format |= Geometry::_FORMATFLAG_TEXCOORD1;
				offset += sizeof( _float ) * 2;

				break;
			}

			case IVertexDeclaration::_USAGE_POSITIONT:
			{
				::glMatrixMode( GL_PROJECTION );
				::glPushMatrix( );
				::glLoadIdentity( );
				::glOrtho( 0, mDeviceWidth, mDeviceHeight, 0, 0, 1.0f );
		
				::glMatrixMode( GL_MODELVIEW );
				::glPushMatrix( );
				::glLoadIdentity( );

				::glVertexPointer( 2, GL_FLOAT, mVertexSize, (_void*) offset );
				::glEnableClientState( GL_VERTEX_ARRAY );

				format |= Geometry::_FORMATFLAG_POSITIONT;
				offset += sizeof( _float ) * 4;

				break;
			}
		}
	}

	::glMatrixMode( GL_MODELVIEW );
	::glPushMatrix( );
	::glMultMatrixf( (_float*) &mWorldTranform );

	return format;
}

_void RenderAPIOpenGL::AfterRender( _dword format )
{
	if ( format & Geometry::_FORMATFLAG_POSITION )
		::glDisableClientState( GL_VERTEX_ARRAY );

	if ( format & Geometry::_FORMATFLAG_NORMAL )
		::glDisableClientState( GL_NORMAL_ARRAY );

	if ( format & Geometry::_FORMATFLAG_DIFFUSE )
		::glDisableClientState( GL_COLOR_ARRAY );

	if ( format & Geometry::_FORMATFLAG_SPECULAR )
		::glDisableClientState( GL_SECONDARY_COLOR_ARRAY );

	if ( format & Geometry::_FORMATFLAG_TEXCOORD1 )
		::glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	if ( format & Geometry::_FORMATFLAG_POSITIONT )
	{
		::glDisableClientState( GL_VERTEX_ARRAY );
		::glMatrixMode( GL_PROJECTION );
		::glPopMatrix( );
		::glMatrixMode( GL_MODELVIEW );
		::glPopMatrix( );
	}

	if ( mIndexBuffer != 0 )
		::glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

	::glPopMatrix( );
}

_dword RenderAPIOpenGL::GetGLFormat( _dword format )
{
	switch ( format )
	{
		case ITextureFactory::_FORMAT_UNKNOWN:
			return 0;

		case ITextureFactory::_FORMAT_R8G8B8:
		case ITextureFactory::_FORMAT_A8R8G8B8:
		case ITextureFactory::_FORMAT_X8R8G8B8:
			return GL_UNSIGNED_BYTE;

		case ITextureFactory::_FORMAT_A4R4G4B4:
		case ITextureFactory::_FORMAT_X4R4G4B4:
			return GL_UNSIGNED_SHORT_4_4_4_4_REV;

		case ITextureFactory::_FORMAT_R5G6B5:
			return GL_UNSIGNED_SHORT_5_6_5_REV;

		case ITextureFactory::_FORMAT_A1R5G5B5:
			return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	}

	return 0;
}

_dword RenderAPIOpenGL::GetAPIType( ) const
{
	return _API_OGL;
}

_bool RenderAPIOpenGL::Initialize( _handle window, _dword devicetype )
{
	mWindowDC = ::GetDC( (HWND) window );
	if ( mWindowDC == _null )
		return _false;

	PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize		= sizeof( PIXELFORMATDESCRIPTOR );
    pfd.nVersion	= 1;
    pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType	= PFD_TYPE_RGBA;
    pfd.cColorBits	= 24;
    pfd.cDepthBits	= 16;
    pfd.iLayerType	= PFD_MAIN_PLANE;

	_long format = ::ChoosePixelFormat( (HDC) mWindowDC, &pfd );
	if ( ::SetPixelFormat( (HDC) mWindowDC, format, &pfd ) == _false )
		return _false;

	mRenderContext = ::wglCreateContext( (HDC) mWindowDC );
	::wglMakeCurrent( (HDC) mWindowDC, (HGLRC) mRenderContext );

	PS_TRACE1( L"[RD] CreateOpenGLDevice At Thread %d\r\n", System::GetCurrentThreadID( ) )

	::glDisable( GL_LIGHTING );
	::glFrontFace( GL_CW );
	::glEnable( GL_DEPTH_WRITEMASK );
	::glEnable( GL_DEPTH_TEST );
	::glDepthFunc( GL_LEQUAL );

	::glEnable( GL_TEXTURE_2D );
	::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// Get pointers to the GL functions.
	glGenBuffersARB				= (PFNGLGENBUFFERSARBPROC)				wglGetProcAddress( "glGenBuffersARB" );
	glBindBufferARB				= (PFNGLBINDBUFFERARBPROC)				wglGetProcAddress( "glBindBufferARB" );
	glBufferDataARB				= (PFNGLBUFFERDATAARBPROC)				wglGetProcAddress( "glBufferDataARB" );
	glMapBufferARB				= (PFNGLMAPBUFFERARBPROC)				wglGetProcAddress( "glMapBufferARB" );
	glUnmapBufferARB			= (PFNGLUNMAPBUFFERARBPROC)				wglGetProcAddress( "glUnmapBufferARB" );
	glDeleteBuffersARB			= (PFNGLDELETEBUFFERSARBPROC)			wglGetProcAddress( "glDeleteBuffersARB" );
	glCompressedTexImage2DARB	= (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)	wglGetProcAddress( "glCompressedTexImage2DARB" );
	glSecondaryColorPointerEXT	= (PFNGLSECONDARYCOLORPOINTEREXTPROC)	wglGetProcAddress( "glSecondaryColorPointerEXT" );

	FreeImage_Initialise( _true );

	Rect clientrect;
	::GetClientRect( (HWND) window, (RECT*) &clientrect );

	mDeviceType		= devicetype;
	mWindow			= window;
	mDeviceWidth	= clientrect.Width( );
	mDeviceHeight	= clientrect.Height( );

	return _true;
}

_void RenderAPIOpenGL::Finalize( )
{
	::FreeImage_DeInitialise( );
}

_bool RenderAPIOpenGL::IsDeviceLost( )
{
	return _false;
}

_bool RenderAPIOpenGL::ResizeDevice( _dword width, _dword height )
{
	mDeviceWidth  = width;
	mDeviceHeight = height;

	::glViewport( 0, 0, mDeviceWidth, mDeviceHeight );

	return _true;
}

_bool RenderAPIOpenGL::ResetDevice( )
{
	return _false;
}

_void RenderAPIOpenGL::ClearBuffer( _bool color, _bool depth, _bool stencil )
{
	_dword mask = 0;

	if ( color )
	{
		mask |= GL_COLOR_BUFFER_BIT;

		Color temp( mClearColor );
		::glClearColor( temp.r, temp.g, temp.b, temp.a );
	}

	if ( depth )
	{
		mask |= GL_DEPTH_BUFFER_BIT;

		::glClearDepth( mClearDepth );
	}

	if ( stencil )
	{
		mask |= GL_STENCIL_BUFFER_BIT;

		::glClearStencil( mClearStencil );
	}

	::glClear( mask );
}

_void RenderAPIOpenGL::BeginScene( )
{
}

_void RenderAPIOpenGL::EndScene( )
{
}

_void RenderAPIOpenGL::SetViewport( const Viewport& viewport )
{
	::glViewport( viewport.x, viewport.y, viewport.w, viewport.h );
	::glDepthRange( viewport.minz, viewport.maxz );
}

_void RenderAPIOpenGL::GetViewport( Viewport& viewport )
{
}

_void RenderAPIOpenGL::SetWorldTransform( const Matrix4& matrix )
{
	mWorldTranform = matrix;
}

_void RenderAPIOpenGL::GetWorldTransform( Matrix4& matrix )
{
	matrix = mWorldTranform;
}

_void RenderAPIOpenGL::SetViewTransform( const Matrix4& matrix )
{
	::glMatrixMode( GL_MODELVIEW );
	::glLoadMatrixf( (_float*) &matrix );
}

_void RenderAPIOpenGL::GetViewTransform( Matrix4& matrix )
{
}

_void RenderAPIOpenGL::SetProjectionTransform( const Matrix4& matrix )
{
	::glMatrixMode( GL_PROJECTION );
	::glLoadMatrixf( (_float*) &matrix );
}

_void RenderAPIOpenGL::GetProjectionTransform( Matrix4& matrix )
{
}

_void RenderAPIOpenGL::SetTextureTransform( _dword index, const Matrix4& matrix )
{
	::glMatrixMode( GL_TEXTURE );
	::glLoadMatrixf( (_float*) &matrix );
}

_void RenderAPIOpenGL::GetTextureTransform( _dword index, Matrix4& matrix )
{
}

_void RenderAPIOpenGL::EnableRenderState( _dword state, _dword value )
{
	switch ( state )
	{
		case D3DRS_FILLMODE:
		{
			if ( value == IRenderMethod::_FILLMODE_POINT )
				value = GL_POINT;
			else if ( value == IRenderMethod::_FILLMODE_WIREFRAME )
				value = GL_LINE;
			else if ( value == IRenderMethod::_FILLMODE_SOLID )
				value = GL_FILL;

			::glPolygonMode( GL_FRONT_AND_BACK, value );

			break;
		}

		case D3DRS_CULLMODE:
		{
			if ( value == IRenderMethod::_CULLMODE_NONE )
				::glDisable( GL_CULL_FACE );
			else if ( value == IRenderMethod::_CULLMODE_CCW )
				::glCullFace( GL_BACK );
			else if ( value == IRenderMethod::_CULLMODE_CCW )
				::glCullFace( GL_FRONT );

			break;
		}

		case D3DRS_SHADEMODE:
		{
			if ( value == IRenderMethod::_SHADEMODE_FLAT )
				::glShadeModel( GL_FLAT );
			else if ( value == IRenderMethod::_SHADEMODE_SMOOTH )
				::glShadeModel( GL_SMOOTH );

			break;
		}

		case D3DRS_CLIPPING:
		{
			break;
		}

		case D3DRS_NORMALIZENORMALS:
		{
			::glEnable( GL_NORMALIZE );

			break;
		}

		case D3DRS_LIGHTING:
		{
			::glEnable( GL_LIGHTING );

			break;
		}

		case D3DRS_SPECULARENABLE:
		{
			break;
		}

		case D3DRS_AMBIENT:
		{
			break;
		}

		case D3DRS_ZWRITEENABLE:
		{
			if ( value )
				::glEnable( GL_DEPTH_WRITEMASK );
			else
				::glDisable( GL_DEPTH_WRITEMASK );

			break;
		}

		case D3DRS_ZENABLE:
		{
			if ( value )
				::glEnable( GL_DEPTH_TEST );
			else
				::glDisable( GL_DEPTH_TEST );

			break;
		}

		case D3DRS_ZFUNC:
		case D3DRS_STENCILFUNC:
		case D3DRS_ALPHAFUNC:
		{
			if ( value == IRenderMethod::_COMPAREFUNC_NEVER )
				value = GL_NEVER;
			else if ( value == IRenderMethod::_COMPAREFUNC_LESS )
				value = GL_LESS;
			else if ( value == IRenderMethod::_COMPAREFUNC_EQUAL )
				value = GL_EQUAL;
			else if ( value == IRenderMethod::_COMPAREFUNC_LESSEQUAL )
				value = GL_LEQUAL;
			else if ( value == IRenderMethod::_COMPAREFUNC_GREATER )
				value = GL_GREATER;
			else if ( value == IRenderMethod::_COMPAREFUNC_NOTEQUAL )
				value = GL_NOTEQUAL;
			else if ( value == IRenderMethod::_COMPAREFUNC_GREATEREQUAL )
				value = GL_GEQUAL;
			else if ( value == IRenderMethod::_COMPAREFUNC_ALWAYS )
				value = GL_ALWAYS;

			if ( state == D3DRS_ZFUNC )
			{
				::glDepthFunc( value );
			}
			else if ( state == D3DRS_STENCILFUNC )
			{
				_long stencilref = 0;
				::glGetIntegerv( GL_STENCIL_REF, &stencilref );

				_long stencilmask = 0;
				::glGetIntegerv( GL_STENCIL_VALUE_MASK, &stencilmask );

				::glStencilFunc( value, stencilref, stencilmask );
			}
			else
			{
				_long alpharef = 0;
				::glGetIntegerv( GL_ALPHA_TEST_REF, &alpharef );

				::glAlphaFunc( value, alpharef / 255.0f );
			}

			break;
		}

		case D3DRS_STENCILENABLE:
		{
			if ( value )
				::glEnable( GL_STENCIL_TEST );
			else
				::glDisable( GL_STENCIL_TEST );

			break;
		}

		case D3DRS_STENCILREF:
		{
			_long stencilfunc = 0;
			::glGetIntegerv( GL_STENCIL_FUNC, &stencilfunc );

			_long stencilmask = 0;
			::glGetIntegerv( GL_STENCIL_VALUE_MASK, &stencilmask );

			::glStencilFunc( stencilfunc, value, stencilmask );

			break;
		}

		case D3DRS_STENCILMASK:
		{
			_long stencilfunc = 0;
			::glGetIntegerv( GL_STENCIL_FUNC, &stencilfunc );

			_long stencilmask = 0;
			::glGetIntegerv( GL_STENCIL_VALUE_MASK, &stencilmask );

			::glStencilFunc( stencilfunc, value, stencilmask );

			break;
		}

		case D3DRS_STENCILWRITEMASK:
		{
			_long stencilfunc = 0;
			::glGetIntegerv( GL_STENCIL_FUNC, &stencilfunc );

			_long stencilref = 0;
			::glGetIntegerv( GL_STENCIL_REF, &stencilref );

			::glStencilFunc( stencilfunc, stencilref, value );

			break;
		}

		case D3DRS_STENCILPASS:
		{
			break;
		}

		case D3DRS_STENCILFAIL:
		{
			break;
		}

		case D3DRS_STENCILZFAIL:
		{
			break;
		}

		case D3DRS_ALPHATESTENABLE:
		{
			if ( value )
				::glEnable( GL_ALPHA_TEST );
			else
				::glDisable( GL_ALPHA_TEST );

			break;
		}

		case D3DRS_ALPHAREF:
		{
			break;
		}

		case D3DRS_ALPHABLENDENABLE:
		{
			if ( value )
				::glEnable( GL_BLEND );
			else
				::glDisable( GL_BLEND );

			break;
		}

		case D3DRS_BLENDOP:
		{
			break;
		}

		case D3DRS_BLENDFACTOR:
		{
			break;
		}

		case D3DRS_SRCBLEND:
		case D3DRS_DESTBLEND:
		{
			if ( value == IRenderMethod::_BLENDFACTOR_ZERO )
				value = GL_ZERO;
			else if ( value == IRenderMethod::_BLENDFACTOR_ONE )
				value = GL_ONE;
			else if ( value == IRenderMethod::_BLENDFACTOR_SRCCOLOR )
				value = GL_SRC_COLOR;
			else if ( value == IRenderMethod::_BLENDFACTOR_INVSRCCOLOR )
				value = GL_ONE_MINUS_SRC_COLOR;
			else if ( value == IRenderMethod::_BLENDFACTOR_SRCALPHA )
				value = GL_SRC_ALPHA;
			else if ( value == IRenderMethod::_BLENDFACTOR_INVSRCALPHA )
				value = GL_ONE_MINUS_SRC_ALPHA;
			else if ( value == IRenderMethod::_BLENDFACTOR_DESTALPHA )
				value = GL_DST_ALPHA;
			else if ( value == IRenderMethod::_BLENDFACTOR_INVDESTALPHA )
				value = GL_ONE_MINUS_DST_ALPHA;
			else if ( value == IRenderMethod::_BLENDFACTOR_DESTCOLOR )
				value = GL_DST_COLOR;
			else if ( value == IRenderMethod::_BLENDFACTOR_INVDESTCOLOR )
				value = GL_ONE_MINUS_DST_COLOR;
			else if ( value == IRenderMethod::_BLENDFACTOR_SRCALPHASAT )
				value = GL_SRC_ALPHA_SATURATE;

			_long srcalpha = 0, desalpha = 0;

			if ( state == D3DRS_SRCBLEND )
			{
				srcalpha = (_long) value;
				::glGetIntegerv( GL_BLEND_DST, &desalpha );
			}
			else
			{
				desalpha = (_long) value;
				::glGetIntegerv( GL_BLEND_SRC, &srcalpha );
			}

			::glBlendFunc( srcalpha, desalpha );

			break;
		}

		case D3DRS_FOGENABLE:
		{
			break;
		}

		case D3DRS_RANGEFOGENABLE:
		{
			break;
		}

		case D3DRS_FOGVERTEXMODE:
		{
			break;
		}

		case D3DRS_FOGTABLEMODE:
		{
			break;
		}

		case D3DRS_FOGCOLOR:
		{
			break;
		}

		case D3DRS_FOGSTART:
		{
			break;
		}

		case D3DRS_FOGEND:
		{
			break;
		}

		case D3DRS_FOGDENSITY:
		{
			break;
		}

		case D3DRS_MULTISAMPLEANTIALIAS:
		{
			break;
		}

		case D3DRS_MULTISAMPLEMASK:
		{
			break;
		}

		case D3DRS_COLORWRITEENABLE:
		{
			break;
		}
	}
}

_void RenderAPIOpenGL::DisableRenderState( _dword state )
{
	switch ( state )
	{
		case D3DRS_FILLMODE:
		{
			::glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

			break;
		}

		case D3DRS_CULLMODE:
		{
			::glEnable( GL_CULL_FACE );
			::glCullFace( GL_BACK );

			break;
		}

		case D3DRS_SHADEMODE:
		{
			::glShadeModel( GL_SMOOTH );

			break;
		}

		case D3DRS_CLIPPING:
		{
			break;
		}

		case D3DRS_NORMALIZENORMALS:
		{
			::glDisable( GL_NORMALIZE );

			break;
		}

		case D3DRS_LIGHTING:
		{
			::glDisable( GL_LIGHTING );

			break;
		}

		case D3DRS_SPECULARENABLE:
		{
			break;
		}

		case D3DRS_AMBIENT:
		{
			break;
		}

		case D3DRS_ZWRITEENABLE:
		{
			::glDisable( GL_DEPTH_WRITEMASK );

			break;
		}

		case D3DRS_ZENABLE:
		{
			::glDisable( GL_DEPTH_TEST );

			break;
		}

		case D3DRS_ZFUNC:
		case D3DRS_STENCILFUNC:
		case D3DRS_ALPHAFUNC:
		{
			break;
		}

		case D3DRS_STENCILENABLE:
		{
			break;
		}

		case D3DRS_STENCILREF:
		{
			break;
		}

		case D3DRS_STENCILMASK:
		{
			break;
		}

		case D3DRS_STENCILWRITEMASK:
		{
			break;
		}

		case D3DRS_STENCILPASS:
		{
			break;
		}

		case D3DRS_STENCILFAIL:
		{
			break;
		}

		case D3DRS_STENCILZFAIL:
		{
			break;
		}

		case D3DRS_ALPHATESTENABLE:
		{
			::glDisable( GL_ALPHA_TEST );

			break;
		}

		case D3DRS_ALPHAREF:
		{
			break;
		}

		case D3DRS_ALPHABLENDENABLE:
		{
			::glDisable( GL_BLEND );

			break;
		}

		case D3DRS_BLENDOP:
		{
			break;
		}

		case D3DRS_BLENDFACTOR:
		{
			break;
		}

		case D3DRS_SRCBLEND:
		case D3DRS_DESTBLEND:
		{
			break;
		}

		case D3DRS_FOGENABLE:
		{
			break;
		}

		case D3DRS_RANGEFOGENABLE:
		{
			break;
		}

		case D3DRS_FOGVERTEXMODE:
		{
			break;
		}

		case D3DRS_FOGTABLEMODE:
		{
			break;
		}

		case D3DRS_FOGCOLOR:
		{
			break;
		}

		case D3DRS_FOGSTART:
		{
			break;
		}

		case D3DRS_FOGEND:
		{
			break;
		}

		case D3DRS_FOGDENSITY:
		{
			break;
		}

		case D3DRS_MULTISAMPLEANTIALIAS:
		{
			break;
		}

		case D3DRS_MULTISAMPLEMASK:
		{
			break;
		}

		case D3DRS_COLORWRITEENABLE:
		{
			break;
		}
	}
}

_void RenderAPIOpenGL::EnableSamplerState( _dword stage, _dword state, _dword value )
{
	return; 

	switch ( state )
	{
		case D3DSAMP_ADDRESSU:
		{
			if ( value == ISamplerMethod::_TEXTUREADDR_WRAP )
				value = GL_REPEAT;
			else if ( value == ISamplerMethod::_TEXTUREADDR_CLAMP )
				value = GL_CLAMP;

			::glEnable( GL_TEXTURE_2D );
			::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, value );

			break;
		}

		case D3DSAMP_ADDRESSV:
		{
			if ( value == ISamplerMethod::_TEXTUREADDR_WRAP )
				value = GL_REPEAT;
			else if ( value == ISamplerMethod::_TEXTUREADDR_CLAMP )
				value = GL_CLAMP;

			::glEnable( GL_TEXTURE_2D );
			::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, value );

			break;
		}

		case D3DSAMP_ADDRESSW:
		{
			break;
		}

		case D3DSAMP_BORDERCOLOR:
		{
			break;
		}

		case D3DSAMP_MAGFILTER:
		{
			if ( value == ISamplerMethod::_TEXTUREFILTER_POINT )
				value = GL_NEAREST;
			else if ( value == ISamplerMethod::_TEXTUREFILTER_LINEAR )
				value = GL_LINEAR;

			::glEnable( GL_TEXTURE_2D );
			::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, value );

			break;
		}

		case D3DSAMP_MINFILTER:
		{
			if ( value == ISamplerMethod::_TEXTUREFILTER_POINT )
				value = GL_NEAREST;
			else if ( value == ISamplerMethod::_TEXTUREFILTER_LINEAR )
				value = GL_LINEAR;

			::glEnable( GL_TEXTURE_2D );
			::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, value );

			break;
		}

		case D3DSAMP_MIPFILTER:
		{
			break;
		}

		case D3DSAMP_MIPMAPLODBIAS:
		{
			break;
		}

		case D3DSAMP_MAXMIPLEVEL:
		{
			break;
		}

		case D3DSAMP_MAXANISOTROPY:
		{
			break;
		}
	}
}

_void RenderAPIOpenGL::DisableSamplerState( _dword stage, _dword state )
{
	switch ( state )
	{
		case D3DSAMP_ADDRESSU:
		{
			break;
		}

		case D3DSAMP_ADDRESSV:
		{
			break;
		}

		case D3DSAMP_ADDRESSW:
		{
			break;
		}

		case D3DSAMP_BORDERCOLOR:
		{
			break;
		}

		case D3DSAMP_MAGFILTER:
		{
			break;
		}

		case D3DSAMP_MINFILTER:
		{
			break;
		}

		case D3DSAMP_MIPFILTER:
		{
			break;
		}

		case D3DSAMP_MIPMAPLODBIAS:
		{
			break;
		}

		case D3DSAMP_MAXMIPLEVEL:
		{
			break;
		}

		case D3DSAMP_MAXANISOTROPY:
		{
			break;
		}
	}
}

_void RenderAPIOpenGL::EnableTextureState( _dword stage, _dword state, _dword value )
{
}

_void RenderAPIOpenGL::DisableTextureState( _dword stage, _dword state )
{
}

_void* RenderAPIOpenGL::CreateVertexDeclaration( _void* vertexdecl, const Array< VertexElement >& elementarray )
{
	mVertexElementArray = (Array< VertexElement >*) &elementarray;

	return _null;
}

_void RenderAPIOpenGL::ReleaseVertexDeclaration( _void* vertexdecl )
{
}

_void* RenderAPIOpenGL::CreateVertexBuffer( _dword type, _dword size )
{
	_dword glbuffer = 0;
	::glGenBuffersARB( 1, (GLuint*) &glbuffer );

	if ( glbuffer == 0 )
		return _null;

	::glBindBufferARB( GL_ARRAY_BUFFER_ARB, glbuffer );
	::glBufferDataARB( GL_ARRAY_BUFFER_ARB, size, 0, GL_DYNAMIC_DRAW_ARB );

	return (_void*) glbuffer;
}

_void* RenderAPIOpenGL::LockVertexBuffer( _void* vertexbuffer, _dword offset, _dword length, _dword flag )
{
	::glBindBufferARB( GL_ARRAY_BUFFER_ARB, (_dword) vertexbuffer );
	return ::glMapBufferARB( GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB );
}

_void RenderAPIOpenGL::UnlockVertexBuffer( _void* vertexbuffer )
{
	::glBindBufferARB( GL_ARRAY_BUFFER_ARB, (_dword) vertexbuffer );
	::glUnmapBufferARB( GL_ARRAY_BUFFER_ARB );
}

_void RenderAPIOpenGL::SetVertexBuffer( _void* vertexbuffer, _dword vertexsize )
{
	mVertexBuffer	= (_dword) vertexbuffer;
	mVertexSize		= vertexsize;
}

_void RenderAPIOpenGL::ReleaseVertexBuffer( _void* vertexbuffer )
{
	_dword glbuffer = (_dword) vertexbuffer;
	::glDeleteBuffersARB( 1, (GLuint*) &glbuffer );
}

_void* RenderAPIOpenGL::CreateIndexBuffer( _dword type, _dword size, _dword indexsize )
{
	_dword glbuffer = 0;
	::glGenBuffersARB( 1, (GLuint*) &glbuffer );

	if ( glbuffer == 0 )
		return _null;

	::glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, glbuffer );
	::glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, size, 0, GL_DYNAMIC_DRAW_ARB );

	return (_void*) glbuffer;
}

_void* RenderAPIOpenGL::LockIndexBuffer( _void* indexbuffer, _dword offset, _dword length, _dword flag )
{
	::glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, (_dword) indexbuffer );
	return ::glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB );
}

_void RenderAPIOpenGL::UnlockIndexBuffer( _void* indexbuffer )
{
	::glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, (_dword) indexbuffer );
	::glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB );
}

_void RenderAPIOpenGL::SetIndexBuffer( _void* indexbuffer, _dword indexsize )
{
	mIndexBuffer	= (_dword) indexbuffer;
	mIndexSize		= indexsize;
}

_void RenderAPIOpenGL::ReleaseIndexBuffer( _void* indexbuffer )
{
	_dword glbuffer = (_dword) indexbuffer;
	::glDeleteBuffersARB( 1, (GLuint*) &glbuffer );
}

_bool RenderAPIOpenGL::GetTextureInfo( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format, _dword& mip )
{
	return _false;
}

_void* RenderAPIOpenGL::CreateTexture( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format, _dword& mip )
{
	FIMEMORY* fimem = ::FreeImage_OpenMemory( (BYTE*) buffer, length );
	if ( fimem == 0 )
		return _null;

	FREE_IMAGE_FORMAT fifmt = ::FreeImage_GetFileTypeFromMemory( fimem, 0 );
	if ( fifmt == FIF_UNKNOWN )
	{
		::FreeImage_CloseMemory( fimem );
		return _null;
	}

	_dword gltexture = 0;
	::glGenTextures( 1, (GLuint*) &gltexture );

	if ( gltexture == 0 )
		return _null;

	::glBindTexture( GL_TEXTURE_2D, gltexture );

	if ( fifmt == FIF_DDS )
	{
		struct DDS_PIXELFORMAT
		{
			_dword dwSize;
			_dword dwFlags;
			_dword dwFourCC;
			_dword dwRGBBitCount;
			_dword dwRBitMask;
			_dword dwGBitMask;
			_dword dwBBitMask;
			_dword dwABitMask;
		};

		struct DDS_HEADER
		{
			_dword dwSize;
			_dword dwFlags;
			_dword dwHeight;
			_dword dwWidth;
			_dword dwPitchOrLinearSize;
			_dword dwDepth;
			_dword dwMipMapCount;
			_dword dwReserved1[11];
			DDS_PIXELFORMAT ddspf;
			_dword dwCaps1;
			_dword dwCaps2;
			_dword dwReserved2[3];
		};

		const _dword DDS_FOURCC  = 0x00000004;
		const _dword FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
		const _dword FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
		const _dword FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

		DDS_HEADER* header = (DDS_HEADER*) ( (_byte*) buffer + 4 );

		if ( ( header->ddspf.dwFlags & DDS_FOURCC ) == 0 )
		{
			::glDeleteTextures( 1, (GLuint*) &gltexture );
			return _null;
		}

		switch ( header->ddspf.dwFourCC )
        {
            case FOURCC_DXT1:	format = ITextureFactory::_FORMAT_DXT1; break;
            case FOURCC_DXT3:	format = ITextureFactory::_FORMAT_DXT3; break;
			case FOURCC_DXT5:	format = ITextureFactory::_FORMAT_DXT5; break;
        }

		width  = header->dwWidth;
		height = header->dwHeight;

		_dword w = width, h = height, size = 0, i = 0;
		_byte* bits = (_byte*) header + header->dwSize;

		while ( w > 0 && h > 0 )
		{
			size = ( ( w + 3 ) / 4 ) * ( ( h + 3 ) / 4 ) * ( format == ITextureFactory::_FORMAT_DXT1 ? 8 : 16 );

			::glCompressedTexImage2DARB( GL_TEXTURE_2D, i, PSDFormat2APIFormat( format ), w, h, 0, size, bits );
			bits += size;

			i ++;
			w >>= 1;
			h >>= 1;
		}
	}
	else
	{
		FIBITMAP* fibmp = ::FreeImage_LoadFromMemory( fifmt, fimem );

		width  = ::FreeImage_GetWidth( fibmp );
		height = ::FreeImage_GetHeight( fibmp );

		_dword pixel = ::FreeImage_GetBPP( fibmp );

		if ( pixel != 24 && pixel != 32 )
		{
			FIBITMAP* fitemp = ::FreeImage_ConvertTo24Bits( fibmp );
			::FreeImage_Unload( fibmp );

			fibmp = fitemp;
		}

		format = pixel == 24 ? ITextureFactory::_FORMAT_R8G8B8 : ITextureFactory::_FORMAT_A8R8G8B8;

		::gluBuild2DMipmaps( GL_TEXTURE_2D, pixel == 24 ? GL_RGB : GL_RGBA, width, height,
			PSDFormat2APIFormat( format ), GetGLFormat( format ), ::FreeImage_GetBits( fibmp ) );

		::FreeImage_Unload( fibmp );
	}

	return (_void*) gltexture;
}

_void* RenderAPIOpenGL::CreateTexture( _dword width, _dword height, _dword format, _dword mip )
{
	_dword gltexture = 0;
	::glGenTextures( 1, (GLuint*) &gltexture );

	if ( gltexture == 0 )
		return _null;

	::glBindTexture( GL_TEXTURE_2D, gltexture );

	for ( _dword i = 0; i < mip; i ++ )
	{
		::glTexImage2D( GL_TEXTURE_2D, i, GetPixelSize( format ), width / ( i + 1 ), height / ( i + 1 ),
			0, PSDFormat2APIFormat( format ), GetGLFormat( format ), 0 );
	}

	return (_void*) gltexture;
}

_void* RenderAPIOpenGL::CreateRenderTexture( _dword width, _dword height, _dword format )
{
	return _null;
}

_void* RenderAPIOpenGL::CreateDepthTexture( _dword width, _dword height )
{
	return _null;
}

_void* RenderAPIOpenGL::LockTexture( _void* texture, _dword format, _dword level, _dword& pitch, const Rect& lockrect )
{
	if ( texture == _null )
		return _null;

	if ( lockrect.Width( ) == 0 || lockrect.Height( ) == 0 )
		return _null;

	_dword pixel = GetPixelSize( format );
	if ( pixel == 0 )
		return _null;

	_dword width  = lockrect.Width( );
	_dword height = lockrect.Height( );

	pitch = width * pixel;

	_byte* buffer = new _byte[ width * height * pixel ];
	Memory::MemSet( buffer, 0, width * height * pixel );

	return buffer;
}

_void RenderAPIOpenGL::UnlockTexture( _void* texture, _dword format, _dword level, _void* buffer, const Rect& lockrect )
{
	if ( texture == _null )
		return;

	_dword width  = lockrect.Width( );
	_dword height = lockrect.Height( );

	::glBindTexture( GL_TEXTURE_2D, (_dword) texture );
	::glTexSubImage2D( GL_TEXTURE_2D, level, lockrect.l, lockrect.t, width, height,
		PSDFormat2APIFormat( format ), GetGLFormat( format ), buffer );

	delete[] (_byte*) buffer;
}

_void RenderAPIOpenGL::SetTexture( _dword stage, _void* texture )
{
	::glEnable( GL_TEXTURE_2D );
	::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	if ( texture == _null )
	{
		::glBindTexture( GL_TEXTURE_2D, (_dword) 0 );
	}
	else
	{
		::glBindTexture( GL_TEXTURE_2D, (_dword) texture );
	}
}

_void RenderAPIOpenGL::SaveTexture( _void* texture, StringPtr filename, _dword format )
{
}

_void RenderAPIOpenGL::GenerateMipmaps( _void* texture )
{
}

_void RenderAPIOpenGL::ReleaseTexture( _void* texture )
{
	_dword gltexture = (_dword) texture;
	::glDeleteTextures( 1, (GLuint*) &gltexture );
}

_void* RenderAPIOpenGL::GetSurface( _void* texture, _dword level )
{
	return _null;
}

_void* RenderAPIOpenGL::CreateSurface( _dword width, _dword height, _dword format )
{
	return _null;
}

_void* RenderAPIOpenGL::CreateSurface( const _void* buffer, _dword length, _dword& width, _dword& height, _dword& format )
{
	return _null;
}

_void* RenderAPIOpenGL::LockSurface( _void* surface, _dword format, _dword& pitch, const Rect& lockrect )
{
	return _null;
}

_void RenderAPIOpenGL::UnlockSurface( _void* surface, _dword format, _void* buffer, const Rect& lockrect )
{
}

_void RenderAPIOpenGL::ReleaseSurface( _void* surface )
{
}

_dword RenderAPIOpenGL::GetRenderTargetFormat( ) const
{
	return 0;
}

_void* RenderAPIOpenGL::CreateRenderTargetSurface( _dword width, _dword height, _dword format )
{
	return _null;
}

_void* RenderAPIOpenGL::GetDefaultRenderTargetSurface( )
{
	return _null;
}

_void* RenderAPIOpenGL::GetCurrentRenderTargetSurface( )
{
	return _null;
}

_void RenderAPIOpenGL::SetRenderTargetSurface( _void* rendertarget )
{
}

_dword RenderAPIOpenGL::GetDepthStencilFormat( ) const
{
	return 0;
}

_void* RenderAPIOpenGL::CreateDepthStencilSurface( _dword width, _dword height )
{
	return _null;
}

_void* RenderAPIOpenGL::GetDefaultDepthStencilSurface( )
{
	return _null;
}

_void* RenderAPIOpenGL::GetCurrentDepthStencilSurface( )
{
	return _null;
}

_void RenderAPIOpenGL::SetDepthStencilSurface( _void* depthstencil )
{
}

_void RenderAPIOpenGL::SetMaterial( _dword diffuse, _dword ambient, _dword specular, _dword emissive, _float power )
{
	{
		Color color = Color( diffuse );
		_float glval[4] = { color.r, color.g, color.b, color.a };
		::glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, glval );
	}

	{
		Color color = Color( ambient );
		_float glval[4] = { color.r, color.g, color.b, color.a };
		::glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, glval );
	}

	{
		Color color = Color( specular );
		_float glval[4] = { color.r, color.g, color.b, color.a };
		::glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, glval );
	}

	{
		Color color = Color( emissive );
		_float glval[4] = { color.r, color.g, color.b, color.a };
		::glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, glval );
	}

	::glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, power );
}

_void RenderAPIOpenGL::SetLight( _dword index, _void* light )
{
	index = GL_LIGHT0 + index;

	if ( light != _null )
	{
		D3DLIGHT9* d3dlight = (D3DLIGHT9*) light;

		::glEnable( index );

		{
			D3DCOLORVALUE color = d3dlight->Diffuse;
			_float glval[4] = { color.r, color.g, color.b, color.a };
			::glLightfv( index, GL_DIFFUSE, glval );
		}

		{
			D3DCOLORVALUE color = d3dlight->Ambient;
			_float glval[4] = { color.r, color.g, color.b, color.a };
			::glLightfv( index, GL_AMBIENT, glval );
		}

		{
			D3DCOLORVALUE color = d3dlight->Specular;
			_float glval[4] = { color.r, color.g, color.b, color.a };
			::glLightfv( index, GL_SPECULAR, glval );
		}

		if ( d3dlight->Type == D3DLIGHT_POINT )
		{
			::glLightfv( index, GL_POSITION, (_float*) &( d3dlight->Position ) );
		}
		else if ( d3dlight->Type == D3DLIGHT_DIRECTIONAL )
		{
			::glLightfv( index, GL_SPOT_DIRECTION, (_float*) &( d3dlight->Direction ) );
		}

		::glLightf( index, GL_CONSTANT_ATTENUATION, d3dlight->Attenuation0 );
		::glLightf( index, GL_LINEAR_ATTENUATION, d3dlight->Attenuation1 );
		::glLightf( index, GL_QUADRATIC_ATTENUATION, d3dlight->Attenuation2 );
		::glLightf( index, GL_SPOT_CUTOFF, d3dlight->Range );
	}
	else
	{
		::glDisable( index );
	}
}

_void* RenderAPIOpenGL::CreateVertexShader( StringPtr code )
{
	return _null;
}

_dword RenderAPIOpenGL::GetVertexShaderVersion( ) const
{
	return 0;
}

_void* RenderAPIOpenGL::CreateVertexShader( StringPtr code, StringPtr function, StringPtr profile )
{
	return _null;
}

_void RenderAPIOpenGL::SetVertexShader( _void* vertexshader )
{
}

_void RenderAPIOpenGL::SetVertexShaderConstant( _dword index, const _float* constant, _dword number )
{
}

_void RenderAPIOpenGL::ReleaseVertexShader( _void* vertexshader )
{
}

_dword RenderAPIOpenGL::GetPixelShaderVersion( ) const
{
	return 0;
}

_void* RenderAPIOpenGL::CreatePixelShader( StringPtr code )
{
	return _null;
}

_void* RenderAPIOpenGL::CreatePixelShader( StringPtr code, StringPtr function, StringPtr profile )
{
	return _null;
}

_void RenderAPIOpenGL::SetPixelShader( _void* pixelshader )
{
}

_void RenderAPIOpenGL::SetPixelShaderConstant( _dword index, const _float* constant, _dword number )
{
}

_void RenderAPIOpenGL::ReleasePixelShader( _void* pixelshader )
{
}

_void RenderAPIOpenGL::RenderPrimitive( _dword type, _dword svertex, _dword pnumber )
{
	if ( mVertexBuffer == 0 || mVertexElementArray == _null )
		return;

	_dword format = BeforeRender( ), vertexnumber = 0, polygonnumber = 0;

	switch ( type )
	{
		case Geometry::_GEOMETRY_POINTLIST:
		{
			vertexnumber	= pnumber;

			::glDrawArrays( GL_POINTS, svertex, pnumber );

			break;
		}

		case Geometry::_GEOMETRY_LINELIST:
		{
			vertexnumber	= pnumber * 2;

			::glDrawArrays( GL_LINES, svertex, pnumber * 2 );

			break;
		}

		case Geometry::_GEOMETRY_LINESTRIP:
		{
			vertexnumber	= pnumber + 1;

			::glDrawArrays( GL_LINE_STRIP, svertex, pnumber + 1 );

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLELIST:
		{
			vertexnumber	= pnumber * 3;
			polygonnumber	= pnumber;

			::glDrawArrays( GL_TRIANGLES, svertex, pnumber * 3 );

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLESTRIP:
		{
			vertexnumber	= pnumber + 2;
			polygonnumber	= pnumber;

			::glDrawArrays( GL_TRIANGLE_STRIP, svertex, pnumber + 2 );

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLEFAN:
		{
			vertexnumber	= pnumber + 2;
			polygonnumber	= pnumber;

			::glDrawArrays( GL_TRIANGLE_FAN, svertex, pnumber + 2 );

			break;
		}
	}

	( (RenderStatistics&) GetRenderStatistics( ) ).DrawPrimitive( vertexnumber, polygonnumber );

	AfterRender( format );
}

_void RenderAPIOpenGL::RenderPrimitive( _dword type, _dword svertex, _dword sindex, _dword vnumber, _dword pnumber )
{
	if ( mVertexBuffer == 0 || mIndexBuffer == 0 || mVertexElementArray == _null )
		return;

	_dword format = BeforeRender( ), vertexnumber = 0, polygonnumber = 0;

	switch ( type )
	{
		case Geometry::_GEOMETRY_POINTLIST:
		{
			vertexnumber	= vnumber;

			::glDrawElements( GL_POINTS, pnumber, mIndexSize == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0 );

			break;
		}

		case Geometry::_GEOMETRY_LINELIST:
		{
			vertexnumber	= vnumber;

			::glDrawElements( GL_LINES, pnumber * 2, mIndexSize == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0 );

			break;
		}

		case Geometry::_GEOMETRY_LINESTRIP:
		{
			vertexnumber	= vnumber;

			::glDrawElements( GL_LINE_STRIP, pnumber + 1, mIndexSize == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0 );

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLELIST:
		{
			vertexnumber	= vnumber;
			polygonnumber	= pnumber;

			::glDrawElements( GL_TRIANGLES, pnumber * 3, mIndexSize == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0 );

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLESTRIP:
		{
			vertexnumber	= vnumber;
			polygonnumber	= pnumber;

			::glDrawElements( GL_TRIANGLE_STRIP, pnumber + 2, mIndexSize == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0 );

			break;
		}

		case Geometry::_GEOMETRY_TRIANGLEFAN:
		{
			vertexnumber	= vnumber;
			polygonnumber	= pnumber;

			::glDrawElements( GL_TRIANGLE_FAN, pnumber + 2, mIndexSize == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0 );

			break;
		}
	}

	( (RenderStatistics&) GetRenderStatistics( ) ).DrawPrimitive( vertexnumber, polygonnumber );

	AfterRender( format );
}

_void RenderAPIOpenGL::Present( _handle window )
{
	::SwapBuffers( (HDC) mWindowDC );
}

_dword RenderAPIOpenGL::APIFormat2PSDFormat( _dword format ) const
{
	switch ( format )
	{
		case GL_BGR:
		case GL_RGB:
			return ITextureFactory::_FORMAT_R8G8B8;

		case GL_BGRA:
		case GL_RGBA:
			return ITextureFactory::_FORMAT_A8R8G8B8;

		case GL_RGBA4:
			return ITextureFactory::_FORMAT_A4R4G4B4;

		case GL_RGB16:
			return ITextureFactory::_FORMAT_R5G6B5;

		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			return ITextureFactory::_FORMAT_DXT1;

		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			return ITextureFactory::_FORMAT_DXT3;

		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			return ITextureFactory::_FORMAT_DXT5;

		case GL_LUMINANCE8_ALPHA8:
			return ITextureFactory::_FORMAT_A8L8;
	}

	return 0;
}

_dword RenderAPIOpenGL::PSDFormat2APIFormat( _dword format ) const
{
	switch ( format )
	{
		case ITextureFactory::_FORMAT_UNKNOWN:
			return 0;

		case ITextureFactory::_FORMAT_R8G8B8:
			return GL_BGR;

		case ITextureFactory::_FORMAT_A8R8G8B8:
			return GL_BGRA;

		case ITextureFactory::_FORMAT_X8R8G8B8:
			return GL_BGRA;

		case ITextureFactory::_FORMAT_A4R4G4B4:
			return GL_BGRA;

		case ITextureFactory::_FORMAT_X4R4G4B4:
			return GL_BGRA;

		case ITextureFactory::_FORMAT_R5G6B5:
			return GL_BGR;

		case ITextureFactory::_FORMAT_A1R5G5B5:
			return GL_BGRA;

		case ITextureFactory::_FORMAT_DXT1:
			return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;

		case ITextureFactory::_FORMAT_DXT3:
			return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;

		case ITextureFactory::_FORMAT_DXT5:
			return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

		case ITextureFactory::_FORMAT_A8L8:
			return GL_LUMINANCE8_ALPHA8;
	}

	return 0;
}

#endif