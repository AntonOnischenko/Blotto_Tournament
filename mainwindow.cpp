#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QTextStream>
#include <QLabel>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), t(100,9)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Tournament::Tournament(int wA, int cA): playersAmount(0), warriorsAmount(wA), cellsAmount(cA) // Обычный конструктор
{ }


void Tournament::addPlayer(std::vector<int> s)
{
    if (s.size() == cellsAmount)  // проверка количества полей
    {
        int sum = 0;
        for (int i = 0; i < cellsAmount; i++)
        {
            sum += s[i];
        }
        if (sum == warriorsAmount)  // проверка количества воинов
        {
            player.push_back(Player(s));
            playersAmount++;
        } else
        {
            QStringList st;
            QString str = "Неверное число воинов в стратегии:\n";
            for (int n = 0 ; n < cellsAmount ; n++){
                st.append(QString::number(s[n]));
            }
            str.append(st.join(","));
            str.append("\n");
            str.append(QString::number(sum));
            str.append("/100");
            QMessageBox msgBox;
            msgBox.setText(str);
            msgBox.exec();
        }
    } else
    {
        QMessageBox msgBox;
         msgBox.setText("Неверное число полей игрока");
         msgBox.exec();
    }
}

void Tournament::addPlayer(int arr[9])
{
    std::vector<int> s;
    for (int i = 0; i < 9; i++)
    {
        s.push_back(arr[i]);
    }
    addPlayer(s);
}

void Tournament::addPlayer(QString str)
{
    str.remove("\"");
    str.replace("\t",", ");
    QStringList slist = str.split(QRegExp("[, ]+"));
    if (slist.size() >= cellsAmount)
    {
        std::vector<int> s;
        for (int i = 0; i < cellsAmount; i++)
        {
            s.push_back(slist[i].toInt());
        }
        Tournament::addPlayer(s);
    } else
    {
        QMessageBox msgBox;
        msgBox.setText("Неверное число полей игрока");
        msgBox.exec();
    }
}

void Tournament::openFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;
    QTextStream out(&file);
    while(!out.atEnd()){
        QString str(out.readLine());
        Tournament::addPlayer(str);
    }
    file.close();
}

void Tournament::saveFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
    QTextStream out(&file);
    if (fileName.right(3) == "csv")
    {
    for (int i = 0; i < playersAmount; i++)
    {
        QString st;
        st.append("\"");
        for (int n = 0 ; n < cellsAmount - 1 ; n++){
            st.append(QString::number(player[i].showStrategy()[n]));
            st.append(", ");
        }
        st.append(QString::number(player[i].showStrategy()[cellsAmount-1]));
        st.append("\",");
        st.append(QString::number(player[i].wins()));
        st.append(",");
        st.append(QString::number(player[i].draws()));
        st.append(",");
        st.append(QString::number(player[i].loses()));
        st.append(",");
        st.append(QString::number(player[i].scores()));
        st.append("\n");
        out << st;
    }
    } else
    for (int i = 0; i < playersAmount; i++)
    {
        QString st;
        for (int n = 0 ; n < cellsAmount ; n++){
            st.append(QString::number(player[i].showStrategy()[n]));
            st.append("\t");
        }
        st.append("\t");
        st.append(QString::number(player[i].wins()));
        st.append("\t");
        st.append(QString::number(player[i].draws()));
        st.append("\t");
        st.append(QString::number(player[i].loses()));
        st.append("\t");
        st.append(QString::number(player[i].scores()));
        st.append("\n");
        out << st;
    }

    file.close();
}

void Player::clear()
{
    winNum = 0;
    drawNum = 0;
    loseNum = 0;
    score = 0;
}

void Tournament::clearScore()
{
    for (int i=0; i < playersAmount; i++)
    {
        player[i].clear();
    }
}

void Tournament::startTournament()
{
    if (playersAmount < 2)
    {
        QMessageBox msgBox;
         msgBox.setText("Недостаточно игроков для проведения турнира");
         msgBox.exec();
    } else
    {
        clearScore();

        int arrDraws[cellsAmount][warriorsAmount+1];
        int arrWins[cellsAmount][warriorsAmount+1];
        for (int i = 0; i < cellsAmount; i++){
            for (int j = 0; j < warriorsAmount+1; j++)
            {
                arrDraws[i][j] = 0;
                arrWins[i][j] = 0;
            }
        }
        for (int i = 0; i < playersAmount; i++){ // массив показывает количество игроков сыгравших определенную стратегию
            for (int j = 0; j < cellsAmount; j++)
            {
                arrDraws[j][player[i].showStrategy()[j]] += 1;
            }
        }
        for (int i = 0; i < cellsAmount; i++){
            int k = 0;
            for (int j = 0; j < warriorsAmount+1; j++)
            {
                arrWins[i][j] += k;
                k += arrDraws[i][j];
            }
        }
        int games = (playersAmount-1)*cellsAmount;
        for (int i = 0; i < playersAmount; i++){
            for (int j = 0; j < cellsAmount; j++)
            {
                int draws = arrDraws[j][player[i].showStrategy()[j]]-1;
                int wins = arrWins[j][player[i].showStrategy()[j]];
                player[i].addScore(0.5*draws);
                player[i].addScore(wins);
                player[i].addWin(wins);
                player[i].addDraw(draws);
            }
            player[i].addLose(games-player[i].wins()-player[i].draws());
        }
         QMessageBox msgBox;
         msgBox.setText("Готово");
         msgBox.exec();
    }
}

void Tournament::sortPlayers()
{
    std::sort(player.rbegin(),player.rend());
    QMessageBox msgBox;
    msgBox.setText("Готово");
    msgBox.exec();
}

void Tournament::deleteAllPlayers()
{
    playersAmount = 0;
    player.clear();
    std::vector<Player> tmp = player;
    player.swap(tmp);
}

Player::Player(std::vector<int> s): strategy(s), winNum(0), drawNum(0), loseNum(0), score(0)
{ }

std::vector<int> Player::showStrategy(){
    return strategy;
}

int Player::wins(){
    return winNum;
}

int Player::draws(){
    return drawNum;
}

int Player::loses(){
    return loseNum;
}

float Player::scores(){
    return score;
}

void Player::addScore(float t){
    score += t;
}

void Player::addDraw(){
    drawNum++;
}

void Player::addLose(){
    loseNum++;
}

void Player::addWin(){
    winNum++;
}
void Player::addWin(int wins){
    winNum += wins;
}
void Player::addDraw(int draws){
    drawNum += draws;
}
void Player::addLose(int loses){
    loseNum += loses;
}

bool operator<(Player const & p1, Player const & p2)
{
    return (p1.score < p2.score);
}

void MainWindow::on_pushButton_clicked()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home/", tr("(*.csv *.txt)"));
   t.openFile(fileName);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"/home/results.csv",tr("(*.csv);;(*.txt)"));
    t.saveFile(fileName);

}

void MainWindow::on_pushButton_3_clicked()
{
    t.sortPlayers();
}

void MainWindow::on_pushButton_4_clicked()
{
   t.startTournament();
}

void MainWindow::on_pushButton_5_clicked()
{
    t.addPlayer(ui->lineEdit->text());
}

void MainWindow::on_pushButton_6_clicked()
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    for (int i = 0; i < ui->lineEdit_2->text().toInt(); i++)
    {
        int arr[9] = {0,0,0,0,0,0,0,0,0};
        for (int j = 0; j < 100; j++)
        {
            arr[qrand() % 9]++;
        }
        t.addPlayer(arr);
    }
    QMessageBox msgBox;
    msgBox.setText("Добавлено");
    msgBox.exec();
}

void MainWindow::on_pushButton_7_clicked()
{
    t.deleteAllPlayers();
    QMessageBox msgBox;
    msgBox.setText("Готово");
    msgBox.exec();
}
