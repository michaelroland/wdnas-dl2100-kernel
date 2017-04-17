#!/bin/bash

################################################################################
## 
## Fetch, extract, patch, and build kernel
## 
## Copyright (c) 2017 Michael Roland <mi.roland@gmail.com>
## 
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
## 
################################################################################


SCRIPT_NAME=$(basename $0)
SCRIPT_PATH=$(readlink -f "$(dirname $0)")

currentdir=$(pwd)
patchesdir=$currentdir/kernel-patches
kernelconfigbase=$currentdir/kernel-config/wd-dl2100
kernelbasedir=$currentdir/kernel
localkernelver=-$(date '+%Y%m%d')-dl2100

usage() {
	echo "Usage: ${SCRIPT_NAME} [options] kernel-source"
	echo "Fetch, extract, patch, and build kernel"
	echo ""
	echo -e "  <kernel-source>"
	echo -e "  \tPath to the kernel to be built"
	echo -e "  "
	echo -e "Options:"
	echo -e "\t-h          Show this message"
	echo ""
	echo "Copyright (c) 2017 Michael Roland <mi.roland@gmail.com>"
	echo "License GPLv3+: GNU GPL version 3 or later <http://www.gnu.org/licenses/>"
	echo ""
	echo "This is free software: you can redistribute and/or modify it under the"
	echo "terms of the GPLv3+.  There is NO WARRANTY; not even the implied warranty"
	echo "of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."
	echo ""
}

while getopts ":h?:" opt; do
    case "$opt" in
    h|\?)
        if [ ! -z $OPTARG ] ; then
            echo "${SCRIPT_NAME}: invalid option -- $OPTARG" >&2
        fi
        usage
        exit 1
        ;;
    esac
done

shift $((OPTIND-1))
[ "$1" = "--" ] && shift

if [ -z "$1" ] ; then
	echo "${SCRIPT_NAME}: missing argument -- kernel-source" >&2
	usage
	exit 1
fi

kerneldir=$(readlink -f $1)
kernelextractdir=$kerneldir

if [ -f $kerneldir ] ; then
	kernelextractdir=$(tar -tf $kerneldir | sed -e 's@/.*@@' | uniq)
	if [ "$(echo "$kernelextractdir" | wc -l)" -ne "1" ] ; then
		echo "${SCRIPT_NAME}: $kerneldir is not a valid kernel source package" >&2
		exit 2
	fi

	kernelextractdir=$kernelbasedir/$kernelextractdir
	if [ -e $kernelextractdir ] ; then
		echo "${SCRIPT_NAME}: $kernelextractdir exists, not unpacking" >&2
		exit 3
	fi

	echo "Unpacking kernel source package $(basename $kernelextractdir)"
	cd $kernelbasedir
	tar -xf $kerneldir
	cd $currentdir
fi

if [ ! -d $kernelextractdir ] ; then
	echo "${SCRIPT_NAME}: $kernelextractdir is not a directory" >&2
	exit 4
fi

cd $kernelextractdir

kernelver=$(make kernelversion)
if [ "$?" -ne "0" ] ; then
	echo "${SCRIPT_NAME}: $kernelextractdir is not a valid kernel source tree" >&2
	exit 5
fi

if [ -f "${kernelconfigbase}-${kernelver}.config" ] ; then
	echo "Applying configuration from $(basename ${kernelconfigbase}-${kernelver}.config)"
	cp -f "${kernelconfigbase}-${kernelver}.config" .config
else
	echo "Applying configuration from $(basename ${kernelconfigbase}.config)"
	cp -f "${kernelconfigbase}.config" .config
fi

if [ -f .localversion ] ; then
	localkernelver=$(cat .localversion)
else
	echo -n "$localkernelver" >.localversion
fi

for patchfile in $patchesdir/*.patch ; do
	if [ -f $patchfile ] ; then
		echo "Applying patch $(basename $patchfile) ..."
		patch -p1 -l -N <$patchfile
	fi
done

kernelrel=$(make kernelrelease LOCALVERSION=$localkernelver)

echo "Building kernel $(basename $kernelextractdir) release $kernelrel ..."
make deb-pkg LOCALVERSION=$localkernelver
kernelsuffix=$(cat .version)

cd $currentdir

