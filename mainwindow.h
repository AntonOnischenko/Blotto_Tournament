#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

namespace Ui {
class MainWindow;
}

class Player
{
private:
    std::vector<int> strategy;
    int winNum;
    int drawNum;
    int loseNum;
    float score;
public:
    friend bool operator<(const Player & p1, const Player & p2);
    void clear();
    void addWin();
    void addLose();
    void addDraw();
    void addWin(int wins);
    void addDraw(int draws);
    void addLose(int loses);
    void addScore(float t);
    Player(std::vector<int> s);
    std::vector<int> showStrategy();
    int wins();
    int draws();
    int loses();
    float scores();
};

class Tournament
{
private:
    int playersAmount; //количество участников турнира
    int warriorsAmount; // количество войнов, доступных игроку (в нашем турнире - по 100 войнов на рыло)
    int cellsAmount; // количество ячеек (в нашем турнире их 9)
    std::vector<Player> player;
    void clearScore(); // обнуляет счет всех игроков
public:
    Tournament(int wA, int cA);
    void sortPlayers();
    void saveFile(QString str);
    void openFile(QString str);
    void addPlayer(int arr[9]);
    void addPlayer(QString s);
    void addPlayer(std::vector<int> s); // добавляет нового игрока со стратегией s
    void deletePlayaer(int playerNum); // удаляет игрока под номером playerNum
    void startTournament();  // проводит турнир. Результаты записываются в файл
    void deleteAllPlayers(); // удаляет всех игроков
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    Tournament t;
};

#endif // MAINWINDOW_H
