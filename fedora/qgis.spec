# WARNING: Rebuild QGIS whenever a new version of GRASS is shipped! Even though the soname might stay the same, it won't work anymore.
#http://hub.qgis.org/issues/5274

%if ! (0%{?fedora} > 12 || 0%{?rhel} > 5)
%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
%endif

# libspatialite is present for PPC and PPC64 from F19 on
%ifarch ppc ppc64
%if ! (0%{?fedora} < 19 || 0%{?rhel})
%global configure_with_spatialite -D WITH_QSPATIALITE:BOOL=TRUE -D WITH_INTERNAL_SPATIALITE:BOOL=FALSE
%endif
%else
%global configure_with_spatialite -D WITH_QSPATIALITE:BOOL=TRUE -D WITH_INTERNAL_SPATIALITE:BOOL=FALSE
%endif

#WITH_GLOBE -- requires osgearth, which is still not packaged
#TODO: Run test suite (see debian/rules)

Name:           qgis
Version:        2.2.0
Release:        %{_relver}%{?dist}
Summary:        A user friendly Open Source Geographic Information System

# There are build issues at the moment; working with upstream
ExcludeArch:    armv7hl

# Update FSF address or ship a GPLv3+ license file
# http://hub.qgis.org/issues/3789
Group:          Applications/Engineering
License:        GPLv3+ with exceptions
URL:            http://www.qgis.org
Source0:        http://qgis.org/downloads/%{name}-%{version}.tar.gz
# The used sources were released as a tarball, the below is only for work in progress
#git archive --format=tar --prefix=qgis-1.8.0/ master | bzip2 >../qgis-1.8.0.tar.gz

# Sample configuration files for QGIS mapserver
Source2:        %{name}-mapserver-httpd.conf
Source4:        %{name}-mapserver-README.fedora

# MIME definitions
# Based on debian/qgis.xml but excluding already defined or proprietary types
Source5:        %{name}-mime.xml

# Fix detection problem for GRASS libraries
Patch0: %{name}-1.5.0-grass.patch

# Remove httplib2 from the build system
Patch2: %{name}-2.2.0-httplib2.patch

# Compilation failure with sip 4.15
# https://hub.qgis.org/issues/8507
# https://github.com/qgis/QGIS/commit/6734ab443a41b5ab5a730651754d9365cf4eaf04
# 8385386493d3b19223825ce2ac9137d6d079a1c2
# 349a4c974fab88fdda0d612dea8defe037ec50ba
# Drop -o option from SIP, as versions older than 4.10 don't have it
#Patch3: %{name}-2.0.1-sip-4.15.patch

# sip: QgsFieldValidator::fixup has versioned and unversioned overloads
# https://hub.qgis.org/issues/8601
#Patch4: %{name}-2.0.1-sip-8601.patch

# Leaving it to make life easier for ELGIS, as long as they target RHEL 5
BuildRoot: %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

# TODO: Pyspatialite is included if you use the bundled libspatialite.
# Some plug-ins need it.
# The license is not totally clear, see:
# http://code.google.com/p/pyspatialite/issues/detail?id=3
# It also is sort of a fork of pysqlite, which is not elegant.

BuildRequires:  cmake
BuildRequires:  desktop-file-utils
BuildRequires:  expat-devel
BuildRequires:  fcgi-devel
BuildRequires:  flex bison
BuildRequires:  gdal-devel
BuildRequires:  geos-devel
BuildRequires:  grass-devel
BuildRequires:  gsl-devel

%ifarch ppc ppc64
%if ! (0%{?fedora} < 19 || 0%{?rhel})
BuildRequires:  libspatialite-devel
%endif
%else
BuildRequires:  libspatialite-devel
%endif

BuildRequires:  postgresql-devel
BuildRequires:  proj-devel
BuildRequires:  PyQt4-devel
BuildRequires:  PyQwt-devel
BuildRequires:  python-devel
BuildRequires:  qt4-devel

# Fails to build with QT_MOBILITY
#http://hub.qgis.org/issues/7753
#BuildRequires:  qt-mobility-devel

BuildRequires:  qt4-webkit-devel
BuildRequires:  qwt-devel

BuildRequires:  qextserialport-devel
BuildRequires:  qwtpolar-devel
BuildRequires:  sip-devel > 4.7
BuildRequires:  spatialindex-devel
BuildRequires:  sqlite-devel
BuildRequires:  qscintilla-devel

# Enable for tests
#BuildRequires:  xorg-x11-server-Xvfb

Requires:       gpsbabel 

# We don't want to provide private Python extension libs
%global __provides_exclude_from ^%{python_sitearch}/.*\.so$

%description
Geographic Information System (GIS) manages, analyzes, and displays
databases of geographic information. Quantum GIS (QGIS) supports shape file
viewing and editing, spatial data storage with PostgreSQL/PostGIS, projection
on-the-fly, map composition, and a number of other features via a plugin
interface. QGIS also supports display of various geo-referenced raster and
Digital Elevation Model (DEM) formats including GeoTIFF, Arc/Info ASCII Grid,
and USGS ASCII DEM.

%package devel
Summary:        Development Libraries for the Quantum GIS
Group:          Development/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description devel
Development packages for Quantum GIS including the C header files.

%package grass 
Summary:        GRASS Support Libraries for Quantum GIS
Group:          Applications/Engineering
Requires:       %{name}%{?_isa} = %{version}-%{release}
# The plug-in requires more than just the grass-libs.
#TODO: I think, that is still correct, but the sense of the libs package is questionable
Requires:       grass = 6.4.3
  
%description grass
GRASS plugin for Quantum GIS required to interface with the GRASS system.

%package python 
Summary:        Python integration and plug-ins for Quantum GIS
Group:          Applications/Engineering
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       gdal-python
Requires:       PyQt4
Requires:       python-httplib2
Requires:       python-psycopg2
Requires:       qscintilla-python
%{?_sip_api:Requires: sip-api(%{_sip_api_major}) >= %{_sip_api}}

%description python
Python integration and plug-ins for Quantum GIS.

%package mapserver 
Summary:        FCGI based OGC web map server
Group:          Applications/Engineering
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       mod_fcgid

%description mapserver
This FastCGI OGC web map server implements OGC WMS 1.3.0 and 1.1.1. 
The services are prepared as regular projects in QGIS. They're rendered using
the QGIS libraries. The server also supports SLD (Styled Layer Descriptor)
for styling. Sample configurations for Httpd and Lighttpd are included.

Please refer to %{name}-mapserver-README.fedora for details!

%prep
%setup -q
%patch0 -p1 -b .grass~
%patch2 -p1 -b .httplib2~
#%patch3 -p1 -b .sip415~
#%patch4 -p1 -b .field_val_overload~

# Readme file for QGIS mapserver configuration and Lighttpd sample
install -pm0644 %{SOURCE4} .

# Delete bundled libs
rm -rf src/core/spatialite
rm -rf src/core/gps/qwtpolar-{0.1,1.0}
rm -rf src/core/gps/qextserialport
rm -rf python/plugins/processing/admintools/httplib2


%build

# Necessary for the test suite
#export LD_LIBRARY_PATH=%{_builddir}%{name}-%{version}/output/%{_lib}

#Info: TOUCH needs Qt >= 4.5
%cmake \
      %{_cmake_skip_rpath} \
      -D QGIS_LIB_SUBDIR=%{_lib} \
      -D QGIS_MANUAL_SUBDIR=/share/man \
      -D QGIS_PLUGIN_SUBDIR=%{_lib}/%{name} \
      -D QGIS_CGIBIN_SUBDIR=%{_libexecdir}/%{name} \
      -D WITH_BINDINGS:BOOL=TRUE \
      -D GRASS_PREFIX=%{_libdir}/grass \
      -D WITH_MAPSERVER:BOOL=TRUE \
      -D BINDINGS_GLOBAL_INSTALL:BOOL=TRUE \
      -D GDAL_INCLUDE_DIR=%{_includedir}/gdal \
      -D GDAL_LIBRARY=%{_libdir}/libgdal.so \
      -D ENABLE_TESTS:BOOL=FALSE \
      -D WITH_INTERNAL_QWTPOLAR:BOOL=FALSE \
      -D WITH_INTERNAL_QEXTSERIALPORT:BOOL=FALSE \
      -D WITH_PYSPATIALITE:BOOL=FALSE \
      -D WITH_TOUCH:BOOL=TRUE \
      %{configure_with_spatialite} \
      .
      #-D WITH_QTMOBILITY:BOOL=TRUE \

make %{?_smp_mflags}


%install
# Necessary for the test suite
#export LD_LIBRARY_PATH=%{_builddir}%{name}-%{version}/output/%{_lib}
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

# Add executable perms to python libs so they get stripped
chmod +x %{buildroot}%{python_sitearch}/%{name}/*.so

# Install desktop file without connecting proprietary file types
desktop-file-install \
%if (0%{?fedora} && 0%{?fedora} < 19) || (0%{?rhel} && 0%{?rhel} < 7)
    --vendor="fedora" \
%endif
    --remove-mime-type="application/x-raster-ecw" \
    --remove-mime-type="application/x-raster-mrsid" \
    --dir=%{buildroot}%{_datadir}/applications \
    debian/qgis.desktop

desktop-file-install --dir=%{buildroot}%{_datadir}/applications \
    debian/qbrowser.desktop

# Install MIME type definitions
install -d %{buildroot}%{_datadir}/mime/packages
install -pm0644 %{SOURCE5} \
    %{buildroot}%{_datadir}/mime/packages/%{name}.xml

# Install application and MIME icons
install -pd %{buildroot}%{_datadir}/pixmaps
install -pd %{buildroot}%{_datadir}/icons/hicolor/16x16/apps
install -pd %{buildroot}%{_datadir}/icons/hicolor/scalable/apps
install -pd %{buildroot}%{_datadir}/icons/hicolor/128x128/mimetypes
install -pm0644 \
    %{buildroot}%{_datadir}/%{name}/images/icons/%{name}-icon.png \
    %{buildroot}%{_datadir}/pixmaps/%{name}-icon.png
install -pm0644 \
    images/icons/%{name}-icon-16x16.png \
    %{buildroot}%{_datadir}/icons/hicolor/16x16/apps/%{name}-icon.png
install -pm0644 \
    images/icons/%{name}_icon.svg \
    %{buildroot}%{_datadir}/icons/hicolor/scalable/apps/%{name}-icon.svg
install -pm0644 \
    %{buildroot}%{_datadir}/%{name}/images/icons/%{name}-mime-icon.png \
    %{buildroot}%{_datadir}/icons/hicolor/128x128/mimetypes/application-x-qgis-layer-settings.png
install -pm0644 \
    %{buildroot}%{_datadir}/%{name}/images/icons/%{name}-mime-icon.png \
    %{buildroot}%{_datadir}/icons/hicolor/128x128/mimetypes/application-x-qgis-project.png

# Install basic QGIS Mapserver configuration for Apache
install -pd %{buildroot}%{_sysconfdir}/httpd/conf.d
install -pm0644 %{SOURCE2} \
    %{buildroot}%{_sysconfdir}/httpd/conf.d/qgis-mapserver.conf

# See qgis-mapserver-README.fedora
rm -f %{buildroot}%{_libexecdir}/%{name}/wms_metadata.xml
rm -f %{buildroot}%{_libexecdir}/%{name}/admin.sld

# Remove files packaged by doc
pushd %{buildroot}%{_datadir}/%{name}/doc
  rm -f BUGS \
      CHANGELOG \
      CODING \
      COPYING \
      INSTALL \
      PROVENANCE \
      README
popd

# Name of locale is wrong
#mv %{buildroot}/usr/share/qgis/i18n/qgis_sr-Latn.qm \
    #%{buildroot}/usr/share/qgis/i18n/qgis_sr@latin.qm

%find_lang %{name} --with-qt


%check
# All tests basically run fine, but one fails using mock, while a different one fails when building with rpmbuild alone
#export LD_LIBRARY_PATH=%{buildroot}%{_libdir}
#xvfb-run -a -n 1 -s "-screen 0 1280x1024x24 -dpi 96" make Experimental
#rm -f %{_bindir}%{name}_bench


%clean
rm -rf %{buildroot}


%post
/sbin/ldconfig
touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :
update-mime-database %{_datadir}/mime &> /dev/null || :

%postun
/sbin/ldconfig
if [ $1 -eq 0 ] ; then
    touch --no-create %{_datadir}/icons/hicolor &>/dev/null
        gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
        fi

update-mime-database %{_datadir}/mime &> /dev/null || :

%posttrans
gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

%post grass -p /sbin/ldconfig

%postun grass -p /sbin/ldconfig

%post python -p /sbin/ldconfig

%postun python -p /sbin/ldconfig

%files -f %{name}.lang
#TODO: Encoding problem on the help page
# CONTRIBUTORS and AUTHORS are intended be viewed in the About-Box
# ChangeLog is 4 MB, therefore don't ship it
%doc BUGS NEWS CODING COPYING Exception_to_GPL_for_Qt.txt PROVENANCE README
# QGIS shows these files in the GUI
%{_datadir}/%{name}/doc

%dir %{_datadir}/%{name}/i18n/
%if 0%{?rhel}
%lang(sr@latin) %{_datadir}/%{name}/i18n/%{name}_sr@latin.qm
%endif
%{_libdir}/lib%{name}_analysis.so.*
%{_libdir}/lib%{name}_core.so.*
%{_libdir}/lib%{name}_gui.so.*
%{_libdir}/lib%{name}sqlanyconnection.so.*
%{_libdir}/lib%{name}_networkanalysis.so.*
%{_libdir}/%{name}
%{_qt4_prefix}/plugins/sqldrivers/libqsqlspatialite.so
%{_bindir}/%{name}
%{_bindir}/qbrowser
%{_mandir}/man1/%{name}.1*
%{_mandir}/man1/qbrowser.1*
%dir %{_datadir}/%{name}/
%{_datadir}/mime/packages/qgis.xml
%{_datadir}/pixmaps/%{name}-icon.png
%{_datadir}/icons/hicolor/16x16/apps/%{name}-icon.png
%{_datadir}/icons/hicolor/128x128/mimetypes/application-x-qgis-project.png
%{_datadir}/icons/hicolor/128x128/mimetypes/application-x-qgis-layer-settings.png
%{_datadir}/icons/hicolor/scalable/apps/%{name}-icon.svg
%{_datadir}/applications/*%{name}.desktop
%{_datadir}/applications/qbrowser.desktop
%{_datadir}/%{name}/images
%{_datadir}/%{name}/resources
%{_datadir}/%{name}/svg
%exclude %{_libdir}/libqgisgrass.so.*
%exclude %{_libdir}/%{name}/libgrassprovider.so
%exclude %{_libdir}/%{name}/libgrassrasterprovider.so
%exclude %{_libdir}/%{name}/libgrassplugin.so
%exclude %{_libdir}/%{name}/grass

%files devel
%{_datadir}/%{name}/FindQGIS.cmake
%{_includedir}/%{name}
%{_libdir}/lib%{name}*.so

%files grass
%{_libdir}/lib%{name}grass.so.*
%{_libdir}/%{name}/libgrassprovider.so
%{_libdir}/%{name}/libgrassrasterprovider.so
%{_libdir}/%{name}/libgrassplugin.so
%{_libdir}/%{name}/grass
%{_datadir}/%{name}/grass

%files python
%{_libdir}/libqgispython.so.*
%{_datadir}/%{name}/python
%{python_sitearch}/%{name}

%files mapserver
%doc src/mapserver/admin.sld src/mapserver/wms_metadata.xml %{name}-mapserver-README.fedora
%config(noreplace) %{_sysconfdir}/httpd/conf.d/%{name}-mapserver.conf
%{_libexecdir}/%{name}

%changelog
* Tue Mar 11 2014 Matthias Kuhn <matthias.kuhn@gmx.ch> - 2.2.0-2
- Updated to latest patchset

* Sat Feb 22 2014 Matthias Kuhn <matthias.kuhn@gmx.ch> - 2.2.0-1
- Update for release 2.2.0

* Fri Feb 07 2014 Volker Fröhlich <volker27@gmx.at> - 2.0.1-8
- Rebuild for minor ABI breakage in spatialindex 1.8.1

* Wed Dec 25 2013 Volker Fröhlich <volker27@gmx.at> - 2.0.1-7
- Python sub-package must require psycopg2 for the Processing plug-in
  (BZ #1043683)

* Fri Nov 08 2013 Volker Fröhlich <volker27@gmx.at> - 2.0.1-6
- Rebuild for new qwt and qwtpolar

* Sat Oct 19 2013 Volker Fröhlich <volker27@gmx.at> - 2.0.1-5
- Patch QGIS #8507 -- QGIS trunk failed to compile with sip 4.15
- Patch QGIS #8601 -- sip: QgsFieldValidator::fixup has versioned and
  unversioned overloads

* Wed Oct 16 2013 Rex Dieter <rdieter@fedoraproject.org> 2.0.1-4
- rebuild (sip)

* Tue Oct  1 2013 Volker Fröhlich <volker27@gmx.at> - 2.0.1-3
- Fix Grass version to make breakage more visible

* Fri Sep 27 2013 Volker Fröhlich <volker27@gmx.at> - 2.0.1-2
- Remove import path for httplib2 module to use the system version
- Use upstream desktop files

* Wed Sep 25 2013 Volker Fröhlich <volker27@gmx.at> - 2.0.1-1
- New upstream release
- Exclude ARM for now, due to build issues

* Sat Sep 14 2013 Volker Fröhlich <volker27@gmx.at> - 1.8.0-17
- Rebuild for grass 6.4.3

* Tue Aug 27 2013 Orion Poplawski <orion@cora.nwra.com> - 1.8.0-16
- Rebuild for gdal 1.10.0

* Sun Aug 04 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.8.0-15
- Rebuilt for https://fedoraproject.org/wiki/Fedora_20_Mass_Rebuild

* Mon Jun 17 2013 Rex Dieter <rdieter@fedoraproject.org> 1.8.0-14
- rebuild (sip)

* Fri Feb 22 2013 Toshio Kuratomi <toshio@fedoraproject.org> - 1.8.0-13
- Remove --vendor from desktop-file-install https://fedorahosted.org/fesco/ticket/1077

* Thu Feb 14 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.8.0-12
- Rebuilt for https://fedoraproject.org/wiki/Fedora_19_Mass_Rebuild

* Sat Dec 15 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-11
- Rebuild for spatialindex soname bump

* Sun Dec  2 2012 Bruno Wolff III <bruno@wolff.to> - 1.8.0-10
- Rebuild for libspatialite soname bump

* Thu Nov  8 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-9
- Build with system version of qextserialport
- Update config file to fit httpd 2.4 (BZ#871471)

* Wed Oct 03 2012 Rex Dieter <rdieter@fedoraproject.org> 1.8.0-8
- rebuild (sip)

* Thu Aug  9 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-7
- Append ppc to ppc64 conditionals

* Wed Jul 18 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-6
- Add patch for QGIS bug #5809

* Sat Jul  7 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-5
- Rebuilt too quick

* Sat Jul  7 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-4
- One more rebuild, due to a broken GDAL

* Fri Jul  6 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-3
- Rebuild for Spatialite 3

* Thu Jul  5 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-2
- Correct locale if clause
- Apply patch for older versions of SIP

* Fri Jun 29 2012 Volker Fröhlich <volker27@gmx.at> - 1.8.0-1
- New upstream release
- Correct provides-filtering as of https://fedoraproject.org/wiki/Packaging:AutoProvidesAndRequiresFiltering#Usage
- Drop obsolete spatialindex patch
- FSF addresses are now correct
- Explicitly set PYSPATIALITE to false
- Don't ship the 4 MB changelog
- Use wildcard for soname versions

* Tue Apr 10 2012 Volker Fröhlich <volker27@gmx.at> - 1.7.4-4
- Been to quick with rebuilding, proper spatialindex build wasn't used yet 

* Mon Apr  9 2012 Volker Fröhlich <volker27@gmx.at> - 1.7.4-3
- Apply patch for Spatialindex 1.7's include dir 
- Rebuild for Spatialindex 1.7.1

* Fri Mar 23 2012 Volker Fröhlich <volker27@gmx.at> - 1.7.4-2
- Rebuild for GRASS 6.4.2

* Sun Feb 19 2012 Volker Fröhlich <volker27@gmx.at> - 1.7.4-1
- Update for new release

* Sat Jan 14 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.7.3-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_17_Mass_Rebuild

* Sat Dec 10 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.3-1
- Update for new release
- Is also the rebuild for BZ#761147
- Arch-specifically require the base package

* Tue Nov 15 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.2-1
- Updated for new release
- No more themes directory
- Remove dispensable geo-referencing patch

* Sun Oct 16 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.1-2
- Findlang doesn't recognize sr@latin in Fedora 14 and older
- Build with system-wide spatialindex
- Remove if structures intended for EPEL package
  Due to the rapid development in QGIS and the libraries it uses,
  QGIS will not go to EPEL now; ELGIS provides rebuilds with more
  current versions: http://elgis.argeo.org/

* Sat Sep 24 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.1-1
- Update for new release
- Drop one patch that made it into the release
- Correct permissions for two cpp files
- Change spelling for changelog file
- Findlang seems to find sr@latin now, so don't explicitly list it
- Build with Qwtpolar
- Remove grass as BR
- Add GRASS_PREFIX again
  Cmake uses the first entry in FindGRASS.cmake,
  which is not fine for 64 bit systems

* Sun Jul 24 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.0-5
- Extend histogram patch, see BZ 725161

* Wed Jul 06 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.0-4
- Remove icon path macros, that don't work in EPEL
- Avoid creating unnecessary directories there
- Be explicit about the shared lib names in the files section
- Put libqgispython.so.1.7.0 in Python sub-package
- Correct FSF address

* Wed Jul 06 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.0-3
- Withdraw immature Lighty configuration
- Update README and provide a better sample configuration,
  that utilizes mod_rewrite

* Sun Jun 26 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.0-2
- Add histogram patch
- Add scriplets to refresh icon cache
- Mention Fedora specific readme file in QGIS mapserver description
- Update the aforementioned file and the Apache configuration
  with rewrite rules

* Sun Jun 26 2011 Volker Fröhlich <volker27@gmx.at> - 1.7.0-1
- Update for 1.7
- License is now GPLv3+ (Sqlanyconnect)
- Add mapserver sub-package and require mod_fcgi
- Use upstreams current description text
- Drop now needless iconv 
- Rename new Serbian translations
- Install MIME type definitions and icons
- Add Readme file and sample configuration for Mapserver
- Add patch to avoid segfault when geo-referencing
- Add conditional for Spatialite and PPC64
- Add conditional for GRASS and EPEL
- Delete bundled libspatialite before building
- Removed glob from /usr/bin/qgis in files section

* Thu Nov 11 2010 Volker Fröhlich <volker27@gmx.at> - 1.6.0-2
- Disabled smp-flags again 

* Thu Nov 11 2010 Volker Fröhlich <volker27@gmx.at> - 1.6.0-1
- Avoid rpaths in the first place
- Dropped superfluid QWT and SIP patches
- Dropped test section
- Added dependency for fcgi-devel
- Abbreviated syntax for setup macro
- Qt translations are no longer shipped

* Wed Sep 29 2010 jkeating - 1.5.0-6
- Rebuilt for gcc bug 634757

* Mon Sep 13 2010 Volker Fröhlich <volker27@gmx.at> - 1.5.0-5
- Added workaround patch for SIP 4.11, see http://trac.osgeo.org/qgis/ticket/2985

* Thu Sep 09 2010 Rex Dieter <rdieter@fedoraproject.org> 1.5.0-4
- rebuild (sip)
- BR: qt4-devel

* Fri Jul 30 2010 Volker Fröhlich <volker27@gmx.at> - 1.5.0-3
- Added dependency for gdal-python to fulfill standard plugins' requirements

* Tue Jul 27 2010 Rex Dieter <rdieter@fedoraproject.org> - 1.5.0-2.py27
- one more time for python27, with feeling

* Thu Jul 22 2010 David Malcolm <dmalcolm@redhat.com> - 1.5.0-2
- Rebuilt for https://fedoraproject.org/wiki/Features/Python_2.7/MassRebuild

* Sat Jul 17 2010 Volker Fröhlich <volker27@gmx.at> - 1.5.0-1
- Updated for 1.5.0
- Added support for qwt

* Wed Jul 14 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-9
- Re-added missing dependency of PyQt4 and sip for python sub-package

* Fri Jul 09 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-8
- Further completed qt47 patch

* Fri Jul 09 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-7
- Further completed qt47 patch

* Fri Jul 09 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-6
- Added patch to remove redundant ::QVariant in function-style cast

* Fri Jul 09 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-5
- Removed smp_mflags because of race conditions
- Simplified member initializer syntax in qt47 patch

* Wed Jul 07 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-4
- Added preliminary patch for Qt 4.7 -- still won't build for Rawhide because of https://bugzilla.redhat.com/show_bug.cgi?id=587707
- Put version variable in filelist for GRASS
- Added qt-webkit as build require, removed gettext
- Corrected erroneous dependency on GRASS subpackage

* Thu Jul 01 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-3
- Changed license to reflect exception for Qt; packaged exception file
- Added find_lang and excluded qt-translations
- Added ownership for directory in share
- Dropped docdir statement
- Changed description for the subpackages python and grass

* Wed Jun 23 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-2
- Grouped corresponding entries for description and package macro
- Corrected swapped summaries for python- and grass-subpackage
- Set theme subpackages as obsolete 
- Removed nonsensical build-requires statements from subpackages
- Dropped redundant definition of GRASS_PREFIX
- Removed verbose-option from make
- Removed unnecessary chmod-command
- Removed nonsensical ldconfig for devel-subpackage
- Made the file list more elegant
- Removed unnecessary call for chrpath

* Thu Jun 17 2010 Volker Fröhlich <volker27@gmx.at> - 1.4.0-1
- Updated for 1.4.0
- Dropped theme packages

* Wed Feb 10 2010 Kevin Kofler <Kevin@tigcc.ticalc.org> - 1.0.2-6
- rebuild for sip 4.10
- fix sip version check so it works with sip 4.10 (#553713)

* Wed Jan 13 2010 Devrim GÜNDÜZ <devrim@gunduz.org> - 1.0.2-5
- Rebuild against new GEOS.

* Wed Dec 23 2009 Rex Dieter <rdieter@fedoraproject.org> - 1.0.2-4
- qgis rebuild for sip-4.9.x (#538119)

* Fri Dec 04 2009 Devrim GÜNDÜZ <devrim@gunduz.org> - 1.0.2-3
- Rebuild for new Geos.

* Tue Nov 17 2009 Rex Dieter <rdieter@fedoraproject.org> - 1.0.2-2 
- -python: Requires: sip-api(%%_sip_api_major) >= %%_sip_api (#538119) 

* Thu Oct 22 2009 Alex Lancaster <alexlan[AT] fedoraproject org> - 1.0.2-1.1
- Rebuilt to fix python problem (#518121)

* Thu Jul 30 2009 Douglas E. Warner <silfreed@silfreed.net> 1.0.2-1
- updating for 1.0.2
- moving libqgispython.so to python subpackage for bug#507381

* Wed Feb 25 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.0.1-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_11_Mass_Rebuild

* Wed Feb 18 2009 Douglas E. Warner <silfreed@silfreed.net> 1.0.1-1
- updating for 1.0.1

* Mon Dec 22 2008 Douglas E. Warner <silfreed@silfreed.net> 1.0.0-1
- updating for 1.0.0
- added gis theme and quick print plugin
- added requirement for gpsbabel

* Mon Dec 22 2008 Douglas E. Warner <silfreed@silfreed.net> - 0.11.0-8
- cleaning up patch

* Mon Dec 22 2008 Douglas E. Warner <silfreed@silfreed.net> - 0.11.0-7
- bump to add patch

* Thu Dec 18 2008 Douglas E. Warner <silfreed@silfreed.net> - 0.11.0-6
- adding patch to fix typedef problems in python build

* Thu Dec 18 2008 Douglas E. Warner <silfreed@silfreed.net> - 0.11.0-5
- Rebuild for Python 2.6

* Sat Nov 29 2008 Ignacio Vazquez-Abrams <ivazqueznet+rpm@gmail.com> - 0.11.0-4
- Rebuild for Python 2.6

* Sun Oct 19 2008 Alex Lancaster <alexlan[AT]fedoraproject org> - 0.11.0-3
- Rebuild for new geos, fixes broken deps

* Mon Aug 11 2008 Douglas E. Warner <silfreed@silfreed.net> 0.11.0-2
- building against blas and lapack instead of atlas and blas to fix missing
  library calls

* Wed Jul 16 2008 Douglas E. Warner <silfreed@silfreed.net> 0.11.0-1
- update to metis 0.11.0
- remove python patch
- enabling python subpackage
- fixed executable perms on new headers/source
- stripping rpath with chrpath
- making python libs executable to get files stripped

* Fri Mar 28 2008 Douglas E. Warner <silfreed@silfreed.net> 0.10.0-2
- added patch to support cmake 2.6

* Fri Mar 28 2008 Balint Cristian <rezso@rdsor.ro> - 0.10.0-1
- upgraded to 0.10.0 release candidate
- removed gcc 4.3 patches
- adding devel package since libraries are now versioned

* Fri Mar 28 2008 Balint Cristian <rezso@rdsor.ro> - 0.9.1-5
- fix build by adding more gcc43 patches
- rebuild against grass63

* Tue Feb 19 2008 Fedora Release Engineering <rel-eng@fedoraproject.org> - 0.9.1-4
- Autorebuild for GCC 4.3

* Mon Feb 18 2008 Douglas E. Warner <silfreed@silfreed.net> 0.9.1-4
- adding gcc43-INT_MAX-qgis-0.9.1.patch
- adding gcc43-duplicate_function_param-qgis-0.9.1.patch
- adding gcc43-memcpy-qgis-0.9.1.patch
- adding gcc43-memcpy-strtod-qgis-0.9.1.patch

* Mon Feb 18 2008 Douglas E. Warner <silfreed@silfreed.net> 0.9.1-3
- adding Requires sip, PyQt4 for python bindings support

* Mon Jan 28 2008 Douglas E. Warner <silfreed@silfreed.net> 0.9.1-2
- defining lib path in build
- installing python bindings globally
- adding patch to determine python site packages dir correctly

* Mon Dec 17 2007 Douglas E. Warner <silfreed@silfreed.net> 0.9.1-1
- update to 0.9.1
- removing lib64 and man instal path patches (included upstream)
- enabling python integration

* Fri Oct 05 2007 Douglas E. Warner <silfreed@silfreed.net> 0.9.0-2
- enabling build for PPC64 (bug#247152)

* Wed Sep 26 2007 Douglas E. Warner <silfreed@silfreed.net> 0.9.0-1
- update to 0.9.0
- remove settings-include-workdir.patch
- updated man-install-share.patch to man-install-share-0.9.0.patch
- updated lib64-suffix.patch to lib64-suffix-0.9.0.patch
- enabled python to support msexport tool
- added Requires: grass to grass subpackage

* Tue Aug 28 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-13
- bump for expat 2.0 rebuild bug#195888

* Thu Aug 02 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-12
- updated License from GPL to GPLv2+

* Tue Jul 10 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-11
- allowing docs to be installed by qgis so they can be referenced by internal
  help system (bug#241403)
- updated lib64 patch (bug#247549) to try to get plugins found on x86_64

* Thu Jul 05 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-10
- updated lib64 patch for core and grass libraries

* Thu Jul 05 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-9
- updated lib64 patch

* Thu Jul 05 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-8
- adding ExcludeArch: ppc64 for bug#247152 (lrelease segfault)

* Thu Jul 05 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-7
- adding patch for lib64 support through lib_suffix

* Thu Jun 28 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-6
- fixed date of changelog entry for 0.8.1-5 from Wed Jun 27 2007 to
  Thu Jun 28 2007
- linking icon to included png instead of packaging it again

* Thu Jun 28 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-5
- adding comment on why grass is required in addition to grass-devel for BR
- fixing typo

* Wed Jun 27 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-4
- adding contributors to doc
- adding desktop file and icon

* Mon Jun 25 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-3
- updating detection of grass libraries to not use RPM call
- disabling building of -devel package due to shared libraries not being 
  versioned and having no other packages that compile against qgis
  (see bug #241403)
- removing chmod of test_export.py due to lack of python requirement
- removing msexport and share/python directory due to removal of python

* Fri Jun 22 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-2
- added BuildRequires: cmake
- updated build to use cmake macro and make verbose

* Mon Jun 18 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.1-1
- updating version
- removed BuildRequires: python-devel due to lack of PyQt4 bindings
- updated build for use of cmake instead of autotools
- added patch for setting WORKDIR in settings.pro file
- added patch for fixing install path of man pages
- updated library names

* Tue May 29 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.0-6
- adding BuildRequires bison, flex

* Tue May 29 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.0-5
- fixing more directory owernship (themes, themes-default)
- fixing qt4 hardcoded lib path
- removing Requires ldconfig
- adding BuildRequires sqlite-devel
- adding patch for supporting python 2.5 in configure script

* Sat May 26 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.0-4
- moving all BuildRequires to main section
- dropping use of makeinstall macro
- making sure directories are owned by this package
- removing *.a and *.la files
- disabled stripping of libraries and binaries to allow debuginfo package
  to be built
- fixing macros in changelog
- removing executable bits on source files

* Wed May 16 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.0-3
- fixing Requires statements for sub-packages

* Tue May 15 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.0-2
- added devel dependancy on qgis
- moved qgis-config to devel package
- moving doc directory
- removed zero-length NEWS doc
- added postin/postun ldconfig calls
- split packages up to reduce package size and split out dependancies
  grass, theme-nkids

* Mon May 14 2007 Douglas E. Warner <silfreed@silfreed.net> 0.8.0-1
- Initial RPM release.

