class QgsVectorLayerTools
{
%TypeHeaderCode
#include <qgsvectorlayertools.h>
%End
  public:
     virtual bool addFeature( QgsVectorLayer* layer, QgsAttributeMap defaultValues = QgsAttributeMap(), const QgsGeometry& = QgsGeometry() ) = 0;
};
