#!/bin/bash

################################################################################
## 
## Build and install kernel and additional modules
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

usage() {
	echo "Usage: ${SCRIPT_NAME} [options] kernel-source"
	echo "Build and install kernel and additional modules"
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

kerneldir=$1

source ./build-kernel.sh $kerneldir
sudo ./install-kernel.sh $kerneldir
./build-modules.sh $kernelrel
sudo ./install-modules.sh $kernelrel

