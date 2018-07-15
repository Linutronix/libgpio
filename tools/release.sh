#! /bin/bash

set -e

VERSION=$(echo "$1" | grep -E "[0-9]+\.[0-9]+\.[0-9]+")

if [ x"$VERSION" == x ]
then
    echo "usage: $0 <version>"
    exit 1
fi

UNCLEAN=$(git status --porcelain | grep -v -E 'libgpio-.*tar.*' || true)

if [ x"$UNCLEAN" != x ]
then
    echo "Source code changes detected:"
    echo "$UNCLEAN"
    echo "Abort!"

    exit 1
fi

echo "Release version $VERSION"

sed -i "s/\\(AC_INIT([^,]*,\\)[^,]*/\1 [$VERSION]/" configure.ac

./autogen.sh
./configure
make check
make distcheck
cp libgpio-$VERSION.tar* ..
make maintainer-clean

git commit -a -s -m "release v$VERSION"
git tag -a -m "release v$VERSION" "v$VERSION"
