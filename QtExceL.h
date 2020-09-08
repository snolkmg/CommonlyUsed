#ifndef QTEXCEL_H
#define QTEXCEL_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QAxBase>
#include <QAxObject>

typedef unsigned int UINT;
class QtExcel
{
public:
	QtExcel();
	QtExcel(QString xlsFile);
	~QtExcel();
public:
	bool Open(UINT nSheet = 1, bool visible = false);          //打开xls文件
    bool Open(QString xlsFile, UINT nSheet = 1, bool visible = false);
    void Save();                                              //保存xls报表
    void SaveAs(QString path, bool isXls = true);             //另存为xls报表
	void Close();                                             //关闭xls报表
	QVariant GetCellData(UINT row, UINT column);               //获取指定单元数据
	QVariantList GetCellData(QString strRange);	//获取多个单元数据，写法为“A2:C60”
	bool SetCellData(UINT row, UINT column, QVariant data); //修改指定单元数据
	bool IsOpen();
	bool IsValid();
protected:
	void Clear();
private:
	QAxObject *m_pExcel;       //指向整个excel应用程序
	QAxObject *m_pWorkbooks;   //指向工作簿集,excel有很多工作簿
	QAxObject *m_pWorkbook;    //指向sXlsFile对应的工作簿
	QAxObject *m_pWorksheet;   //指向工作簿中的某个sheet表单
	QString   m_strXlsFile;      //xls文件路径
	UINT      m_nCurrSheet;    //当前打开的第几个sheet
	bool      m_bIsVisible;    //excel是否可见
	int       m_nRowCount;     //行数
	int       m_nColumnCount;  //列数
	int       m_nStartRow;     //开始有数据的行下标值
	int       m_nStartColumn;  //开始有数据的列下标值
	bool      m_bIsOpen;       //是否已打开
	bool      m_bIsValid;      //是否有效
	bool      m_bIsANewFile;   //是否是一个新建xls文件，用来区分打开的excel是已存在文件还是有本类新建的
	bool      m_bIsSaveAlready;//防止重复保存
};
#endif // QTEXCEL_H
