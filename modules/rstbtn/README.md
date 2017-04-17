# Reset Button Linux Kernel Module for Western Digital My Cloud DL2100 NAS

This is a re-implementation of the reset button kernel module <samp>rstbtn.ko</samp>.
That module is provided by Western Digital in binary form only. However, it seems
that this module must be loaded in order for various WD tools (e.g. fan control)
to work. Hence, a re-implementation was required in order to build the module for
other kernel version than the one provided in the WD firmware.


## WARNING

Modifications to the firmware of your device may **render your device unusable**.
Moreover, modifications to the firmware may **void the warranty for your device**.

You are using the programs in this repository at your own risk. *We are not
responsible for any damage caused by these programs, their incorrect usage, or
inaccuracies in this manual.*


## GETTING STARTED

You can build the module for your currently loaded kernel using the provided
Makefile:

    cd src/
    make

Then, you can install the kernel module with:

    sudo make install
    sudo depmod
    sudo update-initramfs -u

Alternatively, you can specify a specific kernel release to build against
(headers for this release must be installed on your system):

    make BUILD_KERNEL=3.16.XX-YYYYMMDD-dl2100
    sudo make install BUILD_KERNEL=3.16.XX-YYYYMMDD-dl2100
    sudo depmod 3.16.XX-YYYYMMDD-dl2100
    sudo update-initramfs -c -k 3.16.XX-YYYYMMDD-dl2100

Finally, you can load the module:

    sudo modprobe rstbtn


## GET LATEST VERSION

Find documentation and grab the latest version on GitHub
<https://github.com/michaelroland/wdnas-dl2100-kernel>


## COPYRIGHT

- Copyright (c) 2017 Michael Roland <<mi.roland@gmail.com>>


## DISCLAIMER

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


## LICENSE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

**License**: [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.txt)

