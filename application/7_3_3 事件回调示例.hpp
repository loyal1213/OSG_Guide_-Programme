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

// 继承自 osg::NodeCallback 类， 写一个事件回调类
class MyeventCallback : public osg::NodeCallback{
public:
	virtual void operator()(osg::Node* node,osg::NodeVisitor* nv){
		// 判断访问器类型
		if (nv->getVisitorType() == osg::NodeVisitor::EVENT_VISITOR){
			// 创建一个事件访问器并初始化
			osg::ref_ptr<osgGA::EventVisitor> ev = dynamic_cast<osgGA::EventVisitor*>(nv);
			if (ev){
				// 得到执行动作
				osgGA::GUIActionAdapter *aa =  ev->getActionAdapter();
				// 得到事件队列
				osgGA::EventQueue::Events & events = ev->getEvents();
				for (osgGA::EventQueue::Events::iterator itr = events.begin();itr!=events.end();++itr){
					// 处理事件
					osgGA::Event* event = itr->get();

					handle( *event->asGUIEventAdapter(), *aa);


				}
			}

		}
	}

	/** Deprecated, Handle events, return true if handled, false otherwise. */
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {

		// 得到场景数据
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*>(viewer->getSceneData());
		static float x_step = 0.0f, y_step = 0.0f,z_step = 0.0f;
		switch(ea.getEventType()){
		case (osgGA::GUIEventAdapter::KEYDOWN):{
			
			


			// 按下 key_up 按键时，向 X 正方向平移一个单位
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Right){
				osg::Matrix matrix;
				x_step+=1.0f;
				matrix.makeTranslate(x_step, 0.0f, 0.0f);
				mt->setMatrix(matrix);
			}

			// 按下 key_down 按键时，向 X 负方向平移一个单位
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left){
				osg::Matrix matrix;
				x_step-=1.0f;
				matrix.makeTranslate(x_step, 0.0f, 0.0f);
				mt->setMatrix(matrix);
			}
			
			// 按下 key_up 按键时，向 Y 正方向平移一个单位
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Up){
				osg::Matrix matrix;
				y_step+=10.0f;
				matrix.makeTranslate(0.0f, y_step, 0.0f);
				mt->setMatrix(matrix);
			}

			// 按下 key_down 按键时，向 Y 负方向平移一个单位
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Down){
				osg::Matrix matrix;
				y_step-=10.0f;
				matrix.makeTranslate(0.0f, y_step, 0.0f);
				mt->setMatrix(matrix);
			}


			// 按下 key_up 按键时，向 Z 正方向平移一个单位
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up){
				osg::Matrix matrix;
				z_step+=1.0f;
				matrix.makeTranslate(0.0f, 0.0f, z_step);
				mt->setMatrix(matrix);
			}

			// 按下 key_down 按键时，向 Z 负方向平移一个单位
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

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	// 加载牛的模型，注意需要创建 osg::matrixTransform 矩阵变换对象
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	// 创建矩阵变换节点
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	mt->addChild(node.get());

	// 得到相机
	osg::ref_ptr<osg::Camera> camera = viewer->getCamera();

	// 设置相机事件回调
	camera->setEventCallback(new MyeventCallback());


	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(mt.get());

	viewer->setSceneData(mt.get());

	viewer->realize();
	viewer->run();
	return 0;
}




