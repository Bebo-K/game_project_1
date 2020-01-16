#include <windows.h>
#include <wingdi.h>
#include "gfx/gload.h"
#include "io/log.h"
#include "io/config.h"
#include <tchar.h>
#include "game.h"

const TCHAR window_title[] = _T("3D MAZE");
const TCHAR window_classname[] = _T("3D MAZE");

HWND window;
HDC device_context;
HGLRC gl_rendering_context;


void SetupOpenGL(HWND window_handle,WPARAM wparam,LPARAM  lparam);
void DestroyOpenGL();
LRESULT CALLBACK WindowCallback(HWND window_handle,UINT msg,WPARAM wparam,LPARAM  lparam);

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance,LPSTR command_string, int show_hint){

    logger::start("log.txt");

    WNDCLASSEX window_class;
        window_class.cbSize=sizeof(WNDCLASSEX);
        window_class.style= CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
        window_class.lpfnWndProc=WindowCallback;
        window_class.cbClsExtra=0;
        window_class.cbWndExtra=0;
        window_class.hInstance=instance;
        window_class.hCursor=LoadCursor(NULL, IDC_ARROW);
        window_class.hIcon=LoadIcon(instance,IDI_APPLICATION);
        window_class.hIconSm=LoadIcon(instance,IDI_APPLICATION);
        window_class.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
        window_class.lpszClassName=window_classname;
        window_class.lpszMenuName=NULL; 

    WORD register_winclass_result = RegisterClassEx(&window_class);
    if(register_winclass_result){
        window = CreateWindow(
            window_classname,
            window_title,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            config::window_width,config::window_height,
            NULL,NULL,
            instance,NULL);
        if(window){ 
            ShowWindow(window,show_hint);
            UpdateWindow(window);
            glClearColor(0,0,0,1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SwapBuffers(device_context);

/*Loop Entry Point*/ 

            MSG window_message;
            do {
                if (PeekMessage(&window_message,0,0,0,PM_REMOVE)){
                    TranslateMessage(&window_message);
                    DispatchMessage(&window_message);
                }
                Game::Poll();
            } while (window_message.message != WM_QUIT) ;
        }
        else{
            MessageBox(NULL,_T("Call to CreateWindow failed!"),window_title,MB_OK);
            logger::fatal("Call to CreateWindow failed! Aborting!");
            return 1;
        }
    }
    else{
        MessageBox(NULL,_T("Call to RegisterClassEx failed!"),window_title,MB_OK);
        logger::fatal("Call to RegisterClassEx failed! Aborting!");
        return 1;
    }
    return 0;
}

LRESULT CALLBACK WindowCallback(HWND window_handle,UINT msg,WPARAM wparam,LPARAM  lparam){
    switch(msg){
        case WM_CREATE:
            SetupOpenGL(window_handle,wparam,lparam);
            logger::info("Initializing engine...");
            Game::Start();
            break;
        case WM_PAINT: /*Ignore for games as we're constantly redrawing anyways.*/
            Game::Paint();
            SwapBuffers(device_context);
            break;
        case WM_DESTROY:
            DestroyOpenGL();
            logger::info("Exiting.");
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            RECT new_client_area;
            if(GetClientRect(window_handle,&new_client_area)){
                Game::window_width = new_client_area.right;
                Game::window_height = new_client_area.bottom;
                glViewport(0, 0,Game::window_width,Game::window_height);
            }
            break;
        default:
            return DefWindowProc(window_handle,msg,wparam,lparam);
            break;
    }
    return (LRESULT) 0;
}

void SetupOpenGL(HWND window_handle,WPARAM wparam,LPARAM  lparam){
    device_context = GetDC(window_handle);

    PIXELFORMATDESCRIPTOR target_format =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,
            8,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

    int pixel_format = ChoosePixelFormat(device_context, &target_format);
    SetPixelFormat(device_context,pixel_format,&target_format);

    gl_rendering_context = wglCreateContext(device_context);
    wglMakeCurrent(device_context, gl_rendering_context);

    GLenum err=glewInit();
    if(err!=GLEW_OK) {
        logger::info("glewInit failure (code %d). Error string:\n",err);
        logger::info((char*)glewGetErrorString(err));
        MessageBox(NULL,_T("Fatal error: glewInit failed. See log for more info."),window_title,MB_OK);
        exit(1);
    }

    
	glViewport(0, 0,config::window_width,config::window_height);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void DestroyOpenGL(){
    wglMakeCurrent(NULL,NULL);
    wglDeleteContext(gl_rendering_context);
}

