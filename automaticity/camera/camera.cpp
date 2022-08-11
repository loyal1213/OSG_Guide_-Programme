// camera.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "��27�� �������.hpp"
#include "Compass.h"
#include "8_1_7 ����ͼ�����.hpp"
#include "8_1_4 ����ͼ�����.hpp"
#include "8_1_6 ����ͼ�������Ⱦʾ��.hpp"
#include "8_1_9 ����ͼ�ര����Ⱦʾ��.hpp"
#include "8_1_2 �ü�ƽ��ʾ��(һ).hpp"
#include "8_1_3 �ü�ƽ��ʾ��(��).hpp"


#include "13.hpp"
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
	compass->setPlate(createCompassPart("d:/data/image/compass_plate.jpeg", 1.5f, -1.0f)); //Բ��ͼƬ
	compass->setNeedle(createCompassPart("d:/data/image/needle.png", 1.5f, 0.0f));//ָ��ͼƬ
	compass->setWidthHeight(0,0,200,200); //��ʼ�㡢���
	compass->setMainCamera(viewer->getCamera());

	compass->setRenderOrder(osg::Camera::POST_RENDER);
	compass->setClearMask(GL_DEPTH_BUFFER_BIT);
	compass->setAllowEventFocus(false);
	compass->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	compass->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	compass->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

	group->addChild(compass.get()); //������ڵ�


	group->addChild(node.get());

	// CreateCamera(viewer);

	viewer->setSceneData(group.get());

	viewer->realize();

	viewer->run();
}

int _tmain(int argc, _TCHAR* argv[])
{


	// TestCamera();
	// TestCompass();
	// TestCompositeViewer();
	// TestSingleCameras();
	//TestSingleViewMultipleCamera();
	// TestMultipleViewMultipleWindows();
	// TestClipPlane1();
	// Test37_2();
	Test13();
	return 0;
}

