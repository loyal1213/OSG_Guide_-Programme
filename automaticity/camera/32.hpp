#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osgGA/CameraManipulator>
#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>


osg::Node* aix = osgDB::readNodeFile("axes.osgt");
osg::Node* glider = osgDB::readNodeFile("glider.osg");
osg::Camera* camera = new osg::Camera;

osgViewer::Viewer viewer;

class UpdateCallback : public osg::NodeCallback
{
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(node);

        //�������ǰ�Ĺ۲����
        osg::Matrix viewerViewMatrix = viewer.getCamera()->getViewMatrix();

        //camera�Լ��о��󣬸ı�����ĳ��򣬴˴�Ҫ�Ƚ��л�ԭ
        mt->setMatrix(osg::Matrix::rotate(viewerViewMatrix.getRotate()) *osg::Matrix::translate(osg::Vec3(0, 0, -5.0)));
        
    }
};


//�������½ǵ�������ָʾͼ
osg::Node* createCorner()
{
	osg::Group* root = new osg::Group;


	root->addChild(camera);

	//ֻ����һ��������
	camera->setProjectionMatrixAsPerspective(35.0, 1, 0.01, 1000.0);
    camera->setViewMatrix(osg::Matrix::identity());
	camera->setViewport(new osg::Viewport(0, 0, 200, 200));

	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);
    osg::MatrixTransform* mt = new osg::MatrixTransform;
    mt->setUpdateCallback(new UpdateCallback());
    mt->addChild(aix);
	camera->addChild(mt);
    camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

	return root;
}


osg::Node* buildScene()
{
	osg::Group* root = new osg::Group;

	root->addChild(aix);
	root->addChild(glider);
	root->addChild(createCorner());

	return root;
}

int Test32()
{
	viewer.setSceneData(buildScene());
	
	return viewer.run();
}