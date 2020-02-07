#include "gload.h"
#include <stdio.h>

//since GLuints are unsigned, we're reserving the 0 buffer as "empty"
GLuint zero_buffer;

void gload_init(){
    glGenBuffers(1,&zero_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, zero_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zero_buffer), &zero_buffer ,GL_STATIC_DRAW);
}
void gload_destroy(){
    glDeleteBuffers(1,&zero_buffer);
}

/*
////////////////////////////////////////////////////INTERNAL SINGLETON
PFNGLACTIVETEXTUREPROC blActiveTexture;
PFNGLGENBUFFERSPROC blGenBuffers;
PFNGLDELETEBUFFERSPROC blDeleteBuffers;
PFNGLBUFFERDATAPROC blBufferData;
PFNGLCREATEPROGRAMPROC blCreateProgram;
PFNGLCREATESHADERPROC blCreateShader;
PFNGLSHADERSOURCEPROC blShaderSource;
PFNGLCOMPILESHADERPROC blCompileShader;
PFNGLGETSHADERIVPROC blGetShaderiv;
PFNGLATTACHSHADERPROC blAttachShader;
PFNGLLINKPROGRAMPROC blLinkProgram;
PFNGLGETPROGRAMIVPROC blGetProgramiv;
PFNGLGETUNIFORMLOCATIONPROC blGetUniformLocation;
PFNGLBINDATTRIBLOCATIONPROC blBindAttribLocation;
PFNGLGETSHADERINFOLOGPROC blGetShaderInfoLog;
PFNGLUSEPROGRAMPROC blUseProgram;
PFNGLENABLEVERTEXATTRIBARRAYPROC blEnableVertexAttribArray;
PFNGLUNIFORM4FVPROC blUniform4fv;
PFNGLUNIFORMMATRIX4FVPROC blUniformMatrix4fv;
PFNGLBINDBUFFERPROC blBindBuffer;
PFNGLVERTEXATTRIBPOINTERPROC blVertexAttribPointer;
PFNGLUNIFORM1IPROC blUniform1i;
PFNGLDISABLEVERTEXATTRIBARRAYPROC blDisableVertexAttribArray;
PFNGLUSEPROGRAMPROC blUseProgram;
////////////////////////////////////////////////////



int Load_GL_Funcs(){
if (!(blGenBuffers = (PFNGLGENBUFFERSPROC)GETGLPROC("glGenBuffers"))){return 1;}
if (!(blBufferData = (PFNGLBUFFERDATAPROC)GETGLPROC("glBufferData"))){return 2;}
if (!(blCreateProgram = (PFNGLCREATEPROGRAMPROC)GETGLPROC("glCreateProgram"))){return 3;}
if (!(blCreateShader = (PFNGLCREATESHADERPROC)GETGLPROC("glCreateShader"))){return 4;}
if (!(blShaderSource = (PFNGLSHADERSOURCEPROC)GETGLPROC("glShaderSource"))){return 5;}
if (!(blCompileShader = (PFNGLCOMPILESHADERPROC)GETGLPROC("glCompileShader"))){return 6;}
if (!(blGetShaderiv = (PFNGLGETSHADERIVPROC)GETGLPROC("glGetShaderiv"))){return 7;}
if (!(blAttachShader = (PFNGLATTACHSHADERPROC)GETGLPROC("glAttachShader"))){return 8;}
if (!(blLinkProgram = (PFNGLLINKPROGRAMPROC)GETGLPROC("glLinkProgram"))){return 9;}
if (!(blGetProgramiv = (PFNGLGETPROGRAMIVPROC)GETGLPROC("glGetProgramiv"))){return 10;}
if (!(blGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)GETGLPROC("glGetUniformLocation"))){return 11;}
if (!(blBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)GETGLPROC("glBindAttribLocation"))){return 12;}
if (!(blGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)GETGLPROC("glGetShaderInfoLog"))){return 13;}
if (!(blUseProgram = (PFNGLUSEPROGRAMPROC)GETGLPROC("glUseProgram"))){return 14;}
if (!(blEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)GETGLPROC("glEnableVertexAttribArray"))){return 15;}
if (!(blUniform4fv = (PFNGLUNIFORM4FVPROC)GETGLPROC("glUniform4fv"))){return 16;}
if (!(blUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)GETGLPROC("glUniformMatrix4fv"))){return 17;}
if (!(blBindBuffer = (PFNGLBINDBUFFERPROC)GETGLPROC("glBindBuffer"))){return 18;}
if (!(blVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)GETGLPROC("glVertexAttribPointer"))){return 19;}
if (!(blUniform1i = (PFNGLUNIFORM1IPROC)GETGLPROC("glUniform1i"))){return 20;}
if (!(blDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)GETGLPROC("glDisableVertexAttribArray"))){return 21;}
if (!(blUseProgram = (PFNGLUSEPROGRAMPROC)GETGLPROC("glUseProgram"))){return 22;}
if (!(blActiveTexture = (PFNGLACTIVETEXTUREPROC)GETGLPROC("glActiveTexture"))){return 23;}
if (!(blDeleteBuffers = (PFNGLDELETEBUFFERSPROC)GETGLPROC("glDeleteBuffers"))){return 24;}
return 0;
}


*/