#ifndef DIRECTX_GRAPHICS_INFRASTRUCTURE_H
#define DIRECTX_GRAPHICS_INFRASTRUCTURE_H
#pragma once

/*****************************************************************************************************
* DXGI(DirectX Graphics Infrastructure)从DX10开始，包含于D3D10Core.h，提供了一个通用框架
*   管理一些底层的任务(如枚举硬件设备，输出显示帧，全屏转换等)
*
* 接口
*   IDXGIOutput
*   IDXGIAdapter
*   IDXGIFactory
*****************************************************************************************************/

#pragma once

class CDirectXGraphicsInfrastructure
{
public:
    CDirectXGraphicsInfrastructure(void);
    ~CDirectXGraphicsInfrastructure(void);
};

#endif //DIRECTX_GRAPHICS_INFRASTRUCTURE_H