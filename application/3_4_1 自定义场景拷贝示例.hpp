#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geometry>

#include <osg/Image>
#include <osg/Geode>
#include <osg/Group>

#include <osg/Notify>
#include <osg/Texture>
#include <osg/StateSet>
#include <osg/StateAttribute>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <iostream>

class MyCopyOp:public osg::CopyOp
{
public:
	inline MyCopyOp(CopyFlags flags=SHALLOW_COPY):osg::CopyOp(flags),blank_(2),number_(5){}

	virtual ~MyCopyOp() {}


	// 内联函数用于控制输出信息格式，实现下三角规则输出
	inline void moveIn() const{blank_+=number_;}

	inline void moveOut() const{blank_ -= number_;}
	
	inline void writeIndent() const{
		for (int i = 0; i< blank_;++i){
			std::cout<<"";
		}
	}

	// 引用计数器 copy 
	virtual osg::Referenced* operator()(const osg::Referenced* ref) const{
		writeIndent();
		std::cout<<"copying Referenced "<<ref<<std::endl;
		moveIn();
		osg::Referenced * ret_ref  =  CopyOp::operator()(ref);
		moveOut();
		return ret_ref;
	}

	// 对象 copy 
	virtual osg::Object* operator()(const osg::Object* obj) const{
		writeIndent();
		std::cout<<"copying obj "<<obj<<std::endl;
		if (obj){
			std::cout<<"<obj.className() "<<obj->className();
		}
		std::cout<<std::endl;
		moveIn();
		osg::Object * ret_obj  =  CopyOp::operator()(obj);
		moveOut();
		return ret_obj;
	}


	// 节点 copy 
	virtual osg::Node* operator()(const osg::Node* node) const{
		writeIndent();
		std::cout<<"copying node "<<node<<std::endl;
		if (node){
			std::cout<<"node.className() "<<node->className();
		}
		std::cout<<std::endl;
		moveIn();
		osg::Node * ret_node  =  CopyOp::operator()(node);
		moveOut();
		return ret_node;
	}

	// Drawable copy 
	virtual osg::Drawable* operator()(const osg::Drawable* drawable) const{
		writeIndent();
		std::cout<<"copying drawable "<<drawable<<std::endl;
		if (drawable){
			std::cout<<"drawable->className() "<<drawable->className();
		}
		std::cout<<std::endl;
		moveIn();
		osg::Drawable * ret_drawable  =  CopyOp::operator()(drawable);
		moveOut();
		return ret_drawable;
	}


	// 状态集 copy 
	virtual osg::StateSet* operator()(const osg::StateSet* state_set) const{
		writeIndent();
		std::cout<<"copying node "<<state_set<<std::endl;
		if (state_set){
			std::cout<<"node.className() "<<state_set->className();
		}
		std::cout<<std::endl;
		moveIn();
		osg::StateSet * ret_state_set  =  CopyOp::operator()(state_set);
		moveOut();
		return ret_state_set;
	}

	// 状态属性 copy 
	virtual osg::StateAttribute* operator()(const osg::StateAttribute* state_attribute) const{
		writeIndent();
		std::cout<<"copying node "<<state_attribute<<std::endl;
		if (state_attribute){
			std::cout<<"node.className() "<<state_attribute->className();
		}
		std::cout<<std::endl;
		moveIn();
		osg::StateAttribute * ret_state_attribute  =  CopyOp::operator()(state_attribute);
		moveOut();
		return ret_state_attribute;
	}

	// 纹理信息 copy 
	virtual osg::Texture* operator()(const osg::Texture* texture) const{
		writeIndent();
		std::cout<<"copying node "<<texture<<std::endl;
		if (texture){
			std::cout<<"node.className() "<<texture->className();
		}
		std::cout<<std::endl;
		moveIn();
		osg::Texture * ret_texture  =  CopyOp::operator()(texture);
		moveOut();
		return ret_texture;
	}

	// 贴图 copy 
	virtual osg::Image* operator()(const osg::Image* image) const{
		writeIndent();
		std::cout<<"copying node "<<image<<std::endl;
		if (image){
			std::cout<<"node.className() "<<image->className();
		}
		std::cout<<std::endl;
		moveIn();
		osg::Image * ret_image  =  CopyOp::operator()(image);
		moveOut();
		return ret_image;
	}


protected:
	// 此处的两个变量必须是可变型变量，因为在 const 函数中使用，需要突破 const 的限制
	// 空格增减变量
	mutable int number_;
	mutable int blank_;
private:

};

void TestCopyOp(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");


	// 浅拷贝一个场景
	osg::ref_ptr<osg::Node> shallowCopy = dynamic_cast<osg::Node*>(node->clone(osg::CopyOp::SHALLOW_COPY));
	std::cout<<std::endl<<"完成浅拷贝一个场景"<<std::endl<<std::endl;

	// 深拷贝一个场景
	osg::ref_ptr<osg::Node> deepCopy = dynamic_cast<osg::Node*>(node->clone(osg::CopyOp::DEEP_COPY_ALL));
	std::cout<<std::endl<<"完成深拷贝一个场景"<<std::endl<<std::endl;
	
	// 自定义输出信息浅拷贝场景
	osg::ref_ptr<osg::Node> myShallowCopy = dynamic_cast<osg::Node*>(node->clone(MyCopyOp::Options(osg::CopyOp::SHALLOW_COPY)));
	std::cout<<std::endl<<"完成一个自定义输出信息浅拷贝场景"<<std::endl<<std::endl;


	// 自定义输出信息深拷贝场景
	osg::ref_ptr<osg::Node> myDeepCopy = dynamic_cast<osg::Node*>(node->clone(MyCopyOp::Options(osg::CopyOp::DEEP_COPY_ALL)));
	std::cout<<std::endl<<"完成一个自定义输出信息深拷贝场景"<<std::endl<<std::endl;


	root->addChild(node.get());

	// root->addChild(osgDB::readNodeFile("lz.osg"));

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

}