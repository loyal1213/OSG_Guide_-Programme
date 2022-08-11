
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/EventHandler>
#include <osgParticle/ParticleSystem>
#include <osgParticle/LinearInterpolator>
#include <osg/MatrixTransform>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/PointPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularProgram>
#include <osgEarth/EarthManipulator>
#include <osgEarth/ExampleResources>
#include <osgEarth/MapNode>
#include <osgGA/NodeTrackerManipulator>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Geometry>

using namespace osgEarth;
using namespace osgEarth::Util;

osg::Node* createBox()
{
    osg::Geode* gnode = new osg::Geode;
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 10000.0, 10000.0, 10000.0));
    gnode->addDrawable(sd);
    return gnode;
}

//创建火球
void createFireBall(osg::MatrixTransform* smokeNode)
{
    // 创建粒子对象，设置其属性并交由粒子系统使用。
    osgParticle::Particle particleTempalte;
    particleTempalte.setShape(osgParticle::Particle::QUAD);
    particleTempalte.setLifeTime(15); // 单位：秒
    particleTempalte.setSizeRange(osgParticle::rangef(3.0f, 1.0f)); // 单位：米
    particleTempalte.setAlphaRange(osgParticle::rangef(1, 0));
    particleTempalte.setColorRange(osgParticle::rangev4(osg::Vec4(1.0f, 0.2f, 0.0f, 1.0f),
        osg::Vec4(0.1f, 0.1f, 0.1f, 0)
    ));

    particleTempalte.setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
    particleTempalte.setVelocity(osg::Vec3(0.0f, 0.0f, 0.0f));
    particleTempalte.setMass(0.1f); //单位：千克
    particleTempalte.setRadius(0.2f);
    particleTempalte.setSizeInterpolator(new osgParticle::LinearInterpolator);
    particleTempalte.setAlphaInterpolator(new osgParticle::LinearInterpolator);
    particleTempalte.setColorInterpolator(new osgParticle::LinearInterpolator);
    // 创建并初始化粒子系统。
    osgParticle::ParticleSystem* particleSystem = new osgParticle::ParticleSystem;
    particleSystem->setDataVariance(osg::Node::STATIC);
    // 设置材质，是否放射粒子，以及是否使用光照。
    particleSystem->setDefaultAttributes("Images/smoke.rgb", true, false);
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(particleSystem);
    smokeNode->addChild(geode);
    //设置为粒子系统的缺省粒子对象。
    particleSystem->setDefaultParticleTemplate(particleTempalte);
    //获取放射极中缺省计数器的句柄，调整每帧增加的新粒子数目
    osgParticle::RandomRateCounter* particleGenerateRate = new osgParticle::RandomRateCounter();
    particleGenerateRate->setRateRange(30, 50);
    // 每秒新生成的粒子范围
    particleGenerateRate->setDataVariance(osg::Node::DYNAMIC);
    // 自定义一个放置器，这里创建并初始化一个点放置器
    osgParticle::PointPlacer* particlePlacer = new osgParticle::PointPlacer;

    particlePlacer->setCenter(osg::Vec3(0.0f, 0.0f, 0.0f));
    particlePlacer->setDataVariance(osg::Node::DYNAMIC);
    // 自定义一个弧度发射器
    osgParticle::RadialShooter* particleShooter = new osgParticle::RadialShooter;
    // 设置发射器的属性
    particleShooter->setDataVariance(osg::Node::DYNAMIC);
    particleShooter->setThetaRange(-0.1f, 0.1f);
    // 弧度值，与Z 轴夹角
    particleShooter->setPhiRange(-0.1f, 0.1f);
    particleShooter->setInitialSpeedRange(5, 7.5f);//单位：米/秒
    //创建标准放射极对象
    osgParticle::ModularEmitter* emitter = new osgParticle::ModularEmitter;
    emitter->setDataVariance(osg::Node::DYNAMIC);
    emitter->setCullingActive(false);
    // 将放射极对象与粒子系统关联。
    emitter->setParticleSystem(particleSystem);
    // 设置计数器
    emitter->setCounter(particleGenerateRate);
    // 设置放置器
    emitter->setPlacer(particlePlacer);
    // 设置发射器
    emitter->setShooter(particleShooter);
    // 把放射极添加为变换节点
    smokeNode->addChild(emitter);
    // 添加更新器，以实现每帧的粒子管理。
    osgParticle::ParticleSystemUpdater* particleSystemUpdater = new osgParticle::ParticleSystemUpdater;
    // 将更新器与粒子系统对象关联。
    particleSystemUpdater->addParticleSystem(particleSystem);
    // 将更新器节点添加到场景中。
    smokeNode->addChild(particleSystemUpdater);
    // 创建标准编程器对象并与粒子系统相关联。
    osgParticle::ModularProgram* particleMoveProgram = new osgParticle::ModularProgram;
    particleMoveProgram->setParticleSystem(particleSystem);
    // 最后，将编程器添加到场景中。
    smokeNode->addChild(particleMoveProgram);
}

int Test16()
{
    osgEarth::initialize();

    osgViewer::Viewer viewer;

    osg::Group* root = new osg::Group;
    root->addChild(osgDB::readNodeFile("simple.earth"));

    osg::MatrixTransform* mt = new osg::MatrixTransform;
    //将这个粒子系统移动到109 34 500米
    osg::EllipsoidModel* em = new osg::EllipsoidModel;
    osg::Matrixd mtd;
    em->computeLocalToWorldTransformFromLatLongHeight(osg::inDegrees(34.0), osg::inDegrees(109.0), 0, mtd);
    mt->setMatrix(mtd);

    osg::Node* glider = createBox();
    root->addChild(mt);
    mt->addChild(glider);

    //粒子系统
    osg::MatrixTransform* mt1 = new osg::MatrixTransform;
    osg::Matrixd mtd1;
    em->computeLocalToWorldTransformFromLatLongHeight(osg::inDegrees(34.0), osg::inDegrees(109.0), 5000, mtd1);
    mt1->setMatrix(mtd1);
    createFireBall(mt1);
    root->addChild(mt1);

    viewer.setCameraManipulator(new EarthManipulator());
    viewer.setSceneData(root);
    return viewer.run();
}