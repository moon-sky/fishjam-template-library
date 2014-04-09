// DLNAHandler.cpp : Defines the exported functions for the DLL application.
//

#include "DLNAHandler.h"
#include "DLNAHandlerImpl.h"
#include "NptLogging.h"

NPT_SET_LOCAL_LOGGER("Handler.DLNA.Create")

ICastingHandler* CreateDLNAHandlerInstance()
{
    NPT_LogManager::GetDefault().Configure("plist:.level=FINE;.handlers=ConsoleHandler;.ConsoleHandler.colors=off;.ConsoleHandler.filter=30");

    NPT_LOG_FINE_1("test Log, %d", 100);
    //NPT_LOG_FINE_1("OnMRRemoved:%s", device->GetFriendlyName());

    return new CDLNAHandlerImpl();
}