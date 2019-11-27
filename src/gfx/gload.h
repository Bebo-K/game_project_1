#ifndef GLOAD_H
#define GLOAD_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <windows.h>
#include <wingdi.h>
#define GETGLPROC(a) wglGetProcAddress(a)


//Typedef function pointers 
/*
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLUSEPROGRAMPROC glUseProgram;

typedef void   (*ptr_glGenBuffers)(GLsizei,GLuint*);
typedef void   (*ptr_glBufferData)(GLenum,GLsizeiptr,const GLvoid*,GLenum);
typedef GLuint (*ptr_glCreateProgram)(void);
typedef GLuint (*ptr_glCreateShader)(GLenum);
typedef void   (*ptr_glShaderSource)(GLuint,GLsizei,const GLchar* const*, const GLint*);
typedef void   (*ptr_glCompileShader)(GLuint);
typedef void   (*ptr_glGetShaderiv)(GLuint,GLenum,GLint*);
typedef void   (*ptr_glAttachShader)(GLuint,GLuint);
typedef void   (*ptr_glLinkProgram)(GLuint);
typedef void   (*ptr_glGetProgramiv)(GLuint,GLenum,GLint*);
typedef GLint  (*ptr_glGetUniformLocation)(GLuint,const GLchar*);
typedef void   (*ptr_glBindAttribLocation)(GLuint,GLuint,const GLchar*);
typedef void   (*ptr_glGetShaderInfoLog)(GLuint,GLsizei,GLsizei*,GLchar*);
typedef void   (*ptr_glUseProgram)(GLuint);
typedef void   (*ptr_glEnableVertexAttribArray)(GLuint);
typedef void   (*ptr_glUniform4fv)(GLint,GLsizei,const GLfloat*);
typedef void   (*ptr_glUniformMatrix4fv)(GLint,GLsizei,GLboolean,const GLfloat*);
typedef void   (*ptr_glBindBuffer)(GLenum,GLuint);
typedef void   (*ptr_glVertexAttribPointer)(GLuint,GLint,GLenum,GLboolean,GLsizei,const GLvoid*);
typedef void   (*ptr_glUniform1i)(GLint,GLint);
typedef void   (*ptr_glDisableVertexAttribArray)(GLuint);
typedef void   (*ptr_glUseProgram)(GLuint);


//Global function pointers 
extern ptr_glGenBuffers			glGenBuffers;
extern ptr_glBufferData			glBufferData;
extern ptr_glCreateProgram		glCreateProgram;
extern ptr_glCreateShader		glCreateShader;
extern ptr_glShaderSource		glShaderSource;
extern ptr_glCompileShader		glCompileShader;
extern ptr_glGetShaderiv		glGetShaderiv;
extern ptr_glAttachShader		glAttachShader;
extern ptr_glLinkProgram		glLinkProgram;
extern ptr_glGetProgramiv		glGetProgramiv;
extern ptr_glGetUniformLocation		glGetUniformLocation;
extern ptr_glBindAttribLocation		glBindAttribLocation;
extern ptr_glGetShaderInfoLog		glGetShaderInfoLog;
extern ptr_glUseProgram			glUseProgram;
extern ptr_glEnableVertexAttribArray	glEnableVertexAttribArray;
extern ptr_glUniform4fv			glUniform4fv;
extern ptr_glUniformMatrix4fv		glUniformMatrix4fv;
extern ptr_glBindBuffer			glBindBuffer;
extern ptr_glVertexAttribPointer	glVertexAttribPointer;
extern ptr_glUniform1i			glUniform1i;
extern ptr_glDisableVertexAttribArray	glDisableVertexAttribArray;
extern ptr_glUseProgram			glUseProgram;

*/

int Load_GL_Funcs();




#endif
