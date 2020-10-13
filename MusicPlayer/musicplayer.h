#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMouseEvent>
#include <QMap>
#include "setting.h"

#define DEFAULT_VOL 50
namespace Ui {
class MusicPlayer;
}

class MusicPlayer : public QWidget
{
    Q_OBJECT
signals:
    void signalsetting(void);

private slots:
    void slotPositionChanged(qint64 pos);
    void slotDurationChanged(qint64 total);
    void slotCurrentIndexChanged(int index);
    void slot_get_filePathChanged(QString filePath);

public:
    explicit MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();
protected:
    ///用于界面移动的相关鼠标事件
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private slots:
    void on_pb_close_clicked();

    void on_pb_max_clicked();

    void on_pb_min_clicked();

    void on_hs_position_sliderMoved(int position);

    void on_hs_volume_sliderMoved(int position);

    void on_pb_play_clicked();

    void on_lw_songNameList_doubleClicked(const QModelIndex &index);

    void on_pb_next_clicked();

    void on_pb_prev_clicked();

    void on_pb_playMode_clicked();

    void on_pb_setting_clicked();

private:
    Ui::MusicPlayer *ui;
    QMediaPlayer *m_player ;
    int m_playMode;
    //显示歌词
    void getCurrentLyric(QString name);
    QStringList m_lyricShow;

    QMap<int , int> m_timeAndLyric;
    //显示歌单
    QMediaPlaylist *m_playlist;
    void initPlayList(void);
    QStringList m_songList;

    //初始化播放器
    void initPlayer(void);
    qint64 m_total;
    qint64 m_pos;
    void initWidgetUnits(void);
    ///定义一个QPoint对象用于界面移动
    QPoint m_widgetMove;

    ///////// 改变音乐文件路径
    QString m_songsFileNamePath;
    Setting *m_setting;
};

#endif // MUSICPLAYER_H
