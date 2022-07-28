//#include <Windows.h>
//#include <iostream>
//#include <string>
//
//#include <osgViewer/Viewer>
//#include <osgDB/ReadFile>
//
//#include <osgEarth/MapNode>
//
//#include <osgEarthDrivers/gdal/GDALOptions>
//#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
//#include <osgEarth/ImageLayer>
//
//#include <osgEarthUtil/EarthManipulator>
//
//using namespace std;

//int TestOsgEarth3D()
//{	   
//	osgEarth::ProfileOptions profileOpts;
//
//	//地图配置：设置缓存目录
//	osgEarth::Drivers::FileSystemCacheOptions cacheOpts;
//	string cacheDir = "D:/Work/OSGNewBuild/tmp";
//	cacheOpts.rootPath() = cacheDir;
//
//	//
//	osgEarth::MapOptions mapOpts;
//	mapOpts.cache() = cacheOpts;
//	mapOpts.profile() = profileOpts;
//
//	//创建地图节点
//	osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map(mapOpts);
//	osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);
//
//	osgEarth::Drivers::GDALOptions gdal;
//	gdal.url() = "D:/Work/OSGNewBuild/osgearth-2.10.1/data/world.tif";
//	osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer("BlueMarble", gdal);
//	map->addLayer(layer);
//
//	osgViewer::Viewer viewer;
//	viewer.setSceneData(mapNode);
//
//	osg::ref_ptr< osgEarth::Util::EarthManipulator> mainManipulator = new osgEarth::Util::EarthManipulator;
//	viewer.setCameraManipulator(mainManipulator);
//
//	viewer.setUpViewInWindow(100, 100, 800, 600);
//
//	return viewer.run();
//}