#include "musicplayer.h"
#include "ui_musicplayer.h"

#include <QMediaPlayer>
#include <QTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

MusicPlayer::MusicPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    ui->hs_volume->setValue(DEFAULT_VOL);
    m_setting = new Setting;//设置界面

    m_songsFileNamePath = "D:/day day up/UNIGRESS/UnigressTrain/UnigressMusic";
    this->initPlayList();
    this->initPlayer();
    this->initWidgetUnits();
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}
void MusicPlayer::slot_get_filePathChanged(QString filePath)/// 修改音乐文件路径
{
    m_songsFileNamePath = filePath;

    //initPlayList();
    m_playlist->clear();
    m_songList.clear();
    ui->lw_songNameList->clear();
    ui->lw_lyricShow->clear();



    QDir songs(m_songsFileNamePath);
    QFileInfoList song_list = songs.entryInfoList(QStringList() << "*mp3");//获取MP3文件路径
    for(int i = 0; i < song_list.length(); i++)
    {
        m_playlist->addMedia(QUrl::fromLocalFile(song_list.at(i).absoluteFilePath()));
        m_songList.append(song_list.at(i).baseName());
    }
    ui->lw_songNameList->addItems(m_songList);
    m_setting->hide();

}
////////////////////////////////////////初始化播放列表
void MusicPlayer::initPlayList(void)
{
    m_playlist = new QMediaPlaylist(this);

    QDir songs(m_songsFileNamePath);

    connect(m_playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentIndexChanged(int)));
    connect(m_setting, SIGNAL(signalsaveFilePath(QString)), this, SLOT(slot_get_filePathChanged(QString)));
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_playMode = QMediaPlaylist::Loop;

    QFileInfoList song_list = songs.entryInfoList(QStringList() << "*mp3");//获取MP3文件路径

    for(int i = 0; i < song_list.length(); i++)
    {
        //qDebug() << song_list.at(i).absoluteFilePath();
        m_playlist->addMedia(QUrl::fromLocalFile(song_list.at(i).absoluteFilePath()));
        m_songList.append(song_list.at(i).baseName());
    }
    ui->lw_songNameList->addItems(m_songList);
    //qDebug() << m_songList;

}


///////////////////////////////////初始化播放器
void MusicPlayer::initPlayer(void)
{
    m_player = new QMediaPlayer(this);
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(slotPositionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(slotDurationChanged(qint64)));
    m_player->setPlaylist(m_playlist);

    m_player->setVolume(DEFAULT_VOL);
    //m_player->play();
    m_pos = 0;
    m_total = 0;
}

////////////////////////////////////////////////////////////////////////////////////
void MusicPlayer::slotCurrentIndexChanged(int index)
{
    //qDebug() << index;
    if(index < 0)
    {
//        index = m_songList.length() - 1;
//        m_playlist->setCurrentIndex(index);

    }else
    {
        ui->lb_songName->setText(m_songList.at(index));
        ui->lw_songNameList->setCurrentRow(index);
    }
    m_lyricShow.clear();
    ui->lw_lyricShow->clear();
    m_timeAndLyric.clear();
    getCurrentLyric(m_songList.at(index));

    ui->lw_lyricShow->addItems(m_lyricShow);
    for(int i = 0; i < m_lyricShow.size(); i++)
    {
        ui->lw_lyricShow->item(i)->setTextAlignment(Qt::AlignCenter);
    }

}
void MusicPlayer::getCurrentLyric(QString name)
{
    int index = 0;
    QString current_file_name = "D:/day day up/UNIGRESS/UnigressTrain/UnigressMusic/"
                                                 + name +".lrc";
    QStringList minuteAndSecond;
    //qDebug() << current_file_name;
    QFile lyric_file(current_file_name);
    if(lyric_file.open(QIODevice::ReadOnly))
    {
        QTextStream lyric_stream(&lyric_file);  //将歌词文件转化成文本流
        QStringList TimeAndLyric;
        while(!lyric_stream.atEnd())
        {
            //qDebug() << lyric_stream.readLine();
            TimeAndLyric = lyric_stream.readLine().split("]");

            if(!TimeAndLyric.at(1).isEmpty())
            {
                 m_lyricShow.append(TimeAndLyric.at(1));
            }
            //qDebug() << TimeAndLyric[0].remove("[");
            minuteAndSecond = TimeAndLyric[0].remove("[").split(":");
            //qDebug() << minuteAndSecond;
            int time = minuteAndSecond.at(0).toInt() * 60 + minuteAndSecond.at(1).toDouble();
            //qDebug() << "time : " << time << "index: " << index;
            if(time == 0)
            {
                index = -1;
            }
            m_timeAndLyric.insert(time, index++);
            

        }
        lyric_file.close();
    }
}
void MusicPlayer::slotPositionChanged(qint64 pos)// 当前位置
{

    QTime t_song((m_total/3600)%60, (m_total / 60) % 60, m_total%60); //总时长

    m_pos = pos/1000;
    QTime t_pos((m_pos/3600)%60, (m_pos / 60) % 60, m_pos%60);
    //qDebug() << t_pos.toString("mm:ss");
    ui->lb_position->setText(t_pos.toString("mm:ss") + "/" + t_song.toString("mm:ss"));
    ui->hs_position->setValue(m_pos);
    //歌词同步
    if(m_timeAndLyric.contains(m_pos))
    {
        //qDebug() << "pos: " << m_pos << "value" << m_timeAndLyric.value(m_pos);
        ui->lw_lyricShow->setCurrentRow(m_timeAndLyric.value(m_pos));
    }
}

void MusicPlayer::slotDurationChanged(qint64 total)//总时长
{
    m_total = total/1000;
    QTime t_song((m_total/3600)%60, (m_total / 60) % 60, m_total%60);
    ui->lb_position->setText("00:00/" + t_song.toString("mm:ss"));
    //qDebug() << t_song.toString("mm:ss");
    ui->hs_position->setMaximum(m_total);
}

void MusicPlayer::initWidgetUnits(void)
{
    //去除Widget界面边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->lw_lyricShow->setFocusPolicy(Qt::NoFocus);
    ui->lw_lyricShow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->lw_lyricShow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //消除列表边框
    ui->lw_songNameList->setFocusPolicy(Qt::NoFocus);
    ui->lw_songNameList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->lw_songNameList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MusicPlayer::on_pb_close_clicked()
{
    this->close();
}

void MusicPlayer::on_pb_max_clicked()
{
    if(this->isMaximized())
    {
    this->showNormal();
    }else{
    this->showMaximized();
    }
}

void MusicPlayer::on_pb_min_clicked()
{
    this->showMinimized();
}

//当鼠标左键被按下时，获取主窗口左上角的位置
void MusicPlayer::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton)
    {
        m_widgetMove = ev->globalPos() - this->frameGeometry().topLeft();

        ev->accept(); //鼠标事件被系统接收
    }
}
//当鼠标左键被按下时，移动主窗口
void MusicPlayer::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton)
    {
        this->move(ev->globalPos() - m_widgetMove);
        ev->accept();
    }
}

void MusicPlayer::on_hs_position_sliderMoved(int position)
{
    m_player->setPosition(position * 1000);
}

void MusicPlayer::on_hs_volume_sliderMoved(int position)
{
    m_player->setVolume(position);
}

void MusicPlayer::on_pb_play_clicked()
{
    if(m_player->state() == QMediaPlayer::PlayingState)
    {
        m_player->pause();
        ui->pb_play->setStyleSheet("border-image: url(:/images/skins/images/playStartNormal.png);");

    }else
    {
        m_player->play();
        ui->pb_play->setStyleSheet("border-image: url(:/images/skins/images/pausePressed.png);");
    }
}


//双击歌曲列表
void MusicPlayer::on_lw_songNameList_doubleClicked(const QModelIndex &index)
{
    if(m_player->state() != QMediaPlayer::PlayingState)
    {
        m_player->play();
        ui->pb_play->setStyleSheet("border-image: url(:/images/skins/images/pausePressed.png);");
    }
    m_playlist->setCurrentIndex(index.row());
}
//下一曲
void MusicPlayer::on_pb_next_clicked()
{
//    int cur_index = 0;
//    if(m_playlist->currentIndex() != (m_songList.length() - 1))
//    {
//        cur_index = m_playlist->currentIndex() + 1;
//    }

//    //qDebug() << cur_index;
//    m_playlist->setCurrentIndex(cur_index);
    m_playlist->next();
}
//上一曲
void MusicPlayer::on_pb_prev_clicked()
{
//    int cur_index = m_playlist->currentIndex() - 1;
//    m_playlist->setCurrentIndex(cur_index);
      m_playlist->previous();
}
//播放模式
void MusicPlayer::on_pb_playMode_clicked()
{
    m_playMode++;
    m_playMode %= 5;

    switch (m_playMode)
    {
    case QMediaPlaylist::CurrentItemOnce:
        m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        ui->pb_playMode->setStyleSheet("border-image: "
                                       "url(:/images/skins/images/currentItemOncePressed.png)");
        break;
    case QMediaPlaylist::CurrentItemInLoop:
        m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        ui->pb_playMode->setStyleSheet("border-image: "
                                       "url(:/images/skins/images/currentItemInLoopPressed.png)");
        break;
    case QMediaPlaylist::Sequential:
        m_playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        ui->pb_playMode->setStyleSheet("border-image: "
                                       "url(:/images/skins/images/sequentialPressed.png)");
        break;
    case QMediaPlaylist::Loop:
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
        ui->pb_playMode->setStyleSheet("border-image: "
                                       "url(:/images/skins/images/loopPressed.png)");
        break;
    case QMediaPlaylist::Random:
        m_playlist->setPlaybackMode(QMediaPlaylist::Random);
        ui->pb_playMode->setStyleSheet("border-image: "
                                       "url(:/images/skins/images/randomPressed.png)");
        break;
    default: break;
}
}

void MusicPlayer::on_pb_setting_clicked()
{
    //emit signalsetting();
    m_setting->show();
//    set.get_musicFilePath();
}
