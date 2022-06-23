#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osg/Timer>

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>

#include <osgGA/TrackballManipulator>


#include <iostream>

// ģ��һ���¼��� ��Ӧh
class PrintName : public osgGA::GUIEventHandler
{
public:
	virtual void getUsage(osg::ApplicationUsage& usage) const{
		usage.addKeyboardMouseBinding("ysx","Great.");
	};

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa){
		return false;
	}
protected:
private:
};

int test_osgviewer(){
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Node> node = new osg::Node(); 

	// ����һ����ʱ����
	osg::Timer  *timer = new osg::Timer();
	osg::Timer_t start_frame_time = 0, end_frame_time = 0;

	// ����֡��ʹ�õ�˯��ʱ��
	float sleep_time = 0.0f;

	// ÿ֡��ʵ��ʹ��ʱ��
	float current_time = 0.0f;
	
	
	// ����֡��
	int counts = 0;

	double start_time = 0.0f, end_time = 0.0f;

	// �õ�һ��tickֵΪ������
	std::cout<<timer->getSecondsPerTick() <<std::endl;

	// start_time = timer->tick();
	start_time = viewer->elapsedTime();
	timer->setStartTick();
	// ��ȡģ��
	node = osgDB::readNodeFile("cow.osg");
	std::cout<<"����һ. ��ȡģ��ʱ��Ϊ��"<< timer->time_s() << std::endl;

	// end_time = timer->tick();
	end_time = viewer->elapsedTime();
	// std::cout<<"������. ��ȡģ��ʱ��Ϊ��"<< timer->delta_s(start_time,end_time) << std::endl;
	std::cout<<"������. ��ȡģ��ʱ��Ϊ��"<< end_time - start_time<< std::endl;

	// ��Ӱ����¼�
	viewer->addEventHandler(new PrintName);
	viewer->addEventHandler(new osgViewer::HelpHandler);			
	viewer->addEventHandler(new osgViewer::StatsHandler);			// ��ʾ֡��
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);		// ��ʾȫ��
	viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);

	// ��Ӳ�����
	viewer->setCameraManipulator(new osgGA::TrackballManipulator());

	viewer->setSceneData(node.get());
	// return viewer->run();

	while(!viewer->done()){
		if (counts == 0){
			start_frame_time = timer->tick();
		}
		counts++;
		viewer->frame();
		OpenThreads::Thread::microSleep(sleep_time*1000000);

		if (counts == 3){
			// ����֡��Ϊ10��ÿ֡���� 0.1s
			counts = 0;
			end_frame_time = timer->tick();

			std::cout<< "��ǰ֡��Ϊ:" << 3 / timer->delta_s(start_frame_time,end_frame_time) <<std::endl;

			// �õ���ǰÿ֡��������ʱ��
			sleep_time = 0.1 - timer->delta_s(start_frame_time,end_frame_time)/3;
			if (sleep_time < 0){
				sleep_time = 0;
			}
		}
		
	}


	return 0;


}
