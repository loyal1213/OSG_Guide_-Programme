/*
在场景中添加光源主要步骤为：

在渲染状态中指定场景模型的法线——在渲染状态中开启光照并允许光源——创建osg::Light对象以定义光源参数——创建光源osg::LightSource对象，将光源参数加入其中——将光源节点添加到场景图形中。

其中，需要明确的是：场景模型只有在设有单位法线时才会正确显示光照。

*/
// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/Camera>

#include <osg/Light>			//	继承自osg::StateAttribute，保存灯光的模式与属性参数信息
#include <osg/LightSource>	    //  继承自osg::Group，灯光管理类，继承了osg::Group类的管理节点的接口，将灯光作为一个节点加入到场景图中进行渲染

#include <osg/BoundingSphere>
#include <osg/BoundingBox>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgViewer/ViewerEventHandlers> //  事件监听
#include <osgGA/StateSetManipulator>     //	事件响应类，对渲染状态进行控制，按w在体/线/点模式间切换，按l在照明与非照明模式下切换，按b在是否开启背面剔除模式下切换，按t在是否开启纹理的情况下切换

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>


//.cpp

/*
在场景中添加光源：
在渲染状态中指定场景模型的法线
在渲染状态中开启光照并允许光源
创建osg::Light对象以定义光源参数
创建光源osg::LightSource对象，将光源参数加入其中
将光源节点添加到场景图形中
场景模型只有在设有单位法线时才会正确显示光照
*/

//向场景中添加光源
osg::ref_ptr<osg::Group> CreateLight(osg::ref_ptr<osg::Node> node)
{
	osg::ref_ptr<osg::Group> lightRoot = new osg::Group();
	lightRoot->addChild(node);

	// 开启光照
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = lightRoot->getOrCreateStateSet();

	//当光照计算结果过于明亮或者暗淡时，允许法线的重缩放
	//stateset->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

	//场景中的缩放变换非均匀时，允许法线归一化，以保证法线为单位长度
	//stateset->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

	//为获得光照效果允许光照，需要允许光照并至少允许一个光源
	//开启光照 将ON改为OFF则关闭光照
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	//允许一个光源GL_LIGHT0
	stateset->setMode(GL_LIGHT0, osg::StateAttribute::ON);

	//计算包围盒
	osg::BoundingSphere bs;
	node->computeBound();
	bs = node->getBound();

	//创建一个Light对象，定义光源参数
	osg::ref_ptr<osg::Light> light = new osg::Light();
	light->setLightNum(0);

	//设置方向
	light->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
	//设置位置
	light->setPosition(osg::Vec4(bs.center().x(), bs.center().y(), bs.center().z() + bs.radius(), 1.0f));
	//设置环境光的颜色
	light->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//设置散射光的颜色
	light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//设置恒衰减指数
	light->setConstantAttenuation(1.0f);
	//设置线性衰减指数
	light->setLinearAttenuation(0.0f);
	//设置二次方衰减指数
	light->setQuadraticAttenuation(0.0f);

	// 创建光源
	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
	lightSource->setLight(light.get());


	lightRoot->addChild(lightSource.get());

	return lightRoot.get();
}

int TestLight()
{
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(CreateLight(node.get()));

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
	viewer->setSceneData(root.get());
	viewer->realize();

	return viewer->run();
}