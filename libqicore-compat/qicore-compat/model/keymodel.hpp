/**
* @author Aldebaran Robotics
* Aldebaran Robotics (c) 2012 All Rights Reserved
*/

#pragma once

#ifndef KEY_H_
#define KEY_H_

#include <qicore-compat/api.hpp>
#include <alserial/alserial.h>

namespace qi
{
  class KeyModelPrivate;
  class TangentModel;

  class QICORECOMPAT_API KeyModel
  {
  public:
    KeyModel(int frame, float value, bool smooth, bool symmetrical);
    KeyModel(boost::shared_ptr<const AL::XmlElement> elt);
    virtual ~KeyModel();

    int   getFrame()       const;
    float getValue()       const;
    bool  getSmooth()      const;
    bool  getSymmetrical() const;

    const std::list<boost::shared_ptr<TangentModel> > &getTangents() const;

    bool isValid() const;

    void setFrame(int frame);
    void setValue(float value);
    void setSmooth(bool smooth);
    void setSymmetrical(bool symmetrical);

    void setTangents(boost::shared_ptr<TangentModel> tangent1, boost::shared_ptr<TangentModel> tangent2);

  private:
    QI_DISALLOW_COPY_AND_ASSIGN(KeyModel);
    KeyModelPrivate* _p;
  };
  typedef boost::shared_ptr<KeyModel> KeyModelPtr;
}

#endif /* !KEY_H_ */