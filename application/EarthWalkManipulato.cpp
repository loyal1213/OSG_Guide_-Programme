#include "StdAfx.h"
#include "EarthWalkManipulato.h"
#include <osgViewer\Viewer>

#include <osgDB\ReadFile>
#include <osg\MatrixTransform>


using namespace osgEarth;

EarthWalkManipulator::EarthWalkManipulator()
{
	_eye = osg::Vec3d(0, 0, 0);
	//_rotate = osg::Quat(-osg::PI_2, osg::X_AXIS);
	_speed = 1.0;
}

EarthWalkManipulator::~EarthWalkManipulator()
{
}

//��ȡ�������̬���� 
osg::Matrixd EarthWalkManipulator::getMatrix() const
{
	osg::Matrix mat;
	mat.setRotate(_rotate);//����ת
	mat.postMultTranslate(_eye);//��ƽ��
	return mat;
}

osg::Matrixd EarthWalkManipulator::getInverseMatrix() const
{
	osg::Matrix mat;
	mat.setRotate(-_rotate);
	mat.preMultTranslate(-_eye);
	return mat;
	//return osg::Matrixd::inverse(getMatrix());
}

void
	EarthWalkManipulator::home(double unused)
{
	_eye = osg::Vec3d(0, 0, 0);
	_speed = 1.0;
}

void
	EarthWalkManipulator::home(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter& us)
{
	home(0.0);
	us.requestRedraw();
}

void
	EarthWalkManipulator::setNode(osg::Node* node)
{
	// you can only set the node if it has not already been set, OR if you are setting
	// it to NULL. (So to change it, you must first set it to NULL.) This is to prevent
	// OSG from overwriting the node after you have already set on manually.
	if (node == 0L || !_node.valid())
	{
		_root = node;
		_node = node;
		_mapNode = 0L;
		_srs = 0L;

		established();

		osg::Matrix matrixGood1;
		GeoPoint point1(_srs, 0, 0, 10000.0);
		point1.createLocalToWorld(matrixGood1);

		_eye = matrixGood1.getTrans();

		osg::Vec3d worldup;
		point1.createWorldUpVector(worldup);

		osg::Matrix mat;
		matrixGood1.getRotate().get(mat);
		osg::Vec3d eye, center, up;
		mat.getLookAt(eye, center, up);
		mat.makeLookAt(eye, -worldup, up);

		_rotate = mat.getRotate();

	}
}

osg::Node*
	EarthWalkManipulator::getNode()
{
	return _node.get();
}

bool EarthWalkManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::FRAME):
		{
			if (calcMovement(ea))//�����������Ļ�е�λ�õ������ת��
				us.requestRedraw();
			return true;
		}break;
	case(osgGA::GUIEventAdapter::SCROLL):
		{
			osg::Quat qat;
			osg::Matrix mat;
			_rotate.get(mat);
			osg::Vec3d eye, center, up;
			mat.getLookAt(eye, center, up);

			osg::Vec3d dirction = center - eye;
			dirction.normalize();
			up.normalize();
			osg::Vec3d cross = dirction^up;
			cross.normalize();

			cross *= 0.01;
			switch (ea.getScrollingMotion())
			{
			case osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP://��ʱ����ת���
				{
					mat = osg::Matrix::lookAt(eye, center, up + cross);
					_rotate = mat.getRotate();
				}break;
			case osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN://˳ʱ����ת���
				{
					mat = osg::Matrix::lookAt(eye, center, up - cross);
					_rotate = mat.getRotate();
				}break;
			}
			return true;
		}break;
	case (osgGA::GUIEventAdapter::KEYDOWN):
		{
			osg::Vec3   v3Direction;         //�ӵ㷽��  
			osg::Matrix mCameraQuat;
			osg::Vec3d  v3Eye, v3Center, v3Up;
			_rotate.get(mCameraQuat);
			mCameraQuat.getLookAt(v3Eye, v3Center, v3Up);//�����v3Eye����ʵ�������λ�ã�����0��0��0
			v3Direction = v3Center - v3Eye;
			v3Direction.normalize();
			osg::Vec3d v3CrossVector = v3Up^v3Direction;
			v3CrossVector.normalize();
			if (ea.getKey() == 'w' || ea.getKey() == 'W' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Up)//ǰ��
			{
				_eye += v3Direction * _speed;
			}
			if (ea.getKey() == 's' || ea.getKey() == 'S' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Down)//����
			{
				_eye -= v3Direction * _speed;
			}
			if (ea.getKey() == 'a' || ea.getKey() == 'A' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Left)//����
			{
				_eye += v3CrossVector * _speed;
			}
			if (ea.getKey() == 'd' || ea.getKey() == 'D' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Right)//����
			{
				_eye -= v3CrossVector * _speed;
			}
			if (ea.getKey() == '-' || ea.getKey() == '_' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L)//��10���ƶ��ٶ�
			{
				_speed /= 10.0;
				if (_speed < 1.0)
				{
					_speed = 1.0;
				}
			}
			if (ea.getKey() == '=' || ea.getKey() == '+' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Shift_L)//��10���ƶ��ٶ�
			{
				_speed *= 10.0;
				if (_speed > 100000.0)
				{
					_speed = 100000.0;
				}
			}

			if (ea.getKey() == 'h' || ea.getKey() == 'H')//�ڵ�ǰ��γ�ȣ���̬������1.�ӵ������ 2.ͷ��������
			{
				v3Eye = _eye;//ʹ�����ʵ��λ��
				osg::Vec3d v3EyeLonLat;
				_srs->transformFromWorld(v3Eye, v3EyeLonLat);
				//�Ȼ�ȡ��ǰλ�õľ�γ�ȣ��ٻ�ȡ��ǰ���ϣ�����
				osg::Matrix mRealAttitude;

				if (v3EyeLonLat.z() < 0)//v3EyeLonLat.z()���۵�ʵ�ʺ���
					v3EyeLonLat.z() = 100;//������0���µ�������������100��

				GeoPoint gEyeGeo(_srs, v3EyeLonLat.x(), v3EyeLonLat.y(), v3EyeLonLat.z());
				gEyeGeo.createLocalToWorld(mRealAttitude);

				osg::Vec3d v3HorizonUp;//ָ������
				gEyeGeo.createWorldUpVector(v3HorizonUp);

				_eye = mRealAttitude.getTrans();

				mRealAttitude.getLookAt(v3Eye, v3Center, v3Up);//��ȡ�µ�λ�ú���̬

				osg::Matrix mDeviationAttitude;//��λ��ƫ��0.00001γ�ȣ�Ϊ�˼�����������
				GeoPoint gDeviationEyeGeo(_srs, v3EyeLonLat.x(), v3EyeLonLat.y() + 0.00001, v3EyeLonLat.z());
				gDeviationEyeGeo.createLocalToWorld(mDeviationAttitude);
				osg::Vec3d v3DeviationNorthPoint = mDeviationAttitude.getTrans();
				osg::Vec3d v3NorthHeadUp = v3DeviationNorthPoint - v3Eye;
				v3NorthHeadUp.normalize();//ָ������

				//if (v3EyeLonLat.y() < 90.0 && v3EyeLonLat.y() > 0.0)//û�о���Ϊʲô��������ϰ�����Ҫ�෴����ʵ��ʹ��û����
				//{
				//    mRealAttitude.makeLookAt(osg::Vec3d(0,0,0), -v3HorizonUp, -v3NorthHeadUp);
				//}
				if (v3EyeLonLat.y() < 89.99999  && v3EyeLonLat.y() > -90.0)
				{
					mRealAttitude.makeLookAt(osg::Vec3d(0, 0, 0), -v3HorizonUp, v3NorthHeadUp);
				}
				_rotate = mRealAttitude.getRotate();
			}
		}break;
	default:
		return false;
	}
}

bool
	EarthWalkManipulator::established()
{
	if (_srs.valid() && _mapNode.valid() && _node.valid())
		return true;

	// lock down the observed node:
	osg::ref_ptr<osg::Node> safeNode;
	if (!_node.lock(safeNode))
		return false;

	// find a map node or fail:
	_mapNode = osgEarth::MapNode::findMapNode(safeNode.get());
	if (!_mapNode.valid())
		return false;

	// Cache the SRS.
	_srs = _mapNode->getMapSRS();
	return true;
}

void EarthWalkManipulator::addMouseEvent(const osgGA::GUIEventAdapter& ea)
{
	_ga_t1 = _ga_t0;
	_ga_t0 = &ea;
}

bool EarthWalkManipulator::calcMovement(const osgGA::GUIEventAdapter& ea)
{
	osg::Quat qat;
	osg::Matrix mat;
	_rotate.get(mat);
	osg::Vec3d eye, center, up;
	mat.getLookAt(eye, center, up);

	osg::Vec3d dirction = center - eye;
	dirction.normalize();
	up.normalize();
	osg::Vec3d cross = dirction^up;
	cross.normalize();

	double x1 = ea.getXnormalized();
	double y1 = ea.getYnormalized();

	osg::Vec3d deviation(0, 0, 0);
	if (x1 > 0.1)
	{
		deviation += cross * 0.001;
	}
	else if (x1 < -0.1)
	{
		deviation -= cross * 0.001;
	}
	if (y1 > 0.1)
	{
		deviation += up * 0.001;
	}
	else if (y1 < -0.1)
	{
		deviation -= up * 0.001;
	}

	mat = osg::Matrix::lookAt(eye, deviation + center, up);
	_rotate = mat.getRotate();

	return true;
}