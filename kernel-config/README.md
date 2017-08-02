## CONFIGURATIONS

This folder contains configurations for Linux kernels targeting Western Digital
My Cloud DL2100 NAS Systems.


### Default configuration: <samp>wd-dl2100.config</samp>

<samp>[wd-dl2100.config](wd-dl2100.config)</samp> is the default configuration
for Linux kernels for Debian 9 on the WD My Cloud DL2100 NAS. The configuration
is based on the default Debian kernel configuration from the Debian package
<samp>[linux-image-4.9.0-3-amd64\_4.9.30-2+deb9u2\_amd64.deb](http://ftp.debian.org/debian/pool/main/l/linux/linux-image-4.9.0-3-amd64_4.9.30-2+deb9u2_amd64.deb)</samp>
and the default configuration from the
[DL2100 GPL source code package (v2.30.165 20170321)](http://downloads.wdc.com/gpl/WDMyCloud_DL2100_GPL_v2.30.165_20170321.tar.gz).


### Kernel-specific configuration files

Instead of the generic default configuration, you can create configurations
for specific kernel versions. Configuration files must be named
<samp>wd-dl2100-X.Y.Z.config</samp>, where X.Y.Z is the kernel version (as
output by <samp>make kernelversion</samp>).

