#include "MancalaBoard.h"
#include <iostream>

int checkSum(int pits[10],int stores[2])
{
    int sum = 0;

    for(int i = 0; i < 10; i++)
        sum += pits[i];
    for(int i = 0; i < 2; i++)
        sum += stores[i];
    return sum;
}


CMancalaBoard::CMancalaBoard(){
    DTurn = 0;
    for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
        DPits[Index] = 4;   
    }
    for(int Index = 0; Index < MANCALA_PLAYERS; Index++){
        DStores[Index] = 0;
    }
}

CMancalaBoard::CMancalaBoard(int turn, const int pits[MANCALA_TOTAL_PITS], const int stores[MANCALA_PLAYERS]){
    DTurn = turn;
    for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
        DPits[Index] = pits[Index];   
    }
    for(int Index = 0; Index < MANCALA_PLAYERS; Index++){
        DStores[Index] = stores[Index];
    }
}

void CMancalaBoard::ResetBoard(){
    for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
        DPits[Index] = 4;   
    }
    DStores[0] = 0;
    DStores[1] = 0;
}

int CMancalaBoard::PlayerTurn() const{
    return DTurn;
}

int CMancalaBoard::PlayerScore(int player) const{
    
    int score = -1;
 
    if(player == 0 || player == 1)
	    score = DStores[player];
    
    return score;
}

int CMancalaBoard::PitStoneCount(int player, int pit){
    
    int count = -1;

    if((player == 0 || player == 1)&& (pit >= 0 && pit <= 4 ))
	count = DPits[player * MANCALA_PIT_SLOTS + pit];
    
    return count; 
}

bool CMancalaBoard::GameOver() const{
    for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
        if(DPits[Index]){
            return false;   
        }
    }
    return true;
}

std::string CMancalaBoard::ToString() const{
    std::string ReturnString  = "P1          PITS\n";
    ReturnString += "      5   4   3   2   1\n";
    ReturnString += "/---------------------------\\\n";
    ReturnString += "|   |";
    for(int Index = 0; Index < MANCALA_PIT_SLOTS; Index++){
        if(DPits[Index] >= 10){
            ReturnString += std::to_string(DPits[Index]) + " |";
        }
        else{
            ReturnString += std::string(" ") + std::to_string(DPits[Index]) + " |";    
        }
    }
    ReturnString += "   |\n|";
    
    if(DStores[0] > 10){
        ReturnString += std::to_string(DStores[0]);
    }
    else{
        ReturnString += std::string(" ") + std::to_string(DStores[0]);
    }
    ReturnString += " |-------------------|";
    if(DStores[1] >= 10){
        ReturnString += std::to_string(DStores[1]);
    }
    else{
        ReturnString += std::string(" ") + std::to_string(DStores[1]);
    }
    ReturnString += " |\n|   |";
    for(int Index = 0; Index < MANCALA_PIT_SLOTS; Index++){
        if(DPits[MANCALA_PIT_SLOTS + Index] >= 10){
            ReturnString += std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";
        }
        else{
            ReturnString += std::string(" ") + std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";    
        }
    }
    ReturnString += "   |\n";
    ReturnString += "\\---------------------------/\n";
    ReturnString += "      1   2   3   4   5\n";
    ReturnString += "             PITS          P2\n";
                
    return ReturnString;
}

CMancalaBoard::operator std::string() const{
    std::string ReturnString  = "P1          PITS\n";
    ReturnString += "      5   4   3   2   1\n";
    ReturnString += "/---------------------------\\\n";
    ReturnString += "|   |";
    for(int Index = MANCALA_PIT_SLOTS-1; Index >= 0; Index--){
        if(DPits[Index] >= 10){
            ReturnString += std::to_string(DPits[Index]) + " |";
        }
        else{
            ReturnString += std::string(" ") + std::to_string(DPits[Index]) + " |";    
        }
    }
    ReturnString += "   |\n|";
    
    if(DStores[0] >= 10){
        ReturnString += std::to_string(DStores[0]);
    }
    else{
        ReturnString += std::string(" ") + std::to_string(DStores[0]);
    }
    ReturnString += " |-------------------|";
    if(DStores[1] > 10){
        ReturnString += std::to_string(DStores[1]);
    }
    else{
        ReturnString += std::string(" ") + std::to_string(DStores[1]);
    }
    ReturnString += " |\n|   |";
    for(int Index = 0; Index < MANCALA_PIT_SLOTS; Index++){
        if(DPits[MANCALA_PIT_SLOTS + Index] >= 10){
            ReturnString += std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";
        }
        else{
            ReturnString += std::string(" ") + std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";    
        }
    }
    ReturnString += "   |\n";
    ReturnString += "\\---------------------------/\n";
    ReturnString += "      1   2   3   4   5\n";
    ReturnString += "             PITS          P2\n";
                
    return ReturnString;
}

bool CMancalaBoard::Move(int player, int pit){
    
    bool flag = false, check1 = false, check2 = false;

    int PitIndex = player * MANCALA_PIT_SLOTS + pit;
    //std::cout << "pitindex: " << PitIndex << "\n";
    if((PitIndex < 0) or (PitIndex > MANCALA_TOTAL_PITS)){
        return false;    
    }
    if(player == 0 && (PitIndex > 4 || PitIndex < 0))
    {   
        DTurn = player;
        return false;
    }
    if(player == 1 && (PitIndex < 5 || PitIndex > 9))
    {   
        DTurn = player;
        return false;
    }

    int Stones = DPits[PitIndex];

    if(Stones == 0)
    {
        DTurn = player;
        return false;
    }
    //int LastPitDrop = PitIndex, k = 0;
    DPits[PitIndex] = 0;
    //std::cout<<"@ line "<<__LINE__<<" "<<Stones<<std::endl;
    while(Stones > 1){
       PitIndex++;
        PitIndex %= MANCALA_TOTAL_PITS;
        if((player == 0 && PitIndex == 5) || (player == 1 && PitIndex == 0)){
           
            DStores[player]++;
            Stones--;
            //std::cout << "add one in player " << k + 1;
            //std::cout << "when the index is " << PitIndex << "\n";
	}
	
	/*PitIndex++;
        if((PitIndex % MANCALA_PIT_SLOTS) == 0){
            if(player == (PitIndex / MANCALA_PIT_SLOTS) - 1){
                DStores[player]++;
                Stones--;
            }
        }
    */
        
        PitIndex %= MANCALA_TOTAL_PITS;
        if(Stones){
            DPits[PitIndex]++;
            //std::cout << "add one in " << PitIndex + 1 << "\n";
            Stones--;
            //LastPitDrop = PitIndex;
        }
    }
    PitIndex++;

    if(Stones != 0){
    if(PitIndex == 10)
        PitIndex %= MANCALA_TOTAL_PITS;

    if(DPits[PitIndex] == 0 &&PitIndex != 0 && PitIndex != 5){
        //std::cout<<"@ line "<<__LINE__<<std::endl;
        int OppositeSide = 9 - PitIndex;
        if((OppositeSide >= 5 && player == 0) || (OppositeSide <= 4 && player == 1) )
        {
            DStores[player] += DPits[OppositeSide] + 1;
            DPits[OppositeSide] = 0;
        }
        else
            DPits[PitIndex] += 1;
        Stones--;
    }
    else if(Stones == 1){ // index =  0 or 5
        if(PitIndex == 5 && player == 0){
            DStores[player]++;
            Stones--;
            flag = true;
            //std::cout<<"@ line "<<__LINE__<<std::endl;

        }
        else if(PitIndex == 0 && player == 1)
        {
            DStores[player]++;
            Stones--;
            flag = true;
            //std::cout<<"@ line "<<__LINE__<<std::endl;
        }
        PitIndex %= MANCALA_TOTAL_PITS;
        if(Stones == 1){
            DPits[PitIndex]++;
            Stones--;
            //LastPitDrop = PitIndex;
        }
    }
}
    //1. last drop in the player's store, this player's pits 0

    for(int i = 0; i < 5; i++){
        if(DPits[i])
            check1 = true;
    }

    for(int i = 5; i < 10; i++)
        if(DPits[i])
            check2 = true;

    if(!flag)
        DTurn = 1 - DTurn;
    
    if(DTurn == 0)
    {
        if(!check1)
            DTurn =  1 - DTurn;
    }
    else
    {   

        if(!check2)
            DTurn = 1 - DTurn;
    }

    if(checkSum(DPits,DStores) != 40)
    {
        std::cout << "------------------------" << "\n";
        std::cout << "Sum not 40!" << "\n";
        std::cout << "------------------------" << "\n";
    }

    return true;
}
