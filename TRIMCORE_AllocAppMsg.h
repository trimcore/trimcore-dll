#ifndef TRIMCORE_DLL_ALLOCAPPMSG_H
#define TRIMCORE_DLL_ALLOCAPPMSG_H

namespace TRIMCORE {
    
    // AllocAppMessage
    //  - allocates window message from WM_APP (0x8000) ... 0xBFFF range
    //     - the allocator only uses range: 0x9A00 ... 0x9DFF; 1024 messages
    //  - returns 0 if no more is available
    //
    TRIMCORE_DLL_IMPORT USHORT TRIMCORE_APIENTRY AllocAppMessage ();

    // FreeAppMessage
    //  - releases the window message
    //
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY FreeAppMessage (USHORT message);

    // AllocClassMessage
    //  - 
    //  - returns 0 if unable
    //
    // TRIMCORE_DLL_IMPORT USHORT AllocClassMessage (LPCTSTR classname);

    // FreeClassMessage
    //  - 
    //
    // TRIMCORE_DLL_IMPORT bool FreeClassMessage (LPCTSTR classname, USHORT message);

    // FreeClassMessages
    //  - 
    //
    // TRIMCORE_DLL_IMPORT bool FreeClassMessages (LPCTSTR classname);
}

#endif
