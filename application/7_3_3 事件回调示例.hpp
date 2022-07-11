#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/EventVisitor>
#include <osgGA/EventQueue>

#include <osgUtil/Optimizer>
#include <iostream>
#include <osgGA/EventHandler>

// �̳��� osg::NodeCallback �࣬ дһ���¼��ص���
class MyeventCallback : public osg::NodeCallback{
public:
	virtual void operator()(osg::Node* node,osg::NodeVisitor* nv){
		// �жϷ���������
		if (nv->getVisitorType() == osg::NodeVisitor::EVENT_VISITOR){
			// ����һ���¼�����������ʼ��
			osg::ref_ptr<osgGA::EventVisitor> ev = dynamic_cast<osgGA::EventVisitor*>(nv);
			if (ev){
				// �õ�ִ�ж���
				osgGA::GUIActionAdapter *aa =  ev->getActionAdapter();
				// �õ��¼�����
				osgGA::EventQueue::Events & events = ev->getEvents();
				for (osgGA::EventQueue::Events::iterator itr = events.begin();itr!=events.end();++itr){
					// �����¼�
					osgGA::Event* event = itr->get();

					handle( *event->asGUIEventAdapter(), *aa);


				}
			}

		}
	}

	/** Deprecated, Handle events, return true if handled, false otherwise. */
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {

		// �õ���������
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*>(viewer->getSceneData());
		static float x_step = 0.0f, y_step = 0.0f,z_step = 0.0f;
		switch(ea.getEventType()){
		case (osgGA::GUIEventAdapter::KEYDOWN):{
			
			


			// ���� key_up ����ʱ���� X ������ƽ��һ����λ
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Right){
				osg::Matrix matrix;
				x_step+=1.0f;
				matrix.makeTranslate(x_step, 0.0f, 0.0f);
				mt->setMatrix(matrix);
			}

			// ���� key_down ����ʱ���� X ������ƽ��һ����λ
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left){
				osg::Matrix matrix;
				x_step-=1.0f;
				matrix.makeTranslate(x_step, 0.0f, 0.0f);
				mt->setMatrix(matrix);
			}
			
			// ���� key_up ����ʱ���� Y ������ƽ��һ����λ
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Up){
				osg::Matrix matrix;
				y_step+=10.0f;
				matrix.makeTranslate(0.0f, y_step, 0.0f);
				mt->setMatrix(matrix);
			}

			// ���� key_down ����ʱ���� Y ������ƽ��һ����λ
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Down){
				osg::Matrix matrix;
				y_step-=10.0f;
				matrix.makeTranslate(0.0f, y_step, 0.0f);
				mt->setMatrix(matrix);
			}


			// ���� key_up ����ʱ���� Z ������ƽ��һ����λ
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up){
				osg::Matrix matrix;
				z_step+=1.0f;
				matrix.makeTranslate(0.0f, 0.0f, z_step);
				mt->setMatrix(matrix);
			}

			// ���� key_down ����ʱ���� Z ������ƽ��һ����λ
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down){
				osg::Matrix matrix;
				z_step-=1.0f;
				matrix.makeTranslate(0.0f, 0.0f, z_step);
				mt->setMatrix(matrix);
			}

			std::cout<<"x_step:"<<x_step<<std::endl;
			std::cout<<"y_step:"<<y_step<<std::endl;
			std::cout<<"z_step:"<<z_step<<std::endl;

			break;
		}
		default:
			break;
		}

		return false; 

	}


protected:
private:
};


int TestEventCallback(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	// ����ţ��ģ�ͣ�ע����Ҫ���� osg::matrixTransform ����任����
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	// ��������任�ڵ�
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	mt->addChild(node.get());

	// �õ����
	osg::ref_ptr<osg::Camera> camera = viewer->getCamera();

	// ��������¼��ص�
	camera->setEventCallback(new MyeventCallback());


	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(mt.get());

	viewer->setSceneData(mt.get());

	viewer->realize();
	viewer->run();
	return 0;
}




