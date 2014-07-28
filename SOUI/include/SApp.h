#pragma once
#include "core/ssingleton.h"
#include "unknown/obj-ref-impl.hpp"
#include "interface/render-i.h"
#include "interface/SScriptModule-i.h"
#include "interface/STranslator-i.h"

#include "res.mgr/SResProviderMgr.h"


#include "core/smsgloop.h"
#include "core/SWndFactoryMgr.h"
#include "core/SSkinFactoryMgr.h"


#define SOUI_VERSION    _T("0.0.0.1")

#define LOADXML(p1,p2,p3) SApplication::getSingleton().LoadXmlDocment(p1,p2,p3)
#define LOADIMAGE(p1,p2) SApplication::getSingleton().LoadImage(p1,p2)
#define GETRESPROVIDER    SApplication::getSingletonPtr()
#define GETRENDERFACTORY SApplication::getSingleton().GetRenderFactory()
#define TR(p1,p2)       SApplication::getSingleton().GetTranslator()->tr(p1,p2)
               
#define RT_UIDEF _T("UIDEF")
#define RT_LAYOUT _T("LAYOUT")
#define RT_XML _T("XML")

namespace SOUI
{

class SOUI_EXP SApplication :public SSingleton<SApplication>
                        ,public SWindowFactoryMgr
                        ,public SSkinFactoryMgr
                        ,public SResProviderMgr
                        ,public SMessageLoop
{
    friend class CSimpleWnd;
public:
    SApplication(IRenderFactory *pRendFactory,HINSTANCE hInst,LPCTSTR pszHostClassName=_T("SOUIHOST"));
    ~SApplication(void);

    IRenderFactory * GetRenderFactory();

    HINSTANCE GetInstance();

    LPCTSTR GetVersion();

    BOOL Init(LPCTSTR pszName ,LPCTSTR pszType=RT_UIDEF);

    BOOL SetMsgBoxTemplate(LPCTSTR pszXmlName,LPCTSTR pszType=RT_LAYOUT);

    BOOL LoadXmlDocment(pugi::xml_document & xmlDoc,LPCTSTR pszXmlName ,LPCTSTR pszType);

    IScriptModule * GetScriptModule();

    void SetScriptModule(IScriptModule *pScriptModule);
    
    ITranslator * GetTranslator();
    
    void SetTranslator(ITranslator * pTrans);
    
    int Run(HWND hMainWnd);
protected:
    void createSingletons();
    void destroySingletons();

    HINSTANCE m_hInst;
    CAutoRefPtr<IScriptModule>  m_pScriptModule;
    CAutoRefPtr<IRenderFactory> m_RenderFactory;
    CAutoRefPtr<ITranslator>    m_Translator;
};

}//namespace SOUI