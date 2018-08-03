#pragma once
namespace FL{
class IFairyLandApplication
{
public:
	IFairyLandApplication(){}
	virtual ~IFairyLandApplication(){}
public:
	virtual _bool Create()=0;
};
class IFairyLandWorker
{
public:
	IFairyLandWorker(){}
	virtual ~IFairyLandWorker(){}
public:
	virtual _bool Create(IFairyLandApplication* pApp)=0;
	virtual _void Release()=0;
};

}