# Kernel modules for ARM platform

Includes:

* `Makefile` to build modules
* `enable_arm_pmu.c` to enable user-mode access to PMU (enable by loading module using `insmod`)
* `disable_wsna.c` to toggle write-stream-no-allocate mode (disable WSNA by loading module using `insmod`; (re)enable WSNA by unloading module using `rmmod`)
