
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

//��������
void createFireBall(osg::MatrixTransform* smokeNode)
{
    // �������Ӷ������������Բ���������ϵͳʹ�á�
    osgParticle::Particle particleTempalte;
    particleTempalte.setShape(osgParticle::Particle::QUAD);
    particleTempalte.setLifeTime(15); // ��λ����
    particleTempalte.setSizeRange(osgParticle::rangef(3.0f, 1.0f)); // ��λ����
    particleTempalte.setAlphaRange(osgParticle::rangef(1, 0));
    particleTempalte.setColorRange(osgParticle::rangev4(osg::Vec4(1.0f, 0.2f, 0.0f, 1.0f),
        osg::Vec4(0.1f, 0.1f, 0.1f, 0)
    ));

    particleTempalte.setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
    particleTempalte.setVelocity(osg::Vec3(0.0f, 0.0f, 0.0f));
    particleTempalte.setMass(0.1f); //��λ��ǧ��
    particleTempalte.setRadius(0.2f);
    particleTempalte.setSizeInterpolator(new osgParticle::LinearInterpolator);
    particleTempalte.setAlphaInterpolator(new osgParticle::LinearInterpolator);
    particleTempalte.setColorInterpolator(new osgParticle::LinearInterpolator);
    // ��������ʼ������ϵͳ��
    osgParticle::ParticleSystem* particleSystem = new osgParticle::ParticleSystem;
    particleSystem->setDataVariance(osg::Node::STATIC);
    // ���ò��ʣ��Ƿ�������ӣ��Լ��Ƿ�ʹ�ù��ա�
    particleSystem->setDefaultAttributes("Images/smoke.rgb", true, false);
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(particleSystem);
    smokeNode->addChild(geode);
    //����Ϊ����ϵͳ��ȱʡ���Ӷ���
    particleSystem->setDefaultParticleTemplate(particleTempalte);
    //��ȡ���伫��ȱʡ�������ľ��������ÿ֡���ӵ���������Ŀ
    osgParticle::RandomRateCounter* particleGenerateRate = new osgParticle::RandomRateCounter();
    particleGenerateRate->setRateRange(30, 50);
    // ÿ�������ɵ����ӷ�Χ
    particleGenerateRate->setDataVariance(osg::Node::DYNAMIC);
    // �Զ���һ�������������ﴴ������ʼ��һ���������
    osgParticle::PointPlacer* particlePlacer = new osgParticle::PointPlacer;

    particlePlacer->setCenter(osg::Vec3(0.0f, 0.0f, 0.0f));
    particlePlacer->setDataVariance(osg::Node::DYNAMIC);
    // �Զ���һ�����ȷ�����
    osgParticle::RadialShooter* particleShooter = new osgParticle::RadialShooter;
    // ���÷�����������
    particleShooter->setDataVariance(osg::Node::DYNAMIC);
    particleShooter->setThetaRange(-0.1f, 0.1f);
    // ����ֵ����Z ��н�
    particleShooter->setPhiRange(-0.1f, 0.1f);
    particleShooter->setInitialSpeedRange(5, 7.5f);//��λ����/��
    //������׼���伫����
    osgParticle::ModularEmitter* emitter = new osgParticle::ModularEmitter;
    emitter->setDataVariance(osg::Node::DYNAMIC);
    emitter->setCullingActive(false);
    // �����伫����������ϵͳ������
    emitter->setParticleSystem(particleSystem);
    // ���ü�����
    emitter->setCounter(particleGenerateRate);
    // ���÷�����
    emitter->setPlacer(particlePlacer);
    // ���÷�����
    emitter->setShooter(particleShooter);
    // �ѷ��伫���Ϊ�任�ڵ�
    smokeNode->addChild(emitter);
    // ��Ӹ���������ʵ��ÿ֡�����ӹ���
    osgParticle::ParticleSystemUpdater* particleSystemUpdater = new osgParticle::ParticleSystemUpdater;
    // ��������������ϵͳ���������
    particleSystemUpdater->addParticleSystem(particleSystem);
    // ���������ڵ���ӵ������С�
    smokeNode->addChild(particleSystemUpdater);
    // ������׼���������������ϵͳ�������
    osgParticle::ModularProgram* particleMoveProgram = new osgParticle::ModularProgram;
    particleMoveProgram->setParticleSystem(particleSystem);
    // ��󣬽��������ӵ������С�
    smokeNode->addChild(particleMoveProgram);
}

int Test16()
{
    osgEarth::initialize();

    osgViewer::Viewer viewer;

    osg::Group* root = new osg::Group;
    root->addChild(osgDB::readNodeFile("simple.earth"));

    osg::MatrixTransform* mt = new osg::MatrixTransform;
    //���������ϵͳ�ƶ���109 34 500��
    osg::EllipsoidModel* em = new osg::EllipsoidModel;
    osg::Matrixd mtd;
    em->computeLocalToWorldTransformFromLatLongHeight(osg::inDegrees(34.0), osg::inDegrees(109.0), 0, mtd);
    mt->setMatrix(mtd);

    osg::Node* glider = createBox();
    root->addChild(mt);
    mt->addChild(glider);

    //����ϵͳ
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