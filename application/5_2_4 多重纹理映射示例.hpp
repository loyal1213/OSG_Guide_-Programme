// stdafx.h
// 纹理映射（二）——一维/二维/简单立方图纹理映射
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
void createTexture1D(osg::StateSet &ss)
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
osg::ref_ptr<osg::StateSet> createTexture1D()
{
	//创建贴图对象，实际上是一个高度为1的二维图像
	osg::ref_ptr<osg::Image> image = new osg::Image;
	//为image分配一个空间
	image->allocateImage(1024, 1, 1, GL_RGBA, GL_FLOAT);
	//设置纹理图像数据格式RGBA
	image->setInternalTextureFormat(GL_RGBA);
	//为image填充数据
	osg::Vec4 *dataPtr = (osg::Vec4 *)image->data();
	for (int i = 0; i < 1024; ++i)
	{
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
 
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
	//启用一维纹理
	stateset->setTextureAttribute(0, texture.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_1D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	//启用纹理生成器
	stateset->setTextureAttribute(0, texgen.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_GEN_S, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
 
	return stateset.get();
}
 
/*
 *设置二维纹理的数据
 *直接使用已有的图像文件即可
 *还设置了纹理的边界颜色
 *
 *函数返回值为纹理
 */
osg::ref_ptr<osg::Texture2D> createTexture2D()
{
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setImage(osgDB::readImageFile("Images/clockface.jpg"));
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
	texture->setBorderColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));
 
	return texture.get();
}
 
/*
 *设置立方体纹理的数据
 *立方图纹理需要6个面的图像数据
 *常用于环境贴图的实现
 *额外的渲染属性TexGen
 *用于自动为立方图纹理的作用对象生成纹理坐标
 *
 *函数返回值为渲染状态
 */
osg::ref_ptr<osg::StateSet> createTextureCubeMap()
{
	osg::ref_ptr<osg::TextureCubeMap> texture = new osg::TextureCubeMap();
	texture->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile("Cubemap_snow/posx.jpg"));
	texture->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile("Cubemap_snow/negx.jpg"));
	texture->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile("Cubemap_snow/posy.jpg"));
	texture->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile("Cubemap_snow/negy.jpg"));
	texture->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile("Cubemap_snow/posz.jpg"));
	texture->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile("Cubemap_snow/negz.jpg"));
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
	texture->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
 
	osg::ref_ptr<osg::StateSet> sta = createTexture1D();
	sta->setTextureAttributeAndModes(0, texture.get());
	sta->setTextureAttributeAndModes(0, new osg::TexGen);
 
	return sta.get();
}
 
int TestTexture1DOr2DOrMap()
{
	//第1个一维纹理图像
	osg::ref_ptr<osg::Geode> texl1D = new osg::Geode();
	//使用createTexturedQuadGeometry()快速构建一个四边形，并设置其纹理坐标范围为（0, 0）到（3,1），以实现水平方向的重复条带
	//createTexturedQuadGeometry(const Vec3& corner,const Vec3& widthVec,const Vec3& heightVec, float l, float b, float r, float t);用于创建具有纹理坐标的四边形几何图形
	//参数1是从左下角起始的点，是这个起始点的世界坐标，这个起点按照参数2和参数3来画矩形，纹理坐标范围为（l, b）到（r, t）
	texl1D->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(-3.0, 0.0, -0.5), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0), 0.0, 0.0, 3.0, 1.0));
	//函数参数为当前节点的渲染状态，无返回值，则函数内进行渲染
	createTexture1D(*(texl1D->getOrCreateStateSet()));
 
	//第2个一维纹理图像
	osg::ref_ptr<osg::Node> texl1D2 = osgDB::readNodeFile("cessna.osg");
	//函数返回值为渲染状态，需要设置节点的渲染状态
	osg::ref_ptr<osg::StateSet> sta = createTexture1D();
	texl1D2->setStateSet(sta.get());
	//使用矩阵转换节点缩小模型，一般使用矩阵转换节点可以缩放模型（缩放到原来的多少倍）、更改模型的位置（向某个方向移动多少）、绕轴旋转
	//不能分成三个写，这样最后一个会把前面的覆盖，由于这是矩阵乘法，因此顺序不同结果也不同
	//setMatrix(osg::Matrix::scale(osg::Vec3(double sx, double sy, double sz)) * osg::Matrix::translate(osg::Vec3(double tx, double ty, double tz)) * osg::Matrix::rotate(double angle, const Vec3d &axis));
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	mt->setMatrix(osg::Matrix::scale(0.05, 0.05, 0.05));
	mt->addChild(texl1D2);
 
	//第1个二维纹理图像
	osg::ref_ptr<osg::Geode> texl2D = new osg::Geode();
	//设置四边形的纹理坐标范围为（-0.1, -0.1）到（1.1, 1.1），因此会额外绘制出纹理的边界
	texl2D->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(2, 0.0, -0.5), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0), -0.1, -0.1, 1.1, 1.1));
	//函数返回值为纹理，它为渲染状态的属性，因此需要先获取或设置节点的渲染状态，然后设置渲染状态的属性和模式
	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D();
	tex = createTexture2D();
	osg::ref_ptr<osg::StateSet> sta2 = new osg::StateSet();
	sta2->setTextureAttributeAndModes(0, createTexture2D());
	texl2D->setStateSet(sta2.get());
	//上面5句可以合并成下面1句
	//texl2D->getOrCreateStateSet()->setTextureAttributeAndModes(0, createTexture2D());
 
	//第1个立方体纹理图像
	osg::ref_ptr<osg::Geode> texlCube = new osg::Geode();
	texlCube->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(5.0, 0.0, 0.0), 0.7)));
	//函数返回值为渲染状态，需要设置节点的渲染状态
	osg::ref_ptr<osg::StateSet> sta1 = new osg::StateSet();
	texlCube->setStateSet(createTextureCubeMap());
 
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(texl1D.get());
	root->addChild(mt.get());
	root->addChild(texl2D.get());
	root->addChild(texlCube.get());
 
	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
 
	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}