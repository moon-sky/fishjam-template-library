#pragma once

/*********************************************************************************************************************
* DirectInput 是一个不依赖硬件的虚拟输入系统，允许硬件制造商制造在统一接口下应用的传统的和非传统的输入设备。
*   各种设备(鼠标、键盘、操作敢、游戏柄等)都是设备(Device)
*
* 接口(好像有 A 和 W 两个版本)
*   IDirectInput -- 主接口，创建或获取想采用的任何输入设备。通过 DirectInputCreate 等方法创建
*   IDirectInputDevice(0,2,7,8) -- 设备的通信管道
*
* 数据采集模式
*   1.即时数据模式(默认)
*   2.缓冲数据模式
*
* 获取数据有两种方式
*   1.操作杆 -- 通过 IDirectInputDevice2::Poll 轮询设备(操作杆驱动使用中断，数据总是新的)
*     if(SUCCESSED(pDIJoyStick->Poll()){...}
*   2 键盘鼠标 -- 通过 GetDeviceState 从设备获取数据
*
* 鼠标数据的获取 -- 通过 SetProperty 函数设置
*   1.绝对模式 -- 返回相对于屏幕坐标的位置
*   2.相对模式 -- 返回相对于每个时钟(即上一次？)的相对值
*
* 合并各种输入设备的输入，统一处理（如鼠标右移、键盘的右方向键、操作杆向右推）
*   即将各种设备(鼠标、键盘、操作杆)的输入映射到统一的结构中
*   typedef struct INPUT_EVENT_TYPE
*   {
*     int dx;         //X轴移动量
*     int dy;         //Y轴移动量
*     int fire;       //开火
*     int special;
*     //...
*   };
*
* 力反馈设备
*   IDirectInputDevice2::CreateEffect  
*********************************************************************************************************************/
 
#include <dinput.h>
#pragma comment(lib, "dinput.lib")
#include <ftlDirectx.h>

inline HRESULT DirectInputDemo(HINSTANCE hins, HWND hWnd)
{
    HRESULT hr = E_FAIL;
    IDirectInput*   pDInput = NULL;
    IDirectInputDevice* pDIKeyboard = NULL;

    //1.通过 DirectInputCreate 方法创建主 IDirectInput 接口;
    DX_VERIFY(DirectInputCreate(hins, DIRECTINPUT_VERSION, &pDInput, NULL));

    //2.(可选)枚举查询可用的设备GUIDS，主要是为了查找操作杆等非标准设备,回调函数返回 DIENUM_CONTINUE 表示要继续枚举
    //pDInput->EnumDevices(DIDEVTYPE_JOYSTICK, FUN_PTR, FUN_PARAM, DIEDFL_ATTACHEDONLY) //DI8DEVCLASS_ALL

    //3.通过已知的GUID创建通用的鼠标(GUID_SysMouse)和键盘(GUID_SysKeyboard)
    //  注意：操作杆、手柄至少需要 QI IDirectInputDevice2
    DX_VERIFY(pDInput->CreateDevice(GUID_SysKeyboard, &pDIKeyboard, NULL));
    

    //4.设置设备的协作等级 -- 注意：鼠标、键盘都是独占设备，获得焦点的程序才能使用；力反馈设备应工作在独占模式下
    //  DISCL_EXCLUSIVE -- 独占访问，一旦要求该设备，其他应用程序都不能访问，但其他应用程序能够要求非独占地访问（什么意思？）
    //  DISCL_NONEXCLUSIVE -- 非独占访问
    //  DISCL_BACKGROUND -- 应用程序即使是 非激活、最小化 状态，仍然能接收输入
    //  DISCL_FOREGROUND -- 要求前台访问，当相关窗口移到后台时，该设备自动不获得(释放？）
    DX_VERIFY(pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))

    //5.设置设备的数据包格式 -- 允许在设备对象的等级下决定从输入设备得来的数据如何格式化
    //  可以自己设置，也可以使用系统预先定义好的设备数据格式
    //    DIDATAFORMAT df = {0};
    //    df.dwSize = sizeof(DIDATAFORMAT);
    //    df.dwFlags = ...
    DX_VERIFY(pDIKeyboard->SetDataFormat(&c_dfDIKeyboard)); //通用键盘

    //6.设置设备的性能，如操作杆轴的范围、盲区等(操作杆、手柄时需要), SetProperty
#if 0
    IDirectInputDevice2* pDIJoyStick = NULL;

    //设置手柄操作杆的范围： [-1024 ~ 1024]
    DIPROPRANGE joy_axis_rang;
    joy_axis_rang.diph.dwSize = sizeof(joy_axis_rang);
    joy_axis_rang.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    joy_axis_rang.diph.dwObj = DIJOFS_X;
    joy_axis_rang.diph.dwHow = DIPH_BYOFFSET;
    joy_axis_rang.lMin = -1024;
    joy_axis_rang.lMax = 1024;
    
    DX_VERIFY(pDIJoyStick->SetProperty(DIPROP_RANGE, &joy_axis_rang.diph));
    joy_axis_rang.diph.dwObj = DIJOFS_Y;
    DX_VERIFY(pDIJoyStick->SetProperty(DIPROP_RANGE, &joy_axis_rang.diph));

    //设置手柄X、Y轴盲区 -- 中性区域的范围，处于其中时不发送数据，注意：盲区的值在 0～10000 之间
    //因此，如果要设置 %10的盲区，应该设置为 1000
    DIPROPDWORD dead_band;
    dead_band.diph.dwSize = sizeof(dead_band);
    dead_band.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dead_band.diph.dwObj = DIJOFS_X;
    dead_band.diph.dwHow = DIPH_BYOFFSET;
    //X轴的盲区为10%
    dead_band.dwData = 1000;
    DX_VERIFY(pDIJoyStick->SetProperty(DIPROP_DEADZONE, &dead_band.diph));

    //Y轴的盲区
    dead_band.diph.dwObj = DIJOFS_Y;
    DX_VERIFY(pDIJoyStick->SetProperty(DIPROP_DEADZONE, &dead_band.diph));
    
#endif

    //7.通过 Acquire 获得设备，即将设备同应用程序相连并告诉DirectInput，将来要从这些设备中获取数据。
    DX_VERIFY(pDIKeyboard->Acquire());

    //8.获取数据--两种方式
    //8.1 操作杆时通过 IDirectInputDevice2::Poll 轮询设备(确保没有中断驱动程序，操作杆具有有效数据)，获取输入状态
    //    DX_VERIFY(pDIJoyStick->Poll());

    //8.2 键盘鼠标通过 GetDeviceState 从设备获取数据
    
    //获取键盘信息--每次循环之前都调用获取一次
    //typedef UCHAR[256] _DIKEYSTATE ;
    UCHAR keyState[256] = {0};
    //其他预定义的数据格式： DIMOUSESTATE、DIJOYSTATE 等
    DX_VERIFY(pDIKeyboard->GetDeviceState(sizeof(keyState), (LPVOID)keyState));
DIJOYSTATE

    //测试按键
    if(DIKEYDOWN(keyState,DIK_ESCAPE))
    {
        //如果按下了 Esc 键
    }
    
    //使用完毕后释放
    DX_VERIFY(pDIKeyboard->Unacquire());

    SAFE_RELEASE(pDIKeyboard);
    SAFE_RELEASE(pDInput);
}