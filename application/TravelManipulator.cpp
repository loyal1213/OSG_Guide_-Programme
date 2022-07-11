#include "stdafx.h"
#include "TravelManipulator.h"

TravelManipulator::TravelManipulator(void):m_fMoveSpeed(1.0f)
	,m_bLeftButtonDown(false), m_fpushX(0), m_fpushY(0), m_fAngle(2.5), m_bPeng(false){

		m_vPosition = osg::Vec3(-22.0f, -274.0f, 100.0f);
		m_vRotation = osg::Vec3(osg::PI_2, 0.0f, 0.0f);

}

TravelManipulator::~TravelManipulator(void)
{

}

// �����μ��뵽����֮��
TravelManipulator* TravelManipulator::TravelToScreen(osg::ref_ptr<osgViewer::Viewer> viewer){
	osg::ref_ptr<TravelManipulator> camera = new TravelManipulator();

	viewer->setCamera(dynamic_cast<osg::Camera*>(camera.get()));
	camera->m_pHostViewer = viewer;
	return camera;
}


void TravelManipulator::setByMatrix(const osg::Matrixd& matrix){

}

void TravelManipulator::setByInverseMatrix(const osg::Matrixd& matrix){

}

// �õ�����
osg::Matrixd TravelManipulator::getMatrix() const{
	osg::Matrixd mat;
	mat.makeRotate(m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f),
		m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f),
		m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f)				   
		);


	return mat*osg::Matrixd::translate(m_vPosition);

}

// �õ������
osg::Matrixd TravelManipulator::getInverseMatrix() const{
	osg::Matrixd mat;
	mat.makeRotate(m_vRotation._v[0],osg::Vec3(1.0f,0.0f,0.0f),
		m_vRotation._v[1],osg::Vec3(0.0f,1.0f,0.0f),
		m_vRotation._v[2],osg::Vec3(0.0f,0.0f,1.0f)
		);

	return mat*osg::Matrixd::inverse(mat*osg::Matrixd::translate(m_vPosition));
}

bool TravelManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us){
	// �õ����λ��
	float mouse_x = ea.getX(), mouse_y = ea.getY();
	switch(ea.getEventType()){
	case osgGA::GUIEventAdapter::KEYDOWN:
		{
			// �ո��
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Space){
				us.requestRedraw();
				us.requestContinuousUpdate(false);
			}

			// ���ƶ�
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Home){

				ChangePosition(osg::Vec3(0,0,m_fMoveSpeed));

				return true;
			}

			// ���ƶ�
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_End){

				ChangePosition(osg::Vec3(0, 0, -m_fMoveSpeed));

				return true;
			}

			// �����ٶ�
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Plus){
				m_fMoveSpeed += 1.0f;
				return true;
			}

			// �����ٶ�
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Minus){
				m_fMoveSpeed -= 1.0f;
				if (m_fMoveSpeed < 1.0f){
					m_fMoveSpeed = 1.0f;
				}
				return true;
			}

			// ǰ��
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up){
				ChangePosition(osg::Vec3(0, m_fMoveSpeed*sinf(osg::PI_2 + m_vRotation._v[2]), 0));
				ChangePosition(osg::Vec3(m_fMoveSpeed*cosf(osg::PI_2 + m_vRotation._v[2]), 0, 0));
				return true;
			}

			// ����
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down){
				ChangePosition(osg::Vec3(0, -m_fMoveSpeed*sinf(osg::PI_2 +m_vRotation._v[2]), 0));
				ChangePosition(osg::Vec3(-m_fMoveSpeed*cosf(osg::PI_2 + m_vRotation._v[2]), 0, 0));
				return true;
			}

			// ��
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left){
				ChangePosition(osg::Vec3(0, -m_fMoveSpeed*sinf(osg::PI_2 +m_vRotation._v[2]), 0));
				ChangePosition(osg::Vec3(-m_fMoveSpeed*cosf(osg::PI_2 + m_vRotation._v[2]), 0, 0));
				return true;
			}

			// ��
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Right){
				ChangePosition(osg::Vec3(0, -m_fMoveSpeed*sinf(osg::PI_2 +m_vRotation._v[2]), 0));
				ChangePosition(osg::Vec3(-m_fMoveSpeed*cosf(osg::PI_2 + m_vRotation._v[2]), 0, 0));
				return true;
			}

			if (ea.getKey() == 0xff53){
				m_vRotation._v[2] -= osg::DegreesToRadians(m_fAngle);
			}

			if (ea.getKey() == 0xff51){
				m_vRotation._v[2] += osg::DegreesToRadians(m_fAngle);
			}

			if (ea.getKey() == 0x46){
				m_fAngle-=0.2;
				return true;
			}

			if (ea.getKey() == 0x47){
				m_fAngle+=0.2;
				return true;
			}

			return false;
		}
	case (osgGA::GUIEventAdapter::PUSH):
		{

			return false;
		}

	case (osgGA::GUIEventAdapter::DRAG):
		{

			return false;
		}
	case (osgGA::GUIEventAdapter::RELEASE):
		{

			return false;
		}

	default:
		return false;
	}

}

void TravelManipulator::ChangePosition(osg::Vec3& delta){
	// ��ײ���
	if (m_bPeng){
		// �õ��µ�λ��
		osg::Vec3 pos1 = m_vPosition + delta;

		osgUtil::IntersectVisitor ivXY;

		// �����µ�λ�õõ������߶μ��
		osg::ref_ptr<osg::LineSegment> line_xy = new osg::LineSegment(pos1,m_vPosition);

		osg::ref_ptr<osg::LineSegment> line_z = new osg::LineSegment();
		
		ivXY.addLineSegment(line_z.get());
		ivXY.addLineSegment(line_xy.get());

		// �ṹ�������
		m_pHostViewer->getSceneData()->accept(ivXY);

		// ���û����ײ���
		if (!ivXY.hits()){
			m_vPosition += delta;
		}
	}else{
	
		m_vPosition += delta;
	}

}

float TravelManipulator::getSpeed(){
	return m_fMoveSpeed;
}

void TravelManipulator::setSpeed(float &sp)
{
	m_fMoveSpeed = sp;
}

void TravelManipulator::SetPositon(osg::Vec3 &position){
	m_vPosition = position;
}

osg::Vec3 TravelManipulator::GetPosition(){
	return m_vPosition;
}
