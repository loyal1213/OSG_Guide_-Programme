// stdafx.h 
// 纹理映射（一）——多重纹理映射
#include <osg/Node>  
#include <osg/Geometry>  
#include <osg/Geode> //是个几何节点，可以说是一个几何Group节点，一般的可绘制几何体都是通过它来传向root进行渲染，是OSG几何绘制的最高管理节点  
#include <osg/Group> //对节点起到组织作用，一般作为父节点或者根节点出现

#include <osg/Texture2D> //二维纹理映射类
#include <osg/TexGen> //指定用于自动生成纹理坐标的函数，可以设置纹理的计算方式是以物体坐标空间还是相机坐标空间来进行不同的计算
#include <osg/TexEnv> //设置当前纹理映射方式 

#include <osgDB/ReadFile>  
#include <osgDB/WriteFile>  

#include <osgViewer/Viewer>  

#include <osgUtil/Optimizer> 

//.cpp
 
/*
多重纹理映射
标准的二维纹理映射处理是一次把一幅纹理图像应用到一个多边形上
多重纹理映射允许应用几个纹理
在操作管理栈中依次把他们应用到同一个多边形上
多重纹理存在一些列的纹理单元
每个纹理单元执行单独的纹理操作
并把它的结果传递给下一个纹理单元
直到所有纹理单元的操作完成为止
最终显示处理后的效果
多重纹理能实现高级渲染技巧：光照、贴花、合成、细节纹理等
步骤：
指定用户几何体的纹理坐标
创建多个纹理属性对象并保存纹理多个图形数据
为StateSet设置合适的纹理属性和模式
对于不同的纹理属性对象需要指定不同的纹理单元及纹理坐标
否则就不会启用该纹理单元
或者该纹理单元会被覆盖
*/
static void textureMultiple(osg::Node *node, osg::Image *image)
{
	if (image)
	{
		//创建二维纹理
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		texture->setImage(image);
 
		/*
		纹理坐标的自动生成模式：
		enum Mode
		{
			OBJECT_LINEAR, //物体线性，纹理贴图与移动物体保持固定
			EYE_LINEAR, //产生移动物体的动态轮廓线
			SPHERE_MAP, //球体贴图
			NORMAL_MAP, //发现贴图，用于立方图纹理
			REFLECTION_MAP, //反射贴图
		}
		*/
		osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
		texgen->setMode(osg::TexGen::SPHERE_MAP);
 
		/*
		纹理映射模式（处理纹理图像数据与物体本身的融合）：
		enum Mode
		{
			DECAL, //贴花
			MODULATE, //调整
			BLEND, //混合
			REPLACE, //替换，覆盖
			ADD
		}
		*/
		osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv;
		texenv->setMode(osg::TexEnv::BLEND);
		//设置混合模式BLEND操作的颜色
		texenv->setColor(osg::Vec4(0.6f, 0.6f, 0.6f, 0.0f));
 
		//启用单元1自动生成纹理坐标，并使用纹理
		//纹理单元可理解为纹理的索引
		//一个纹理单元与一个纹理图像相互对应绑定，控制着该纹理图像的处理步骤
		//一个纹理单元在一个纹理通道中维护纹理的各种状态，如滤波、环绕方式、环境、纹理坐标
		//在Mipmap中，多重纹理包含一组连续的纹理单元，Mipmap类似于数据金字塔
		//即当可视化对象距离视角越近，图像像素越高，看到的图像也就越清晰，当距离视角较远时，像素质量降低
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
		stateset->setTextureAttributeAndModes(1, texture.get(), osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(1, texgen.get(), osg::StateAttribute::ON);
 
		//设置纹理环境
		stateset->setTextureAttribute(1, texenv.get());
 
		//设置纹理状态
		node->setStateSet(stateset.get());
	}
}
 
int TestTextureMultiple()
{
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");
	//读取贴图
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");
 
	//多重纹理映射
	textureMultiple(node, image);
 
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(node.get());
 
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
 
	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}