#!/bin/bash

# Load default config
source default.cfg
# Load local config file
if [ -f local.cfg ]
then
  source local.cfg
fi

# Clean logfiles
if [ -f $OUTDIR/build.log ]
then
  echo ".. Cleaning log file"
  rm $OUTDIR/build.log
fi

# Get the version string
major=$(grep -e 'SET(CPACK_PACKAGE_VERSION_MAJOR' ../CMakeLists.txt |
    sed 's/.*\([0-9]\).*/\1/')
minor=$(grep -e 'SET(CPACK_PACKAGE_VERSION_MINOR' ../CMakeLists.txt |
    sed 's/.*\([0-9]\).*/\1/')
patch=$(grep -e 'SET(CPACK_PACKAGE_VERSION_PATCH' ../CMakeLists.txt |
    sed 's/.*\([0-9]\).*/\1/')

version=$(echo $major.$minor.$patch)

echo ".. Building version $version"

# Current git branch name
branch=$(git branch --no-color 2> /dev/null |
    sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/')

echo ".. Creating source tarball"
# Create source tarball
cd ..
git archive --format=tar --prefix=qgis-$version/ $BRANCH | bzip2 >fedora/sources/qgis-$version.tar.gz
cd fedora

echo ".. Creating source package"
# Build source package
mock --buildsrpm --spec qgis.spec --sources ./sources --define "_relver $RELVER" --resultdir=$OUTDIR
if [ $? -ne 0 ]
then
  echo ".. Creating source package failed"
  exit 1
fi

srpm=$(grep -e 'Wrote: .*\.src\.rpm' $OUTDIR/build.log |
    sed 's_Wrote: /builddir/build/SRPMS/\(.*\)_\1_')

echo ".. Source package created: $srpm"

for arch in "${ARCHS[@]}"
do :
  echo ".. Building packages for $arch"
  mkdir $OUTDIR/$arch
  mock -r $arch --rebuild $OUTDIR/$srpm --define "_relver $RELVER" --resultdir=$OUTDIR/$arch
  if [ $? -eq 0 ]
  then
    echo ".. Signing packages for $arch"
    rpm --resign $OUTDIR/$arch/*-$version-$RELVER.*.rpm
  else
    exit 1
  fi
done
