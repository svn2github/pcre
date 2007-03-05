#!/bin/sh

set -ex

rm -rf autom4te.cache

aclocal --force

libtoolize -c -f

autoconf -f -W all,no-obsolete

automake -a -c -f -W all

autoheader -f -W all

rm -rf autom4te.cache

exit 0

# end autogen.sh
