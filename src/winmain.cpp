#include <windows.h>
#include <wingdi.h>
#include "gfx/gload.h"
#include "log.h"
#include "config.h"
#include <tchar.h>
#include "game.h"

const TCHAR window_title[] = _T("3D MAZE");
const TCHAR window_classname[] = _T("3D MAZE");

HWND window;
HDC device_context;
HGLRC gl_rendering_context;

Performance::Alarm second;
Performance::Counter polls_per_second;
Performance::Counter paintevents_per_second;

void SetupOpenGL(HWND window_handle,WPARAM wparam,LPARAM  lparam);
void DestroyOpenGL();
LRESULT CALLBACK WindowCallback(HWND window_handle,UINT msg,WPARAM wparam,LPARAM  lparam);

//********************************************//
//                 Polling Loop               //
//********************************************//
int LoopMain(){
    MSG window_message;
    do {
        Sleep(5);  
        Game::Poll();
        polls_per_second.Increment();
        if(second.Time_Over()){
            //TODO: blit this data to screen
            //logger::info("Polls: %d, Frames: %d, Paints: %d\n",
            //polls_per_second.GetCount(),
            //Game::updates_per_second.GetCount(),
            //paintevents_per_second.GetCount()
            //);

            polls_per_second.Reset();
            Game::updates_per_second.Reset();
            paintevents_per_second.Reset();
        }
        if (PeekMessage(&window_message,0,0,0,PM_REMOVE)){
            TranslateMessage(&window_message);
            DispatchMessage(&window_message);
        }
    } while (window_message.message != WM_QUIT) ;
    return 0;
}

//********************************************//
//                 Entry Point                //
//********************************************//
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance,LPSTR command_string, int show_hint){
    second.interval=1000;
    logger::start("log.txt");
    config::Init();
    Input::Init();

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
            return LoopMain();
        }
        else{
            MessageBox(NULL,_T("Call to CreateWindow failed!"),window_title,MB_OK);
            logger::fatal("Call to CreateWindow failed! Aborting!\n");
            return 1;
        }
    }
    else{
        MessageBox(NULL,_T("Call to RegisterClassEx failed!"),window_title,MB_OK);
        logger::fatal("Call to RegisterClassEx failed! Aborting!\n");
        return 1;
    }
    return 1;
}

LRESULT CALLBACK WindowCallback(HWND window_handle,UINT msg,WPARAM wparam,LPARAM  lparam){
    switch(msg){
        case WM_CREATE:
            SetupOpenGL(window_handle,wparam,lparam);
            logger::info("Initializing engine...\n");
            Game::Start();
            break;
        case WM_DESTROY:
            DestroyOpenGL();
            logger::info("Exiting.\n");
            Game::Exit();
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            Input::HandleKey(wparam,true);
            break;
        case WM_KEYUP:
             Input::HandleKey(wparam,false);
            break;
        case WM_CHAR:
             Input::HandleCharacter(wparam);
            break;
        case WM_MOUSEMOVE:
            Input::HandleCursor((short)LOWORD(lparam),(short)HIWORD(lparam));
            break;
        case WM_MOUSEWHEEL:
            //short wheel_delta = (short)HIWORD(wparam);
            break;
        case WM_LBUTTONDOWN:
            Input::HandleKey(Input::MOUSE_IDS::LEFT_CLICK,true);
            break;
        case WM_LBUTTONUP:
            Input::HandleKey(Input::MOUSE_IDS::LEFT_CLICK,false);
            break;
        case WM_RBUTTONDOWN:
            Input::HandleKey(Input::MOUSE_IDS::RIGHT_CLICK,true);
            break;
        case WM_RBUTTONUP:
            Input::HandleKey(Input::MOUSE_IDS::RIGHT_CLICK,false);
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

    gload_init();

    
	glViewport(0, 0,config::window_width,config::window_height);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDrawBuffer(GL_BACK);
}

void DestroyOpenGL(){
    gload_destroy();
    wglMakeCurrent(NULL,NULL);
    wglDeleteContext(gl_rendering_context);
}

//********************************************//
//                 OS Hooks                   //
//********************************************//

void Game::PostRender(){
    paintevents_per_second.Increment();
    SwapBuffers(device_context);
}
