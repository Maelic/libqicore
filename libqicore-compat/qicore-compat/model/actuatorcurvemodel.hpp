/**
* @author Aldebaran Robotics
* Aldebaran Robotics (c) 2012 All Rights Reserved
*/

#pragma once

#ifndef ACTUATORCURVE_H_
#define ACTUATORCURVE_H_

#include <alserial/alserial.h>
#include <qicore-compat/api.hpp>

namespace qi
{
  class ActuatorCurveModelPrivate;
  class KeyModel;

  class QICORECOMPAT_API ActuatorCurveModel {
  public:
    enum UnitType
    {
      UnitType_Undefined = -1,
      UnitType_Degree    =  0,
      UnitType_Percent   =  1
    };

    ActuatorCurveModel(const std::string &name,
                       const std::string &actuator,
                       bool recordable,
                       bool mute,
                       UnitType unit,
                       const std::list<boost::shared_ptr<KeyModel> > &keys = std::list<boost::shared_ptr<KeyModel> >());
    ActuatorCurveModel(boost::shared_ptr<const AL::XmlElement> elt);
    virtual ~ActuatorCurveModel();

    const std::string& getName() const;
    const std::string& getActuator() const;
    bool getRecordable() const;
    bool getMute() const;
    UnitType getUnit() const;
    const std::list<boost::shared_ptr<KeyModel> >& getKeys() const;

    bool isValid() const;

    void setName(const std::string &name);
    void setActuator(const std::string &actuator);
    void setRecordable(bool recordable);
    void setMute(bool mute);
    void setUnit(UnitType unit);

    void addKey(boost::shared_ptr<KeyModel> key);

  private:
    QI_DISALLOW_COPY_AND_ASSIGN(ActuatorCurveModel);
    ActuatorCurveModelPrivate* _p;
  };
  typedef boost::shared_ptr<ActuatorCurveModel> ActuatorCurveModelPtr;
}

#endif /* !ACTUATORCURVE_H_ */