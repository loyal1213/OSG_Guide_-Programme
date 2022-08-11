
/* -*-c++-*- OpenSceneGraph Cookbook
 * Chapter 8 Recipe 2
 * Author: Wang Rui <wangray84 at gmail dot com>
*/

#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>

#define COMPRESS_TEXTURE  // Comment this to disable compressing textures

float randomValue(float min, float max)
{
    return (min + (float)rand() / (RAND_MAX + 1.0f) * (max - min));
}

osg::Vec3 randomVector(float min, float max)
{
    return osg::Vec3(randomValue(min, max),
        randomValue(min, max),
        randomValue(min, max));
}

osg::Matrix randomMatrix(float min, float max)
{
    osg::Vec3 rot = randomVector(-osg::PI, osg::PI);
    osg::Vec3 pos = randomVector(min, max);
    return osg::Matrix::rotate(rot[0], osg::X_AXIS, rot[1], osg::Y_AXIS, rot[2], osg::Z_AXIS) *
        osg::Matrix::translate(pos);
}

osg::Image* createRandomImage(int width, int height)
{
    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);

    unsigned char* data = image->data();
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            *(data++) = randomValue(0.0f, 255.0f);
            *(data++) = randomValue(0.0f, 255.0f);
            *(data++) = randomValue(0.0f, 255.0f);
        }
    }
    return image.release();
}

osg::Node* createQuads(unsigned int cols, unsigned int rows)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    for (unsigned int y = 0; y < rows; ++y)
    {
        for (unsigned int x = 0; x < cols; ++x)
        {
            osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
            texture->setImage(createRandomImage(512, 512));
#ifdef COMPRESS_TEXTURE
            texture->setInternalFormatMode(osg::Texture2D::USE_S3TC_DXT1_COMPRESSION);
            texture->setUnRefImageDataAfterApply(true);
#endif

            osg::Vec3 center((float)x, 0.0f, (float)y);
            osg::ref_ptr<osg::Drawable> quad = osg::createTexturedQuadGeometry(
                center, osg::Vec3(0.9f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 0.9f));
            quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
            geode->addDrawable(quad.get());
        }
    }
    return geode.release();
}

int Test22_2(int argc, char** argv)
{
    osgViewer::Viewer viewer;
    viewer.setSceneData(createQuads(20, 20));
    viewer.addEventHandler(new osgViewer::StatsHandler);
    return viewer.run();
}


