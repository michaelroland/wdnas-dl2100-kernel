## CONFIGURATIONS

This folder contains configurations for Linux kernels targeting Western Digital
My Cloud DL2100 NAS Systems.


### Default configuration: <samp>wd-dl2100.config</samp>

<samp>[wd-dl2100.config](wd-dl2100.config)</samp> is the default configuration
for Linux kernels for Debian on the WD My Cloud DL2100 NAS. The configuration
is based on the default Debian kernel configuration from the Debian package
<samp>[linux-image-3.16.0-4-amd64\_3.16.39-1+deb8u2\_amd64.deb](http://security.debian.org/debian-security/pool/updates/main/l/linux/linux-image-3.16.0-4-amd64_3.16.39-1+deb8u2_amd64.deb)</samp>
and the default configuration from the
[DL2100 GPL source code package (v2.21.126 20161110)](http://downloads.wdc.com/gpl/WDMyCloud_DL2100_GPL_v2.21.126_20161110.zip).


### Kernel-specific configuration files

Instead of the generic default configuration, you can create configurations
for specific kernel versions. Configuration files must be named
<samp>wd-dl2100-X.Y.Z.config</samp>, where X.Y.Z is the kernel version (as
output by <samp>make kernelversion</samp>).

