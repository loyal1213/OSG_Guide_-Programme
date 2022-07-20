/*
三维纹理映射是一大类应用范畴的一部分，称为体渲染。它虽然能达到很好的效果，但当场景较大时，渲染负担极大，占用的内存资源极多，因此应用于实际的项目极少。

三维纹理可以看成是一层层的二维纹理图像矩阵所构成的一个有多层深度的纹理图像。

三维纹理osg::Texture3D类继承自osg::Texture类，但它不支持立方图纹理。使用三维纹理的方法同使用二维纹理基本类似，三维纹理图像的二维纹理子图的大小和像素格式需要一致，使用三维纹理时需要初始化一个图形环境。
*/
// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Group>

#include <osg/Image> 
#include <osg/StateSet>

#include <osg/TexGen> //指定用于自动生成纹理坐标的函数，可以设置纹理的计算方式是以物体坐标空间还是相机坐标空间来进行不同的计算
#include <osg/TexEnv>
#include <osg/Texture3D>  //继承自osg::Texture类，但它不支持立方图纹理

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//.cpp
 
/*
 *三维纹理映射
 *使用三维纹理的方法同使用二维纹理基本类似
 *三维纹理图像的二维纹理子图的大小和像素格式需要一致
 *使用三维纹理时需要初始化一个图形环境
 *
 *这个例子是将多张二维纹理数据压入一个渲染体
*/
 
 
//创建一个四边形节点
osg::ref_ptr<osg::Node> createNode(){

	//添加到叶节点
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();


	//设置顶点
	osg::ref_ptr<osg::Vec3Array> vc = new osg::Vec3Array;
	//添加数据  
	vc->push_back(osg::Vec3(0.0, 0.0, 0.0));
	vc->push_back(osg::Vec3(1.0, 0.0, 0.0));
	vc->push_back(osg::Vec3(1.0, 0.0, 1.0));
	vc->push_back(osg::Vec3(0.0, 0.0, 1.0));
 
	geom->setVertexArray(vc.get());

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	//添加数据
	vt->push_back(osg::Vec2(0.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 1.0f));
	vt->push_back(osg::Vec2(0.0f, 1.0f));
 
	//设置纹理坐标数组setTexCoordArray(unsigned int unit, Array *)参数纹理单元/纹理坐标数组
	geom->setTexCoordArray(0, vt.get());

	//设置法线
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	//添加法线
	nc->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));	
 

	//设置法线数组setNormalArray(Array *array)
	geom->setNormalArray(nc.get());
	//设置法线的绑定方式setNormalBinding(AttributeBinding ab)为全部顶点
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
 
	//添加图元，绘制基元为四边形
	//数据解析，即指定向量数据和绑定方式后，指定渲染几何体的方式，不同方式渲染出的图形不同，即时效果相同，可能面数或内部机制等也有区别，函数为：
	//bool addPrimitiveSet(PrimitiveSet *primitiveset)参数说明：osg::PrimitiveSet是无法初始化的虚基类，因此主要调用它的子类指定数据渲染，最常用为osg::DrawArrays
	//osg::DrawArrays(GLenum mode, GLint first, GLsizei count)参数为指定的绘图基元、绘制几何体的第一个顶点数在指定顶点的位置数、使用的顶点的总数
	//PrimitiveSet类继承自osg::Object虚基类，但不具备一般一般场景中的特性，PrimitiveSet类主要封装了OpenGL的绘图基元，常见绘图基元如下
	//POINTS点/LINES线/LINE_STRIP多线段/LINE_LOOP封闭线
	//TRIANGLES一系列三角形(不共顶点)/TRIANGLE_STRIP一系列三角形(共用后面两个顶点)/TRIANGLE_FAN一系列三角形，顶点顺序与上一条语句绘制的三角形不同
	//QUADS四边形/QUAD_STRIP一系列四边形/POLYGON多边形
 
	// 添加图元
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
 
	// 绘制
	geode->addDrawable(geom.get());
 
	return geode.release();
}
 
//初始化一个图形环境
class MyGraphicsContext{
public:
	MyGraphicsContext(){

		//设置图形环境特性
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
		//设置左上角坐标
		traits->x = 0;
		traits->y = 0;
		//设置宽度和高度
		traits->width = 1;
		traits->height = 1;

		//设置窗口扩展
		traits->windowDecoration = false;
		//设置双缓冲
		traits->doubleBuffer = false;
		traits->sharedContext = 0;
		//设置pbuffer
		traits->pbuffer = true;
		//创建图形环境
		_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
 
		//如果创建失败
		if (!_gc){
			// 设置pbuffer为false
			traits->pbuffer = false;
			// 重新创建图形环境
			_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		}
 
		//是否初始化
		if (_gc.valid()){
			//初始化
			_gc->realize();
			_gc->makeCurrent();
		}
	}

	bool valid() const{
		return _gc.valid() && _gc->isRealized();
	}
 
private:
	osg::ref_ptr<osg::GraphicsContext> _gc;
};
 
//创建三维纹理属性
osg::ref_ptr<osg::StateSet> createState()
{
	//创建图形环境
	MyGraphicsContext gc;
	if (!gc.valid()){
		//如果创建失败，则返回
		osg::notify(osg::NOTICE) << "Unable to create the graphics context required to build 3d image." << std::endl;
		return 0;
	}
 
	//读取4张二维纹理图像
	osg::ref_ptr<osg::Image> image_0 = osgDB::readImageFile("Images/lz.rgb");
	osg::ref_ptr<osg::Image> image_1 = osgDB::readImageFile("Images/reflect.rgb");
	osg::ref_ptr<osg::Image> image_2 = osgDB::readImageFile("Images/tank.rgb");
	osg::ref_ptr<osg::Image> image_3 = osgDB::readImageFile("Images/skymap.jpg");
 
	//判断是否正确读取
	if (!image_0 || !image_1 || !image_2 || !image_3){
		std::cout << "Warning:could not open files." << std::endl;
		return new osg::StateSet();
	}
 
	//判断纹理格式是否一致
	if (image_0->getPixelFormat() != image_1->getPixelFormat() || 
		image_0->getPixelFormat() != image_2->getPixelFormat() || 
		image_0->getPixelFormat() != image_3->getPixelFormat() ){
		std::cout << "Warning:image pixel formats not compatible." << std::endl;
		return new osg::StateSet();
	}
 
	//得到支持的、最大的三维纹理单元的大小
	/*GLint textureSize = osg::Texture3D::getExtensions(0, true)->maxTexture3DSize();
	if (textureSize > 256){
		textureSize = 256;
	}*/
 
	GLint textureSize = 256;
 
	//对4张二维纹理图像缩放，以达到相同的大小
	image_0->scaleImage(textureSize, textureSize, 1);
	image_1->scaleImage(textureSize, textureSize, 1);
	image_2->scaleImage(textureSize, textureSize, 1);
	image_3->scaleImage(textureSize, textureSize, 1);
 
	//创建一个三维纹理数据图像，注意深度为4
	osg::ref_ptr<osg::Image> image_3d = new osg::Image();
 
	//设置三维纹理数据图像属性，第一个和第二个参数是纹理的大小，第三个参数是三维纹理数据图像的深度
	image_3d->allocateImage(textureSize, textureSize, 4, image_0->getPixelFormat(), image_0->getDataType());
 
	// 把4张二维纹理图像压入三维纹理数据图像
	// 前三个参数分别是s/t/r上的偏移，这里只是r上的偏移
	// 第四个参数是子二维纹理图像数据
	image_3d->copySubImage(0, 0, 0, image_0.get());
	image_3d->copySubImage(0, 0, 1, image_0.get());
	image_3d->copySubImage(0, 0, 2, image_0.get());
	image_3d->copySubImage(0, 0, 3, image_0.get());
 
	//设置纹理格式
	image_3d->setInternalTextureFormat(image_0->getInternalTextureFormat());
 
	//创建三维纹理对象
	osg::ref_ptr<osg::Texture3D> texture3D = new osg::Texture3D();
	//设置滤波，不支持Mipmap滤波
	/*设置纹理的过滤方法/过滤处理
	enum FilterParameter
	{
		MIN_FILTER, //缩小
		MAG_FILTER //放大
	};
	enum FilterMode
	{
		LINEAR, //以周围4个像素的平均值作为纹理
		LINEAR_MIPMAP_LINEAR, //使用线性均和计算两个纹理的值
		LINEAR_MIPMAP_NEAREST, //线性地改写临近的纹理单元值
		NEAREST, //取比较接近的像素作为纹理
		NEAREST_MIPMAP_LINEAR, //在两个纹理中选择最临近的纹理，并取它们之间的线性均和值
		NEAREST_MIPMAP_NEAREST //选择最临近的纹理单元值
	};
	*/
	texture3D->setFilter(osg::Texture3D::MIN_FILTER, osg::Texture3D::LINEAR);
	texture3D->setFilter(osg::Texture3D::MAG_FILTER, osg::Texture3D::LINEAR);
	//设置环绕模式
	/*设置纹理的坐标/包装模式
	enum WrapParmeter
	{
		WRAP_S, //x轴
		WRAP_T, //y轴
		WRAP_T //z轴
	};
	enum WrapMode
	{
		CLAMP, //截取
		CLAMP_TO_EDGE, //边框始终被忽略
		CLAMP_TO_BORDER, //它使用的纹理取自图像的边框，没有边框就使用常量边框的颜色
		REPEAT, //纹理的重复映射
		MIRROR //纹理镜像的重复映射
	};
	*/
	texture3D->setWrap(osg::Texture3D::WRAP_R, osg::Texture3D::REPEAT);
	//关联三维纹理图像数据
	texture3D->setImage(image_3d.get());
 
	//设置自动生成纹理坐标
	/*设置纹理坐标的自动生成模式
	enum Mode
	{
		OBJECT_LINEAR, //物体线性，纹理贴图与移动物体保持固定
		EYE_LINEAR, //产生移动物体的动态轮廓线
		SPHERE_MAP, //球体贴图
		NORMAL_MAP, //法线贴图，用于立方图纹理
		REFLECTION_MAP //反射贴图
	};
	*/
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen();
	//设置自动生成纹理坐标为视觉线性
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	//指定参考平面
	texgen->setPlane(osg::TexGen::R, osg::Plane(0.0f, 0.0f, 0.0f, 0.2f));
 
	//创建状态属性对象
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	//设置在R上自动生成纹理坐标
	stateset->setTextureMode(0, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
	//启用自动生成纹理对象
	stateset->setTextureAttribute(0, texgen.get());
	stateset->setTextureAttributeAndModes(0, texture3D.get(), osg::StateAttribute::ON);
 
	return stateset.release();
}
 
int Test3DTextureMap()
{
	osg::ref_ptr<osg::Node> node = createNode();
	osg::ref_ptr<osg::StateSet> stateset = createState();
	node->setStateSet(stateset.get());
 
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(node.get());
 
	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}