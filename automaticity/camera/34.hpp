#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osgGA/GUIEventHandler>
#include <osgGA/CameraManipulator>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osgUtil/SmoothingVisitor>
#include <osg/PolygonOffset>
#include <osgGA/StateSetManipulator>

osg::Group* _root = new osg::Group;

osg::Node* CreateSphere()
{
    //�뾶���ó�1.0����������ֵ�Ϳ���ֱ�����ó���ɫֵ
    float _r = 1.0;

    osg::Geode* _gnode = new osg::Geode;
    //_gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    //����
    osg::Geometry* geom = new osg::Geometry;  
    _gnode->addDrawable(geom);
    osg::PolygonOffset* po = new osg::PolygonOffset(1.0, 1.0);
    geom->getOrCreateStateSet()->setAttributeAndModes(po, osg::StateAttribute::ON|osg::StateAttribute::PROTECTED);

    osg::StateSet* ss = geom->getOrCreateStateSet();
    osg::Program* prom = new osg::Program;
    ss->setAttributeAndModes(prom, osg::StateAttribute::ON);
    prom->addShader(osgDB::readRefShaderFile("shader/basic.vert"));
    prom->addShader(osgDB::readRefShaderFile("shader/basic.frag"));

    //����
    osg::Geometry* geomLine = new osg::Geometry;
    _gnode->addDrawable(geomLine);
    geomLine->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

    osg::Vec3Array* _vertex = new osg::Vec3Array();
    geom->setVertexArray(_vertex);
    geomLine->setVertexArray(_vertex);

    //�����ߵ���ɫ
    osg::Vec4Array* lineColor = new osg::Vec4Array();
    lineColor->push_back(osg::Vec4(0.0, 0.0, 0.0, 1.0));
    geomLine->setColorArray(lineColor, osg::Array::BIND_OVERALL);

    //ʹ�������λ���
    osg::DrawElementsUInt* up = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
    geom->addPrimitiveSet(up);

    osg::DrawElementsUInt* upLine = new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_LOOP, 0);
    geomLine->addPrimitiveSet(upLine);


    float delta_degree = 15.0;
    int delta_theta_i = 1 + (360.0 - 0.001) / delta_degree;
    int delta_phi_i = 1 + (180.0 + 0.001) / delta_degree;

    int thetaIndex = 0;
    int phiIndex = 0;
    for (float theta = 0.0; theta < (360.0 - 0.001); theta += delta_degree, thetaIndex++)
    {
        //һ�еĶ������

        float cos_theta = std::cos(osg::inDegrees(theta));
        float sin_theta = std::sin(osg::inDegrees(theta));

        phiIndex = 0;
        for (float phi = -90.0; phi < (90 + 0.001); phi += delta_degree, phiIndex++)
        {
            //���һ��Ҫ���ϵ�һ��
            if (thetaIndex < (delta_theta_i - 1))
            {

                //��0��ѹ��һ��������
                if (0 == phiIndex)
                {
                    up->push_back(thetaIndex * delta_phi_i + phiIndex + 1);
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);

                }
                else if (phiIndex < (delta_phi_i - 1))//�м��ѹ����������
                {
                    up->push_back(thetaIndex * delta_phi_i + phiIndex + 1);
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);

                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex);
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex - 1);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);


                }
                else //���һ��ѹ��һ��������
                {
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex);
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex - 1);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);
                }
            }
            else
            {

                //��0��ѹ��һ��������
                if (0 == phiIndex)
                {
                    up->push_back(phiIndex + 1);
                    up->push_back(phiIndex);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);
                }
                else if (phiIndex < (delta_phi_i - 1))//�м��ѹ����������
                {
                    up->push_back(thetaIndex * delta_phi_i + phiIndex + 1);
                    up->push_back(phiIndex);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);

                    up->push_back(phiIndex);
                    up->push_back(phiIndex - 1);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);
                }
                else //���һ��ѹ��һ��������
                {
                    up->push_back(phiIndex);
                    up->push_back(phiIndex - 1);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);

                }
            }
            _vertex->push_back((osg::Vec3(_r*cos_theta*std::cos(osg::inDegrees(phi)), _r*std::sin(osg::inDegrees(phi)), _r*sin_theta*std::cos(osg::inDegrees(phi)))));

        }
    }

    //����
    for (int i = 0; i < up->size(); i++)
    {
        upLine->push_back(up->at(i));
    }

    //�ع�����
    osgUtil::SmoothingVisitor::smooth(*geom);
    return _gnode;
}


int Test34()
{
	osgViewer::Viewer viewer;

    _root->addChild(CreateSphere());
	viewer.setSceneData(_root);
    viewer.getCamera()->setClearColor(osg::Vec4(0.4, 0.4, 0.4, 1.0));
    viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
    viewer.realize();

    viewer.getCamera()->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    viewer.getCamera()->getGraphicsContext()->getState()->setUseVertexAttributeAliasing(true);
	return viewer.run();
}