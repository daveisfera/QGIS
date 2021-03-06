class QgsSimpleLineSymbolLayerV2 : QgsLineSymbolLayerV2
{
%TypeHeaderCode
#include <qgslinesymbollayerv2.h>
%End

  public:
    QgsSimpleLineSymbolLayerV2( QColor color = DEFAULT_SIMPLELINE_COLOR,
                                double width = DEFAULT_SIMPLELINE_WIDTH,
                                Qt::PenStyle penStyle = DEFAULT_SIMPLELINE_PENSTYLE );

    // static stuff

    static QgsSymbolLayerV2* create( const QgsStringMap& properties = QgsStringMap() ) /Factory/;
    static QgsSymbolLayerV2* createFromSld( QDomElement &element ) /Factory/;

    // implemented from base classes

    QString layerType() const;

    void startRender( QgsSymbolV2RenderContext& context );

    void stopRender( QgsSymbolV2RenderContext& context );

    void renderPolyline( const QPolygonF& points, QgsSymbolV2RenderContext& context );

    //overriden so that clip path can be set when using draw inside polygon option
    void renderPolygonOutline( const QPolygonF& points, QList<QPolygonF>* rings, QgsSymbolV2RenderContext& context );

    QgsStringMap properties() const;

    QgsSymbolLayerV2* clone() const /Factory/;

    void toSld( QDomDocument &doc, QDomElement &element, QgsStringMap props ) const;

    QString ogrFeatureStyle( double mmScaleFactor, double mapUnitScaleFactor ) const;

    void setOutputUnit( QgsSymbolV2::OutputUnit unit );
    QgsSymbolV2::OutputUnit outputUnit() const;

    void setMapUnitScale( const QgsMapUnitScale &scale );
    QgsMapUnitScale mapUnitScale() const;

    double estimateMaxBleed() const;

    // new stuff

    Qt::PenStyle penStyle() const;
    void setPenStyle( Qt::PenStyle style );

    Qt::PenJoinStyle penJoinStyle() const;
    void setPenJoinStyle( Qt::PenJoinStyle style );

    Qt::PenCapStyle penCapStyle() const;
    void setPenCapStyle( Qt::PenCapStyle style );

    double offset() const;
    void setOffset( double offset );

    void setOffsetUnit( QgsSymbolV2::OutputUnit unit );
    QgsSymbolV2::OutputUnit offsetUnit() const;

    void setOffsetMapUnitScale( const QgsMapUnitScale& scale);
    const QgsMapUnitScale& offsetMapUnitScale() const;

    bool useCustomDashPattern() const;
    void setUseCustomDashPattern( bool b );

    void setCustomDashPatternUnit( QgsSymbolV2::OutputUnit unit );
    QgsSymbolV2::OutputUnit customDashPatternUnit() const;

    const QgsMapUnitScale& customDashPatternMapUnitScale() const;
    void setCustomDashPatternMapUnitScale( const QgsMapUnitScale& scale);

    QVector<qreal> customDashVector() const;
    void setCustomDashVector( const QVector<qreal>& vector );

    //Returns true if the line should only be drawn inside the polygon
    bool drawInsidePolygon() const;
    //Set to true if the line should only be drawn inside the polygon
    void setDrawInsidePolygon( bool drawInsidePolygon );

    QVector<qreal> dxfCustomDashPattern( QgsSymbolV2::OutputUnit& unit ) const;
    Qt::PenStyle dxfPenStyle() const;

    double dxfWidth( const QgsDxfExport& e, const QgsSymbolV2RenderContext& context ) const;
    QColor dxfColor( const QgsSymbolV2RenderContext& context ) const;
};

/////////

class QgsMarkerLineSymbolLayerV2 : QgsLineSymbolLayerV2
{
%TypeHeaderCode
#include <qgslinesymbollayerv2.h>
%End

  public:
    QgsMarkerLineSymbolLayerV2( bool rotateMarker = DEFAULT_MARKERLINE_ROTATE,
                                double interval = DEFAULT_MARKERLINE_INTERVAL );

    ~QgsMarkerLineSymbolLayerV2();

    enum Placement
    {
      Interval,
      Vertex,
      LastVertex,
      FirstVertex,
      CentralPoint
    };

    // static stuff

    static QgsSymbolLayerV2* create( const QgsStringMap& properties = QgsStringMap() ) /Factory/;
    static QgsSymbolLayerV2* createFromSld( QDomElement &element ) /Factory/;

    // implemented from base classes

    QString layerType() const;

    void startRender( QgsSymbolV2RenderContext& context );

    void stopRender( QgsSymbolV2RenderContext& context );

    void renderPolyline( const QPolygonF& points, QgsSymbolV2RenderContext& context );

    void renderPolygonOutline( const QPolygonF& points, QList<QPolygonF>* rings, QgsSymbolV2RenderContext& context );

    QgsStringMap properties() const;

    QgsSymbolLayerV2* clone() const;

    void toSld( QDomDocument &doc, QDomElement &element, QgsStringMap props ) const;

    void setColor( const QColor& color );

    QgsSymbolV2* subSymbol();
    bool setSubSymbol( QgsSymbolV2* symbol );

    virtual void setWidth( double width );
    virtual double width() const;

    double estimateMaxBleed() const;

    // new stuff

    bool rotateMarker() const;
    void setRotateMarker( bool rotate );

    double interval() const;
    void setInterval( double interval );

    double offset() const;
    void setOffset( double offset );

    Placement placement() const;
    void setPlacement( Placement p );

    /**Returns the offset along the line for the marker placement. For Interval placements, this is the distance
     * between the start of the line and the first marker. For FirstVertex and LastVertex placements, this is the
     * distance between the marker and the start of the line or the end of the line respectively.
     * This setting has no effect for Vertex or CentralPoint placements.
     * @returns The offset along the line. The unit for the offset is retrievable via offsetAlongLineUnit.
     * @note added in 2.3
     * @see setOffsetAlongLine
     * @see offsetAlongLineUnit
     * @see placement
    */
    double offsetAlongLine() const;

    /**Sets the the offset along the line for the marker placement. For Interval placements, this is the distance
     * between the start of the line and the first marker. For FirstVertex and LastVertex placements, this is the
     * distance between the marker and the start of the line or the end of the line respectively.
     * This setting has no effect for Vertex or CentralPoint placements.
     * @param offsetAlongLine Distance to offset markers along the line. The offset
     * unit is set via setOffsetAlongLineUnit.
     * @note added in 2.3
     * @see offsetAlongLine
     * @see setOffsetAlongLineUnit
     * @see setPlacement
    */
    void setOffsetAlongLine( double offsetAlongLine );

    /**Returns the unit used for calculating the offset along line for markers.
     * @returns Offset along line unit type.
     * @see setOffsetAlongLineUnit
     * @see offsetAlongLine
    */
    QgsSymbolV2::OutputUnit offsetAlongLineUnit() const;

    /**Sets the unit used for calculating the offset along line for markers.
     * @param unit Offset along line unit type.
     * @see offsetAlongLineUnit
     * @see setOffsetAlongLine
    */
    void setOffsetAlongLineUnit( QgsSymbolV2::OutputUnit unit );

    /**Returns the map unit scale used for calculating the offset in map units along line for markers.
     * @returns Offset along line map unit scale.
     */
    const QgsMapUnitScale& offsetAlongLineMapUnitScale() const;

    /**Sets the map unit scale used for calculating the offset in map units along line for markers.
     * @param scale Offset along line map unit scale.
     */
    void setOffsetAlongLineMapUnitScale( const QgsMapUnitScale& scale );

    void setIntervalUnit( QgsSymbolV2::OutputUnit unit );
    QgsSymbolV2::OutputUnit intervalUnit() const;

    void setIntervalMapUnitScale( const QgsMapUnitScale& scale );
    const QgsMapUnitScale& intervalMapUnitScale() const;

    void setOffsetUnit( QgsSymbolV2::OutputUnit unit );
    QgsSymbolV2::OutputUnit offsetUnit() const;

    void setOffsetMapUnitScale( const QgsMapUnitScale& scale );
    const QgsMapUnitScale& offsetMapUnitScale() const;

    void setOutputUnit( QgsSymbolV2::OutputUnit unit );
    QgsSymbolV2::OutputUnit outputUnit() const;

    void setMapUnitScale( const QgsMapUnitScale& scale );
    QgsMapUnitScale mapUnitScale() const;

  protected:

    void renderPolylineInterval( const QPolygonF& points, QgsSymbolV2RenderContext& context );
    void renderPolylineVertex( const QPolygonF& points, QgsSymbolV2RenderContext& context, Placement placement = Vertex );
    void renderPolylineCentral( const QPolygonF& points, QgsSymbolV2RenderContext& context );
    double markerAngle( const QPolygonF& points, bool isRing, int vertex );
};
