#!/bin/bash

################################################################################
## 
## Install kernel packages
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
kernelbasedir=$currentdir/kernel

usage() {
	echo "Usage: ${SCRIPT_NAME} [options] kernel-source"
	echo "Install kernel packages"
	echo ""
	echo -e "  <kernel-source>"
	echo -e "  \tPath to the kernel to be installed"
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

	kernelextractdir=$kernelbasedir/$kerneldir
fi

if [ ! -d $kernelextractdir ] ; then
	echo "${SCRIPT_NAME}: $kernelextractdir is not a directory" >&2
	exit 4
fi

cd $kernelextractdir

kernelver=$(make kernelversion)
if [ "$?" -ne "0" ] ; then
	echo "${SCRIPT_NAME}: $kernelextractdir is not a valid kernel source tree" >&2
	exit 4
fi
if [ -f .localversion ] ; then
        localkernelver=$(cat .localversion)
else
        echo "${SCRIPT_NAME}: missing file .localversion -- did you build?"
fi

kernelrel=$(make kernelrelease LOCALVERSION=$localkernelver)
kernelsuffix=$(cat .version)

cd ..
echo "Installing kernel $(basename $kernelextractdir) release $kernelrel ..."
dpkg -i linux-headers-$kernelrel_$kernelrel-$kernelsuffix_*.deb
dpkg -i linux-libc-dev_$kernelrel-$kernelsuffix_*.deb
dpkg -i linux-image-$kernelrel_$kernelrel-$kernelsuffix_*.deb

cd $currentdir

