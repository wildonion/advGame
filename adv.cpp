

// needed libraries 

#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<sstream>
#include<cassert>
using namespace std;
#define QUST_DEC 200 // 200 chars for questions and their answers

// ===================================================
// defining link structure for linked list (narrative) class
struct link{
    // string array for the whole contents of a narrative
    string content[QUST_DEC], id, question, decision1, decision2;
    // effective params for each decision
    int people_1, court_1, treasury_1;
    int people_2, court_2, treasury_2;
    int possible_happen = 3;
    link *next; // a pointer that points to the next node 
};

// =============================NARRATIVE CLASS UI=============================
// linked list class for our narratives
class narrative{ 
    public:
        narrative(string);
        ~narrative();
        int remainingNarrative();
        void removeNarrative(int);
        bool isEmpty();
        string* GetNth(int);
        void chooseRandomNarrative(int*);
        void saveRemainingNarrative();
        void loadNarrative(string, string);
    private:
        int chooseFlag = 0;
        link* head; 
        link* tail;
};
// =============================NARRATIVE CLASS BACKEND=============================
// narrative class functions
narrative::narrative(string name){ 
    head = tail = NULL;
    string choice;
    ifstream choices("choices.txt");
        if (!choices) {
            cout << "[-] can't open file" << endl;
            exit(1);
        }
        while (choices >> choice) {
            this->loadNarrative(choice, name);
        }
        choices.close();
}
        // -------------------------------------------------------
        // load choices from file into the list
void narrative::loadNarrative(string choice, string name){
    string fileName = choice + ".txt";
    string nrtvName = choice + name + "_list.txt";
    ifstream nrtv(nrtvName.c_str());
    ifstream ch(fileName.c_str());

    if (nrtv && chooseFlag==1){ // new choice created by the player
        
            link* temp = new link;
            short loop = 0;
            string line;

            while (! ch.eof() ){
                getline (ch, line); //get one line from the file
                temp->content[loop] = line;
                loop++;
            }
             
            temp->question = temp->content[1];
            temp->decision1 = temp->content[2];
            temp->decision2 = temp->content[6];

            string ph(temp->content[0]);
            temp->possible_happen = stoi(ph);
            
            string p1(temp->content[3]);
            temp->people_1 = stoi(p1);
            string c1(temp->content[4]);
            temp->court_1 = stoi(c1);
            string t1(temp->content[5]);
            temp->treasury_1 = stoi(t1);

            string p2(temp->content[7]);
            temp->people_2 = stoi(p2);
            string c2(temp->content[8]);
            temp->court_2 = stoi(c2);
            string t2(temp->content[9]);
            temp->treasury_2 = stoi(t2);

            
            temp->id = choice+name;
            temp->next = NULL;
            if (head == NULL && tail == NULL){
                head = temp;
                tail = temp;
                } else{
                tail->next = temp;
                tail = tail->next;
            }

        nrtv.close();
        chooseFlag = 1;
     }

        if (ch && chooseFlag!=1) { // player first run the game
            
            link* temp = new link;
            short loop = 0;
            string line;

            while (! ch.eof() ){
                getline (ch, line); //get one line from the file
                temp->content[loop] = line;
                loop++;
            }
             
            temp->question = temp->content[0];
            temp->decision1 = temp->content[1];
            temp->decision2 = temp->content[5];

            string p1(temp->content[2]);
            temp->people_1 = stoi(p1);
            string c1(temp->content[3]);
            temp->court_1 = stoi(c1);
            string t1(temp->content[4]);
            temp->treasury_1 = stoi(t1);

            string p2(temp->content[6]);
            temp->people_2 = stoi(p2);
            string c2(temp->content[7]);
            temp->court_2 = stoi(c2);
            string t2(temp->content[8]);
            temp->treasury_2 = stoi(t2);
            
            temp->id = choice+name;
            temp->next = NULL;
            if (head == NULL && tail == NULL){
                head = temp;
                tail = temp;
                } else{
                tail->next = temp;
                tail = tail->next;
            }
        ch.close();
    }
}
        // -------------------------------------------------------
        // get the number of remaining narratives during the play
int narrative::remainingNarrative(){
    int nrtvNumber = 0;
    for (link* p = head; p != NULL; p = p->next) nrtvNumber++;
    cout << "\t[::KNIGDOM COME ISSUES::] ==== " << nrtvNumber << endl;
    return nrtvNumber;
} 
        // -------------------------------------------------------
        // save a narrative into the file
void narrative::saveRemainingNarrative(){
    for (link* p = head; p != NULL; p = p->next){
        string fileName = p->id + "_list.txt";
        ofstream sl(fileName.c_str());
        if (sl.is_open()) {
            sl << p->possible_happen << endl;
            for(int i = 0 ; i<=QUST_DEC; i++){
                sl << p->content[i] << " " << endl;
            }
            sl.close();
        } else{
            cout << "[-] can't open file";
        }
    }
}
        // -------------------------------------------------------
narrative::~narrative(){
    // this->saveRemainingNarrative();
}
        // -------------------------------------------------------
        // get the nth narrative using the its index
string* narrative::GetNth(int index){  
      
    link* current = head; 
    int count = 0;
    while (current != NULL){  
        if (count == index){
            return(current->content);  
        }  
        count++;
        current = current->next;  
    }  

    assert(0);        
}  
        // -------------------------------------------------------
        // choose a random narrative algo using its index

void narrative::chooseRandomNarrative(int* EffectiveParamPtr){
    
    int answer;
    link* temp = new link;
    int no_nrtv = this->remainingNarrative();
    int index = (rand() % no_nrtv) + 1;
    for (link* p = head; p != NULL; p = p->next){
        if(p->question == this->GetNth(index)[0]){
            temp = p;
            break;
        } 
    }

    // if the happen possibility is 0 the remove that narrative from list and its file
    if(temp->possible_happen == 0){
        string cmd = "rm -r ";
        cmd = cmd + temp->id + "_list.txt";
        const char *command = cmd.c_str(); 
        system(command);
        cout << "[+] removing narrative... " << endl;
        this->removeNarrative(index);
    } else{
        // otherwise mines one its possible happen var
        temp->possible_happen = temp->possible_happen - 1;
        cout << "\t" << temp->question << endl;
        cout << "DECISIONS : " << endl;
        cout << "\t[1] " << temp->decision1 << endl;
        cout << "\t[2] " << temp->decision2 << endl;

        cin >> answer;
        // according to the answers run the algo of the game to update the effective params
        if(answer == 1){
            EffectiveParamPtr[0] = 0 < (EffectiveParamPtr[0] + (temp->people_1)) < 100 ? (EffectiveParamPtr[0] + (temp->people_1)) : (EffectiveParamPtr[0] + (temp->people_1)) < 0 ? 0 : 100;  
            EffectiveParamPtr[1] = 0 < (EffectiveParamPtr[1] + (temp->court_1)) < 100 ? (EffectiveParamPtr[1] + (temp->court_1)) : (EffectiveParamPtr[1] + (temp->court_1)) < 0 ? 0 : 100; 
            EffectiveParamPtr[2] = 0 < (EffectiveParamPtr[2] + (temp->treasury_1)) < 100 ? (EffectiveParamPtr[2] + (temp->treasury_1)) : (EffectiveParamPtr[2] + (temp->treasury_1)) < 0 ? 0 : 100;  
            cout << "\tpeople --> " << EffectiveParamPtr[0] << " | court --> " << EffectiveParamPtr[1] << " | treasury --> " << EffectiveParamPtr[2] << endl;
        } else if(answer == 2){
            EffectiveParamPtr[0] = 0 < (EffectiveParamPtr[0] + (temp->people_2)) < 100 ? (EffectiveParamPtr[0] + (temp->people_2)) : (EffectiveParamPtr[0] + (temp->people_2)) < 0 ? 0 : 100; 
            EffectiveParamPtr[1] = 0 < (EffectiveParamPtr[1] + (temp->court_2)) < 100 ? (EffectiveParamPtr[1] + (temp->court_2)) : (EffectiveParamPtr[1] + (temp->court_2)) < 0 ? 0 : 100; 
            EffectiveParamPtr[2] = 0 < (EffectiveParamPtr[2] + (temp->treasury_2)) < 100 ? (EffectiveParamPtr[2] + (temp->treasury_2)) : (EffectiveParamPtr[2] + (temp->treasury_2)) < 0 ? 0 : 100;  
            cout << "\tpeople --> " << EffectiveParamPtr[0] << " | court --> " << EffectiveParamPtr[1] << " | treasury --> " << EffectiveParamPtr[2] << endl;
        }
    }   

}
        // -------------------------------------------------------
        // remove a narrative from the list
void narrative::removeNarrative(int pos){
    link *current = new link;
    link *previous = new link;
    current = head;
    for(int i = 1; i<pos; i++)
    {
      previous = current;
      current = current->next;
    }
    previous->next = current->next;
}
        // -------------------------------------------------------
        // check that the list is empty or not
bool narrative::isEmpty(){
    link* p;
    bool res = p->next == NULL ? true : false;
    return res;
}

// =================================GAME CLASS UI=================================
// define the game class
class Game{
    public:
        Game(string);
        int checkPoint(); // save the game info in file
        ~Game(); // calling checkPoint function
        void change_save_mode(int);
        string getEmpName();
        int getSaveMode();
        void saveRunTimeVarsTofile(string);
        int* getEffectiveParam();
        void current_check_point(narrative);
        void resetGameData();
    private:
        string emp_name;
        int save_mode = 2; // 0 means : defeated saving mode , 1 means : middle saving mode , 2 means : none of them.. program errors and crashes
        int EffectiveParam[3] = {50, 50 , 50}; // EffectiveParam[0] : people , EffectiveParam[1] : court , EffectiveParam[2] : treasury
};
// =================================GAME CLASS BACKEND=================================
// game class functions
Game::Game(string name){
    emp_name = name;
}
        // -------------------------------------------------------------------
        // get the current checkpoint
void Game::current_check_point(narrative nrtv){
    string mode = save_mode == 1 ? "middle" : "defeated";
    cout << "\t[::YOUR EMP NAME::] ==== " << emp_name << endl;
    cout << "\t[::YOUR CURRENT SAVE MODE::] ==== " << mode << endl;
    cout << "\t[::PEOPLE::] ==== " << EffectiveParam[0] << endl;
    cout << "\t[::COURT::]  ==== " << EffectiveParam[1] << endl;
    cout << "\t[::TREASURY::]  ==== " << EffectiveParam[2] << endl;
    nrtv.remainingNarrative();
}
        // -------------------------------------------------------------------
        // checkpoint algo ; it'll create the player file for the first time
        // or update the run time vars using the existing file
int Game::checkPoint(){

    int chkFlag;
    string name;
    int mode, people, court, treasury;
    string fileName = emp_name + ".txt";
    ifstream sg(fileName.c_str());
    if (sg) { // user Exists
        while (sg >> name >> mode >> people >> court >> treasury) { // save file vars into run-time vars
                emp_name = name;
                save_mode = mode;
                EffectiveParam[0] = people;
                EffectiveParam[1] = court;
                EffectiveParam[2] = treasury;
        }
        sg.close();
        chkFlag = 1; // player exits
    } else {
        if(chkFlag != 1) chkFlag = 0;
    }
       return chkFlag;
}
        // -------------------------------------------------------------------
        // save run time vars into the player file ; update it
void Game::saveRunTimeVarsTofile(string emp_name){
    string fileName = emp_name + ".txt";
    ofstream asg(fileName.c_str());
        if (asg.is_open()) {
            asg << emp_name << endl;
            asg << save_mode << endl;
            asg << EffectiveParam[0] << endl;
            asg << EffectiveParam[1] << endl;
            asg << EffectiveParam[2] << endl;
            asg.close();
        } else{
            cout << "[-] can't to open file";
        }
}
        // -------------------------------------------------------------------
        // saving the player status
Game::~Game(){
    this->checkPoint();
}
        // -------------------------------------------------------------------
        // change the status of the game ; defeated or middle exit
void Game::change_save_mode(int mode){
    save_mode = mode;
}
        // -------------------------------------------------------------------
        // return the array of effective param
int* Game::getEffectiveParam(){
    return EffectiveParam;
}
        // -------------------------------------------------------------------
        // return the save mode status
int Game::getSaveMode(){
    return save_mode;
}
        // -------------------------------------------------------------------
        // return the empire(player) name
string Game::getEmpName(){
    return emp_name;
}
        // -------------------------------------------------------------------
        // reset the game data
void Game::resetGameData(){
    save_mode = 2; // 0 means : defeated saving mode , 1 means : middle saving mode , 2 means : none of them
    EffectiveParam[0] = 50; 
    EffectiveParam[1] = 50;
    EffectiveParam[2] = 50;
    this->saveRunTimeVarsTofile(emp_name);
}
// ====================================UTILITY FUNCTIONS DECLARATION====================================
void game_process(){


        /* ---------------------------
         *  initialization process
         */

            string name;
            int answer;
            int startOver = 0;
            int* EffectiveParamPtr;
            cout << "[+] Enter Your EMP_NAME >>> ";
            getline(cin, name);
            if(name == "") name = getenv("USER");
            Game g(name);
            narrative nrtv(g.getEmpName());
            EffectiveParamPtr = g.getEffectiveParam();
            
        
         /* ---------------------------
         *  game resuming...
         */
            if(g.checkPoint() == 0){
                g.saveRunTimeVarsTofile(g.getEmpName());
                cout << "[+] new player detected ; saved your check point ...." << endl;
                cout << "[+] your current check point saved. " << endl;
            } else{
                cout << "[+] welcome back " << g.getEmpName() << ", loading your last check point ...." << endl;
                cout << "[+] your current check point saved. " << endl;
                if(g.getSaveMode() == 0){
                    cout << "[!!] last time you failed in controling your kingdom com, this time do your best." << endl;
                    g.resetGameData();
                }
            }

            g.current_check_point(nrtv);


        /* ---------------------------
        *  wait for player to press 
        */

                cin.ignore(1024, '\n');
                cout << "[*] Press enter to continue...";
                cin.get();


        /* ---------------------------
        *  our narrative happening !
        */
                system("clear");
            while(true){
                    // ===========================================================
                    // if the list was empty just remove fill it 
                    // again but remove the last saved files
                if(nrtv.isEmpty()){
                    cout <<"\n\n\t[*] there is no remaining issue in your kingdom com, overloading game..." << endl;
                    string cmd = "rm -r ";
                    cmd = cmd + "*" + g.getEmpName() + "_list.txt";
                    const char *command = cmd.c_str(); 
                    system(command);
                    narrative nrtv(g.getEmpName());
                }
                    // ===========================================================
                cout << "\n\t[( -1 to stop playing | 3 to start a new game | 4 play  )]\n\n";
                cin >> answer;
                    // ===========================================================
                    // player wants to quit the game in the middle of playing
                if(answer == -1){
                    system("clear");
                    g.change_save_mode(1); // middle mode
                    nrtv.saveRemainingNarrative();
                    g.saveRunTimeVarsTofile(g.getEmpName());
                    cout << "[+] your current check point has been saved. " << endl;
                    g.current_check_point(nrtv);
                    exit(1);
                } else if(answer == 3){
                    // player wants to start a new game 
                    // remove the old files and start the new one
                    system("clear");
                    string filenameToRemove = "rm -r " + g.getEmpName() + ".txt"; 
                    const char *ftr = filenameToRemove.c_str(); 
                    system(ftr);
                    string cmd = "rm -r ";
                    cmd = cmd + "*" + g.getEmpName() + "_list.txt";
                    const char *command = cmd.c_str(); 
                    system(command);
                    g.resetGameData();
                    g.current_check_point(nrtv);
                    cout << "[+] restart the game ..." << endl;
                    startOver = 1;
                    exit(1);
                } else if(answer == 4){
                    // player wants to play in the narratives
                    system("clear");
                    cin.ignore(1024, '\n');
                    cout << "[*] Press enter to continue...";
                    cin.get();
                    nrtv.chooseRandomNarrative(EffectiveParamPtr);
                    nrtv.saveRemainingNarrative();
                    g.saveRunTimeVarsTofile(g.getEmpName());
                    
                } else{
                    // wrong the answers
                    system("clear");
                    cout << "[!] incorrect answer, try again!" << endl; 
                }
                    // ===========================================================
                    // check that player is defeated or not
                EffectiveParamPtr = g.getEffectiveParam();
                if(EffectiveParamPtr[0] == 0 ||
                   EffectiveParamPtr[1] == 0 ||
                   EffectiveParamPtr[2] == 0 || 
                  (EffectiveParamPtr[0] + EffectiveParamPtr[1] + EffectiveParamPtr[2])/3 < 10){
                        g.change_save_mode(0); // defeated mode
                }
                    // ===========================================================
                    // if he/she was defeated just remove all saved narrative files
                if(g.getSaveMode() == 0 && startOver != 1){
                    cout << "[+] Sorry , GAME OVER" << endl;
                    nrtv.saveRemainingNarrative();
                    g.saveRunTimeVarsTofile(g.getEmpName());
                    g.current_check_point(nrtv);
                    string cmd = "rm -r ";
                    cmd = cmd + "*" + g.getEmpName() + "_list.txt";
                    const char *command = cmd.c_str(); 
                    system(command);
                    cout << "[+] your current check point has been saved. " << endl;
                    exit(1);
                }
            }            

    }
    
// ------------------------------MAIN & INITIALIZATION------------------------------
int main(){
    
    game_process(); // calling the game process
    return 0;
}

