## CONFIGURATIONS

This folder contains configurations for Linux kernels targeting Western Digital
My Cloud DL2100 NAS Systems.


### Configuration: <samp>wd-dl2100-4.9.config</samp>

<samp>[wd-dl2100-4.9.config](wd-dl2100-4.9.config)</samp> is the default configuration
for Linux 4.9 kernels for Debian 9 on the WD My Cloud DL2100 NAS. The configuration
is based on the default Debian kernel configuration from the Debian package
<samp>[linux-image-4.9.0-3-amd64\_4.9.30-2+deb9u2\_amd64.deb](http://ftp.debian.org/debian/pool/main/l/linux/linux-image-4.9.0-3-amd64_4.9.30-2+deb9u2_amd64.deb)</samp>
and the default configuration from the
[DL2100 GPL source code package (v2.30.165 20170321)](http://downloads.wdc.com/gpl/WDMyCloud_DL2100_GPL_v2.30.165_20170321.tar.gz).


### Configuration: <samp>wd-dl2100-3.16.config</samp>

<samp>[wd-dl2100-3.16.config](wd-dl2100-3.16.config)</samp> is the default configuration
for Linux 3.16 kernels for Debian 8 on the WD My Cloud DL2100 NAS. The configuration
is based on the default Debian kernel configuration from the Debian package
<samp>[linux-image-3.16.0-4-amd64\_3.16.39-1+deb8u2\_amd64.deb](http://security.debian.org/debian-security/pool/updates/main/l/linux/linux-image-3.16.0-4-amd64_3.16.39-1+deb8u2_amd64.deb)</samp>
and the default configuration from the
[DL2100 GPL source code package (v2.21.126 20161110)](http://downloads.wdc.com/gpl/WDMyCloud_DL2100_GPL_v2.21.126_20161110.zip).


### Kernel-specific configuration files

You can create configurations for specific kernel versions by naming them
<samp>wd-dl2100-X.Y[.Z].config</samp>, where X.Y or X.Y.Z is the kernel version.
A configuration file with version X.Y.Z will have precedence over one with X.Y.
X.Y.Z is the version as output by <samp>make kernelversion</samp>.


### Fallback configuration file

A file named <samp>wd-dl2100.config</samp> will be used as default configuration
for kernels that do not have a version-specific configuration file.
