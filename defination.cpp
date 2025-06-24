#include"header.h"
//Abstract Base Class: GameObject
GameObject::GameObject(int x, int y) : x(x), y(y) {}
GameObject::~GameObject() {}
int GameObject::getX() const { return x; }
int GameObject::getY() const { return y; }
void GameObject::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}
//Player Class: Cheeseburger
Cheeseburger::Cheeseburger(int x, int y) : GameObject(x, y), lives(3), score(0), speed(1), shieldActive(false), shieldActivationTime(0), multiplierActive(false), multiplierActivationTime(0) {}
void Cheeseburger::move(char direction) {
    if (direction == 75 && x > 0) x -= speed; // Move left
    if (direction == 77 && x < 29) x += speed; // Move right (width = 30)

}
void Cheeseburger::activateMultiplier(int t) {
    cout << "\t\t\t\t\t\033[31mMultiplier activated...\033[0m\n";
    multiplierActive = true;
    multiplierDuration = t;
    multiplierActivationTime = GetTickCount64(); // Record the time
}
void Cheeseburger::updateMultiplier() {
    if (multiplierActive && GetTickCount64() - multiplierActivationTime >= multiplierDuration) {
        cout << "\t\t\t\t\t\033[31mDeactivate MUliplier!!!\033[0m" << endl;
        multiplierActive = false;
    }
}
void Cheeseburger::activateShield(int t) {
 
    shieldActive = true;
    shieldDuration = t;
    shieldActivationTime = GetTickCount64(); // Record the time when shield is activated
}

void Cheeseburger::gainScore(int points) {
    if (multiplierActive) {
        points *= 2; // Double the score
    }
    score += points;
}
void Cheeseburger::loseLife() {
    if (shieldActive) {
        shieldActive = false;// deactivateShield();
    }
    else {
       
        lives--;
        if (lives < 0) lives = 0; // Ensure lives do not go below 0
    }
}
void Cheeseburger::updateShield() {
    if (shieldActive) {
        cout << "\t\t\t\t\t\033[40m\033[31mShield is Activated...\033[0m" << endl;
        if (GetTickCount64() - shieldActivationTime >= shieldDuration) {
      
            shieldActive = false;// deactivateShield();
            cout << "\t\t\t\t\t\033[40m\033[31mShield is Deactivated!!!\033[0m" << endl;
        }
    }
}
int Cheeseburger::getLives() const { return lives; }
int Cheeseburger::getScore() const { return score; }
void Cheeseburger::set() {
    score = 0;
    lives = 3;
}
void Cheeseburger::draw() {
    cout << "\033[33mC\033[0m";
}
void Cheeseburger::move() {} // Unused in this context

//Obstacle Class: NyanCat
NyanCat::NyanCat(int x, int y, int speed) : GameObject(x, y), fallSpeed(speed) {}
void NyanCat::move() {
    y += fallSpeed; // Fall down
    if (y > 14) { // Reset to top (height = 15)
        y = 0;
        x = rand() % 30; // Random new position (width = 30)
    }
}
void NyanCat::setSpeed(int speed) { fallSpeed = speed; } // Adjust speed dynamically
int NyanCat::getSpeed() const { return fallSpeed; }
void NyanCat::draw() {
    cout << "\033[91mN\033[0m";
}

//PowerUp Class
PowerUp::PowerUp(int x, int y, string type) : GameObject(x, y), type(type) {}
void PowerUp::draw() {
    if (type == "Shield") {
        cout << "\033[34mS\033[0m";
    }
    else if (type == "Score") {
        cout << "\033[32m$\033[0m";
    }
    else if (type == "Multiplier") {
        cout << "\033[36m*\033[0m";
    }

}
void PowerUp::move() {
    y++; // Fall down
    if (y > 14) { // Reset to top (height = 15)
        y = 0;
        x = rand() % 30; // Random new position (width = 30)
    }
}
string PowerUp::getType() const { return type; }

//Game Class
void Game::render() {
    system("cls");
    // Display score, lives, and level at the top

    cout << "\n\n\n\n\n\t\t\t\t\t\t";
    if (!gameMode) {
        cout << "\033[33m   Easy Mode\033[0m\n";
    }
    else {
        cout << "\033[31m   Hard Mode\033[0m\n";
    }
    cout << "\t\t\t\t\t\033[92mScore: \033[33m" << player->getScore() << "\033[92m | Lives: \033[33m";
    string C;
    if (player->getLives() <= 1) {
        C = "\033[91m";
    }
    else {
        C = "\033[33m"; // Yellow || Red for lives
    }
    cout << C << (isGameOver ? 0 : player->getLives()) << "\033[92m | Level: \033[33m" << level << "\033[0m\n\t\t\t\t\t";
    // Top border
    for (int i = 0; i < 32; i++) { // 30 width + 2 for the border
        cout << "\033[95m" << (i == 0 || i == 31 ? "+" : "-") << "\033[0m";
    }
    cout << endl;
    //Game area with side borders
    for (int i = 0; i < 15; i++) { // Screen height = 15
        cout << "\t\t\t\t\t\033[95m|\033[0m"; // Left border
        for (int j = 0; j < 30; j++) { // Screen width = 30
            if (isGameOver && i == 7 && j == 9) { // Centered at row 7 and column 9
                cout << "\033[91mGAME OVER!\033[0m";
                j += 9; // Skip the length of the message to align properly
            }
            else if (isGameOver && i == 8 && j == 9) { // Second line for final score
                cout << "\033[92mScore: " << player->getScore() << "\033[0m";
                j += ((player->getScore() == 0) ? 7 : ((player->getScore() < 100) ? 8 : 9));// Skip the length of the score message
            }
            else {
                //  Draw Cheese Burger
                if (player->getX() == j && player->getY() == i && !isGameOver) {
                    player->draw();
                }
                //Draw NyanCats
                bool drawn = false;
                for (int k = 0; k < 5; k++) {
                    if (nyanCats[k]->getX() == j && nyanCats[k]->getY() == i && !isGameOver) {
                        nyanCats[k]->draw();
                        drawn = true;
                        break;
                    }
                }
                // Draw PowerUps
                for (int k = 0; k < 3; k++) {
                    if (powerUps[k]->getX() == j && powerUps[k]->getY() == i && !isGameOver) {
                        powerUps[k]->draw();
                        drawn = true;
                        break;
                    }
                }
                if (!isGameOver && !drawn && i == 14 && j == 29) {
                    cout << "";
                }
                else if (!drawn)
                    cout << " "; // Empty space
            }
        }
        cout << "\033[95m|\033[0m"; // Right border
        cout << endl;
    }
    // Bottom border
    cout << "\t\t\t\t\t";
    for (int i = 0; i < 32; i++) { // 30 width + 2 for the border
        cout << "\033[95m" << (i == 0 || i == 31 ? "+" : "-") << "\033[0m";
    }
    cout << endl;
    cout << "\t\t\t\t\t\033[40m\033[48;5232mPress Esc to Exit the game...\n\t\t\t\t\t\033[36mPress (P or p) for pause the game..\033[0m" << endl;
    // Display shield status when the game is not over
    if (!isGameOver) {
        player->updateShield();
        player->updateMultiplier();
    }
    else {
       
    }
}
void Game::update() {
    // Move NyanCats and PowerUps
    for (int i = 0; i < 5; i++) nyanCats[i]->move();
    for (int i = 0; i < 3; i++) powerUps[i]->move();
    //Check collisions (simplified)
    for (int i = 0; i < 5; i++) {
        if (level > 2) {// Check if NyanCat is at the player's position
            if (nyanCats[i]->getX() == player->getX() && nyanCats[i]->getY() == player->getY()) {
                player->loseLife();
                if (player->getLives() <= 0) isGameOver = true;
            }
            //  Check if NyanCat's movement passes through the player's position
            else {
                // Check if the NyanCat's Y position has passed the player
                if (nyanCats[i]->getY() >= player->getY() - nyanCats[i]->getSpeed() && nyanCats[i]->getY() <= player->getY()) {
                    if (nyanCats[i]->getX() == player->getX()) {
                        player->loseLife();
                        if (player->getLives() <= 0) isGameOver = true;
                    }
                }
            }
        }
        else {
            if (nyanCats[i]->getX() == player->getX() && nyanCats[i]->getY() == player->getY()) {
                player->loseLife();
                if (player->getLives() <= 0) isGameOver = true;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        if (powerUps[i]->getX() == player->getX() && powerUps[i]->getY() == player->getY()) {
            if (powerUps[i]->getType() == "Shield") {
                if (!gameMode)
                    player->activateShield(4000);
                else
                    player->activateShield(3000);
            }
            else if (powerUps[i]->getType() == "Score") {
             
                if (!gameMode) {
                    player->gainScore(10);
                }
                else {
                    player->gainScore(5);
                }
            }
            else if (powerUps[i]->getType() == "Multiplier") {
         
                if (!gameMode)
                    player->activateMultiplier(4000); // Activate multiplier
                else
                    player->activateMultiplier(3000);
            }

        }
    }
    // Level up based on score
    if (player->getScore() >= level * 50) {
    
        level++;
        for (int i = 0; i < 5; i++) {
            nyanCats[i]->setSpeed(nyanCats[i]->getSpeed() + 1);
        }
    }
}
Game::Game() : isGameOver(false), gameMode(false), level(1) {
    player = new Cheeseburger(15, 14); // Center bottom
    for (int i = 0; i < 5; i++) {
        nyanCats[i] = new NyanCat(rand() % 30, rand() % 15, (rand() % 2) + 1); // Random positions
    }
    for (int i = 0; i < 4; i++) {
        if (i % 3 == 0) {
            powerUps[i] = new PowerUp(rand() % 30, rand() % 15, "Shield");
        }
        else if (i % 3 == 1) {
            powerUps[i] = new PowerUp(rand() % 30, rand() % 15, "Score");
        }
        else {
            powerUps[i] = new PowerUp(rand() % 30, rand() % 15, "Multiplier");
        }
    }

}
Game::~Game() {
    delete player;
    for (int i = 0; i < 5; i++) delete nyanCats[i];
    for (int i = 0; i < 4; i++) delete powerUps[i];

}
void Game::run() {
    while (!isGameOver) {
        render();
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'P' || ch == 'p') {

                cout << "\t\t\t\t\t\033[31mGame Paused.\n\t\t\t\t\t\033[35mPress '(R or r)' to resume.\033[0m" << endl;
                while (_getch() != 'r' && _getch() != 'R');
            }
            else if (ch == 27)
                break; // Escape key to quit
            player->move(ch);
        }
        update();
        Sleep(100);
    }

    render(); // Final render for "Game Over" screen

}
void Game::reset() {
    isGameOver = false;
    level = 1;
    player->set();
}
void Game::saveScore() {
    ofstream write;
    string filename;

    // Check the game mode and determine the file name
    if (!gameMode) {
        filename = "Scores_Essay.txt";
    }
    else {
        filename = "Scores_Hard.txt";
    }

    // Open the appropriate file in append mode
    write.open(filename, ios::app);
    if (!(write.is_open())) {
        cout << "Error...Not open.\n";
        return;
    }

    // Save the player's score to the file
    write << player->getScore() << endl;
    write.close();

    // After saving the score, maintain the top 5 scores
    int scores[5] = { 0 };  // Array to store the top 5 scores
    int count = 0;

    // Read the scores from the file
    ifstream read(filename);
    if (!read.is_open()) {
        cout << "Error...Not open.\n";
        return;
    }

    // Read scores into the array
    int score;
    while (read >> score) {
        if (count < 5) {
            scores[count] = score;
            count++;
        }
        else {
            // Find the smallest score in the array
            int minScoreIndex = 0;
            for (int i = 1; i < 5; i++) {
                if (scores[i] < scores[minScoreIndex]) {
                    minScoreIndex = i;
                }
            }

            // If the new score is higher than the smallest score, replace it
            if (score > scores[minScoreIndex]) {
                scores[minScoreIndex] = score;
            }
        }
    }
    read.close();

    // Sort the scores in descending order
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (scores[i] < scores[j]) {
                int temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Write the top 5 scores back into the file
    write.open(filename, ios::trunc);  // Clear the file first
    if (!write.is_open()) {
        cout << "Error...Not open.\n";
        return;
    }

    // Write the top 5 scores back into the file
    for (int i = 0; i < 5; i++) {
        write << scores[i] << endl;
    }
    write.close();
}


void Game::displayScore() {
    ifstream read;
    string filename;

    // Check the game mode and determine the file name
    if (!gameMode) {
        filename = "Scores_Essay.txt";
    }
    else {
        filename = "Scores_Hard.txt";
    }

    // Open the appropriate file
    read.open(filename);
    if (!(read.is_open())) {
        cout << "Error...Not open.\n";
        return;
    }

    int scores[5] = { 0 };  // Array to store the top 5 scores
    int count = 0;

    // Read scores into the array
    int score;
    while (read >> score) {
        if (count < 5) {
            scores[count] = score;
            count++;
        }
        else {
            // Find the smallest score in the array
            int minScoreIndex = 0;
            for (int i = 1; i < 5; i++) {
                if (scores[i] < scores[minScoreIndex]) {
                    minScoreIndex = i;
                }
            }

            // If the new score is higher than the smallest score, replace it
            if (score > scores[minScoreIndex]) {
                scores[minScoreIndex] = score;
            }
        }
    }
    read.close();

    // Sort the scores in descending order
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (scores[i] < scores[j]) {
                int temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Display the top 5 scores
    cout << "\t\t\t\t\t\t\033[31mTop 5 Scores for ";
    if (!gameMode) {
        cout << "Essay Mode:\n";
    }
    else {
        cout << "Hard Mode:\n";
    }
    cout << "\033[0m";
    int countDisplay = 1;
    for (int i = 0; i < 5; i++) {
        if (scores[i] > 0) {
            cout << "\t\t\t\t\t\033[32m" << countDisplay++ << ". \033[33m" << scores[i] << "\033[0m" << endl;
        }
    }
}


void Game::Menu() {
  
    cout << "\n\n\t\t" << "\033[31m                                                                          d8P  " << endl; Sleep(100);
    cout << "\t\t" << "                                                                       d888888P" << endl; Sleep(100);
    cout << "\t\t" << "  88bd88b  d888b8b  ?88   d8P  d888b8b    88bd88b      d8888b  d888b8b    ?88'  " << endl; Sleep(100);
    cout << "\t\t" << "  88P' ?8b d8P' ?88  d88   88  d8P' ?88    88P' ?8b    d8P' `  d8P' ?88   `88P   " << endl; Sleep(100);
    cout << "\t\t" << " d88   88P 88b  ,88b ?8(  d88  88b  ,88b  d88   88P    88b     88b  ,88b  ?88b   " << endl; Sleep(100);
    cout << "\t\t" << "d88'   88b`?88P'`88b`?88P'?8b `?88P'`88bd88'   88b    `?888P'  ?88P'`88b  `?8b  " << endl; Sleep(100);
    cout << "\t\t" << "                           )88                                                  " << endl; Sleep(100);
    cout << "\t\t" << "                          ,d8P                                                  " << endl; Sleep(100);
    cout << "\t\t" << "                       `?888P'                                                  " << endl; Sleep(100);
    cout << "\n\t    " << "\033[32m                                   ?88   d8P     .d888b," << endl; Sleep(100);
    cout << "\t    " << "                                   d88  d8P'     ?8b,   " << endl; Sleep(100);
    cout << "\t    " << "                                   ?8b ,88'        `?8b " << endl; Sleep(100);
    cout << "\t    " << "                                   `?888P'      `?888P' " << endl; Sleep(100);

    cout << "\n\t" << "\033[33m        d8b                                        d8b                                                  " << endl; Sleep(100);
    cout << "\t" << "        ?88                                        ?88                                                  " << endl; Sleep(100);
    cout << "\t" << "         88b                                        88b                                                 " << endl; Sleep(100);
    cout << "\t" << " d8888b  888888b  d8888b d8888b .d888b, d8888b      888888b ?88   d8P  88bd88b d888b8b   d8888b  88bd88b" << endl; Sleep(100);
    cout << "\t" << "d8P' `P  88P `?8 d8b_,dPd8b_,dP ?8b,   d8b_,dP      88P `?8b 88   88   88P'  `d8P' ?88  d8b_,dP  88P'  `" << endl; Sleep(100);
    cout << "\t" << "88b     d88   88 88b    88b       `?8b 88b         d88,  d88 8(  d88  d88     88b  ,88b 88b     d88     " << endl; Sleep(100);
    cout << "\t" << "'?888P`d88'   88b'?888P'`?888P'`?888P' `?888P'    d88'`?88P' ?88P'?8bd88`     `?88P'`88b`?888P'd88'     " << endl; Sleep(100);
    cout << "\t" << "                                                                                     )88                " << endl; Sleep(100);
    cout << "\t" << "                                                                                    ,88P                " << endl; Sleep(100);
    cout << "\t" << "                                                                                `?8888P                 \033[0m" << endl; Sleep(2000);

    char choice;
    do {
        system("cls");
        cout << "\n\n\n\n\n";
        for (int i = 0; i < 15; i++) {
            cout << "\t\t\t\t\t";
            for (int j = 0; j < 32; j++) {
                if (i == 0 && j == 0 || i == 14 && j == 31 || i == 0 && j == 31 || i == 14 && j == 0) {
                    cout << "\033[95m+\033[0m";
                }
                else if (i == 0 && j > 0 && j < 31 || i == 14 && j>0 && j < 31) {
                    cout << "\033[95m-\033[0m";
                }
                else if (j == 0 && i > 0 && i < 14 || j == 31 && i>0 && i < 14) {
                    cout << "\033[95m|\033[0m";
                }
                else {
                    if (i == 2 && j == 3) {
                        cout << "\033[31mNayanCat \033[32m.Vs.\033[33m CheeseBurger\033[0m";
                        j += 25;
                    }
                    else if (i == 4 && j == 13) {
                        cout << "\033[32mMenu\033[0m";
                        j = j + 3;
                    }
                    else if (i == 6 && j == 1) {
                        cout << "\033[33m1   : \033[36mPlay Game\033[0m";
                        j += 14;
                    }
                    else if (i == 7 && j == 1) {
                        cout << "\033[33m2   : \033[36mInstructions\033[0m";
                        j += 17;
                    }
                    else if (i == 8 && j == 1) {
                        cout << "\033[33m3   : \033[36mCredit\033[0m";
                        j += 11;
                    }
                    else if (i == 9 && j == 1) {
                        cout << "\033[33m4   : \033[36mTop Score\033[0m";
                        j += 14;
                    }
                    else if (i == 10 && j == 1) {
                        cout << "\033[33mEsc : \033[36mExit\033[0m";
                        j += 9;
                    }
                    else if (i == 12 && j == 1) {
                        cout << "\033[31mPress key according to Menu...\033[0m";
                        j += 29;
                    }
                    else {
                        cout << " ";
                    }
                }
            }
            cout << endl;
        }
        // Exception handling
        try {
            choice = _getch();
            switch (choice) {
            case '1':
                system("cls");
                cout << "\n\n\n\n\n";
                for (int i = 0; i < 15; i++) {
                    cout << "\t\t\t\t\t";
                    for (int j = 0; j < 32; j++) {
                        if (i == 0 && j == 0 || i == 14 && j == 31 || i == 0 && j == 31 || i == 14 && j == 0) {
                            cout << "\033[95m+\033[0m";
                        }
                        else if (i == 0 && j > 0 && j < 31 || i == 14 && j>0 && j < 31) {
                            cout << "\033[95m-\033[0m";
                        }
                        else if (j == 0 && i > 0 && i < 14 || j == 31 && i>0 && i < 14) {
                            cout << "\033[95m|\033[0m";
                        }
                        else {
                            if (i == 2 && j == 7) {
                                cout << "\033[33mGame Mode\033[0m";
                                j += 8;
                            }
                            else if (i == 5 && j == 1) {
                                cout << "\033[34m1 : Essay mode\033[0m";
                                j += 13;
                            }
                            else if (i == 6 && j == 1) {
                                cout << "\033[31m2 : Hard mode\033[0m";
                                j += 12;
                            }
                            else {
                                cout << " ";
                            }
                        }
                    }
                    cout << endl;
                }

                char c;
                c = _getch();
                if (c == '1') {
                    gameMode = false;
                    run();
                }
                else if (c == '2') {
                    gameMode = true;
                    run();
                }
                saveScore();
                reset();
                break;
            case '2':
                system("cls");
                cout << "\n\n\n\n\n";
                for (int i = 0; i < 15; i++) {
                    cout << "\t\t\t\t\t";
                    for (int j = 0; j < 32; j++) {
                        if (i == 0 && j == 0 || i == 14 && j == 31 || i == 0 && j == 31 || i == 14 && j == 0) {
                            cout << "\033[95m+\033[0m";
                        }
                        else if (i == 0 && j > 0 && j < 31 || i == 14 && j>0 && j < 31) {
                            cout << "\033[95m-\033[0m";
                        }
                        else if (j == 0 && i > 0 && i < 14 || j == 31 && i>0 && i < 14) {
                            cout << "\033[95m|\033[0m";
                        }
                        else {
                            if (i == 2 && j == 8) {
                                cout << "\033[31mInstructions\033[0m";
                                j += 11;
                            }
                            else if (i == 3 && j == 1) {
                                cout << "\033[34m1)\033[32mCheeseBuger(C),NayanCat(N)\033[0m";
                                j += 27;
                            }
                            else if (i == 4 && j == 1) {
                                cout << "\033[32mShield(S),Score($)\033[0m";
                                j += 17;
                            }
                            else if (i == 5 && j == 1) {
                                cout << "\033[34m2)\033[32mSave CheeseBuger from\033[0m";
                                j = j + 22;
                            }
                            else if (i == 6 && j == 1) {
                                cout << "\033[32mNayanCat & collect Score\033[0m";
                                j += 23;
                            }
                            else if (i == 7 && j == 1) {
                                cout << "\033[34m3)\033[32m->Key used to move Right\033[0m";
                                j += 25;
                            }
                            else if (i == 8 && j == 1) {
                                cout << "\033[32mand <-to left for CheeseBurger\033[0m";
                                j += 29;
                            }
                            else if (i == 9 && j == 1) {
                                cout << "\033[34m4)\033[32mShield Duration 4 sec..\033[0m";
                                j += 24;
                            }
                            else if (i == 10 && j == 1) {
                                cout << "\033[34m5)\033[32mScore($) add by 10..\033[0m";
                                j += 21;
                            }
                            else if (i == 11 && j == 1) {
                                cout << "\033[34m6)\033[32mLevel Increase based\033[0m";
                                j += 21;
                            }
                            else if (i == 12 && j == 1) {
                                cout << "\033[32mon score($) by 50..\033[0m";
                                j += 18;
                            }
                            else {
                                cout << " ";
                            }
                        }
                    }
                    cout << endl;
                }
                break;
            case '3':
                system("cls");
                cout << "\n\n\n\n\n";
                for (int i = 0; i < 15; i++) {
                    cout << "\t\t\t\t\t";
                    for (int j = 0; j < 32; j++) {
                        if (i == 0 && j == 0 || i == 14 && j == 31 || i == 0 && j == 31 || i == 14 && j == 0) {
                            cout << "\033[95m+\033[0m";
                        }
                        else if (i == 0 && j > 0 && j < 31 || i == 14 && j>0 && j < 31) {
                            cout << "\033[95m-\033[0m";
                        }
                        else if (j == 0 && i > 0 && i < 14 || j == 31 && i>0 && i < 14) {
                            cout << "\033[95m|\033[0m";
                        }
                        else {
                            if (i == 2 && j == 11) {
                                cout << "\033[33mCredit\033[0m";
                                j += 5;
                            }
                            else if (i == 5 && j == 10) {
                                cout << "\033[32m23F-0674\033[0m";
                                j += 7;
                            }
                            else if (i == 6 && j == 6) {
                                cout << "\033[32mMalik Kamran Ali\033[0m";
                                j += 15;
                            }
                            else {
                                cout << " ";
                            }
                        }
                    }
                    cout << endl;
                }
                break;
            case '4':
                system("cls");
                cout << "\n\n\n\n\n";
                cout << "\t\t\t\t\t\033[95m+----------------------------+\033[0m" << endl;
                gameMode = false;
                displayScore();
                cout << endl;
                gameMode = true;
                displayScore();
                cout << endl;
                cout << "\t\t\t\t\t\033[95m+----------------------------+\033[0m" << endl;
                break;
            case 27:
                break;
            default:
                throw runtime_error("Press Invalid Key..");
            }
        }
        catch (const runtime_error& c) {
            cout << "\t\t\t\t\t\033[31mException Error: " << c.what() << "\033[0m" << endl;
        }
        if (choice != 27) {
            cout << "\t\t\t\t\t\033[40m\033[33mPress Backspace key to back Menu...\033[0m" << endl;
            while (_getch() != 8) {}
        }

    } while (choice != 27);
}
