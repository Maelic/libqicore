/**
* @author Aldebaran Robotics
* Aldebaran Robotics (c) 2012 All Rights Reserved
*/

#include <qicore-compat/model/contentmodel.hpp>
#include <qicore-compat/model/animationmodel.hpp>
#include <qicore-compat/model/behaviorsequencemodel.hpp>

#include "contentmodel_p.hpp"

namespace qi
{
  //----------------------------Private Class-------------------------------//
  ContentModelPrivate::ContentModelPrivate() :
    _type(),
    _path(),
    _checksum()
  {
  }

  ContentModelPrivate::ContentModelPrivate(int type, const std::string &path, const std::string &checksum) :
    _type(type),
    _path(path),
    _checksum(checksum)
  {
  }

  ContentModelPrivate::ContentModelPrivate(boost::shared_ptr<const AL::XmlElement> elt, const std::string &dir)
  {
    elt->getAttribute("type",     _type);
    elt->getAttribute("path",     _path);
    elt->getAttribute("checksum", _checksum);
    _dir = dir;
  }

  //-----------------------------Public Class-------------------------------//

  ContentModel::ContentModel(ContentType type, const std::string &path, const std::string &checksum) :
    _p(new ContentModelPrivate(type, path, checksum))
  {
  }

  ContentModel::ContentModel(boost::shared_ptr<const AL::XmlElement> elt, const std::string &dir) :
    _p(new ContentModelPrivate(elt, dir))
  {
  }

  ContentModel::~ContentModel()
  {
    delete _p;
  }

  int ContentModel::getType() const
  {
    return _p->_type;
  }

  const std::string& ContentModel::getPath() const
  {
    return _p->_path;
  }

  const std::string& ContentModel::getChecksum() const
  {
    return _p->_checksum;
  }

  AnimationModelPtr ContentModel::getAnimationModel()
  {
    if(_p->_type != ContentModel::ContentType_Animation)
      return AnimationModelPtr();

    AnimationModelPtr animation = AnimationModelPtr(new AnimationModel(_p->_dir + "/" + _p->_path));

    if(!animation->loadFromFile())
      return AnimationModelPtr();

    return animation;
  }

  BehaviorSequenceModelPtr ContentModel::getBehaviorSequenceModel()
  {
    if(_p->_type != ContentModel::ContentType_BehaviorSequence)
      return BehaviorSequenceModelPtr();

    BehaviorSequenceModelPtr behaviorSequence = BehaviorSequenceModelPtr(new BehaviorSequenceModel(_p->_dir + "/" + _p->_path));

    if(!behaviorSequence->loadFromFile())
      return BehaviorSequenceModelPtr();

    return behaviorSequence;
  }

  void ContentModel::setType(ContentType type)
  {
    _p->_type = type;
  }

  void ContentModel::setPath(const std::string& path)
  {
    _p->_path = path;
  }

  void ContentModel::setChecksum(const std::string& checksum)
  {
    _p->_checksum = checksum;
  }
}