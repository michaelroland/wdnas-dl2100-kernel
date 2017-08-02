## PATCHES

This folder contains patches for Linux kernels targeting Western Digital
My Cloud DL2100 NAS Systems.


### Format

Patches are expected to be in the format output by <samp>git</samp> (e.g. when
creating patches for specific commits) or in any format that can be applied to
the kernel source tree with the command:

    patch -p1 -l -N <patch-file


### Naming convention

Names of patch files must end in <samp>.patch</samp> in order to be picked up
by the build script. Files should be named according to the following
convention:

    <YYYYMMDD>-<commit revision hash>-<description>.patch


### Version-specific patches

Patches for specific kernel versions may be placed in sub-folders named
<samp>X.Y.Z/</samp> for patches that apply to only one specific kernel version
or <samp>X.Y/</samp> for patches that apply to a whole kernel release.


### List of required patches

#### Kernel 3.16.39-1+deb8u2

- <samp>[3.16.39/20150205-eb3d80f729e07394685239ddd137fbee5c13a2ea-acpica\_events\_gpe.patch](3.16.39/20150205-eb3d80f729e07394685239ddd137fbee5c13a2ea-acpica_events_gpe.patch)</samp>:
  This patch adds the missing API implementation <samp>acpi\_finish\_gpe()</samp>
  that is required for the implementation of the [rstbtn module](../modules/rstbtn/).


