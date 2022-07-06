// camera.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "第27讲 相机基础.hpp"
#include "Compass.h"


osg::MatrixTransform * createCompassPart(const std::string &image, float radius, float height)
{
	osg::Vec3 center(-radius, -radius, height);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(createTexturedQuadGeometry(center, osg::Vec3(radius*2.0f, 0.0f, 0.0f), osg::Vec3(0.0f, radius*2.0f, 0.0f)));

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(image));

	osg::ref_ptr<osg::MatrixTransform> part = new osg::MatrixTransform;
	part->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	part->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	part->addChild(geode.get());
	return part.release();
}

void TestCompass(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> group = new osg::Group();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");

	


	osg::ref_ptr<Compass> compass = new Compass;
	compass->setProjectionMatrix(osg::Matrixd::ortho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0));
	compass->setPlate(createCompassPart("d:/data/image/compass_plate.jpeg", 1.5f, -1.0f)); //圆盘图片
	compass->setNeedle(createCompassPart("d:/data/image/needle.png", 1.5f, 0.0f));//指针图片
	compass->setWidthHeight(0,0,200,200); //起始点、宽高
	compass->setMainCamera(viewer->getCamera());

	compass->setRenderOrder(osg::Camera::POST_RENDER);
	compass->setClearMask(GL_DEPTH_BUFFER_BIT);
	compass->setAllowEventFocus(false);
	compass->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	compass->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	compass->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

	group->addChild(compass.get()); //加入跟节点


	group->addChild(node.get());

	// CreateCamera(viewer);

	viewer->setSceneData(group.get());

	viewer->realize();

	viewer->run();
}

int _tmain(int argc, _TCHAR* argv[])
{


	// TestCamera();
	TestCompass();
	
	return 0;
}

