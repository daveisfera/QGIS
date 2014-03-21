#!/bin/bash

# Define some default values...
RELVER=1
OUTDIR="result/"
ARCHS=( "fedora-19-i386"
        "fedora-19-x86_64"
        "fedora-20-i386"
        "fedora-20-x86_64"
      )

# Load config values from config file
source config

# Clean logfiles
rm $OUTDIR/build.log

# Get the version string
major=$(grep -e 'SET(CPACK_PACKAGE_VERSION_MAJOR' ../CMakeLists.txt |
    sed 's/.*\([0-9]\).*/\1/')
minor=$(grep -e 'SET(CPACK_PACKAGE_VERSION_MINOR' ../CMakeLists.txt |
    sed 's/.*\([0-9]\).*/\1/')
patch=$(grep -e 'SET(CPACK_PACKAGE_VERSION_PATCH' ../CMakeLists.txt |
    sed 's/.*\([0-9]\).*/\1/')

version=$(echo $major.$minor.$patch)

# Current git branch
branch=$(git branch --no-color 2> /dev/null |
    sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/')

# Create source tarball
cd ..
git archive --format=tar --prefix=qgis-$version/ $branch | bzip2 >fedora/qgis-$version.tar.gz
cd fedora

# Build source package
mock --buildsrpm --spec qgis.spec --sources ./sources --define "_relver $RELVER" --resultdir=$OUTDIR


srpm=$(grep -e 'Wrote: .*\.src\.rpm' $OUTDIR/build.log |
    sed 's_Wrote: /builddir/build/SRPMS/\(.*\)_\1_')

echo "-- SRPM: $srpm"

for arch in "${ARCHS[@]}"
do :
  echo "Building for $arch"
  mkdir $OUTDIR/$arch
  mock -r $arch --rebuild $OUTDIR/$srpm --define "_relver $RELVER" --resultdir=$OUTDIR/$arch
done
