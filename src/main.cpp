#include <iostream>
#include <Windows.h>
#include <time.h>
#include <iomanip>
#include <chrono>
#include <thread>
#include <winuser.h>
#include <fstream>
#include <cstring>


#define SLOWNESS 8000

using namespace std;

const int minWheelMovement = 120;
struct Coords
{
    string username;
    POINT join;
    POINT Audio;
    POINT joinMeeting;
    POINT clickChat;
    POINT clickBox;
    POINT clickLeave;
};



inline bool exists (const std::string& name)
{ifstream f(name.c_str());return f.good();}


void joinClass(Coords joinCoords, int howSlow, const string message, int classDuration);
void leaveClass(Coords leave,int sleepDuration, int slowness);
void click(int xCord, int yCord, int waitDuration);
void move_mouse(int xCord, int yCord);
void makeExcuse(const string message);
void properSleepTimer(int duration);
void initProgram(Coords &points);
void scroll(int howMuch);
bool isThereClass();
int askForInput();
int waitforEndofClass();


int main()
{
    string check = "Data.txt";
    bool firstTime = exists(check);
    Coords MainCoords;
    if (!firstTime)
    {
        cout << "please, what is your desktop username(copy paste it preferably)?" << endl;
        cin >> MainCoords.username;
        ofstream fs("Data.txt");
        fs << MainCoords.username << "\n";
        cout << "After each hover click on numpad 1" << endl;
        cout << "please open teams and hover on where the join button would be and press on numpad 1" << endl;
        cout << "and then hover on where the option to join without audio would be" << endl;
        cout << "and then hover on where the join class button would be." << endl;
        cout << "and then hover on where the chat button would be" << endl;
        cout << "and then hover on where the text box for chatting would be" << endl;
        cout << "and then hover on where exiting the meeting button would be" << endl;
        cout << "Once you're done, press numpad 0: " << endl;
             while (true)
        {
            Sleep(50);
            if (GetAsyncKeyState(VK_NUMPAD0))   // Exit
            {
                fs.close();
                return 0;
            }
            if (GetAsyncKeyState(VK_NUMPAD1))  //Mouseposition
            {
                POINT p;
                GetCursorPos(&p);
                fs << p.x << " " << p.y << endl;
                Sleep(1000);
            }

        }
    }
    else
        {
        initProgram(MainCoords);
        if (isThereClass())
        {
            int duration = waitforEndofClass();
            leaveClass(MainCoords, duration, SLOWNESS);
        }
        else
        {
            joinClass(MainCoords, SLOWNESS, "Hey professor, I apologize but I have mic problems.", 70);
        }
        return 0;
    }
}
void initProgram(Coords &points)
{
    ifstream readCoords("Data.txt");
    readCoords >> points.username;
    readCoords >> points.join.x >> points.join.y;
    readCoords >> points.Audio.x >> points.Audio.y;
    readCoords >> points.joinMeeting.x >> points.joinMeeting.y;
    readCoords >> points.clickBox.x >> points.clickBox.y;
    readCoords >> points.clickChat.x >> points.clickChat.y ;
    readCoords >> points.clickLeave.x >> points.clickLeave.y;
}

void move_mouse(int xCord, int yCord)
{
    int screen_x = GetSystemMetrics(SM_CXSCREEN);
    int screen_y = GetSystemMetrics(SM_CYSCREEN);
    long xConverted = (65535 * (xCord)) / screen_x;
    long yConverted = (65535 * (yCord)) / screen_y;
    INPUT iNPUT = { 0 };
    iNPUT.type = INPUT_MOUSE;
    iNPUT.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
    iNPUT.mi.dx = xConverted;
    iNPUT.mi.dy = yConverted;
    SendInput(1, &iNPUT, sizeof(iNPUT));
}

void scroll(int howMuch)
{
    mouse_event(MOUSEEVENTF_WHEEL, 0,0,howMuch/2*minWheelMovement, 0);
    mouse_event(MOUSEEVENTF_WHEEL, 0,0,howMuch/2*minWheelMovement, 0);
}

void click(int xCord, int yCord, int waitDuration)
{
    move_mouse(xCord, yCord);
    //wait a bit (for bad computers/connections)
    Sleep(waitDuration);
    INPUT iNPUT = { 0 };
    iNPUT = { 0 };
    iNPUT.type = INPUT_MOUSE;
    iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &iNPUT, sizeof(iNPUT));
    ZeroMemory(&iNPUT, sizeof(iNPUT));
    iNPUT.type = INPUT_MOUSE;
    iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &iNPUT, sizeof(iNPUT));
}
bool isThereClass()
{
    int i;
    cout << "Please keep your Teams open on the channel of the meeting.\nJoin or leave a meeting?\n1.Leave\n2.Join" << endl;
    cin >> i;
    return i == 1;
}

void joinClass(Coords joinCoords, int howSlow, const string message, int classDuration)
{
    char a;
    cout << "excuse? y/n"<< endl;
    cin >> a;
    int sleepDuration= askForInput();
    properSleepTimer(sleepDuration-1);
    string dir = "C:/Users/" + joinCoords.username + "/AppData/Local/Microsoft/Teams/previous/Teams.exe";
    system(dir.c_str());
    int screen_x = GetSystemMetrics(SM_CXSCREEN);
    int screen_y = GetSystemMetrics(SM_CYSCREEN);
    move_mouse((screen_x/2), (screen_y/2));
    scroll(10);
    click(joinCoords.join.x,joinCoords.join.y,howSlow);
    click(joinCoords.Audio.x,joinCoords.Audio.y,howSlow);
    click(joinCoords.joinMeeting.x,joinCoords.joinMeeting.y,howSlow);
    if ( a == 'y')
    {
        Sleep(howSlow);
        click(joinCoords.clickChat.x,joinCoords.clickChat.y,howSlow);
        click(joinCoords.clickBox.x,joinCoords.clickBox.y,howSlow);
        Sleep(howSlow);
        makeExcuse(message);
    }
    leaveClass(joinCoords, classDuration, SLOWNESS);
}

void makeExcuse(const string message)
{
    for (char a: message)
    {
        if (a == ' ')
        {
            keybd_event(VK_SPACE,0x9C,0,0); //  Press
            keybd_event(VK_SPACE,0x9C,KEYEVENTF_KEYUP,0); // Release
        }
        else
        {
            keybd_event(VkKeyScan(a),0x9e,0, 0);  //  Press
            keybd_event(VkKeyScan(a),0x9e, KEYEVENTF_KEYUP,0); // Release
        }

    }
    keybd_event(VK_RETURN,0x9C, 0, 0);
    keybd_event(VK_RETURN,0x9C,KEYEVENTF_KEYUP,0);
}

void leaveClass(Coords leave,int sleepDuration, int slowness)
{
    properSleepTimer(sleepDuration);
    click(leave.clickLeave.x,leave.clickLeave.y,slowness);
}


int waitforEndofClass()
{
    cout << "What is the current hour?" << endl;
    int currHour;
    cin >> currHour;
    cout << "How many minutes past " << currHour << " is it?" <<endl;
    int currMins;
    cin >> currMins;
    cout << "At what hour is the meeting going to end?" << endl;
    int destHour;
    cin >> destHour;
    cout << "How many minutes past " << destHour << " does the meeting end at?" <<endl;
    int destMins;
    cin >> destMins;
    int sleephrs;
    int sleepmins;
    int sleeptotal;
    if (destHour < currHour)
    {
        if(currHour != destHour)
        {
            sleephrs  = (abs(currHour - (destHour+12))-1);
            sleepmins =  (60-currMins)+destMins;
        }
        else
        {
            sleephrs  = (abs(currHour - (destHour+12)));
            sleepmins = abs(currMins-destMins);

        }
        sleeptotal= sleephrs* 60 + sleepmins;
    }
    else
    {
        if(currHour != destHour)
        {
            sleephrs  = (abs(currHour - (destHour))-1);
            sleepmins =  (60-currMins)+destMins;
        }
        else
        {
            sleephrs  = (abs(currHour - (destHour)));
            sleepmins = abs(currMins-destMins);
        }
        sleeptotal= sleephrs* 60 + sleepmins;
    }
    cout << "The meeting will be left in " << sleephrs << " hours and " << sleepmins << " minutes. Thank you for your patience." << endl;
    return sleeptotal;
}


int askForInput()
{
    cout << "What is the current hour?" << endl;
    int currHour;
    cin >> currHour;
    cout << "How many minutes past " << currHour << " is it?" <<endl;
    int currMins;
    cin >> currMins;
    cout << "At what hour is the meeting?" << endl;
    int destHour;
    cin >> destHour;
    cout << "How many minutes past " << destHour << " does the meeting start at?" <<endl;
    int destMins;
    cin >> destMins;
    int sleephrs;
    int sleepmins;
    int sleeptotal;
    if (destHour < currHour)
    {
        if(currHour != destHour)
        {
            sleephrs  = (abs(currHour - (destHour+12))-1);
            sleepmins =  (60-currMins)+destMins;
        }
        else
        {
            sleephrs  = (abs(currHour - (destHour+12)));
            sleepmins = abs(currMins-destMins);

        }
        sleeptotal= sleephrs* 60 + sleepmins;
    }
    else
    {
        if(currHour != destHour)
        {
            sleephrs  = (abs(currHour - (destHour))-1);
            sleepmins =  (60-currMins)+destMins;
        }
        else
        {
            sleephrs  = (abs(currHour - (destHour)));
            sleepmins = abs(currMins-destMins);
        }
        sleeptotal= sleephrs* 60 + sleepmins;
    }
    cout << "The meeting will be joined in " << sleephrs << " hours and " << sleepmins << " minutes. Thank you for your patience." << endl;
    return sleeptotal;
}


void properSleepTimer(int duration)
{
    for(int i=1; duration != 0; duration--)
    {
        cout << "Remaining Time: " << duration/60 <<"Hours and " << duration%60 << "Mintues."<< endl;
        this_thread::sleep_for(chrono::minutes(i));
    }
}
