#ifndef TRIMCORE_DLL_TOPOLOGY_H
#define TRIMCORE_DLL_TOPOLOGY_H

#include "TRIMCORE.h"

namespace TRIMCORE {

    // TBD:
    //  - [1] - second core on non-HT single socket CPU
    //  - [1b] - second HT on second core
    //  - [N2:1b] - second HT on second core on 3rd NUMA node on single-socket machine
    //  - [0N2:1b] - second HT on second core on 3rd NUMA node on first socket of dual-socket machine
    //  - [0:1b] - dual socket machine, where numa = socket or there's global numa
    //  - [0N2a:1b] - second HT on second core on 3rd NUMA node that spans multiple groups on first socket of dual-socket machine
    //  - TODO: tiles? shared L3 cache blocks?
    //  - TODO: generate internal map, map PROCESSOR_NUMBER to it

    struct HwThreadDetails {
        PROCESSOR_NUMBER number;

        ULONG  domain; // SRAT proximity domain
        ULONG  apic;   // SRAT APIC ID
        
        USHORT numa;   // within system
        USHORT core;   // within socket/numa whichever is smaller
        UCHAR  group;  // within numa node
        UCHAR  thread; // within core
        UCHAR  efficiency; // class, higher means faster

        KAFFINITY affinity; // module or L2 affinity
        KAFFINITY affinityLLC; // last level cache affinity
    };

    struct HwMemoryDetails {
        USHORT numa;
        ULONG  domain; // SRAT proximity domain

        ULONG64 base;
        ULONG64 size;
    };

    void InitializeTopology ();

    // bool Expand (PROCESSOR_NUMBER, HwThread *);
    // UINT Core (PROCESSOR_NUMBER);

    /*
    *     // SystemTopology
    //  -
    //
    struct SystemTopology {
        DWORD processors_active;
        DWORD processors_maximum;
        DWORD processor_groups_active;
        DWORD processor_groups_maximum;
        UCHAR prevalent_smt;
        UCHAR highest_efficiency_class;
        
        // ...
        DWORD numas;
        DWORD processors_details_offset;
        DWORD memory_details_offset;

        // TODO: add data here
        // HwThreadDetails processors [0];
    };

*/
}

#endif
