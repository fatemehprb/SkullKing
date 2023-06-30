#include "menuselection.h"
#include "ui_menuselection.h"
#include"signin.h"
#include"server.h"
#include"client.h"
#include<QMessageBox>
#include"signup.h"
#include"algorithm"
MenuSelection::MenuSelection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuSelection)
{
    connect(cln,&Client::startTheGame,this,&MenuSelection::startTheGame);
    ui->setupUi(this);
}
//**********************************************
MenuSelection::~MenuSelection()
{
    delete ui;
}
//***********************************************
void MenuSelection::on_start_clicked()
{
    if(currentPlayer.get_server()){
        if(currentPlayer.get_coin()<50){
            QMessageBox mb;
            mb.critical(0,"Error","You don't have enough coins to start the game");
        }
        else{
            this->hide();
            srv->show();
            cards server_order;
            server_order.setOrder("Start The Game");
            sendCard.push_back(server_order);
            for(int i=0 ; i<sendCard.size();i++){
                qDebug()<<sendCard[i].getId()<<"\t"<<sendCard[i].getNumber()<<"\t"<<sendCard[i].getValue()<<"\n";
            }
            writeToFileCards("sendCard.bin",sendCard);
            QFile file("sendCard.bin");
            file.open(QFile::ReadOnly | QFile::Text);
            QByteArray file_content = file.readAll();
            srv->get_socket()->write(file_content);
            srv->get_socket()->flush();
            file.close();
            currentPlayer.set_coin(currentPlayer.get_coin()-50);
            auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
            foundPlayer->set_coin(currentPlayer.get_coin());
             writeToFile("myfile.bin");
              // server pick cards first
             int turncount = currentPlayer.get_countOfTurn();
             sendCard = currentPlayer.creat_cards();
             currentPlayer.set_randomCards(sendCard,turncount);
             writeToFileCards("sendCard.bin",sendCard);
             file.open(QFile::ReadOnly | QFile::Text);
             QByteArray file_content2 = file.readAll();
             srv->get_socket()->write(file_content2);
             srv->get_socket()->flush();
             // end

             /// making parrote cards
             srv->whoShouldStartTheGameFirst();
             srv->set_picture(srv->get_server_card());
             srv->set_picture(srv->get_client_card());
             //end

     }
    }
    else {
        QMessageBox MQ;
        MQ.warning(0,"Warning","Only server can start the game!");
    }
}
//***********************************************
void MenuSelection::startTheGame(){
    auto it = find_if(listOfPlayer.begin(),listOfPlayer.end(),[&](auto p){
       return( p.get_username() == currentPlayer.get_username());
    });
    if(currentPlayer.get_coin()<50){
        QMessageBox mb;
        mb.critical(0,"Error","You don't have enough coins to start the game");
        //exit....
    }
    else{
    it->set_coin(it->get_coin()-50);
    writeToFile("myfile.bin");
    this->hide();
    cln->show();
    }
}
//***********************************************