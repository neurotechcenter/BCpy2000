//  DCHelpers.cpp
#include "DCHelpers.h"

bool bContains(unsigned int * list, unsigned int n, unsigned int findme)
{
    unsigned int i;
    for (i=0;i<n;i++)
        if (list[i]==findme)
            return 1;
    return 0;
}

//  This function removes the COM error and displays it in a Mesage Box.
void dump_daqcom_error(_com_error &e)
{
    USES_CONVERSION;

    // check if an Automation error is set
    IErrorInfo* pei = NULL;
    HRESULT hr = GetErrorInfo(0, &pei);

    if ( pei && hr==S_OK)
    {

        CComBSTR desc;
        CComBSTR bstrDescription;
        pei->GetDescription(&desc.m_str);
        pei->Release();
        MessageBox(NULL, OLE2T(desc), "DaqCOM error", MB_OK|MB_ICONEXCLAMATION);
        //if no error message looking in the com error object
        if(desc.Length()==0){
            //if no error in description try ErrorMessage
            bstrDescription.m_str = e.Description();
            if(bstrDescription.Length()==0){
                CString str=e.ErrorMessage();
                bstrDescription.m_str=str.AllocSysString();
            }
        }
        SetErrorInfo(0, NULL);
    }
    // try to find system-provided message
    else
    {
        CComBSTR bstrDescription;
        bstrDescription.m_str = e.Description();
        if(bstrDescription.Length()==0){
            CString str=e.ErrorMessage();
            bstrDescription.m_str=str.AllocSysString();
        }

        MessageBox(NULL, OLE2T(bstrDescription), "DaqCOM error", MB_OK|MB_ICONSTOP);
    }
    SetLastError(0);
}

bool DoEvents()
{
    MSG msg;
    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    { 
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        return TRUE;
    }
    else
        return FALSE;
}
