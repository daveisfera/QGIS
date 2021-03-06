#############################################################
# sources

SET(QGIS_CORE_SRCS

  gps/qgsgpsconnection.cpp
  gps/qgsgpsconnectionregistry.cpp
  gps/qgsnmeaconnection.cpp
  gps/qgsgpsdconnection.cpp
  gps/qgsgpsdetector.cpp
  gps/parse.c
  gps/sentence.c
  gps/info.c
  gps/time.c
  gps/gmath.c
  gps/tok.c
  gps/context.c

  symbology-ng/qgssymbolv2.cpp
  symbology-ng/qgssymbollayerv2.cpp
  symbology-ng/qgssymbollayerv2registry.cpp
  symbology-ng/qgssymbollayerv2utils.cpp
  symbology-ng/qgslegendsymbolitemv2.cpp
  symbology-ng/qgslinesymbollayerv2.cpp
  symbology-ng/qgsmarkersymbollayerv2.cpp
  symbology-ng/qgsfillsymbollayerv2.cpp
  symbology-ng/qgsrendererv2.cpp
  symbology-ng/qgsrendererv2registry.cpp
  symbology-ng/qgssinglesymbolrendererv2.cpp
  symbology-ng/qgscategorizedsymbolrendererv2.cpp
  symbology-ng/qgsgraduatedsymbolrendererv2.cpp
  symbology-ng/qgsrulebasedrendererv2.cpp
  symbology-ng/qgsinvertedpolygonrenderer.cpp
  symbology-ng/qgsvectorcolorrampv2.cpp
  symbology-ng/qgscptcityarchive.cpp
  symbology-ng/qgsstylev2.cpp
  symbology-ng/qgssymbologyv2conversion.cpp
  symbology-ng/qgssvgcache.cpp
  symbology-ng/qgsellipsesymbollayerv2.cpp
  symbology-ng/qgspointdisplacementrenderer.cpp
  symbology-ng/qgsvectorfieldsymbollayer.cpp
  symbology-ng/qgscolorbrewerpalette.cpp

  diagram/qgsdiagram.cpp
  diagram/qgspiediagram.cpp
  diagram/qgstextdiagram.cpp
  diagram/qgshistogramdiagram.cpp

  layertree/qgslayertreegroup.cpp
  layertree/qgslayertreelayer.cpp
  layertree/qgslayertreemodel.cpp
  layertree/qgslayertreemodellegendnode.cpp
  layertree/qgslayertreenode.cpp
  layertree/qgslayertreeregistrybridge.cpp
  layertree/qgslayertreeutils.cpp

  qgis.cpp
  qgsapplication.cpp
  qgsattributeaction.cpp
  qgsbrowsermodel.cpp
  qgscachedfeatureiterator.cpp
  qgscacheindex.cpp
  qgscacheindexfeatureid.cpp
  qgsclipper.cpp
  qgscolorscheme.cpp
  qgscolorschemeregistry.cpp
  qgscontexthelp.cpp
  qgscontexthelp_texts.cpp
  qgscoordinatereferencesystem.cpp
  qgscoordinatetransform.cpp
  qgscredentials.cpp
  qgsdartmeasurement.cpp
  qgscrscache.cpp
  qgsdatadefined.cpp
  qgsdatasourceuri.cpp
  qgsdataitem.cpp
  qgsdatumtransformstore.cpp
  qgsdbfilterproxymodel.cpp
  qgsdiagramrendererv2.cpp
  qgsdistancearea.cpp
  qgserror.cpp
  qgsexpression.cpp
  qgsexpression_texts.cpp
  qgsexpressionfieldbuffer.cpp
  qgsfeature.cpp
  qgsfeatureiterator.cpp
  qgsfeaturerequest.cpp
  qgsfeaturestore.cpp
  qgsfield.cpp
  qgsfontutils.cpp
  qgsgeometry.cpp
  qgsgeometrycache.cpp
  qgsgeometrysimplifier.cpp
  qgsgeometryvalidator.cpp
  qgsgml.cpp
  qgsgmlschema.cpp
  qgshttptransaction.cpp
  qgslabel.cpp
  qgslabelattributes.cpp
  qgslabelsearchtree.cpp
  qgslegacyhelpers.cpp
  qgslegendrenderer.cpp
  qgslegendsettings.cpp
  qgslogger.cpp
  qgsmaphittest.cpp
  qgsmaplayer.cpp
  qgsmaplayerlegend.cpp
  qgsmaplayerregistry.cpp
  qgsmaprenderer.cpp
  qgsmaprenderercache.cpp
  qgsmaprenderercustompainterjob.cpp
  qgsmaprendererjob.cpp
  qgsmaprendererparalleljob.cpp
  qgsmaprenderersequentialjob.cpp
  qgsmapsettings.cpp
  qgsmaptopixel.cpp
  qgsmaptopixelgeometrysimplifier.cpp
  qgsmessagelog.cpp
  qgsmessageoutput.cpp
  qgsmimedatautils.cpp
  qgsmultirenderchecker.cpp
  qgsnetworkaccessmanager.cpp
  qgsnetworkcontentfetcher.cpp
  qgsnetworkreplyparser.cpp
  qgsobjectcustomproperties.cpp
  qgsofflineediting.cpp
  qgsogcutils.cpp
  qgsowsconnection.cpp
  qgspaintenginehack.cpp
  qgspallabeling.cpp
  qgspluginlayer.cpp
  qgspluginlayerregistry.cpp
  qgspoint.cpp
  qgsproject.cpp
  qgsprojectfiletransform.cpp
  qgsprojectproperty.cpp
  qgsprojectversion.cpp
  qgsprovidercountcalcevent.cpp
  qgsproviderextentcalcevent.cpp
  qgsprovidermetadata.cpp
  qgsproviderregistry.cpp
  qgspythonrunner.cpp
  qgsrelation.cpp
  qgsrelationmanager.cpp
  qgsrenderchecker.cpp
  qgsrendercontext.cpp
  qgsrectangle.cpp
  qgsrunprocess.cpp
  qgsscalecalculator.cpp
  qgsscaleutils.cpp
  qgssimplifymethod.cpp
  qgssnapper.cpp
  qgsspatialindex.cpp
  qgstolerance.cpp
  qgsvectordataprovider.cpp
  qgsvectorfilewriter.cpp
  qgsvectorlayer.cpp
  qgsvectorlayercache.cpp
  qgsvectorlayereditbuffer.cpp
  qgsvectorlayereditutils.cpp
  qgsvectorlayerfeatureiterator.cpp
  qgsvectorlayerimport.cpp
  qgsvectorlayerjoinbuffer.cpp
  qgsvectorlayerrenderer.cpp
  qgsvectorlayerundocommand.cpp
  qgsvectorsimplifymethod.cpp
  qgsxmlutils.cpp

  composer/qgsaddremoveitemcommand.cpp
  composer/qgsaddremovemultiframecommand.cpp
  composer/qgscomposermultiframecommand.cpp
  composer/qgscomposerarrow.cpp
  composer/qgscomposerframe.cpp
  composer/qgscomposermousehandles.cpp
  composer/qgscomposerobject.cpp
  composer/qgscomposerutils.cpp
  composer/qgscomposeritem.cpp
  composer/qgscomposeritemcommand.cpp
  composer/qgscomposeritemgroup.cpp
  composer/qgscomposerlabel.cpp
  composer/qgscomposerlegenditem.cpp
  composer/qgscomposerpicture.cpp
  composer/qgscomposermap.cpp
  composer/qgscomposermapitem.cpp
  composer/qgscomposermapgrid.cpp
  composer/qgscomposermapoverview.cpp
  composer/qgscomposertable.cpp
  composer/qgscomposertablev2.cpp
  composer/qgscomposertablecolumn.cpp  
  composer/qgscomposerattributetable.cpp
  composer/qgscomposerattributetablev2.cpp
  composer/qgscomposerattributetablemodel.cpp
  composer/qgscomposerattributetablemodelv2.cpp
  composer/qgscomposertexttable.cpp
  composer/qgscomposerscalebar.cpp
  composer/qgscomposershape.cpp
  composer/qgscomposereffect.cpp
  composer/qgsatlascomposition.cpp
  composer/qgslegendmodel.cpp
  composer/qgscomposerlegend.cpp
  composer/qgscomposerlegendstyle.cpp
  composer/qgspaperitem.cpp
  composer/qgsscalebarstyle.cpp
  composer/qgsdoubleboxscalebarstyle.cpp
  composer/qgsnumericscalebarstyle.cpp
  composer/qgssingleboxscalebarstyle.cpp
  composer/qgsticksscalebarstyle.cpp
  composer/qgscomposerhtml.cpp
  composer/qgscomposermultiframe.cpp
  composer/qgscomposermodel.cpp
  composer/qgscomposition.cpp
  
  dxf/qgsdxfexport.cpp
  dxf/qgsdxfpaintdevice.cpp
  dxf/qgsdxfpaintengine.cpp
  dxf/qgsdxfpallabeling.cpp

  pal/costcalculator.cpp
  pal/feature.cpp
  pal/geomfunction.cpp
  pal/labelposition.cpp
  pal/layer.cpp
  pal/pal.cpp
  pal/palstat.cpp
  pal/pointset.cpp
  pal/priorityqueue.cpp
  pal/problem.cpp
  pal/util.cpp
  pal/linkedlist.hpp
  pal/hashtable.hpp
  pal/rtree.hpp

  raster/qgscliptominmaxenhancement.cpp
  raster/qgsraster.cpp
  raster/qgsrasterblock.cpp
  raster/qgscolorrampshader.cpp
  raster/qgscontrastenhancement.cpp
  raster/qgscontrastenhancementfunction.cpp
  raster/qgsrasterdataprovider.cpp
  raster/qgslinearminmaxenhancement.cpp
  raster/qgslinearminmaxenhancementwithclip.cpp
  raster/qgspseudocolorshader.cpp
  raster/qgsrasterprojector.cpp
  raster/qgsrasterchecker.cpp
  raster/qgsrasteridentifyresult.cpp
  raster/qgsrasterinterface.cpp
  raster/qgsrasteriterator.cpp
  raster/qgsrasterlayer.cpp
  raster/qgsrasterlayerrenderer.cpp
  raster/qgsrasternuller.cpp
  raster/qgsrastertransparency.cpp
  raster/qgsrasterpipe.cpp
  raster/qgsrasterrange.cpp
  raster/qgsrastershader.cpp
  raster/qgsrastershaderfunction.cpp

  raster/qgsrasterdrawer.cpp
  raster/qgsrasterfilewriter.cpp
  raster/qgsrasterresamplefilter.cpp
  raster/qgsrasterrendererregistry.cpp
  raster/qgsrasterrenderer.cpp
  raster/qgsbilinearrasterresampler.cpp
  raster/qgscubicrasterresampler.cpp
  raster/qgspalettedrasterrenderer.cpp
  raster/qgsmultibandcolorrenderer.cpp
  raster/qgssinglebandcolordatarenderer.cpp
  raster/qgssinglebandgrayrenderer.cpp
  raster/qgssinglebandpseudocolorrenderer.cpp
  raster/qgsbrightnesscontrastfilter.cpp
  raster/qgshuesaturationfilter.cpp  

)

IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # spatialindex headers produce warnings
  SET_SOURCE_FILES_PROPERTIES(qgsspatialindex.cpp PROPERTIES COMPILE_FLAGS -Wno-overloaded-virtual)
ENDIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

IF (WITH_INTERNAL_QEXTSERIALPORT)
  SET(QGIS_CORE_SRCS ${QGIS_CORE_SRCS}
    gps/qextserialport/qextserialport.cpp
    gps/qextserialport/qextserialenumerator.cpp
  )

  IF(WIN32)
    SET(QGIS_CORE_SRCS ${QGIS_CORE_SRCS}
      gps/qextserialport/win_qextserialport.cpp
    )
    ADD_DEFINITIONS(-D_TTY_WIN_)
  ELSE(WIN32)
    SET(QGIS_CORE_SRCS ${QGIS_CORE_SRCS}
      gps/qextserialport/posix_qextserialport.cpp
    )
    ADD_DEFINITIONS(-D_TTY_POSIX_)
  ENDIF(WIN32)
ELSE (WITH_INTERNAL_QEXTSERIALPORT)
  INCLUDE_DIRECTORIES(${QEXTSERIALPORT_INCLUDE_DIR})
ENDIF (WITH_INTERNAL_QEXTSERIALPORT)

IF (QT_MOBILITY_LOCATION_FOUND)
  SET(QGIS_CORE_SRCS ${QGIS_CORE_SRCS}
    gps/qgsqtlocationconnection.cpp
  )
ENDIF (QT_MOBILITY_LOCATION_FOUND)

IF (WITH_INTERNAL_SPATIALITE)
  IF (WIN32 OR APPLE OR ANDROID OR CYGWIN)
    INCLUDE_DIRECTORIES(${ICONV_INCLUDE_DIR})
  ENDIF (WIN32 OR APPLE OR ANDROID OR CYGWIN)
  IF (WIN32)
    ADD_DEFINITIONS(-DDLL_EXPORT)
  ENDIF (WIN32)

#
# sqlite3 requires some extraflags to be defined
# in order to support:
# - huge database > 2 GB
# - RTree Spatial Index
#
  ADD_DEFINITIONS(-D_LARGE_FILE=1 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE=1)
  ADD_DEFINITIONS(-DSQLITE_ENABLE_RTREE=1)

  SET(QGIS_CORE_SRCS ${QGIS_CORE_SRCS}
    spatialite/sqlite3.c
    spatialite/spatialite.c
    )

  IF(MSVC)
    # dirty code
    SET_SOURCE_FILES_PROPERTIES(spatialite/sqlite3.c spatialite/spatialite.c PROPERTIES COMPILE_FLAGS /W0)
  ELSE(MSVC)
    SET_SOURCE_FILES_PROPERTIES(spatialite/sqlite3.c spatialite/spatialite.c PROPERTIES COMPILE_FLAGS -w)
  ENDIF(MSVC)

  INCLUDE_DIRECTORIES(BEFORE spatialite/headers/spatialite spatialite/headers)
ELSE (WITH_INTERNAL_SPATIALITE)
  ADD_DEFINITIONS(-DEXTERNAL_SPATIALITE)
ENDIF (WITH_INTERNAL_SPATIALITE)

ADD_FLEX_FILES(QGIS_CORE_SRCS qgsexpressionlexer.ll)

ADD_BISON_FILES(QGIS_CORE_SRCS qgsexpressionparser.yy)

SET(QGIS_CORE_MOC_HDRS

  qgsapplication.h
  qgsbrowsermodel.h
  qgscontexthelp.h
  qgscoordinatetransform.h
  qgsdataitem.h
  qgsdataprovider.h
  qgsgml.h
  qgsgmlschema.h
  qgshttptransaction.h
  qgsmaplayer.h
  qgsmaplayerlegend.h
  qgsmaplayerregistry.h
  qgsmaprenderer.h
  qgsmaprenderercache.h
  qgsmaprenderercustompainterjob.h
  qgsmaprendererjob.h
  qgsmaprendererparalleljob.h
  qgsmaprenderersequentialjob.h
  qgsmessageoutput.h
  qgsmessagelog.h
  qgsnetworkreplyparser.h
  qgsnetworkcontentfetcher.h
  qgsofflineediting.h
  qgscredentials.h
  qgspluginlayer.h
  qgsproject.h
  qgsrunprocess.h
  qgsrelationmanager.h
  qgsvectorlayer.h
  qgsvectorlayereditbuffer.h
  qgsnetworkaccessmanager.h
  qgsvectordataprovider.h
  qgsvectorlayercache.h
  qgsvectorlayerjoinbuffer.h
  qgsgeometryvalidator.h

  composer/qgsaddremoveitemcommand.h
  composer/qgscomposerlegend.h
  composer/qgscomposermap.h
  composer/qgscomposermapitem.h
  composer/qgscomposermapgrid.h
  composer/qgscomposermapoverview.h
  composer/qgscomposerpicture.h
  composer/qgscomposerscalebar.h
  composer/qgscomposerobject.h
  composer/qgscomposeritem.h
  composer/qgscomposeritemgroup.h
  composer/qgscomposermousehandles.h  
  composer/qgscomposerlabel.h
  composer/qgscomposershape.h
  composer/qgscomposerattributetable.h
  composer/qgscomposerattributetablev2.h
  composer/qgscomposerattributetablemodel.h
  composer/qgscomposerattributetablemodelv2.h    
  composer/qgscomposertable.h  
  composer/qgscomposertablev2.h
  composer/qgscomposertablecolumn.h
  composer/qgscomposerhtml.h
  composer/qgscomposermultiframe.h
  composer/qgscomposereffect.h
  composer/qgsatlascomposition.h
  composer/qgscomposermodel.h
  composer/qgscomposition.h

  composer/qgslegendmodel.h
  raster/qgsrasterlayer.h
  raster/qgsrasterdataprovider.h

  gps/qgsgpsconnection.h
  gps/qgsgpsdetector.h
  gps/qgsnmeaconnection.h
  gps/qgsgpsdconnection.h

  symbology-ng/qgscptcityarchive.h
  symbology-ng/qgssvgcache.h
  symbology-ng/qgsstylev2.h

  layertree/qgslayertreegroup.h
  layertree/qgslayertreelayer.h
  layertree/qgslayertreemodel.h
  layertree/qgslayertreemodellegendnode.h
  layertree/qgslayertreenode.h
  layertree/qgslayertreeregistrybridge.h
)

IF (WITH_INTERNAL_QEXTSERIALPORT)
  SET(QGIS_CORE_MOC_HDRS ${QGIS_CORE_MOC_HDRS}
    gps/qextserialport/qextserialport.h
    gps/qextserialport/qextserialenumerator.h
  )
ENDIF (WITH_INTERNAL_QEXTSERIALPORT)

IF (QT_MOBILITY_LOCATION_FOUND)
  SET(QGIS_CORE_MOC_HDRS ${QGIS_CORE_MOC_HDRS}
    gps/qgsqtlocationconnection.h
  )
ENDIF (QT_MOBILITY_LOCATION_FOUND)

QT4_WRAP_CPP(QGIS_CORE_MOC_SRCS ${QGIS_CORE_MOC_HDRS})

IF(MSVC)
  SET_SOURCE_FILES_PROPERTIES(${QGIS_CORE_MOC_SRCS} PROPERTIES COMPILE_FLAGS "/wd4996" )
ELSE(MSVC)
  SET_SOURCE_FILES_PROPERTIES(${QGIS_CORE_MOC_SRCS} PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations" )
ENDIF(MSVC)

# install headers
# install qgsconfig.h and plugin.h here so they can get into
# the OS X framework target

SET(QGIS_CORE_HDRS
  ${CMAKE_BINARY_DIR}/qgsconfig.h
  ../plugins/qgisplugin.h
  qgis.h
  qgsapplication.h
  qgsattributeaction.h
  qgscachedfeatureiterator.h
  qgscacheindex.h
  qgscacheindexfeatureid.h
  qgsclipper.h
  qgscolorscheme.h
  qgscolorschemeregistry.h
  qgscontexthelp.h
  qgscoordinatereferencesystem.h
  qgscoordinatetransform.h
  qgscredentials.h
  qgscrscache.h
  qgscsexception.h
  qgsdartmeasurement.h
  qgsdatadefined.h
  qgsdatasourceuri.h
  qgsdataitem.h
  qgsdatumtransformstore.h
  qgsdbfilterproxymodel.h
  qgsdistancearea.h
  qgseditorwidgetconfig.h
  qgserror.h
  qgsexception.h
  qgsexpression.h
  qgsexpressionfieldbuffer.h
  qgsfeature.h
  qgsfeatureiterator.h
  qgsfeaturerequest.h
  qgsfeaturestore.h
  qgsfield.h
  qgsfontutils.h
  qgsgeometry.h
  qgsgml.h
  qgsgmlschema.h
  qgsgeometrycache.h
  qgshttptransaction.h
  qgslabel.h
  qgslabelattributes.h
  qgslabelsearchtree.h
  qgslegacyhelpers.h
  qgslegendrenderer.h
  qgslegendsettings.h
  qgslogger.h
  qgsmaphittest.h
  qgsmaplayer.h
  qgsmaplayerlegend.h
  qgsmaplayerregistry.h
  qgsmaprenderer.h
  qgsmaprenderercache.h
  qgsmaprenderercustompainterjob.h
  qgsmaprendererjob.h
  qgsmaprendererparalleljob.h
  qgsmaprenderersequentialjob.h
  qgsmapsettings.h
  qgsmaptopixel.h
  qgsmapunitscale.h
  qgsmessageoutput.h
  qgsmimedatautils.h
  qgsmultirenderchecker.h
  qgsnetworkcontentfetcher.h
  qgsnetworkreplyparser.h
  qgsobjectcustomproperties.h
  qgsofflineediting.h
  qgsogcutils.h
  qgsowsconnection.h
  qgspaintenginehack.h
  qgspallabeling.h
  qgspluginlayer.h
  qgspluginlayerregistry.h
  qgspoint.h
  qgsproject.h
  qgsprojectfiletransform.h
  qgsprojectproperty.h
  qgsprojectversion.h
  qgsprovidercountcalcevent.h
  qgsproviderextentcalcevent.h
  qgsprovidermetadata.h
  qgsproviderregistry.h
  qgspythonrunner.h
  qgsrectangle.h
  qgsrelation.h
  qgsrelationmanager.h
  qgsrenderchecker.h
  qgsrendercontext.h
  qgsrunprocess.h
  qgsscalecalculator.h
  qgsscaleutils.h
  qgssimplifymethod.h
  qgssnapper.h
  qgsspatialindex.h
  qgstolerance.h
  qgsvectordataprovider.h
  qgsvectorlayercache.h
  qgsvectorfilewriter.h
  qgsvectorlayer.h
  qgsvectorlayercache.h
  qgsvectorlayereditbuffer.h
  qgsvectorlayereditutils.h
  qgsvectorlayerfeatureiterator.h
  qgsvectorlayerimport.h
  qgsvectorlayerundocommand.h
  qgsvectorlayerjoinbuffer.h
  qgsvectorsimplifymethod.h

  qgsdiagramrendererv2.h
  diagram/qgsdiagram.h
  diagram/qgspiediagram.h
  diagram/qgstextdiagram.h
  diagram/qgshistogramdiagram.h

  composer/qgslegendmodel.h
  composer/qgscomposerlegenditem.h
  composer/qgscomposermultiframecommand.h
  composer/qgsscalebarstyle.h
  composer/qgsnumericscalebarstyle.h
  composer/qgsaddremovemultiframecommand.h
  composer/qgsdoubleboxscalebarstyle.h
  composer/qgscomposertable.h
  composer/qgscomposertablev2.h
  composer/qgscomposertablecolumn.h
  composer/qgspaperitem.h
  composer/qgsticksscalebarstyle.h
  composer/qgscomposerlegendstyle.h
  composer/qgssingleboxscalebarstyle.h
  composer/qgscomposerframe.h
  composer/qgscomposerarrow.h
  composer/qgscomposertexttable.h
  composer/qgscomposeritemcommand.h

  raster/qgsraster.h
  raster/qgsrasterblock.h
  raster/qgsrasterdataprovider.h
  raster/qgsrasterresamplefilter.h
  raster/qgscliptominmaxenhancement.h
  raster/qgscolorrampshader.h
  raster/qgscolorrampshader.h
  raster/qgscontrastenhancement.h
  raster/qgscontrastenhancementfunction.h
  raster/qgslinearminmaxenhancement.h
  raster/qgslinearminmaxenhancementwithclip.h
  raster/qgspseudocolorshader.h
  raster/qgsrasterchecker.h
  raster/qgsrasterpyramid.h
  raster/qgsrasterbandstats.h
  raster/qgsrasterhistogram.h
  raster/qgsrasteridentifyresult.h
  raster/qgsrasterinterface.h
  raster/qgsrasterlayer.h
  raster/qgsrastertransparency.h
  raster/qgsrasterpipe.h
  raster/qgsrasterprojector.h
  raster/qgsrasterrange.h
  raster/qgsrastershader.h
  raster/qgsrastershaderfunction.h
  raster/qgsrasterviewport.h
  raster/qgsbilinearrasterresampler.h
  raster/qgsbrightnesscontrastfilter.h
  raster/qgssinglebandcolordatarenderer.h
  raster/qgssinglebandpseudocolorrenderer.h
  raster/qgsrasterresampler.h
  raster/qgssinglebandgrayrenderer.h
  raster/qgspalettedrasterrenderer.h
  raster/qgsrasternuller.h
  raster/qgsrasterfilewriter.h
  raster/qgsrasterrenderer.h
  raster/qgscubicrasterresampler.h
  raster/qgsrasteriterator.h
  raster/qgsrasterdrawer.h
  raster/qgshuesaturationfilter.h
  raster/qgsmultibandcolorrenderer.h

  symbology-ng/qgscategorizedsymbolrendererv2.h
  symbology-ng/qgscolorbrewerpalette.h
  symbology-ng/qgsellipsesymbollayerv2.h
  symbology-ng/qgsfillsymbollayerv2.h
  symbology-ng/qgsgraduatedsymbolrendererv2.h
  symbology-ng/qgslegendsymbolitemv2.h
  symbology-ng/qgslinesymbollayerv2.h
  symbology-ng/qgsmarkersymbollayerv2.h
  symbology-ng/qgspointdisplacementrenderer.h
  symbology-ng/qgsrendererv2.h
  symbology-ng/qgsrendererv2registry.h
  symbology-ng/qgsrulebasedrendererv2.h
  symbology-ng/qgssinglesymbolrendererv2.h
  symbology-ng/qgsinvertedpolygonrenderer.h
  symbology-ng/qgsstylev2.h
  symbology-ng/qgssvgcache.h
  symbology-ng/qgssymbollayerv2.h
  symbology-ng/qgssymbollayerv2registry.h
  symbology-ng/qgssymbollayerv2utils.h
  symbology-ng/qgssymbologyv2conversion.h
  symbology-ng/qgssymbolv2.h
  symbology-ng/qgsvectorcolorrampv2.h
  symbology-ng/qgscptcityarchive.h
  symbology-ng/qgsvectorfieldsymbollayer.h

  layertree/qgslayertree.h
  layertree/qgslayertreegroup.h
  layertree/qgslayertreelayer.h
  layertree/qgslayertreemodel.h
  layertree/qgslayertreemodellegendnode.h
  layertree/qgslayertreenode.h
  layertree/qgslayertreeregistrybridge.h
  layertree/qgslayertreeutils.h
)

IF (QT_MOBILITY_LOCATION_FOUND)
  SET(QGIS_CORE_HDRS ${QGIS_CORE_HDRS}
    gps/qgsqtlocationconnection.h
    gps/qgsgpsconnectionregistry.h
  )
ENDIF (QT_MOBILITY_LOCATION_FOUND)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  composer
  dxf
  layertree
  pal
  raster
  renderer
  symbology-ng
  gps/qextserialport
  ${SPATIALINDEX_INCLUDE_DIR}
  ${PROJ_INCLUDE_DIR}
  ${GEOS_INCLUDE_DIR}
  ${GDAL_INCLUDE_DIR}
  ${EXPAT_INCLUDE_DIR}
)

IF (NOT WITH_INTERNAL_SPATIALITE)
  INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${SPATIALITE_INCLUDE_DIR})
ENDIF (NOT WITH_INTERNAL_SPATIALITE)

IF (WIN32)
  IF (MSVC)
    ADD_DEFINITIONS("-DCORE_EXPORT=${DLLEXPORT}")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UCORE_EXPORT \"-DCORE_EXPORT=${DLLEXPORT}\"")
  ENDIF (MSVC)
ENDIF (WIN32)

#for PAL classes
IF (WIN32)
    ADD_DEFINITIONS("-D_HAVE_WINDOWS_H_")
ELSE (WIN32)
    ADD_DEFINITIONS("-D_HAVE_PTHREAD_")
ENDIF (WIN32)

# Test data dir for QgsRenderChecker
ADD_DEFINITIONS(-DTEST_DATA_DIR="\\"${TEST_DATA_DIR}\\"")

#############################################################
# qgis_core library

ADD_LIBRARY(qgis_core SHARED ${QGIS_CORE_SRCS} ${QGIS_CORE_MOC_SRCS} ${QGIS_CORE_HDRS} ${QGIS_CORE_MOC_HDRS})

IF(NOT APPLE)
  INSTALL(FILES ${QGIS_CORE_HDRS} ${QGIS_CORE_MOC_HDRS} DESTINATION ${QGIS_INCLUDE_DIR})
ELSE(NOT APPLE)
  SET_TARGET_PROPERTIES(qgis_core PROPERTIES
    CLEAN_DIRECT_OUTPUT 1
    FRAMEWORK 1
    FRAMEWORK_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}"
    MACOSX_FRAMEWORK_INFO_PLIST "${CMAKE_SOURCE_DIR}/mac/framework.info.plist.in"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${COMPLETE_VERSION}
    MACOSX_FRAMEWORK_IDENTIFIER org.qgis.qgis2_core
    BUILD_WITH_INSTALL_RPATH TRUE
    PUBLIC_HEADER "${QGIS_CORE_HDRS};${QGIS_CORE_MOC_HDRS}"
    LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}"
  )
ENDIF(NOT APPLE)

#generate unversioned libs for android
IF (NOT ANDROID)
  SET_TARGET_PROPERTIES(qgis_core PROPERTIES
    VERSION ${COMPLETE_VERSION}
    SOVERSION ${COMPLETE_VERSION}
    )
ENDIF (NOT ANDROID)

# make sure to create qgsversion.h before compiling
ADD_DEPENDENCIES(qgis_core version)

# because of htonl
IF (WIN32)
  FIND_LIBRARY(SETUPAPI_LIBRARY NAMES setupapi PATHS $ENV{LIB})
  TARGET_LINK_LIBRARIES(qgis_core wsock32 ${SETUPAPI_LIBRARY})
ENDIF (WIN32)

IF(APPLE)
  TARGET_LINK_LIBRARIES(qgis_core "-framework CoreFoundation -framework IOKit")
ENDIF(APPLE)

IF (NOT WITH_INTERNAL_QEXTSERIALPORT)
  TARGET_LINK_LIBRARIES(qgis_core ${QEXTSERIALPORT_LIBRARY})
ENDIF (NOT WITH_INTERNAL_QEXTSERIALPORT)

IF (QT_MOBILITY_LOCATION_FOUND)
    TARGET_LINK_LIBRARIES(qgis_core ${QT_MOBILITY_LOCATION_LIBRARY})
ENDIF (QT_MOBILITY_LOCATION_FOUND)

TARGET_LINK_LIBRARIES(qgis_core
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTSVG_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${QSCINTILLA_LIBRARY}

  ${PROJ_LIBRARY}
  ${GEOS_LIBRARY}
  ${GDAL_LIBRARY}
  ${SPATIALINDEX_LIBRARY}
  ${EXPAT_LIBRARY}
)

IF (WITH_INTERNAL_SPATIALITE)
  IF (WIN32 OR APPLE OR CYGWIN)
    TARGET_LINK_LIBRARIES(qgis_core ${ICONV_LIBRARY})
  ENDIF (WIN32 OR APPLE OR CYGWIN)
  IF (ANDROID)
    TARGET_LINK_LIBRARIES(qgis_core ${ICONV_LIBRARY} ${CHARSET_LIBRARY})
  ENDIF (ANDROID)
  IF (UNIX AND NOT ANDROID)
    TARGET_LINK_LIBRARIES(qgis_core pthread ${CMAKE_DL_LIBS})
  ENDIF (UNIX AND NOT ANDROID)
ELSE (WITH_INTERNAL_SPATIALITE)
  TARGET_LINK_LIBRARIES(qgis_core ${SQLITE3_LIBRARY} ${SPATIALITE_LIBRARY})
ENDIF (WITH_INTERNAL_SPATIALITE)

# install

INSTALL(TARGETS qgis_core
  RUNTIME DESTINATION ${QGIS_BIN_DIR}
  LIBRARY DESTINATION ${QGIS_LIB_DIR}
  ARCHIVE DESTINATION ${QGIS_LIB_DIR}
  FRAMEWORK DESTINATION ${QGIS_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QGIS_INCLUDE_DIR})

# Mac dev frameworks

IF (APPLE AND QGIS_MACAPP_INSTALL_DEV)
  INSTALL(TARGETS qgis_core FRAMEWORK DESTINATION ${QGIS_MACAPP_DEV_PREFIX})
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -id \"${QGIS_MACAPP_DEV_PREFIX}/qgis_core.framework/Versions/${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}/qgis_core\" \"$ENV{DESTDIR}${QGIS_MACAPP_DEV_PREFIX}/qgis_core.framework/qgis_core\")")
ENDIF (APPLE AND QGIS_MACAPP_INSTALL_DEV)
