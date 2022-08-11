#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osgGA/CameraManipulator>


class MyCameraManipulator : public osgGA::CameraManipulator
{
public:
	MyCameraManipulator()
	{
		_theta = 0.0;
		_center = osg::Vec3(0.0, 0.0, 0.0);
		_up = osg::Vec3(0.0, 0.0, 1.0);
		_eye = osg::Vec3(0.0, -10.0, 0.0);

		_deltaEye = 0.3;

		_l = -2.0;
		_r = 2.0;
		_b = -2.0;
		_t = 2.0;
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
		if (ea.getEventType() == osgGA::GUIEventAdapter::FRAME)
		{
			us.asView()->getCamera()->setProjectionMatrixAsOrtho2D(_l, _r, _b, _t);
		}

		if (ea.getEventType() == osgGA::GUIEventAdapter::SCROLL)
		{
			//�жϹ����ķ���
			osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

			if (sm == osgGA::GUIEventAdapter::SCROLL_DOWN)
			{
				_deltaEye *= 1.1;

				_l *= 1.1;
				_t *= 1.1;
				_b *= 1.1;
				_r *= 1.1;

			}
			else
			{
				//��Χ��С0.1�����������෴������ͬʱ������
				_deltaEye *= 0.9;

				_l *= 0.9;
				_t *= 0.9;
				_b *= 0.9;
				_r *= 0.9;
			}
		}


		if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
		{

			//��ת�ӽ�
			if ((ea.getKey() == 'Q') || (ea.getKey() == 'q'))
			{
				//��q��1��
				_theta += 1;

				_up = osg::Vec3(0.0, 0.0, 1.0)*osg::Matrix::rotate(osg::inDegrees(_theta), osg::Y_AXIS);
			}

			//����A��
			if ((ea.getKey() == 'A') || (ea.getKey() == 'a'))
			{

				//_eye += osg::Vec3(-_deltaEye, 0.0, 0.0);
				_eye += osg::Vec3(-_deltaEye*std::cos(osg::inDegrees(_theta)), 0.0, _deltaEye*std::sin(osg::inDegrees(_theta)));


				_center = osg::Vec3(_eye.x(), 0.0, _eye.z());
			}
			if ((ea.getKey() == 'D') || (ea.getKey() == 'd'))
			{
				//_eye += osg::Vec3(_deltaEye, 0.0, 0.0);

				_eye -= osg::Vec3(-_deltaEye*std::cos(osg::inDegrees(_theta)), 0.0, _deltaEye*std::sin(osg::inDegrees(_theta)));

				_center = osg::Vec3(_eye.x(), 0.0, _eye.z());
			}

		}
		return false;
	}

	//�ӵ�λ��
	osg::Vec3d              _eye;

	//������a�������Ƶ����ȣ����������ֵķŴ���С���������Ҳ�ڱ仯
	double _deltaEye; 


	//�ӵ㿴������
	osg::Vec3d              _center;
	//ͷ���ĳ���
	osg::Vec3d              _up;

	//�ӵ㿴��0 0 0�ĽǶ�
	float              _theta;


	//��άͶӰ����, left, right, bottom, top
	double _l, _r, _b, _t;
};


osg::Geode* createNet()
{
	osg::Geode* gnode = new osg::Geode;

	osg::Geometry* geom = new osg::Geometry;
	gnode->addDrawable(geom);

	//�����ߵ���ɫΪ��ɫ
	osg::Vec4Array* color = new osg::Vec4Array;
	color->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	geom->setColorArray(color, osg::Array::BIND_OVERALL);

	osg::Vec3Array* vertex = new osg::Vec3Array;
	geom->setVertexArray(vertex);

	//���1�ף���20������20��
	for (int i = 0; i < 20; i++)
	{
		//x����
		vertex->push_back(osg::Vec3(0, 0, i));
		vertex->push_back(osg::Vec3(17, 0, i)); //����Ϊ17��������

		//z����
		vertex->push_back(osg::Vec3(i, 0, 0));
		vertex->push_back(osg::Vec3(i, 0, 17));
	}

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertex->size()));
	geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return gnode;
}


int Test31()
{
	osgViewer::Viewer viewer;
	viewer.setSceneData(createNet());
	viewer.setCameraManipulator(new MyCameraManipulator());
	return viewer.run();
}