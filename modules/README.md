## MODULES

This folder contains additional kernel modules for the Western Digital My Cloud
DL2100 NAS.


### Format

Kernel modules can either be provided as compressed tar archives or as
sub-directories. Compressed tar archives are automatically unpacked by the
module build script.

Each module must consist of a directory with a sub-directory <samp>src/</samp>.
The sub-directory <samp>src/</samp> is expected to contain the Makefile for
building the module. The targeted kernel release is passed in the variable
<samp>BUILD\_KERNEL</samp>.


### List of required modules

#### All kernel versions

- <samp>[rstbtn](./rstbtn/)</samp>:
  A re-implementation of the reset button kernel module (a module provided by
  Western Digital in binary form only).

#### Kernel 3.16.39-1+deb8u2

- <samp>[igb-5.3.5.4](./igb-5.3.5.4.tar.gz)</samp>:
  A newer version of the Intel® Network Adapter Driver for 82575/6, 82580,
  I350, and I210/211-Based Gigabit Network Connections for Linux (igb). This
  is required since the version included in the 3.16.39-1+deb8u2 kernel
  package does not seem to support the ethernet adapters of the DL2100.


