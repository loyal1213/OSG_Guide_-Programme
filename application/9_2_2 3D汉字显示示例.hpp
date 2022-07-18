#include <osgViewer/Viewer>

#include <osg/Group>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Material>
#include <osg/StateSet>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgText/Text3D>
#include <osgText/Font3D>
#include <osgText/Font>


#include <osgUtil/Optimizer>
#include <iostream>


osg::ref_ptr<osg::Geode> Create3DText(){

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osgText::Text3D> text = new osgText::Text3D();


	text->setFont(osgText::readFontFile("simhei.ttf"));
	text->setText(L"http://OsgChina.org -- OpengSceneGraph 中国官方");

	text->setCharacterSize(60.0f);
	text->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));

	// 设置文字渲染模式
	text->setRenderMode(osgText::Text3D::PER_GLYPH);
	// 设置文字深度
	text->setCharacterDepth(10.0f);
	text->setDrawMode(osgText::Text3D::TEXT|osgText::Text3D::BOUNDINGBOX);
	text->setAxisAlignment(osgText::Text3D::XZ_PLANE);


	geode->addDrawable(text);

	// 设置材质
	osg::ref_ptr<osg::Material> material = new osg::Material();
	material->setColorMode(osg::Material::AMBIENT);
	material->setAlpha(osg::Material::FRONT_AND_BACK,1);
	material->setAmbient(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0f,1.0f,0.0f,1.0f));
	material->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0f,1.0f,0.0f,1.0f));
	
	geode->getOrCreateStateSet()->setAttribute(material);


	return geode;
}


void Test3DText(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	root->addChild(Create3DText());

	// root->addChild(osgDB::readNodeFile("lz.osg"));

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}

