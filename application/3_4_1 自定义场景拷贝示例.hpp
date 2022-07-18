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


	// �����������ڿ��������Ϣ��ʽ��ʵ�������ǹ������
	inline void moveIn() const{blank_+=number_;}

	inline void moveOut() const{blank_ -= number_;}
	
	inline void writeIndent() const{
		for (int i = 0; i< blank_;++i){
			std::cout<<"";
		}
	}

	// ���ü����� copy 
	virtual osg::Referenced* operator()(const osg::Referenced* ref) const{
		writeIndent();
		std::cout<<"copying Referenced "<<ref<<std::endl;
		moveIn();
		osg::Referenced * ret_ref  =  CopyOp::operator()(ref);
		moveOut();
		return ret_ref;
	}

	// ���� copy 
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


	// �ڵ� copy 
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


	// ״̬�� copy 
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

	// ״̬���� copy 
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

	// ������Ϣ copy 
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

	// ��ͼ copy 
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
	// �˴����������������ǿɱ��ͱ�������Ϊ�� const ������ʹ�ã���Ҫͻ�� const ������
	// �ո���������
	mutable int number_;
	mutable int blank_;
private:

};

void TestCopyOp(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");


	// ǳ����һ������
	osg::ref_ptr<osg::Node> shallowCopy = dynamic_cast<osg::Node*>(node->clone(osg::CopyOp::SHALLOW_COPY));
	std::cout<<std::endl<<"���ǳ����һ������"<<std::endl<<std::endl;

	// ���һ������
	osg::ref_ptr<osg::Node> deepCopy = dynamic_cast<osg::Node*>(node->clone(osg::CopyOp::DEEP_COPY_ALL));
	std::cout<<std::endl<<"������һ������"<<std::endl<<std::endl;
	
	// �Զ��������Ϣǳ��������
	osg::ref_ptr<osg::Node> myShallowCopy = dynamic_cast<osg::Node*>(node->clone(MyCopyOp::Options(osg::CopyOp::SHALLOW_COPY)));
	std::cout<<std::endl<<"���һ���Զ��������Ϣǳ��������"<<std::endl<<std::endl;


	// �Զ��������Ϣ�������
	osg::ref_ptr<osg::Node> myDeepCopy = dynamic_cast<osg::Node*>(node->clone(MyCopyOp::Options(osg::CopyOp::DEEP_COPY_ALL)));
	std::cout<<std::endl<<"���һ���Զ��������Ϣ�������"<<std::endl<<std::endl;


	root->addChild(node.get());

	// root->addChild(osgDB::readNodeFile("lz.osg"));

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

}