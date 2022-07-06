#include <osgViewer/Viewer>


#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Billboard>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>


#include <osgUtil/Optimizer>


osg::ref_ptr<osg::Node> CreateBillboardTree(osg::ref_ptr<osg::Image> image){

	// 创建四边形
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

	// 设置顶点
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(-0.5f,0.0f,-0.5f));
	v->push_back(osg::Vec3(0.5f,0.0f,-0.5f));
	v->push_back(osg::Vec3(0.5f,0.0f,0.5f));
	v->push_back(osg::Vec3(-0.5f,0.0f,0.5f));

	geometry->setVertexArray(v.get());


	// 设置法线
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	normal->push_back(osg::Vec3(1.0f,0.0f,0.0f)^osg::Vec3(0.0f,0.0f,1.0f));
	
	geometry->setNormalArray(normal.get());
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);


	// 设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));

	geometry->setTexCoordArray(0,vt.get());

	// 绘制四边形
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	if (image.get()){
		// 状态属性对象
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

		// 创建一个 Texture2D 属性对象
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();

		// 关联 image 
		texture->setImage(image.get());
		// 关联 Texture2D 纹理对象，第三个参数默认为ON
		stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
		// 启用混合
		stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		geometry->setStateSet(stateset.get());

	}


	// 创建billboard 对象
	osg::ref_ptr<osg::Billboard> billboard1 = new osg::Billboard();

	// 设置旋转模式为绕视点
	billboard1->setMode(osg::Billboard::POINT_ROT_EYE);
	// 添加 Drawable, 并设置其位置，默认位置为 osg::vec3(0.0F,0.0F,0.0F)
	billboard1->addDrawable(geometry.get(),osg::Vec3(5.0f,0.0f,0.0f));
	

	osg::ref_ptr<osg::Billboard> billboard2 = new osg::Billboard();
	// 设置旋转模式为绕轴转，因此还需要设置旋转轴
	billboard2->setMode(osg::Billboard::AXIAL_ROT);
	// 设置旋转轴
	billboard2->setAxis(osg::Vec3(0.0f, 0.0f, 1.0f));
    billboard2->addDrawable(geometry.get(),osg::Vec3(10.0f,0.0f,0.0f));
	
	osg::ref_ptr<osg::Group> billboard = new osg::Group();


	billboard->addChild(billboard1.get());
	billboard->addChild(billboard2.get());




	return billboard;
}

int TestBillboard(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 读取图像
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/tree0.rgba");

	// 缩放，达到合适的大小
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	pat->setScale(osg::Vec3(5.0f,5.0f,5.0f));
	pat->addChild(CreateBillboardTree(image));

	root->addChild(pat.get());


	// 读取 cow 的模型，以进行对比
	root->addChild(osgDB::readNodeFile("cow.osg"));

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}