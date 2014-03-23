#!/bin/bash
function print_info
{
  echo -e "\e[0;32m$1\033[0m"
}

function print_error
{
  echo -e "\033[0;31m$1\033[0m"
}


# Load default config
source default.cfg
# Load local config file
if [ -f local.cfg ]
then
  source local.cfg
fi

# Get next release version number and increment after
if [ ! -f version.cfg ]
then
  echo "RELVER=1" > version.cfg
fi
source version.cfg
let RELVER+=1
echo "RELVER=$RELVER" > version.cfg

# Clean logfiles
if [ -f $OUTDIR/build.log ]
then
  print_info "Cleaning log file"
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

print_info "Building version $version"

# Current git branch name
branch=$(git branch --no-color 2> /dev/null |
    sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/')

print_info "Creating source tarball"
# Create source tarball
cd ..
git archive --format=tar --prefix=qgis-$version/ $BRANCH | bzip2 >fedora/sources/qgis-$version.tar.gz
cd -

print_info "Creating source package"
# Build source package
mock --buildsrpm --spec qgis.spec --sources ./sources --define "_relver $RELVER" --resultdir=$OUTDIR
if [ $? -ne 0 ]
then
  print_error "Creating source package failed"
  exit 1
fi

srpm=$(grep -e 'Wrote: .*\.src\.rpm' $OUTDIR/build.log |
    sed 's_Wrote: /builddir/build/SRPMS/\(.*\)_\1_')

print_info "Source package created: $srpm"

# Create packages for every ARCH defined in the config file
for arch in "${ARCHS[@]}"
do :
  print_info "Building packages for $arch"
  if [ -f $OUTDIR/$arch/build.log ]
  then
    print_info "Cleaning log file"
    rm $OUTDIR/$arch/build.log
  fi
  mkdir $OUTDIR/$arch
  mock -r $arch --rebuild $OUTDIR/$srpm --define "_relver $RELVER" --resultdir=$OUTDIR/$arch
  if [  $? -eq 0 ]
  then
    # Add to package list
    packages="$packages $(ls $OUTDIR/$arch/*-$version-$RELVER.*.rpm)"
  else
    print_error "Package creation for $arch failed. Abort"
    exit 1
  fi
done

if [ $NOSIGN = false ]
then
  print_info "Signing packages"
  rpm --resign $packages
fi

print_info "Done"
