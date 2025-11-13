#ifndef TRIMCORE_DLL_SYSINFO_INTERNALS_H
#define TRIMCORE_DLL_SYSINFO_INTERNALS_H

#include "TRIMCORE_Rsrc_VersionInfo.h"
#include "TRIMCORE_Topology.h"

namespace TRIMCORE::Implementation {

    // SystemOverview, global 'os' object
    // Implementation details

    namespace ProcessInfo {
        void Initialize ();
    }

    namespace SysInfo {
        void             Initialize (bool primary_service);
        bool             Acquire ();
        void             Release ();

        SystemOverview * Connect ();
        SystemOverview * CreateGlobal ();
        SystemOverview * CreateLocal ();
        void             Collect (SystemOverview *);
        void             Update (SystemOverview *); // dynamic parts
    }
}

#endif
