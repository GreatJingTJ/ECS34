#include <gtest/gtest.h>
#include "MancalaBoard.h"

bool checkSum(const int pits[10], const int stores[2])

{
	int sum = 0;

	for(int i = 0; i < 10; i++)
		sum += pits[i];
	for(int i = 0; i < 2; i++)
		sum += stores[i];


	if(sum == 40)
		return true;
	else
		return false;
}

TEST(MancalaBoardTest, DefaultBoardTest){
    // Needs to test that default board is correct
    CMancalaBoard a;
    std::string str = std::string(a);
    std::string str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 4 | 4 | 4 | 4 | 4 |   |\n";
	str2 += "| 0 |-------------------| 0 |\n";
	str2 +="|   | 4 | 4 | 4 | 4 | 4 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";
    EXPECT_EQ(str, str2);
}

TEST(MancalaBoardTest, SetBoardTest){
    // Needs to test that setting up an initial board is correct

	const int pits[] = {1,2,3,4,3,5,4,2,1,6}, stores[] = {4,5};
	
	
    CMancalaBoard a(1,pits,stores);
    std::string str = std::string(a);
    EXPECT_EQ(a.PlayerTurn(), 1);
    std::string str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 3 | 4 | 3 | 2 | 1 |   |\n";
	str2 += "| 4 |-------------------| 5 |\n";
	str2 += "|   | 5 | 4 | 2 | 1 | 6 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

    EXPECT_EQ(str, str2);

    EXPECT_TRUE(checkSum(pits, stores));


}

TEST(MancalaBoardTest, ResetBoardTest){
    // Needs to test that resetting a board is correct
    CMancalaBoard a;
    
    a.Move(0,1);
    a.Move(1,2);
	a.ResetBoard();
    std::string str = std::string(a);
    std::string str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 4 | 4 | 4 | 4 | 4 |   |\n";
	str2 += "| 0 |-------------------| 0 |\n";
	str2 += "|   | 4 | 4 | 4 | 4 | 4 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";
	EXPECT_EQ(str, str2);
}

TEST(MancalaBoardTest, BasicMoveTest){
    // Needs to test that basic non-scoring moves are correct
    CMancalaBoard a;
    a.Move(0,0);
    a.Move(1,0);
    std::string str = std::string(a);
    std::string str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 5 | 5 | 5 | 5 | 0 |   |\n";
	str2 += "| 0 |-------------------| 0 |\n";
	str2 += "|   | 0 | 5 | 5 | 5 | 5 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";
    EXPECT_EQ(str, str2);
}

TEST(MancalaBoardTest, ScoringMoveTest){
    // Needs to test that basic scoring, non double moves, non steals are correct 
    CMancalaBoard a;
    a.Move(0,0);
    a.Move(1,0);
    a.Move(0,2);
    std::string str = std::string(a);
    std::string str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 6 | 6 | 0 | 5 | 0 |   |\n";
	str2 += "| 1 |-------------------| 0 |\n";
	str2 += "|   | 1 | 6 | 5 | 5 | 5 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";
    EXPECT_EQ(str, str2);

    a.Move(1,2);
    std::string strr = std::string(a);
    std::string str22 = "P1          PITS\n";
	str22 += "      5   4   3   2   1\n";
	str22 += "/---------------------------\\\n";
	str22 += "|   | 6 | 6 | 0 | 6 | 1 |   |\n";
	str22 += "| 1 |-------------------| 1 |\n";
	str22 += "|   | 1 | 6 | 0 | 6 | 6 |   |\n";
	str22 += "\\---------------------------/\n";
	str22 += "      1   2   3   4   5\n";
	str22 += "             PITS          P2\n";

	EXPECT_EQ(strr, str22);

	const int pits[] = {0,0,0,4,0,3,1,4,0,1}, stores[] = {18,9};
	CMancalaBoard b(1, pits, stores);
	
	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 0 | 4 | 0 | 0 | 0 |   |\n";
	str2 += "|18 |-------------------| 9 |\n";
	str2 += "|   | 3 | 1 | 4 | 0 | 1 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	//EXPECT_EQ(std::string(b), str2);

	b.Move(1,2);

	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 0 | 4 | 0 | 0 | 1 |   |\n";
	str2 += "|18 |-------------------| 10 |\n";
	str2 += "|   | 3 | 1 | 0 | 1 | 2 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	EXPECT_EQ(std::string(b), str2);
	
	const int pit[] = {0,2,2,15,1,1,1,1,1,1},store[] = {10,5};
	CMancalaBoard c(1,pit,store);
	EXPECT_TRUE(checkSum(pit, store));
	EXPECT_TRUE(c.Move(0,3));
	//std::cout << std::string(c);

	str2 = "P1          PITS\n";
        str2 += "      5   4   3   2   1\n";
        str2 += "/---------------------------\\\n";
        str2 += "|   | 3 | 1 | 3 | 3 | 1 |   |\n";
        str2 += "|12 |-------------------| 5 |\n";
        str2 += "|   | 3 | 3 | 2 | 2 | 2 |   |\n";
        str2 += "\\---------------------------/\n";
        str2 += "      1   2   3   4   5\n";
        str2 += "             PITS          P2\n";

	
	EXPECT_EQ(std::string(c),str2);

} 

TEST(MancalaBoardTest, DoubleMoveTest){
    // Needs to test that double move is correct
     CMancalaBoard a;
    a.Move(0,1);
    std::string str = std::string(a);
    std::string str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 5 | 5 | 5 | 0 | 4 |   |\n";
	str2 += "| 1 |-------------------| 0 |\n";
	str2 += "|   | 4 | 4 | 4 | 4 | 4 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";
    EXPECT_EQ(str, str2);
    EXPECT_EQ(a.PlayerTurn(), 0);

    const int pits[] = {1,2,3,4,3,5,4,2,1,6}, stores[] = {4,5};
    
    EXPECT_TRUE(checkSum(pits, stores));
    
    CMancalaBoard b(1,pits,stores);

    b.Move(1,1);
    str= std::string(b);
    str2  = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 3 | 4 | 3 | 2 | 1 |   |\n";
	str2 += "| 4 |-------------------| 6 |\n";
	str2 += "|   | 5 | 0 | 3 | 2 | 7 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	EXPECT_EQ(str, str2);
	EXPECT_EQ(b.PlayerTurn(), 1);

	const int pits3[] = {0,0,1,0,0,0,0,0,2,0}, stores3[] = {22,15};

	CMancalaBoard d(1,pits3,stores3);

	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 0 | 0 | 1 | 0 | 0 |   |\n";
	str2 += "|22 |-------------------|15 |\n";
	str2 += "|   | 0 | 0 | 0 | 2 | 0 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	//EXPECT_EQ(std::string(d), str2);

	d.Move(1,3);

	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 0 | 0 | 1 | 0 | 0 |   |\n";
	str2 += "|22 |-------------------|16 |\n";
	str2 += "|   | 0 | 0 | 0 | 0 | 1 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	EXPECT_EQ(std::string(d), str2);
	EXPECT_EQ(d.PlayerTurn(), 1);

}
 
TEST(MancalaBoardTest, StealMoveTest){
    // Needs to test that stealing move works correctly
    const int pits[] = {1,2,3,4,3,5,0,3,2,7}, stores[] = {4,6};
	
	EXPECT_TRUE(checkSum(pits, stores));

    CMancalaBoard a(1,pits,stores);

    std::string str = std::string(a);
    std::string str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 3 | 4 | 3 | 2 | 1 |   |\n";
	str2 += "| 4 |-------------------| 6 |\n";
	str2 += "|   | 5 | 0 | 3 | 2 | 7 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	//EXPECT_EQ(str, str2);

	a.Move(0,4);

	std::string str22 = "P1          PITS\n";
	str22 += "      5   4   3   2   1\n";
	str22 += "/---------------------------\\\n";
	str22 += "|   | 0 | 4 | 3 | 2 | 1 |   |\n";
	str22 += "| 5 |-------------------| 6 |\n";
	str22 += "|   | 6 | 1 | 3 | 2 | 7 |   |\n";
	str22 += "\\---------------------------/\n";
	str22 += "      1   2   3   4   5\n";
	str22 += "             PITS          P2\n";

	std::string strr = std::string(a);
	EXPECT_EQ(strr, str22);

	const int pits2[] = {1,2,3,4,3,1,0,5,3,7}, stores2[] = {4,7};
	EXPECT_TRUE(checkSum(pits2, stores2));
	
	CMancalaBoard c(1,pits2,stores2);
	std::string strrr = std::string(c);
    std::string str222 = "P1          PITS\n";
	str222 += "      5   4   3   2   1\n";
	str222 += "/---------------------------\\\n";
	str222 += "|   | 3 | 4 | 3 | 2 | 1 |   |\n";
	str222 += "| 4 |-------------------| 7 |\n";
	str222 += "|   | 1 | 0 | 5 | 3 | 7 |   |\n";
	str222 += "\\---------------------------/\n";
	str222 += "      1   2   3   4   5\n";
	str222 += "             PITS          P2\n";

	//EXPECT_EQ(strrr, str222);

	c.Move(1,0);

	std::string str2222 = "P1          PITS\n";
	str2222 += "      5   4   3   2   1\n";
	str2222 += "/---------------------------\\\n";
	str2222 += "|   | 3 | 0 | 3 | 2 | 1 |   |\n";
	str2222 += "| 4 |-------------------|12 |\n";
	str2222 += "|   | 0 | 0 | 5 | 3 | 7 |   |\n";
	str2222 += "\\---------------------------/\n";
	str2222 += "      1   2   3   4   5\n";
	str2222 += "             PITS          P2\n";

	std::string strrrr = std::string(c);
	EXPECT_EQ(strrrr, str2222);
	const int pi[] = {4,0,4,1,1,1,1,0,2,9}, stor[] = {6,11};

	EXPECT_TRUE(checkSum(pi, stor));

	CMancalaBoard f(1, pi, stor);
	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 1 | 1 | 4 | 0 | 4 |   |\n";
	str2 += "| 6 |-------------------|11 |\n";
	str2 += "|   | 1 | 1 | 0 | 2 | 9 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";
	
	f.Move(1,4);

	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 2 | 2 | 0 | 1 | 5 |   |\n";
	str2 += "| 6 |-------------------|18 |\n";
	str2 += "|   | 2 | 2 | 0 | 2 | 0 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	EXPECT_EQ(std::string(f), str2);

	EXPECT_EQ(f.PlayerTurn(), 0);

	const int p[] = {0,0,0,0,9,0,0,0,0,0}, sto[] = {20,11};

	EXPECT_TRUE(checkSum(p, sto));

	CMancalaBoard g(1, p, sto);
	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 9 | 0 | 0 | 0 | 0 |   |\n";
	str2 += "|20 |-------------------|11 |\n";
	str2 += "|   | 0 | 0 | 0 | 0 | 0 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";
	
	EXPECT_TRUE(g.Move(0,4));

	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 0 | 0 | 0 | 1 | 1 |   |\n";
	str2 += "|23 |-------------------|11 |\n";
	str2 += "|   | 1 | 1 | 0 | 1 | 1 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	EXPECT_EQ(std::string(g), str2);

	EXPECT_EQ(g.PlayerTurn(), 0);

	

}

TEST(MancalaBoardTest, CantMoveTest){
    // Needs to test when player can't move correctly
   	
	const int pits[] = {0,0,0,0,0,1,1,1,1,1}, stores[] = {20,15};
	CMancalaBoard a(1,pits,stores);
	a.Move(1,0);
	std::string str2 = "P1          PITS\n";
        str2 += "      5   4   3   2   1\n";
        str2 += "/---------------------------\\\n";
        str2 += "|   | 0 | 0 | 0 | 0 | 0 |   |\n";
        str2 += "|20 |-------------------|15 |\n";
        str2 += "|   | 0 | 2 | 1 | 1 | 1 |   |\n";
        str2 += "\\---------------------------/\n";
        str2 += "      1   2   3   4   5\n";
        str2 += "             PITS          P2\n";
	
	EXPECT_EQ(std::string(a), str2);
	EXPECT_EQ(a.PlayerTurn(),1);
	a.Move(1,3);
	str2 = "P1          PITS\n";
        str2 += "      5   4   3   2   1\n";
        str2 += "/---------------------------\\\n";
        str2 += "|   | 0 | 0 | 0 | 0 | 0 |   |\n";
        str2 += "|20 |-------------------|15 |\n";
        str2 += "|   | 0 | 2 | 1 | 0 | 2 |   |\n";
        str2 += "\\---------------------------/\n";
        str2 += "      1   2   3   4   5\n";
        str2 += "             PITS          P2\n";
	
	EXPECT_EQ(std::string(a), str2);
	EXPECT_EQ(a.PlayerTurn(),1);

	const int pit[] = {0,0,0,0,1,0,0,0,2,0}, store[] = {22,15};//cannot move

	CMancalaBoard e(1,pit,store);

	e.Move(0,4);

	str2 = "P1          PITS\n";
	str2 += "      5   4   3   2   1\n";
	str2 += "/---------------------------\\\n";
	str2 += "|   | 0 | 0 | 0 | 0 | 0 |   |\n";
	str2 += "|23 |-------------------|15 |\n";
	str2 += "|   | 0 | 0 | 0 | 2 | 0 |   |\n";
	str2 += "\\---------------------------/\n";
	str2 += "      1   2   3   4   5\n";
	str2 += "             PITS          P2\n";

	EXPECT_EQ(std::string(e), str2);

	EXPECT_EQ(e.PlayerTurn(), 1); 


}
TEST(MancalaBoardTest, BadParametersTest){
    // Needs to check that bad parameters are handled correctly
    CMancalaBoard a;
    EXPECT_TRUE(a.Move(0,1));
    EXPECT_FALSE(a.Move(0,1));

    EXPECT_FALSE(a.Move(12321,321));
    EXPECT_FALSE(a.Move(1,21312312));
    EXPECT_FALSE(a.Move(-12,123));
    EXPECT_FALSE(a.Move(-12312,12312312));
    EXPECT_FALSE(a.Move(12311,321));
    EXPECT_FALSE(a.Move(1,5));
    EXPECT_FALSE(a.Move(0,5));
    EXPECT_FALSE(a.Move(0,-1));
    EXPECT_FALSE(a.Move(1,-1));
    
    EXPECT_EQ(a.PlayerScore(2),-1);
    EXPECT_EQ(a.PlayerScore(-1),-1);
    EXPECT_EQ(a.PlayerScore(1),0);
    EXPECT_EQ(a.PlayerScore(0),1);

    EXPECT_EQ(a.PitStoneCount(1,5),-1);
    EXPECT_EQ(a.PitStoneCount(2,-1),-1);
    EXPECT_EQ(a.PitStoneCount(0,4),5);
    EXPECT_EQ(a.PitStoneCount(1,4),4);
    EXPECT_EQ(a.PitStoneCount(0,1),0);
    EXPECT_EQ(a.PitStoneCount(2,5),-1);



} 

