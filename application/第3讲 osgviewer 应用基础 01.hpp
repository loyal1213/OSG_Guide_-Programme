#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osg/Timer>

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>

#include <osgGA/TrackballManipulator>


#include <iostream>

// 模拟一个事件类 响应h
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

	// 申请一个定时器类
	osg::Timer  *timer = new osg::Timer();
	osg::Timer_t start_frame_time = 0, end_frame_time = 0;

	// 控制帧速使用的睡眠时间
	float sleep_time = 0.0f;

	// 每帧的实际使用时间
	float current_time = 0.0f;
	
	
	// 计算帧速
	int counts = 0;

	double start_time = 0.0f, end_time = 0.0f;

	// 得到一个tick值为多少秒
	std::cout<<timer->getSecondsPerTick() <<std::endl;

	// start_time = timer->tick();
	start_time = viewer->elapsedTime();
	timer->setStartTick();
	// 读取模型
	node = osgDB::readNodeFile("cow.osg");
	std::cout<<"方案一. 读取模型时间为："<< timer->time_s() << std::endl;

	// end_time = timer->tick();
	end_time = viewer->elapsedTime();
	// std::cout<<"方案二. 读取模型时间为："<< timer->delta_s(start_time,end_time) << std::endl;
	std::cout<<"方案二. 读取模型时间为："<< end_time - start_time<< std::endl;

	// 添加帮助事件
	viewer->addEventHandler(new PrintName);
	viewer->addEventHandler(new osgViewer::HelpHandler);			
	viewer->addEventHandler(new osgViewer::StatsHandler);			// 显示帧速
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);		// 显示全屏
	viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);

	// 添加操作器
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
			// 限制帧速为10，每帧绘制 0.1s
			counts = 0;
			end_frame_time = timer->tick();

			std::cout<< "当前帧速为:" << 3 / timer->delta_s(start_frame_time,end_frame_time) <<std::endl;

			// 得到当前每帧绘制所用时间
			sleep_time = 0.1 - timer->delta_s(start_frame_time,end_frame_time)/3;
			if (sleep_time < 0){
				sleep_time = 0;
			}
		}
		
	}


	return 0;


}
