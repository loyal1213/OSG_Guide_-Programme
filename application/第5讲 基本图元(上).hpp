#include <osgDB/ReadFile>

#include <osgViewer/ViewerEventHandlers>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/Geode>


osg::ref_ptr<osg::Geode> CreateBox(){

	osg::ref_ptr<osg::Geode> geode= new osg::Geode();
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints(); // 设置精度
	osg::ref_ptr<osg::ShapeDrawable> shap = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0,0.0,0.0),10.0,1.0,10.0),hints.get());
	osg::ref_ptr<osg::Material> material = new osg::Material();
	osg::ref_ptr<osg::Texture2D> texture2D = new osg::Texture2D();
	osg::ref_ptr<osg::Image> image = new osg::Image();

	// 设置颜色
	shap->setColor(osg::Vec4(1.0,0.5,0.5,0.1));

	// 设置精度
	hints->setDetailRatio(0.5f);

	// 设置材质
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4f(1.0,1.0,1.0,0.5));
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(1.0,1.0,1.0,0.5));
	material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(1.0,1.0,1.0, 0.5));
	material->setShininess(osg::Material::FRONT_AND_BACK, 6.0f);

	// 设置纹理
	image = osgDB::readRefImageFile("Images/skymap.jpg");
	if (image->valid()){
		texture2D->setImage(image);
	}



	//geode->getOrCreateStateSet()->setAttributeAndModes(material.get(),osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture2D, osg::StateAttribute::ON);

	geode->addDrawable(shap.get());


	return geode;
}


int TestGeode(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setSceneData(CreateBox());

	return viewer->run();
}