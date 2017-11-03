#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

char getBlock(void)
{
	char block;
	cin >> block;
    block = toupper(block);
	return block;
}

unsigned int putBlock(char block, unsigned int position, char *blocks)
{
	blocks[position] = block;
	cout << "Block " << block << " inserted into slot " << position << endl;
	return position;
}

bool compareB(char robot, char in_slot)
{
	
	cout << endl <<  "Comparing robot block " << robot << " with block in slot " << in_slot << endl;
	if (robot <= in_slot)
	{
		cout << "returning true. Robot block LESS than or EQUAL to block in slot. " << endl;
		return true;
	}
	else
	{
		cout << "returning false. Robot block GREATER than block in slot. " << endl;
		return false;
	}
}

char switchB(char robot, unsigned int position, char *blocks, unsigned int &switchCnt)
{
	char temp_hold;

	cout << "Switching blocks " << robot << " with " << blocks[position] << endl;
	temp_hold = robot;
	robot = blocks[position];
	blocks[position] = temp_hold;
        switchCnt++;
	return robot;
}

bool testE(unsigned int position, char array[])
{
	if  (array[position] == 0)
	{
		cout << "Slot " << position << " empty. " << endl;
		return true;
	}
	else
	{
		cout << "Slot " << position << " contains a block " << endl;
		return false;
	}

}

char getBlock(void);
unsigned int putBlock(char block, unsigned int position, char array[]);
unsigned int shift_right(unsigned int position);
unsigned int shift_left(unsigned int position);
bool compareB(char robot, char in_slot);
char switchB(char robot, unsigned int position, char array[], unsigned int &switchCnt);
bool testE(unsigned int position, char array[]);
unsigned int getMid(char *blocks);
void printBlock(char *blocks);
char *sort(char *blocks, unsigned int index, char &current_block, int &increment, unsigned int &switchCnt, unsigned int sort_iteration);
unsigned int srch(char &current_block, char *blocks, unsigned int iAmount, int &increment, unsigned int &switchCnt);

int main(void){
    char *blocks, current_block;
    unsigned int start_ind = 0, iAmount = 1, switchCnt = 0;
    int increment = 1;
    
    blocks = new char[20];
    for(int i = 0; i < 20; i++)
        blocks[i] = 0; 
    
    cout << "Hello. Please give a starting index [0-19]:" << endl;
    cin >> start_ind;
    
    cout << left << "Your index is  " << start_ind << "." << endl;
    
    cout << "Please enter character:" << endl;
    blocks[start_ind] = getBlock();
    
    printBlock(blocks);
    
    for(int j = 0; j < 19; j++){
        cout << "Please enter another character:" << endl;
        
        if(j < 9)
            iAmount = 2;
        else
            iAmount = 1;
        
        current_block = getBlock();
        blocks = sort(blocks, srch(current_block, blocks, iAmount, increment, switchCnt), current_block, increment, switchCnt, 0);
        
        printBlock(blocks);
    }
    
    cout << "Total switch count: " << switchCnt << endl;
    return 0;
}

void printBlock(char *blocks){
    cout << endl << "Current arrangement: " << endl;
        for (int i = 0; i < 20; i++) {
            if (blocks[i] != 0)
                cout << setw(3) << blocks[i];
            else
                cout << setw(3) << "-";
        }
    cout << endl;
}
unsigned int getMid(char *blocks){ 
    unsigned int N = 0, mid = 0, index = 0;
    
    for(int i = 0; i < 20; i++){
        if(blocks[i] != 0)
            N++;
    }
    if(N % 2 == 0)
        mid = N/2;
    else
        mid = (N/2)+1;
    
    for(int i = 0; i < 20; i++){
        if(blocks[i] != 0)
            index++;

        if(index == mid)
            return i;
    }
}

unsigned int srch(char &current_block, char *blocks, unsigned int iAmount, int &increment, unsigned int &switchCnt){
    unsigned int index = 0;
    
    index = getMid(blocks);
    if(current_block > blocks[index])
        increment = 1*iAmount;
    else if(current_block < blocks[index]){
        increment = -1*iAmount;
    }
    else
        return index;
    do{
        index += increment;
    }
    while(!(index >= 19 || index <= 0
            || (current_block > blocks[index] && current_block > blocks[index+increment] && increment < 0)
            || (current_block < blocks[index] && (current_block < blocks[index+increment] || blocks[index+increment] == 0) && increment > 0)
            || (current_block == blocks[index]))
            || !(testE(index,blocks)));

    if(index < 19 && index > 0)
	{ //are we outside of the array? 
        if(testE(index,blocks))
            return index;
        
        if(current_block < blocks[index] && increment > 0)
            increment *= -1; // moving backwards to sort now.
        //if((current_block > blocks[index] && increment < 0))
            
        if((current_block > blocks[index] && increment > 0 || current_block < blocks[index] && increment < 0))
		{
                index += increment;
                if(testE(index,blocks))
                    return index;
        }
        
        current_block = switchB(current_block, index, blocks, switchCnt); 
        return index + increment; //switch the blocks then we need to send our new current block into the sorting function with the next index
    }\

    else
	{ //are we outside of the array? 
        if (index >= 19) 
		{ //if outside to the right
            index = 19;
            if(testE(index,blocks)) //If empty, release the block
                return index;
            else
			{
                if (current_block < blocks[index] && increment > 0)
				{
                    index -= increment; //if at end and block does not fit, recenter the robot.
                }
                current_block = switchB(current_block, index, blocks, switchCnt); //if not, just switch the current and last block at index 19.
                increment *= -1;
                return index + increment; //From here we can move forward or backward depending on the situation
            }
        }
        else
		{ 
            index = 0;
            if(testE(index,blocks)) 
                return index;
            else
			{
                if (current_block > blocks[index] && increment < 0)
				{
                    index -= increment; 
                }
                current_block = switchB(current_block, index, blocks, switchCnt); 
                increment *= -1;
                return index + increment; 
            }
        }
    }
        
}

char *sort(char *blocks, unsigned int index, char &current_block, int &increment, unsigned int &switchCnt, unsigned int eoa_count){
    if(index < 19 && index > 0)
	{ 
        if(testE(index, blocks)) 
            putBlock(current_block, index, blocks); 
        
        else
		{ 
            current_block = switchB(current_block, index, blocks, switchCnt); 
            index += increment; 
            blocks = sort(blocks, index, current_block, increment, switchCnt, eoa_count); 
        }
    }
    else{ 
        if(index >= 19) { 
            index = 19;
            if(testE(index,blocks)) 
                putBlock(current_block, index, blocks);
            else{ 
                current_block = switchB(current_block, index, blocks, switchCnt); 
                if(eoa_count == 0) 
                    increment *= -1; 
                index += increment; 
                eoa_count++; 
                blocks = sort(blocks, index, current_block, increment, switchCnt, eoa_count); 
            }
        }
        
        if (index <= 0) { 
            index = 0;
            if(testE(index,blocks)) 
                putBlock(current_block, index, blocks);
            else{ 
                current_block = switchB(current_block, index, blocks, switchCnt); 
                if(eoa_count == 0) 
                    increment *= -1; 
                index += increment; 
                eoa_count++;
                blocks = sort(blocks, index, current_block, increment, switchCnt, eoa_count); 
            }
        }
    }
    return blocks;
}
