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
    //半径设置成1.0，这样顶点值就可以直接设置成颜色值
    float _r = 1.0;

    osg::Geode* _gnode = new osg::Geode;
    //_gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    //球体
    osg::Geometry* geom = new osg::Geometry;  
    _gnode->addDrawable(geom);
    osg::PolygonOffset* po = new osg::PolygonOffset(1.0, 1.0);
    geom->getOrCreateStateSet()->setAttributeAndModes(po, osg::StateAttribute::ON|osg::StateAttribute::PROTECTED);

    osg::StateSet* ss = geom->getOrCreateStateSet();
    osg::Program* prom = new osg::Program;
    ss->setAttributeAndModes(prom, osg::StateAttribute::ON);
    prom->addShader(osgDB::readRefShaderFile("shader/basic.vert"));
    prom->addShader(osgDB::readRefShaderFile("shader/basic.frag"));

    //勾边
    osg::Geometry* geomLine = new osg::Geometry;
    _gnode->addDrawable(geomLine);
    geomLine->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

    osg::Vec3Array* _vertex = new osg::Vec3Array();
    geom->setVertexArray(_vertex);
    geomLine->setVertexArray(_vertex);

    //设置线的颜色
    osg::Vec4Array* lineColor = new osg::Vec4Array();
    lineColor->push_back(osg::Vec4(0.0, 0.0, 0.0, 1.0));
    geomLine->setColorArray(lineColor, osg::Array::BIND_OVERALL);

    //使用三角形绘制
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
        //一列的顶点个数

        float cos_theta = std::cos(osg::inDegrees(theta));
        float sin_theta = std::sin(osg::inDegrees(theta));

        phiIndex = 0;
        for (float phi = -90.0; phi < (90 + 0.001); phi += delta_degree, phiIndex++)
        {
            //最后一列要连上第一列
            if (thetaIndex < (delta_theta_i - 1))
            {

                //第0个压入一个三角形
                if (0 == phiIndex)
                {
                    up->push_back(thetaIndex * delta_phi_i + phiIndex + 1);
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);

                }
                else if (phiIndex < (delta_phi_i - 1))//中间的压两个三角形
                {
                    up->push_back(thetaIndex * delta_phi_i + phiIndex + 1);
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);

                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex);
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex - 1);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);


                }
                else //最后一个压入一个三角形
                {
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex);
                    up->push_back((thetaIndex + 1) * delta_phi_i + phiIndex - 1);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);
                }
            }
            else
            {

                //第0个压入一个三角形
                if (0 == phiIndex)
                {
                    up->push_back(phiIndex + 1);
                    up->push_back(phiIndex);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);
                }
                else if (phiIndex < (delta_phi_i - 1))//中间的压两个三角形
                {
                    up->push_back(thetaIndex * delta_phi_i + phiIndex + 1);
                    up->push_back(phiIndex);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);

                    up->push_back(phiIndex);
                    up->push_back(phiIndex - 1);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);
                }
                else //最后一个压入一个三角形
                {
                    up->push_back(phiIndex);
                    up->push_back(phiIndex - 1);
                    up->push_back(thetaIndex * delta_phi_i + phiIndex);

                }
            }
            _vertex->push_back((osg::Vec3(_r*cos_theta*std::cos(osg::inDegrees(phi)), _r*std::sin(osg::inDegrees(phi)), _r*sin_theta*std::cos(osg::inDegrees(phi)))));

        }
    }

    //拷贝
    for (int i = 0; i < up->size(); i++)
    {
        upLine->push_back(up->at(i));
    }

    //重构法线
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