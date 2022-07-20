// stdafx.h

#include <osg/Geode>    

#include <osg/Texture1D>  //一维纹理映射
#include <osg/Texture2D> //二维纹理映射
#include <osg/TextureCubeMap> //立方体纹理映射
#include <osg/TexGen> //指定用于自动生成纹理坐标的函数，可以设置纹理的计算方式是以物体坐标空间还是相机坐标空间来进行不同的计算
#include <osg/ShapeDrawable> //预定义几何体类，派生自osg::Drawable类。OSG中使用该类来将OSG内嵌的预定义几何体与osg::Drawable关联以渲染这些几何体
#include <osg/MatrixTransform>

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
//.cpp

/*
*设置一维纹理的数据 函数定义的一种方法
*一维纹理只有一个方向有像素变化
*因此此处手动为其指定内容：
*新建一个Image对象并向其中传入颜色数据
*然后设置一维纹理的边界截取方式为重复映射
*则色带被反复绘制在物体上
*
*函数参数为节点的渲染状态，无返回值
*/
void CreateTexture1D(osg::StateSet &ss)
{
	//实例化一个osg::Image类，用于封装存储纹理图像数据的图像类
	osg::ref_ptr<osg::Image> image = new osg::Image();
	/*virtual void osg::Image::setImage(int s, int t, int r, 
	GLint internalTextureformat,
	GLenum pixelFormat,
	GLenum type,
	unsigned char *data,
	AllocationMode mode,
	int packing = 1)
	*/
	image->setImage(256, 1, 1,
		GL_RGBA, GL_RGBA,
		GL_UNSIGNED_BYTE,
		new unsigned char[4 * 256],
		osg::Image::USE_NEW_DELETE);
	unsigned char *ptr = image->data();
	for (unsigned int i = 0; i < 256; ++i)
	{
		*ptr++ = i;
		*ptr++ = i;
		*ptr++ = 255;
		*ptr++ = 255;
	}

	osg::ref_ptr<osg::Texture1D> texture = new osg::Texture1D();
	texture->setImage(image.get());
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
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
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
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	ss.setTextureAttributeAndModes(0, texture.get());
}
/*
*设置一维纹理的数据 函数定义的另一种方法
*
*函数返回值为渲染状态
*/
osg::ref_ptr<osg::StateSet> CreateTexture1D()
{
	//创建贴图对象，实际上是一个高度为1的二维图像
	osg::ref_ptr<osg::Image> image = new osg::Image;
	//为image分配一个空间
	image->allocateImage(1024, 1, 1, GL_RGBA, GL_FLOAT);
	//设置纹理图像数据格式RGBA
	image->setInternalTextureFormat(GL_RGBA);

	//为image填充数据
	osg::Vec4 *dataPtr = (osg::Vec4 *)image->data();
	for (int i = 0; i < 1024; ++i){
		*dataPtr++ = osg::Vec4(1.0f, 0.5f, 0.8f, 0.5f);
	}

	//创建一维纹理
	osg::ref_ptr<osg::Texture1D> texture = new osg::Texture1D();
	//设置环绕模式
	texture->setWrap(osg::Texture1D::WRAP_S, osg::Texture1D::MIRROR);
	//设置滤波
	texture->setFilter(osg::Texture1D::MIN_FILTER, osg::Texture1D::LINEAR);
	//设置贴图
	texture->setImage(image.get());


	//设置自动纹理坐标，并制定相关的平面
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
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
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	/*纹理坐标
	enum Coord
	{
	S, //x
	T, //y
	R, //z
	Q //w
	};
	*/
	texgen->setPlane(osg::TexGen::S, osg::Plane(0.0f, 0.0f, 1.0f, -10000));

	// 创建属性集
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;

	//启用一维纹理
	stateset->setTextureAttribute(0, texture.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_1D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//启用纹理生成器
	stateset->setTextureAttribute(0, texgen.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_GEN_S, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	return stateset.get();
}

void TestTexture1D(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 加载模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cessna.osg");


	// 启用单元 1 自动生成纹理坐标，并使用纹理
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = CreateTexture1D();


	// 设置纹理状态
	node->setStateSet(stateset.get());
	root->addChild(node);

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
}