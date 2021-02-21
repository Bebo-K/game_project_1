#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#define WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include "winmain.h"
#include "os.h"
#include "gfx/gload.h"
#include "log.h"
#include "config.h"
#include "game.h"

const TCHAR window_title[] = _T("Game");
const TCHAR window_classname[] = _T("Game");

int Window::width = 1280;
int Window::height= 720;
int Window::DPI=USER_DEFAULT_SCREEN_DPI;

Performance::Alarm second;
Performance::Counter polls_per_sec;

HWND window;
HDC device_context;
HGLRC gl_rendering_context;

void SetupOpenGL(HWND window_handle,WPARAM wparam,LPARAM  lparam);
void DestroyOpenGL();
LRESULT CALLBACK WindowCallback(HWND window_handle,UINT msg,WPARAM wparam,LPARAM  lparam);


//********************************************//
//              OS Timer (Windows)            //
//********************************************//
unsigned long long start_time=0;

unsigned long long sys_time(){
    FILETIME systime;
    GetSystemTimeAsFileTime(&systime);//1 represents 100 ns, 10000000 represents 1s
    unsigned long long time = systime.dwHighDateTime;
    time = (time << 32) | systime.dwLowDateTime;
    return time;
}
SYSTEMTIME sys_date(){//Minute program is in. (time % (seconds*60))
    FILETIME filetime;
    SYSTEMTIME systime;
    GetSystemTimeAsFileTime(&filetime);
    FileTimeToSystemTime(&filetime,&systime);
    return systime;
}

//Engine timer hooks
long time_ms(){//ms elapsed since program start.  
    return (sys_time()-start_time)/10000;//100 ns = 0.0001ms
}
long long time_nano(){//ns elapsed since program start.  
    return (sys_time()-start_time)*100;
}

int ms_per_second;
void GetTimerData(){
    polls_per_sec.Increment();
    if(second.Time_Over()){
        int new_ms = time_ms();
        Performance::polls_last_second = polls_per_sec.GetCount();
        Performance::updates_last_second = Performance::frames.GetCount();
        Performance::draws_last_second = Performance::draws.GetCount();

        polls_per_sec.Reset();
        Performance::frames.Reset();
        Performance::draws.Reset();
        ms_per_second=new_ms;
    }
}


//********************************************//
//                 Polling Loop               //
//********************************************//
int LoopMain(){
    MSG window_message;
    do {
        Sleep(4);  
        Game::Poll();
        PollJoypads();
        GetTimerData();
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
    start_time = sys_time();
    ms_per_second = time_ms();
    second.interval=1000;
    logger::start("log.txt");
    config::Init();
    Input::Init();
    SetupOSInput();

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


    Input::Destroy();
    config::Destroy();
    return 1;
}

LRESULT CALLBACK WindowCallback(HWND window_handle,UINT msg,WPARAM wparam,LPARAM  lparam){
    switch(msg){
        case WM_CREATE:
            SetupOpenGL(window_handle,wparam,lparam);
            SetupDirectInput(window_handle);
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
            Input::HandleBool(wparam,true);
            break;
        case WM_KEYUP:
             Input::HandleBool(wparam,false);
            break;
        case WM_CHAR:
             Input::HandleCharacter(wparam);
            break;
        case WM_MOUSEMOVE:
            Input::HandleIntAxis(PhysicalInput::MOUSE_CURSOR_AXIS,(short)LOWORD(lparam),(short)HIWORD(lparam));
            break;
        case WM_MOUSEWHEEL:
            //short wheel_delta = (short)HIWORD(wparam);
            break;
        case WM_LBUTTONDOWN:
            Input::HandleBool(PhysicalInput::MOUSE_LEFT_BUTTON,true);
            break;
        case WM_LBUTTONUP:
            Input::HandleBool(PhysicalInput::MOUSE_LEFT_BUTTON,false);
            break;
        case WM_RBUTTONDOWN:
            Input::HandleBool(PhysicalInput::MOUSE_RIGHT_BUTTON,true);
            break;
        case WM_RBUTTONUP:
            Input::HandleBool(PhysicalInput::MOUSE_RIGHT_BUTTON,false);
            break; 
        // DirectInput
        /*
            case MM_JOY1MOVE:
            OnDirectInputJoyEvent();
            break; 
        case MM_JOY1BUTTONDOWN :
            OnDirectInputJoyEvent(wparam,true);
            break; 
        case MM_JOY1BUTTONUP :
            OnDirectInputJoyEvent(wparam,false);
            break; 
            */
        case WM_SIZE:
            RECT new_client_area;
            if(GetClientRect(window_handle,&new_client_area)){
                Window::width = new_client_area.right;
                Window::height = new_client_area.bottom;
                glViewport(0, 0,Window::width, Window::height);
            }
            if(Game::client != null){
                Game::client->ui.OnResize(Window::width, Window::height);
            }
            
            break;
            
        case WM_DPICHANGED:{
            Window::DPI = HIWORD(wparam);
            //UpdateDpiDependentFontsAndResources();

            RECT* const prcNewWindow = (RECT*)lparam;
            SetWindowPos(window_handle,
                NULL,
                prcNewWindow ->left,
                prcNewWindow ->top,
                prcNewWindow->right - prcNewWindow->left,
                prcNewWindow->bottom - prcNewWindow->top,
                SWP_NOZORDER | SWP_NOACTIVATE);
        break;
        }
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
    SwapBuffers(device_context);
}