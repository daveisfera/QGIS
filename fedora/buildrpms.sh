#!/bin/bash

# Some definitions
outdir="result/"
logdir="result/logs"
archs=( "fedora-19-i386"
        "fedora-19-x86_64"
        "fedora-20-i386"
        "fedora-20-x86_64"
      )

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
mock --buildsrpm --spec qgis.spec --sources ./sources --resultdir=$outdir


srpm=$(grep -e 'Wrote: .*\.src\.rpm' $outdir/build.log |
    sed 's_Wrote: /builddir/build/SRPMS/\(.*\)_\1_')

for arch in "${archs[@]}"
do :
  echo "Building for $arch"
  mkdir $outdir/$arch
  mock -r $arch --rebuild $outdir/$srpm --resultdir=$outdir/$arch
done
