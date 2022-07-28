#include <Windows.h>
#include <iostream>
#include <string>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osgEarth/MapNode>

#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <osgEarth/ImageLayer>

#include <osgEarthUtil/EarthManipulator>



using namespace std;

int DisplayEarth2D()
{		
	// CPLSetConfigOption("GDAL_DATA", "D:/Work/OSGNewBuild/OpenSceneGraph-3.6.4/3rdParty/x64/gdal-data");

	//string wktString = "EPSG:3857";			//web墨卡托投影
	string wktString = "EPSG:4326";			//wgs84
	osgEarth::ProfileOptions profileOpts;
	profileOpts.srsString() = wktString;

	// osgEarth::Bounds bs(535139, 3365107, 545139, 3375107);
	// osgEarth::Bounds bs(73, 3, 135, 53);
	// profileOpts.bounds() = bs;

	//地图配置：设置缓存目录
	osgEarth::Drivers::FileSystemCacheOptions cacheOpts;
	string cacheDir =  "c:/osgearth_cache";
	cacheOpts.rootPath() = cacheDir;

	//
	osgEarth::MapOptions mapOpts;   
	mapOpts.cache() = cacheOpts;
	mapOpts.coordSysType() = osgEarth::MapOptions::CSTYPE_PROJECTED;

	mapOpts.profile() = profileOpts;

	//创建地图节点
	osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map(mapOpts);
	osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);

	osgEarth::Drivers::GDALOptions gdal;
	gdal.url() = "../data/world_data/world.tif";
	osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer("BlueMarble", gdal);
	map->addLayer(layer);  

	osgViewer::Viewer viewer;
	viewer.setSceneData(mapNode);

	osg::ref_ptr< osgEarth::Util::EarthManipulator> mainManipulator = new osgEarth::Util::EarthManipulator;
	viewer.setCameraManipulator(mainManipulator);

	viewer.setUpViewInWindow(100, 100, 800, 600);

	return viewer.run();
}