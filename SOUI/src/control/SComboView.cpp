#include "souistd.h"
#include "control\SComboView.h"
#include <algorithm>

namespace SOUI
{
    SComboView::SComboView(void)
        :m_pListBox(NULL)
    {
    }

    SComboView::~SComboView(void)
    {
        if(m_pListBox)
        {
            m_pListBox->SSendMessage(WM_DESTROY);
            delete m_pListBox;
        }
    }

    BOOL SComboView::CreateListBox(pugi::xml_node xmlNode)
    {
        SASSERT(xmlNode);
        //创建列表控件
        m_pListBox=(SListView*)SApplication::getSingleton().CreateWindowByName(SListView::GetClassName());
        m_pListBox->SetContainer(GetContainer());

        m_pListBox->InitFromXml(xmlNode.child(L"liststyle"));
        m_pListBox->SetAttribute(L"pos", L"0,0,-0,-0", TRUE);
        m_pListBox->SetAttribute(L"hotTrack",L"1",TRUE);
        m_pListBox->SetOwner(this);    //chain notify message to combobox
        m_pListBox->SetID(IDC_DROPDOWN_LIST);

        return TRUE;
    }

    int SComboView::GetListBoxHeight()
    {
        int nDropHeight=m_nDropHeight;
        if(GetCount()) 
        {
            IListViewItemLocator * pItemLocator = m_pListBox->GetItemLocator();
            SASSERT(pItemLocator);
			CRect rcMargin = m_pListBox->GetStyle().GetMargin();
            nDropHeight = (std::min)(nDropHeight,(int)(pItemLocator->GetTotalHeight()+rcMargin.top + rcMargin.bottom));
        }
        return nDropHeight;    
    }

    void SComboView::OnCreateDropDown( SDropDownWnd *pDropDown)
    {
        __super::OnCreateDropDown(pDropDown);
        pDropDown->InsertChild(m_pListBox);
		m_pListBox->SDispatchMessage(UM_SETLANGUAGE,0,0);
        pDropDown->UpdateChildrenPosition();

        m_pListBox->SetVisible(TRUE);
        m_pListBox->SetFocus();
        m_pListBox->EnsureVisible(GetCurSel());
    }

    void SComboView::OnDestroyDropDown( SDropDownWnd *pDropDown)
    {
        pDropDown->RemoveChild(m_pListBox);
        m_pListBox->SetVisible(FALSE);
        m_pListBox->SetContainer(GetContainer());
        __super::OnDestroyDropDown(pDropDown);
    }

    void SComboView::OnSelChanged()
    {
        m_pListBox->GetSel();
        if(m_pEdit && !m_pEdit->GetEventSet()->isMuted())
        {
            SStringT strText=GetLBText(m_pListBox->GetSel());
            m_pEdit->GetEventSet()->setMutedState(true);
            m_pEdit->SetWindowText(strText);
            m_pEdit->GetEventSet()->setMutedState(false);
        }
        Invalidate();
        __super::OnSelChanged();
    }

    BOOL SComboView::FireEvent( EventArgs &evt )
    {
        if(evt.idFrom == IDC_DROPDOWN_LIST && m_pDropDownWnd)
        {
            if(evt.GetID()==EventLVSelChanged::EventID)
            {
                OnSelChanged();
                return TRUE;
            }
            if(evt.GetID() == EventCmd::EventID)
            {
                CloseUp();
                return TRUE;
            }
        }
        return SComboBase::FireEvent(evt);
    }

    SListView * SComboView::GetListView()
    {
        return m_pListBox;
    }

    SStringT SComboView::GetLBText(int iItem)
    {
        ILvAdapter *pAdapter = m_pListBox->GetAdapter();
        if(!pAdapter || iItem == -1) return SStringT();
		return TR(pAdapter->getItemDesc(iItem),this->SComboBase::m_TrCtx);
    }

    int SComboView::GetCount() const
    {
        ILvAdapter *pAdapter = m_pListBox->GetAdapter();
        if(!pAdapter) return 0;
        return pAdapter->getCount();
    }

    int SComboView::GetCurSel() const
    {
        return m_pListBox->GetSel();
    }

    BOOL SComboView::SetCurSel(int iSel)
    {
        if(m_pListBox->GetSel()==iSel)
            return FALSE;
        m_pListBox->SetSel(iSel);
        OnSelChanged();
        return TRUE;
    }

}
