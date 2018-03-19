# Linux Kernel for Debian on Western Digital My Cloud DL2100 NAS Systems

This repository contains sources for the kernel and additional modules, and a
number of tools for building and installing the kernel on Debian 9 (stretch)
targeting Western Digital My Cloud DL2100 NAS Systems. The tools allow you to

- unpack kernel source tree tar archives,
- apply a set of patches to the kernel source tree,
- build Debian packages from the kernel source,
- install the kernel image and the kernel headers,
- build additional modules (needed by the WD My Cloud DL2100), and to
- install these additional modules.


## WARNING

Modifications to the firmware of your device may **render your device unusable**.
Moreover, modifications to the firmware may **void the warranty for your device**.

You are using the programs in this repository at your own risk. *We are not
responsible for any damage caused by these programs, their incorrect usage, or
inaccuracies in this manual.*


## GETTING STARTED

All scripts are designed to be run on Debian 9 (stretch) either directly on the
DL2100 or on any other *amd64* (x64, x86-64) architecture. The scripts will
probably run on other Debian and Ubuntu versions too. Currently, no effort was
taken towards cross-compilation. Hence, the requirement to build on an amd64
architecture, which matches the processor architecture of the DL2100.


### Prerequisites

The packages *build-essential*, *fakeroot* and *libncurses5-dev*, as well as the
dependencies to build the linux kernel package need to be installed in order to
use the tools:

    sudo apt install build-essential fakeroot libncurses5-dev
    sudo apt build-dep linux

In addition, you may want to install the latest Linux kernel sources for your
distribution:

    sudo apt install linux-source

You will then have a tar archive with the Linux kernel sources at
<samp>/usr/src/linux-source-\*.tar.\*</samp> (e.g. with Linux kernel 4.9
packaged for Debian 9 this is <samp>/usr/src/linux-source-4.9.tar.xz</samp>).


### Structure of this repository

- <samp>kernel-config/</samp>: Folder containing kernel configuration files (see
  [Configurations](kernel-config/#configurations)).
- <samp>kernel-patches/</samp>: Folder containing patches to apply against the
  kernel source tree (see [Patches](kernel-patches/#patches)).
- <samp>kernel/</samp>: Folder for storing and extracting kernel sources.
- <samp>modules/</samp>: Folder containing additional kernel modules (see
  [Modules](modules/#modules)).
- <samp>\*.sh</samp>: Tools for building and installing.


### Extracting, patching, and building the Linux kernel

You can extract a Linux kernel source archive, apply necessary patches and
configuration, and build the kernel Debian packages with the script
<samp>[build-kernel.sh](build-kernel.sh)</samp>. For instance, use the following
command to build from the sources provided through the <samp>linux-source</samp>
package on Debian 9:

    ./build-kernel.sh /usr/src/linux-source-4.9.tar.xz

This will extract the source tree into <samp>./kernel/linux-source-4.9</samp>,
copy the base configuration from <samp>[./kernel-config/](kernel-config/)</samp>
into <samp>./kernel/linux-source-4.9/.config</samp>, apply all patches from
<samp>[./kernel-patches/](kernel-patches/)</samp>, and build the kernel. The
resulting Debian packages will be

- <samp>./kernel/linux-image-...\_amd64.deb</samp>,
- <samp>./kernel/linux-image-...-dbg\_...\_amd64.deb</samp>,
- <samp>./kernel/linux-headers-...\_amd64.deb</samp>, and
- <samp>./kernel/linux-libc-dev-...\_amd64.deb</samp>.

The argument to <samp>[build-kernel.sh](build-kernel.sh)</samp> can also be a
directory containing a Linux kernel source tree. Extraction is skipped in that
case.


### Install Linux kernel image and headers

You can install the previously built kernel packages with the script
<samp>[install-kernel.sh](install-kernel.sh)</samp>. This script requires *root*.
The argument to this script can either be the tar archive or the path to the
extracted source tree. For instance, to continue with the installation of the
packages created in the previous section, you could use:

    sudo ./install-kernel.sh /usr/src/linux-source-4.9.tar.xz


### Building additional modules

The Western Digital My Cloud DL2100 NAS System requires some additional kernel
modules (see [Modules](modules/#modules)). You can build these modules with the
script <samp>[build-modules.sh](build-modules.sh)</samp>. The script takes the
kernel release as argument (defaults to the currently running kernel). For
instance, to target the previously built and installed kernel, you could use:

    ./build-modules.sh 4.9.XX-YYYYMMDD-dl2100


### Installing additional modules

You can install the previously built kernel modules with the script
<samp>[install-modules.sh](install-modules.sh)</samp>. This script requires *root*.
The script takes the kernel release as argument (defaults to the currently running
kernel). For instance, to target the previously built and installed kernel, you
could use:

    sudo ./install-modules.sh 4.9.XX-YYYYMMDD-dl2100

This will install the modules, update module dependencies, and create an updated
initial ramdisk (initramfs).


## GET LATEST VERSION

Find documentation and grab the latest version on GitHub
<https://github.com/michaelroland/wdnas-dl2100-kernel>


## COPYRIGHT

- Copyright (c) 2017-2018 Michael Roland <<mi.roland@gmail.com>>


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

