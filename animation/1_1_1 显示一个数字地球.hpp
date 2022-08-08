#include <Windows.h>
#include <iostream>
#include <string>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osgEarth/MapNode>
#include <osgEarth/ImageLayer>
#include <osgEarth/GLUtils>

#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>

#include <osgEarthFeatures/FeatureSourceLayer>
#include <osgEarthFeatures/FeatureModelLayer>

#include <osgEarthUtil/EarthManipulator>

using namespace std;

void AddAnno(std::string filePath, osg::ref_ptr<osgEarth::Map> map)
{
	osgEarth::Symbology::Style labelStyle;

	osgEarth::Symbology::TextSymbol* text = labelStyle.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	string name = "[Name]";		//�����Ҫ��ʾ���֣�����Ҫת����UTF-8����

	text->content() = osgEarth::Symbology::StringExpression(name);
	text->priority() = osgEarth::NumericExpression( "[Popu]" );
	text->size() = 16.0f;
	text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	text->fill()->color() = osgEarth::Symbology::Color::White;
	text->halo()->color() = osgEarth::Symbology::Color::Red;
	text->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	
	string fontFile = "simhei.ttf";
	text->font() = fontFile;			//�����ʾ���֣���Ҫ֧�������ֿ������

	// and configure a model layer:
	osgEarth::Features::FeatureModelLayerOptions fmlOpt;
	fmlOpt.name() = filePath + "_labels";
	fmlOpt.featureSourceLayer() = filePath + "_source";
	fmlOpt.styles() = new osgEarth::Symbology::StyleSheet();
	fmlOpt.styles()->addStyle(labelStyle);  

	osg::ref_ptr<osgEarth::Features::FeatureModelLayer> fml = new osgEarth::Features::FeatureModelLayer(fmlOpt);
	map->addLayer(fml);
}

void AddVector(osg::ref_ptr<osgEarth::Map> map)
{
	//
	std::string filePath = "../data/china_data/county_total.shp";
	osgEarth::Drivers::OGRFeatureOptions featureData;
	featureData.url() = filePath;

	//	   ���ȱ�ٿռ�ο��������ֶ�ָ��	
	//    ifstream infile("C:/Data/vector/hs/23.prj");
	//    string line;
	//    getline(infile, line);
	//    featureData.profile()->srsString() = line;

	// Make a feature source layer and add it to the Map:
	osgEarth::Features::FeatureSourceLayerOptions ogrLayer;
	ogrLayer.name() = filePath + "_source";
	ogrLayer.featureSource() = featureData;
	osgEarth::Features::FeatureSourceLayer*  featureSourceLayer = new osgEarth::Features::FeatureSourceLayer(ogrLayer);
	map->addLayer(featureSourceLayer);
	osgEarth::Features::FeatureSource *features = featureSourceLayer->getFeatureSource();
	if (!features){
		printf(("�޷��򿪸�ʸ���ļ���"));
		return;
	}

	//������ʽ
	osgEarth::Symbology::Style style;

	//�ɼ���
	osgEarth::Symbology::RenderSymbol* rs = style.getOrCreate<osgEarth::Symbology::RenderSymbol>();
	rs->depthTest() = false;

	//��������
	osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = alt->CLAMP_TO_TERRAIN;
	alt->technique() = alt->TECHNIQUE_DRAPE;

	//����ʸ������ʽ�������߽��ߣ�
	osgEarth::Symbology::LineSymbol* ls = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	ls->stroke()->color() = osgEarth::Symbology::Color("#FA8072");
	ls->stroke()->width() = 1.0;
	ls->tessellationSize()->set(100, osgEarth::Units::KILOMETERS);

	osgEarth::Symbology::PolygonSymbol *polygonSymbol = style.getOrCreateSymbol<osgEarth::Symbology::PolygonSymbol>();
	polygonSymbol->fill()->color() = osgEarth::Symbology::Color(152.0f / 255, 251.0f / 255, 152.0f / 255, 0.8f); //238 230 133
	polygonSymbol->outline() = true;

	//
	osgEarth::Features::FeatureModelLayerOptions fmlOpt;
	fmlOpt.name() = filePath;
	fmlOpt.featureSourceLayer() = filePath + "_source";
	fmlOpt.enableLighting() = false;
	fmlOpt.styles() = new osgEarth::Symbology::StyleSheet();
	fmlOpt.styles()->addStyle(style);

	osg::ref_ptr<osgEarth::Features::FeatureModelLayer> fml = new osgEarth::Features::FeatureModelLayer(fmlOpt);	
	map->addLayer(fml);

	AddAnno(filePath, map);
}

int DisplayEarth3D()
{
	osgEarth::ProfileOptions profileOpts;

	//��ͼ���ã����û���Ŀ¼
	osgEarth::Drivers::FileSystemCacheOptions cacheOpts;
	string cacheDir = "c:/osgearth_cache";
	cacheOpts.rootPath() = cacheDir;

	//
	osgEarth::MapOptions mapOpts;
	mapOpts.cache() = cacheOpts;
	mapOpts.profile() = profileOpts;

	//������ͼ�ڵ�
	osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map(mapOpts);
	osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);

	osgEarth::Drivers::GDALOptions gdal;
	gdal.url() = "../data/world_data/world.tif";
	osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer("BlueMarble", gdal);
	map->addLayer(layer);

	AddVector(map);

	osgViewer::Viewer viewer;
	viewer.setSceneData(mapNode);

	osg::ref_ptr< osgEarth::Util::EarthManipulator> mainManipulator = new osgEarth::Util::EarthManipulator;
	viewer.setCameraManipulator(mainManipulator);	

	//���Lines or Annotations (FeatureNode, etc.) ������Ⱦ������
	osgEarth::GLUtils::setGlobalDefaults(viewer.getCamera()->getOrCreateStateSet());

	viewer.setUpViewInWindow(100, 100, 800, 600);	
	return viewer.run();
}