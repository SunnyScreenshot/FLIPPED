#ifndef PREFMANAGE_H
#define PREFMANAGE_H

#include <map>
#include <QWidget>

namespace Ui {
class PrefManage;
}

class PrefManage : public QWidget
{
    Q_OBJECT

public:
    explicit PrefManage(QWidget *parent = nullptr);
    ~PrefManage();

private:
    inline bool cbState2Bool(int state);

private slots:

    // tabGeneral
    void onLanuage(const QString& language);
    void onFont();
    void onAutoRun(int sta);
//    void onAsAdmin(int sta);
    void onLogLevel(const QString& language);
    //void onAutoCheck(int sta);
    //void onUpdate();

//    // tabInterface
//    void onBorderStyle(const QString& style);
//    void onBorderColor(const QColor& color);
//    void onBorderWidth(int val);
//    void onCrosshairColor(const QColor& color);
//    void onCrosshairWidth(int val);
//    void onSmartWindow(int val);
//    void onShowCrosshair(int val);
//    void onShowCursor(int val);
//    void onAutoCopyToClip(int val);

//    // tabOutput
//    void onImageQuailty(int val);
//    void onFileName(const QString& name);
//    void onConfigPath(const QString& path);
//    void onQuickSave(int val);
//    void onAutoSave(int val);
//    void onQuickSavePath(const QString& path);
//    void onAutoSavePath(const QString& path);
//    void onChoosePath();

//    // tabPin
//    void onWindowShadow(int sta);
//    void onOpacity(int val);
//    void onMaxSize(int val);

    // about
    void on_generalReset_released();
    void on_interfaceReset_released();
    void on_outputReset_released();
    void on_pinReset_released();
    void on_hotkeysReset_released();

private:
    void initUIGeneral();
    void initUIInterface();
    void initUIOutput();
    void initUIPin();
    void initUIHotkeys();
    void initUIAbout();
    void initUI();

private:
    Ui::PrefManage *ui;
    std::map<const QString, const QString> m_lanuages;


};

#endif // PREFMANAGE_H
