#include <osgViewer/viewer>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgGA/CameraManipulator>

class MyCameraManipulator : public osgGA::CameraManipulator
{
public:
    MyCameraManipulator() 
    {
        _theta = 0.0;
        _center = osg::Vec3(0.0, 0.0, 0.0);
        _up = osg::Vec3(0.0, 0.0, 1.0);
        _r = std::sqrt(5 * 5 + 5 * 5);
        _eye = osg::Vec3(_r*std::cos(osg::DegreesToRadians(_theta)), _r * std::sin(osg::DegreesToRadians(_theta)), 5.0);
    }

    //���������麯����������ʹ��
    virtual void setByMatrix(const osg::Matrixd& matrix) {};
    virtual void setByInverseMatrix(const osg::Matrixd& matrix) {};
    virtual osg::Matrixd getMatrix() const { return osg::Matrix::identity(); };

    //��ؼ����������������صľ���ViewMatrix
    virtual osg::Matrixd getInverseMatrix() const
    {
        return osg::Matrix::lookAt(_eye, _center, _up);
    };

    //�¼���������Ҫ���A��Χ��Z��˳ʱ��ת������D����ʱ��ת����ת��ʱ��ʼ�ճ�0 0 0 �㿴��
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
        {
            //����A��
            if ((ea.getKey() == 'A') || (ea.getKey() == 'a'))
            {
                _theta += 1.0;
            }
            if ((ea.getKey() == 'D') || (ea.getKey() == 'd'))
            {
                _theta -= 1.0;
            }

            _eye = osg::Vec3(_r * std::cos(osg::DegreesToRadians(_theta)), _r * std::sin(osg::DegreesToRadians(_theta)), 5.0);
        }
        return false;
    }

    //�ӵ�λ��
    osg::Vec3d              _eye;
    //�ӵ㿴������
    osg::Vec3d              _center;
    //ͷ���ĳ���
    osg::Vec3d              _up;

    //�ӵ㿴��0 0 0�ĽǶ�
    float              _theta;
    //�ӵ���0 0 0�ľ���
    float              _r;
};

osg::Node* createScene()
{
    osg::Group* root = new osg::Group();
    root->addChild(osgDB::readNodeFile("axes.osgt"));

    osg::Geode* gnode = new osg::Geode;
    root->addChild(gnode);

    osg::Geometry* geom = new osg::Geometry;
    gnode->addChild(geom);

    osg::Vec3Array* vertex = new osg::Vec3Array;
    geom->setVertexArray(vertex);
    //��xyƽ�滭�ߣ����0.5�ף���-5������5
    for (float i = -5; i <= 5; i += 0.5)
    {
        vertex->push_back(osg::Vec3(i, -5, 0));
        vertex->push_back(osg::Vec3(i, 5, 0));
        vertex->push_back(osg::Vec3(-5, i, 0));
        vertex->push_back(osg::Vec3(5, i, 0));
    }
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertex->size()));

    osg::Vec4Array* color = new osg::Vec4Array();
    color->push_back(osg::Vec4(0.7, 0.7, 0.7, 1.0));
    geom->setColorArray(color, osg::Array::BIND_OVERALL);
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    return root;
}


int main()
{
    osgViewer::Viewer viewer;

    viewer.setCameraManipulator(new MyCameraManipulator());
    viewer.setSceneData(createScene());

    return viewer.run();
}
