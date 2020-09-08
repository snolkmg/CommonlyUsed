#include "QtExceL.h"
#include <ole2.h>

QtExcel::QtExcel()
{
    m_pExcel = NULL;
    m_pWorkbooks = NULL;
    m_pWorkbook = NULL;
    m_pWorksheet = NULL;
    m_strXlsFile = "";
    m_nRowCount = 0;
    m_nColumnCount = 0;
    m_nStartRow = 0;
    m_nStartColumn = 0;
    m_bIsOpen = false;
    m_bIsValid = false;
    m_bIsANewFile = false;
    m_bIsSaveAlready = false;
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
}

QtExcel::QtExcel(QString xlsFile)
{
    m_pExcel = NULL;
    m_pWorkbooks = NULL;
    m_pWorkbook = NULL;
    m_pWorksheet = NULL;
    m_strXlsFile = xlsFile;
    m_nRowCount = 0;
    m_nColumnCount = 0;
    m_nStartRow = 0;
    m_nStartColumn = 0;
    m_bIsOpen = false;
    m_bIsValid = false;
    m_bIsANewFile = false;
    m_bIsSaveAlready = false;
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
}
QtExcel::~QtExcel()
{
    if (m_bIsOpen)
    {
        //析构前，先保存数据，然后关闭workbook
        Close();
    }
    OleUninitialize();
}

bool QtExcel::Open(UINT nSheet, bool visible)
{
    if (m_bIsOpen)
    {
        Close();
    }
    m_nCurrSheet = nSheet;
    m_bIsVisible = visible;
    if (NULL == m_pExcel)
    {
        m_pExcel = new QAxObject("Excel.Application");
        if (m_pExcel)
        {
            m_bIsValid = true;
        }
        else
        {
            m_bIsValid = false;
            m_bIsOpen = false;
            return m_bIsOpen;
        }
        m_pExcel->dynamicCall("SetVisible(bool)", m_bIsVisible);
    }
    if (!m_bIsValid)
    {
        m_bIsOpen = false;
        return m_bIsOpen;
    }

    if (m_strXlsFile.isEmpty())
    {
        m_bIsOpen = false;
        return m_bIsOpen;
    }
    /*如果指向的文件不存在，则需要新建一个*/
    QFile f(m_strXlsFile);
    if (!f.exists())
    {
        m_bIsANewFile = true;
    }
    else
    {
        m_bIsANewFile = false;
    }

    if (!m_bIsANewFile)
    {
        m_pWorkbooks = m_pExcel->querySubObject("WorkBooks"); //获取工作簿
        m_pWorkbook = m_pWorkbooks->querySubObject("Open(QString, QVariant)", m_strXlsFile, QVariant(0)); //打开xls对应的工作簿
    }
    else
    {
        m_pWorkbooks = m_pExcel->querySubObject("WorkBooks");      //获取工作簿
        m_pWorkbooks->dynamicCall("Add");                        //添加一个新的工作薄
        m_pWorkbook = m_pExcel->querySubObject("ActiveWorkBook"); //新建一个xls
    }
    m_pWorksheet = m_pWorkbook->querySubObject("WorkSheets(int)", m_nCurrSheet);//打开第一个sheet
                                                                                //至此已打开，开始获取相应属性
    QAxObject *usedrange = m_pWorksheet->querySubObject("UsedRange");       //获取该sheet的使用范围对象
    QAxObject *rows = usedrange->querySubObject("Rows");
    QAxObject *columns = usedrange->querySubObject("Columns");

    //因为excel可以从任意行列填数据而不一定是从0,0开始，因此要获取首行列下标
    m_nStartRow = usedrange->property("Row").toInt();    //第一行的起始位置
    m_nStartColumn = usedrange->property("Column").toInt(); //第一列的起始位置
    m_nRowCount = rows->property("Count").toInt();       //获取行数
    m_nColumnCount = columns->property("Count").toInt();    //获取列数
    m_bIsOpen = true;
    return m_bIsOpen;
}

bool QtExcel::Open(QString xlsFile, UINT nSheet, bool visible)
{
    m_strXlsFile = xlsFile;
    m_nCurrSheet = nSheet;
    m_bIsVisible = visible;
    return Open(m_nCurrSheet, m_bIsVisible);
}


void QtExcel::Save()
{
    if (m_pWorkbook)
    {
        if (m_bIsSaveAlready)
        {
            return;
        }
        if (!m_bIsANewFile)
        {
            m_pWorkbook->dynamicCall("Save()");
        }
        else /*如果该文档是新建出来的，则使用另存为COM接口*/
        {
            m_pWorkbook->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                m_strXlsFile, 56, QString(""), QString(""), false, false);
        }
        m_bIsSaveAlready = true;
    }
}

void QtExcel::SaveAs(QString path, bool isXls)
{
    if (m_pWorkbook)
    {
        int XlFileFormat = isXls ? 56 : 51;
        if (m_bIsSaveAlready)
        {
            return;
        }
        else /*如果该文档是新建出来的，则使用另存为COM接口*/
        {
            m_pWorkbook->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                path, XlFileFormat, QString(""), QString(""), false, false);
        }
        m_bIsSaveAlready = true;
    }
}

void QtExcel::Close()
{
    Save();//关闭前先保存数据
    if (m_pExcel && m_pWorkbook)
    {
        m_pWorkbook->dynamicCall("Close(bool)", true);
        m_pExcel->dynamicCall("Quit()");
        delete m_pExcel;
        m_pExcel = NULL;
        m_bIsOpen = false;
        m_bIsValid = false;
        m_bIsANewFile = false;
        m_bIsSaveAlready = true;
    }
}

QVariant QtExcel::GetCellData(UINT row, UINT column)
{
    QVariant data;
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)", row, column);//获取单元格对象
    if (cell)
    {
        data = cell->dynamicCall("Value2()");
    }
    return data;
}

QVariantList QtExcel::GetCellData(QString strRange)
{
    QVariantList data;
    // strRange = "a1:C10"
    QAxObject *cell = m_pWorksheet->querySubObject("Range(QString)", strRange);//获取单元格对象
    if (cell)
    {
        data = cell->property("Value").toList();
    }
    return data;
}

bool QtExcel::SetCellData(UINT row, UINT column, QVariant data)
{
    bool op = false;
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)", row, column);//获取单元格对象
    if (cell)
    {
        QString strData = data.toString(); //excel 居然只能插入字符串和整型，浮点型无法插入
        cell->dynamicCall("SetValue(const QVariant&)", strData); //修改单元格的数据
        op = true;
    }
    else
    {
        op = false;
    }

    return op;
}

void QtExcel::Clear()
{
    m_strXlsFile = "";
    m_nRowCount = 0;
    m_nColumnCount = 0;
    m_nStartRow = 0;
    m_nStartColumn = 0;
}

bool QtExcel::IsOpen()
{
    return m_bIsOpen;
}
bool QtExcel::IsValid()
{
    return m_bIsValid;
}


