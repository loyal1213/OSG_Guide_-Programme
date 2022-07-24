#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgUtil/Optimizer>

#include <iostream>
#include <fstream>
#include <memory>

class ReadFileCB : public osgDB::Registry::ReadFileCallback{


};




class CRenderingThread : public OpenThreads::Thread
{
public:
	CRenderingThread(osgDB::ifstream* fin) :_fin(fin){
		fin->seekg(0, std::ifstream::end);
		_length = fin->tellg();
		fin->seekg(0, std::ifstream::beg);

	};

	virtual ~CRenderingThread() {};

	virtual void run(){
		int pos = _fin->tellg();
		while (pos < _length){
			pos = _fin->tellg();
			std::cout << 100.0*pos / _length<<"%%" << std::endl;
		}
	};

protected:
	osgDB::ifstream* _fin;
	int _length;
};



void TestProgress(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	std::string filename = "cow.osg";

	// 搜索能否找到读取该文件的插件
	osg::ref_ptr<osgDB::ReaderWriter> rw = osgDB::Registry::instance()->getReaderWriterForExtension(osgDB::getLowerCaseFileExtension(filename));
	if (!rw.get()){ 
		std::cout<<"not find specified file!" <<std::endl;
		return ;
	}

	osgDB::ifstream istream(filename.c_str(), std::ios::in | std::ios::binary);
	CRenderingThread crt(&istream);
	crt.startThread();
	if (istream){
		std::cout << "Using default format readerwriter" << std::endl;

		osgDB::ReaderWriter::ReadResult rr = rw->readNode(istream);
		while (crt.isRunning()) {}
		
	}




	root->addChild(osgDB::readNodeFile("glider.osg"));

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

}