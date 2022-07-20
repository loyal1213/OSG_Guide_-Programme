// SpotLight

// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/Camera>

#include <osg/MatrixTransform> //移动节点的矩阵类，最常用的移动节点的类。可随动、旋转控制节点。
#include <osg/PositionAttitudeTransform>

#include <osg/Light> //继承自osg::StateAttribute，保存灯光的模式与属性参数信息
#include <osg/LightSource> //继承自osg::Group，灯光管理类，继承了osg::Group类的管理节点的接口，将灯光作为一个节点加入到场景图中进行渲染

#include <osg/ShapeDrawable>
#include <osg/TexGen>
#include <osg/Texture2D>
#include <osg/TexGenNode>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>


// .cpp

/*
*创建聚光灯纹理贴图
*创建聚光灯状态属性（前面的纹理贴图也是渲染状态属性之一）
*创建聚光灯节点
*创建路径动画
*创建地形平面（变量vertex报错，注释掉这个类，在场景中将之直接设置为模型牛）
*创建动画模型（路径为前面设置好的路径动画），将聚光灯节点添加添加到其中，则聚光灯是动态的（飞机）
*创建场景：创建动画模型和地形平面（地形平面出错，此处设为飞机），动画模型为聚光灯位置，地形平面（牛）为聚光灯照射的地方；将状态属性添加到组节点
*则飞机飞到哪个地方（即聚光灯在哪里），牛的哪个地方就照亮
*/

// 创建聚光灯纹理的mipmap贴图
osg::ref_ptr<osg::Image> CreateSpotLightImage(const osg::Vec4 centerColour, 
	const osg::Vec4 &backgroudColour, unsigned int size, float power){

		// 创建Image对象
		osg::ref_ptr<osg::Image> image = new osg::Image();
		// 动态分配一个 size*size 大小的image
		image->allocateImage(size, size, 1, GL_RGBA, GL_UNSIGNED_BYTE);

		// 填充image
		// 以中心为原点，颜色逐渐向四周衰减
		float mid = (float(size) - 1)*0.5f;
		float div = 2.0f / float(size);
		for (unsigned int r = 0; r < size; ++r){
			unsigned char *ptr = image->data(0, r, 0);
			for (unsigned int c = 0; c < size; ++c){
				float dx = (float(c) - mid) * div;
				float dy = (float(r) - mid) * div;
				float r = powf(1.0f - sqrtf(dx * dx + dy * dy), power);
				if (r < 0.0f)
					r = 0.0f;
				osg::Vec4 color = centerColour * r + backgroudColour * (1.0f - r);
				*ptr++ = (unsigned char)((color[0]) * 255.0f);
				*ptr++ = (unsigned char)((color[1]) * 255.0f);
				*ptr++ = (unsigned char)((color[2]) * 255.0f);
				*ptr++ = (unsigned char)((color[3]) * 255.0f);
			}
		}
		return image.release();
}

//创建聚光灯状态属性
osg::ref_ptr<osg::StateSet> CreateSpotLightDecoratorState(unsigned int lightNum, unsigned int textureUnit){
	
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	//开启ID为lightNum的光照
	stateset->setMode(GL_LIGHT0 + lightNum, osg::StateAttribute::ON);

	//设置中心的颜色和环境光的颜色
	osg::Vec4 centerColour(1.0f, 1.0f, 1.0f, 1.0f);
	osg::Vec4 ambientColour(0.5f, 0.5f, 0.5f, 1.0f);

	//创建聚光灯纹理
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setImage(CreateSpotLightImage(centerColour, ambientColour, 64, 1.0));
	texture->setBorderColor(osg::Vec4(ambientColour));
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_BORDER);


	//设置自动生成纹理坐标
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_S, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_T, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_Q, osg::StateAttribute::ON);

	//打开纹理单元
	stateset->setTextureAttributeAndModes(textureUnit, texture.get(), osg::StateAttribute::ON);

	return stateset.release();
}

//创建聚光灯节点
osg::ref_ptr<osg::Node> CreateSpotLightNode(const osg::Vec3 &position, const osg::Vec3 &direction, float angle, unsigned int lightNum, unsigned int textureUnit)
{
	osg::ref_ptr<osg::Group> group = new osg::Group();

	//创建光源
	osg::ref_ptr<osg::LightSource> lightsource = new osg::LightSource();
	osg::ref_ptr<osg::Light> light = lightsource->getLight();
	light->setLightNum(lightNum);
	light->setPosition(osg::Vec4(position, 1.0f));
	light->setAmbient(osg::Vec4(0.00f, 0.00f, 0.05f, 1.0f));
	light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	group->addChild(lightsource);

	//计算法向量
	osg::Vec3 up(0.0f, 0.0f, 1.0f);
	up = (direction ^ up) ^ direction;
	up.normalize();

	//创建自动生成纹理坐标节点
	osg::ref_ptr<osg::TexGenNode> texgenNode = new osg::TexGenNode();
	//关联纹理单元
	texgenNode->setTextureUnit(textureUnit);
	//设置纹理坐标生成器
	osg::ref_ptr<osg::TexGen> texgen = texgenNode->getTexGen();
	//设置模式为视觉线性
	texgen->setMode(osg::TexGen::EYE_LINEAR);
	//从视图中指定参考平面
	texgen->setPlanesFromMatrix(osg::Matrixd::lookAt(position, position + direction, up) * osg::Matrixd::perspective(angle, 1.0, 0.1, 100));

	group->addChild(texgenNode.get());

	return group.release();
}

//创建动画路径
osg::ref_ptr<osg::AnimationPath> CreateAnimationPath(const osg::Vec3 center, float radius, double looptime){

	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath();
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	int numSamples = 40;
	float yaw = 0.0f;
	float yaw_delta = 2.0f * osg::PI / ((float)numSamples - 1.0f);
	float roll = osg::inDegrees(30.0f);

	double time = 0.0f;
	double time_delta = looptime / (double)numSamples;
	for (int i = 0; i < numSamples; ++i){
		osg::Vec3 position(center + osg::Vec3(sinf(yaw) * radius, cosf(yaw) * radius, 0.0f));
		osg::Quat rotation(osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(-(yaw + osg::inDegrees(90.0f)), osg::Vec3(0.0, 0.0, 1.0)));
		animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));

		yaw += yaw_delta;
		time += time_delta;
	}
	return animationPath.release();
}

//创建地形平面
/*osg::ref_ptr<osg::Node> createBase(const osg::Vec3 ¢er, float radius)
{

osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/lz.rgb");
if (image.get())
{
osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
texture->setImage(image.get());
stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
}
osg::ref_ptr<osg::HeightField> grid = new osg::HeightField();
grid->allocate(38, 39);
grid->setOrigin(center + osg::Vec3(-radius, -radius, 0.0f));
grid->setXInterval(radius * 2.0f / (float)(38 - 1));
grid->setYInterval(radius * 2.0f / (float)(39 - 1));

float minHeight = FLT_MAX;
float maxHeight = -FLT_MAX;
unsigned int r;
for (r = 0; r < 39; ++r)
{
for (unsigned int c = 0; c < 38; ++c)
{
//error：vertex未定义
float h = vertex[r + c * 39][2];
if (h > maxHeight)
maxHeight = h;
if (h < minHeight)
minHeight = h;
}
}
float heightScale = radius * 0.5f / (maxHeight - minHeight);
float heightOffset = -(minHeight + maxHeight) * 0.5f;
for (r = 0; r < 39; ++r)
{
for (unsigned int c = 0; c < 38; ++c)
{
//error：vertex未定义
float h = vertex[r + c * 39][2];
grid->setHeight(c, r, (h + heightOffset) * heightScale);
}
}
osg::ref_ptr<osg::Geode> geode = new osg::Geode();
geode->setStateSet(stateset.get());
geode->addDrawable(new osg::ShapeDrawable(grid.get()));
osg::ref_ptr<osg::Group> group = new osg::Group();
group->addChild(geode.get());
return group.get();
}*/

//创建动画模型
osg::ref_ptr<osg::Node> CreateMovingModel(const osg::Vec3 center, float radius)
{
	osg::ref_ptr<osg::Group> model = new osg::Group();
	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("cessna.osg");
	if (cessna.get()){

		const osg::BoundingSphere &bs = cessna->getBound();
		float size = radius / bs.radius() * 0.3f;
		osg::ref_ptr<osg::MatrixTransform> positioned = new osg::MatrixTransform();
		positioned->setDataVariance(osg::Object::STATIC);
		positioned->setMatrix(osg::Matrix::translate(-bs.center()) * osg::Matrix::scale(size, size, size) * osg::Matrix::rotate(osg::inDegrees(180.0f), 0.0f, 0.0f, 2.0f));
		positioned->addChild(cessna.get());

		float animationLength = 10.0f;
		osg::ref_ptr<osg::AnimationPath> animationPath = CreateAnimationPath(center, radius, animationLength);

		osg::ref_ptr<osg::MatrixTransform> xform = new osg::MatrixTransform();
		xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath, 0.0f, 2.0));
		xform->addChild(positioned);

		//添加聚光灯节点
		xform->addChild(CreateSpotLightNode(osg::Vec3(0.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, -1.0f), 60.0f, 0, 1));

		model->addChild(xform.get());
	}
	return model.release();
}

//创建场景
osg::ref_ptr<osg::Node> CreateModel()
{
	osg::Vec3 center(0.0f, 0.0f, 0.0f);
	float radius = 100.0f;

	//创建动画模型
	osg::ref_ptr<osg::Node> shadower = CreateMovingModel(center, radius * 0.5f);

	//创建地形平面
	osg::ref_ptr<osg::Node> shadowed = osgDB::readNodeFile("cow.osg");
	//osg::ref_ptr<osg::Node> shadowed = createBase(center - osg::Vec3(0.0f, 0.0f, radius * 0.1), radius);

	//创建场景组节点
	osg::ref_ptr<osg::Group> root = new osg::Group();
	//设置状态属性
	root->setStateSet(CreateSpotLightDecoratorState(0, 1));
	//添加子节点
	root->addChild(shadower.get());
	root->addChild(shadowed.get());

	return root.release();
}

int TestSpotLight(){
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(CreateModel());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setSceneData(root.get());
	viewer->realize();

	return viewer->run();
}

/*
在以上的代码中，创建地形平面的vertex报错了：vertex未定义。我不知道该怎么修改，因为第二个循环的结果生成了格网，不能简单对其进行赋值。
注释掉该类，在场景中将地形平面的节点改为模型牛的节点，则飞机绕着牛转，飞机为聚光灯的灯源，牛为被照射物体，飞机飞到哪里，找到的牛的对应部位亮起。

*/