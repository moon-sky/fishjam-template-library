#ifndef FTL_DUI_H
#define FTL_DUI_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDUI.h requires ftlbase.h to be included first
#endif


/*************************************************************************************************************************
* DUI -- DirectUI
*   DirectUI and Duser Architecture
*   Resource Management and Build System
*   UI Files: Markup, Styles, and Macros
*   Layout, Rendering, and Animations
*   Notifications: Events, Listeners, Handlers, and Behaviors
*   Common Controls, Custom Controls, and Compound Controls
*   Best Practices and Future Work
*
* DUser -- Streamlined low level window manager (extending Window's User architecture. ) Includes painting coordination, 
*	gadget tree management, input management, messaging, and simple animations.
* DirectUI -- System above DUser including declarative layout, stylesheets, keyboard navigation, MSAA support, 
*	common controls, and content rendering	.
* Gadget -- Can be thought of as light weight HWNDs.  Can host HWNDs, or other Gadgets.
* DUI -- The whole of DUser and DirectUI as a platform.  Commonly used to refer to UX Platform.
* Parser -- Parser that takes UI markup in, and creates DirectUI visual tree.
* Layout -- Instructions given to element to allow for child positioning.
* Resource Management -- System designed to allow for build time processing and packaging of resources, 
*	runtime resource lookup, and extensions to the Win32 resource model.
* RCXML -- Build time resource processing tool.

*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_DUI_H

#ifndef USE_EXPORT
#  include "ftlDUI.hpp"
#endif 