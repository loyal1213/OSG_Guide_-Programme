#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>

#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexEnv>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

void TestTextureMultiple2(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 加载模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");


	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");

	if (image.get()){

		// 创建二维纹理
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
		texture->setImage(image.get());

		// 设置自动生成坐标
		osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen();
		texgen->setMode(osg::TexGen::SPHERE_MAP);

		// 设置纹理环境，模式为BLEND
		osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv();
		texenv->setMode(osg::TexEnv::BLEND);
		// 设置BLEND 操作的颜色
		texenv->setColor(osg::Vec4(0.6f,0.6f,0.6f,0.0f));

		// 启用单元 1 自动生成纹理坐标，并使用纹理
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
		stateset->setTextureAttributeAndModes(1,texture.get(),osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(1,texgen.get(),osg::StateAttribute::ON);

		// 设置纹理环境
		stateset->setTextureAttribute(1,texenv.get());

		// 设置纹理状态
		node->setStateSet(stateset.get());

	}


	root->addChild(node);

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
}