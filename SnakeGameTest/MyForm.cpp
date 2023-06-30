#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;
using namespace System::Windows::Forms;

// Constants
const int gameWidth = 500;
const int gameHeight = 500;
const int blockSize = 20;
const int initialSnakeSize = 3;

// Enum for directions
enum class Direction { Up, Down, Left, Right };

public ref class SnakeGameForm : public System::Windows::Forms::Form
{
private:
    Graphics^ graphics;
    Timer^ timer;
    Direction direction;
    List<System::Drawing::Rectangle>^ snake;
    int snakeLength;
    System::Drawing::Rectangle appleRect;
    bool gameRunning;
    int score;
    System::ComponentModel::Container^ components;
    System::Windows::Forms::TrackBar^ speedTrackBar; // TrackBar control for speed

public:
    SnakeGameForm()
    {
        InitializeComponent();
        // Set up the game area and timer
        graphics = gameArea->CreateGraphics();
        timer = gcnew Timer();
        timer->Tick += gcnew EventHandler(this, &SnakeGameForm::timer_Tick);
        // Set initial values
        direction = Direction::Right;
        snake = gcnew List<System::Drawing::Rectangle>();
        snakeLength = 0;
        gameRunning = false;
        score = 0;
    }

protected:
    ~SnakeGameForm()
    {
        if (components)
        {
            delete components;
        }
    }

private:
    System::Windows::Forms::Panel^ gameArea;
    System::Windows::Forms::Button^ startButton;
    System::Windows::Forms::Button^ stopButton;
    System::Windows::Forms::Button^ upButton;
    System::Windows::Forms::Button^ downButton;
    System::Windows::Forms::Button^ leftButton;
    System::Windows::Forms::Button^ rightButton;

    void InitializeComponent()
    {
        this->gameArea = (gcnew System::Windows::Forms::Panel());
        this->startButton = (gcnew System::Windows::Forms::Button());
        this->stopButton = (gcnew System::Windows::Forms::Button());
        this->upButton = (gcnew System::Windows::Forms::Button());
        this->downButton = (gcnew System::Windows::Forms::Button());
        this->leftButton = (gcnew System::Windows::Forms::Button());
        this->rightButton = (gcnew System::Windows::Forms::Button());
        this->speedTrackBar = (gcnew System::Windows::Forms::TrackBar()); // TrackBar control
        this->SuspendLayout();
        // 
        // gameArea
        // 
        this->gameArea->BackColor = System::Drawing::Color::Black;
        this->gameArea->Location = System::Drawing::Point(12, 12);
        this->gameArea->Name = L"gameArea";
        this->gameArea->Size = System::Drawing::Size(500, 500);
        this->gameArea->TabIndex = 0;
        this->gameArea->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &SnakeGameForm::gameArea_Paint);
        // 
        // startButton
        // 
        this->startButton->Location = System::Drawing::Point(600, 12);
        this->startButton->Name = L"startButton";
        this->startButton->Size = System::Drawing::Size(75, 23);
        this->startButton->TabIndex = 1;
        this->startButton->Text = L"Start";
        this->startButton->UseVisualStyleBackColor = true;
        this->startButton->Click += gcnew System::EventHandler(this, &SnakeGameForm::startButton_Click);
        // 
        // stopButton
        // 
        this->stopButton->Location = System::Drawing::Point(600, 41);
        this->stopButton->Name = L"stopButton";
        this->stopButton->Size = System::Drawing::Size(75, 23);
        this->stopButton->TabIndex = 2;
        this->stopButton->Text = L"Stop";
        this->stopButton->UseVisualStyleBackColor = true;
        this->stopButton->Click += gcnew System::EventHandler(this, &SnakeGameForm::stopButton_Click);
        // 
        // upButton
        // 
        this->upButton->Location = System::Drawing::Point(590, 170);
        this->upButton->Name = L"upButton";
        this->upButton->Size = System::Drawing::Size(75, 23);
        this->upButton->TabIndex = 3;
        this->upButton->Text = L"Up";
        this->upButton->UseVisualStyleBackColor = true;
        this->upButton->Click += gcnew System::EventHandler(this, &SnakeGameForm::upButton_Click);
        // 
        // downButton
        // 
        this->downButton->Location = System::Drawing::Point(590, 230);
        this->downButton->Name = L"downButton";
        this->downButton->Size = System::Drawing::Size(75, 23);
        this->downButton->TabIndex = 4;
        this->downButton->Text = L"Down";
        this->downButton->UseVisualStyleBackColor = true;
        this->downButton->Click += gcnew System::EventHandler(this, &SnakeGameForm::downButton_Click);
        // 
        // leftButton
        // 
        this->leftButton->Location = System::Drawing::Point(530, 200);
        this->leftButton->Name = L"leftButton";
        this->leftButton->Size = System::Drawing::Size(75, 23);
        this->leftButton->TabIndex = 5;
        this->leftButton->Text = L"Left";
        this->leftButton->UseVisualStyleBackColor = true;
        this->leftButton->Click += gcnew System::EventHandler(this, &SnakeGameForm::leftButton_Click);
        // 
        // rightButton
        // 
        this->rightButton->Location = System::Drawing::Point(650, 200);
        this->rightButton->Name = L"rightButton";
        this->rightButton->Size = System::Drawing::Size(75, 23);
        this->rightButton->TabIndex = 6;
        this->rightButton->Text = L"Right";
        this->rightButton->UseVisualStyleBackColor = true;
        this->rightButton->Click += gcnew System::EventHandler(this, &SnakeGameForm::rightButton_Click);
        // 
        // speedTrackBar
        // 
        this->speedTrackBar->Location = System::Drawing::Point(437, 128);
        this->speedTrackBar->Maximum = 10;
        this->speedTrackBar->Minimum = 1;
        this->speedTrackBar->Name = L"speedTrackBar";
        this->speedTrackBar->Size = System::Drawing::Size(237, 45);
        this->speedTrackBar->TabIndex = 7;
        this->speedTrackBar->Value = 1;
        this->speedTrackBar->TickStyle = TickStyle::TopLeft;
        this->speedTrackBar->TickFrequency = 1;
        this->speedTrackBar->Width = gameWidth;
        this->speedTrackBar->Height = 45;
        this->speedTrackBar->Top = gameHeight + 5;
        this->speedTrackBar->Left = 0;
        this->speedTrackBar->Scroll += gcnew System::EventHandler(this, &SnakeGameForm::speedTrackBar_Scroll);

        // 
        // SnakeGameForm
        // 
        this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->ClientSize = System::Drawing::Size(800, 600);
        this->Controls->Add(this->speedTrackBar);
        this->Controls->Add(this->rightButton);
        this->Controls->Add(this->leftButton);
        this->Controls->Add(this->downButton);
        this->Controls->Add(this->upButton);
        this->Controls->Add(this->stopButton);
        this->Controls->Add(this->startButton);
        this->Controls->Add(this->gameArea);
        this->Name = L"SnakeGameForm";
        this->Text = L"Snake Game";
        this->ResumeLayout(false);
    }

    void startButton_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (!gameRunning)
        {
            // Reset the game
            ResetGame();
            // Start the timer
            timer->Interval = CalculateTimerInterval();
            timer->Start();
            gameRunning = true;
        }
    }

    void stopButton_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (gameRunning)
        {
            // Stop the timer
            timer->Stop();
            gameRunning = false;
        }
    }

    void upButton_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (direction != Direction::Down)
        {
            direction = Direction::Up;
        }
    }

    void downButton_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (direction != Direction::Up)
        {
            direction = Direction::Down;
        }
    }

    void leftButton_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (direction != Direction::Right)
        {
            direction = Direction::Left;
        }
    }

    void rightButton_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (direction != Direction::Left)
        {
            direction = Direction::Right;
        }
    }

    void speedTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e)
    {
        // Adjust the timer interval based on the trackbar value
        timer->Interval = CalculateTimerInterval();
    }

    int CalculateTimerInterval()
    {
        // Map the trackbar value (1-10) to a reasonable interval (e.g., 100-1000 milliseconds)
        int trackBarValue = speedTrackBar->Value;
        return 1100 - (100 * trackBarValue);
    }

    void ResetGame()
    {
        // Reset snake properties
        snake->Clear();
        snakeLength = initialSnakeSize;
        direction = Direction::Right;
        // Generate the initial snake
        int startX = gameWidth / 2 - blockSize / 2;
        int startY = gameHeight / 2 - blockSize / 2;
        for (int i = 0; i < snakeLength; i++)
        {
            int x = startX - (i * blockSize);
            int y = startY;
            System::Drawing::Rectangle rect(x, y, blockSize, blockSize);
            snake->Add(rect);
        }
        // Generate the first apple
        GenerateApple();
        // Reset score
        score = 0;
    }

    void GenerateApple()
    {
        // Generate a random apple position that does not collide with the snake
        Random^ random = gcnew Random();
        int maxX = gameWidth / blockSize;
        int maxY = gameHeight / blockSize;
        bool collision;
        do
        {
            int appleX = random->Next(0, maxX) * blockSize;
            int appleY = random->Next(0, maxY) * blockSize;
            appleRect = System::Drawing::Rectangle(appleX, appleY, blockSize, blockSize);
            collision = false;
            for each (System::Drawing::Rectangle segment in snake)
            {
                if (segment.IntersectsWith(appleRect))
                {
                    collision = true;
                    break;
                }
            }
        } while (collision);
    }

    void gameArea_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
    {
        // Draw the snake
        for each (System::Drawing::Rectangle segment in snake)
        {
            e->Graphics->FillRectangle(Brushes::Green, segment);
        }
        // Draw the apple
        e->Graphics->FillRectangle(Brushes::Red, appleRect);
    }

    void timer_Tick(System::Object^ sender, System::EventArgs^ e)
    {
        // Move the snake
        MoveSnake();
        // Check for collisions
        if (CheckCollisions())
        {
            // Game over
            timer->Stop();
            gameRunning = false;
            MessageBox::Show("Game over!\nScore: " + score, "Game Over");
        }
        else
        {
            // Check for apple collision
            if (snake[0].IntersectsWith(appleRect))
            {
                // Increase score and snake length
                score++;
                snakeLength++;
                // Generate a new apple
                GenerateApple();
                // Adjust the timer interval based on the trackbar value
                timer->Interval = CalculateTimerInterval();
            }
            // Refresh the game area to update the visuals
            gameArea->Invalidate();
        }
    }

    void MoveSnake()
    {
        // Move each segment of the snake
        for (int i = snake->Count - 1; i > 0; i--)
        {
            snake[i] = snake[i - 1];
        }

        // Move the head of the snake based on the current direction
        int headX = snake[0].X;
        int headY = snake[0].Y;
        switch (direction)
        {
        case Direction::Up:
            headY -= blockSize;
            break;
        case Direction::Down:
            headY += blockSize;
            break;
        case Direction::Left:
            headX -= blockSize;
            break;
        case Direction::Right:
            headX += blockSize;
            break;
        }

        // Create a new head rectangle
        System::Drawing::Rectangle newHead(headX, headY, blockSize, blockSize);
        snake[0] = newHead;

        // Check if the head collides with the apple
        if (newHead.IntersectsWith(appleRect))
        {
            // Increase score and snake length
            score++;
            snakeLength++;

            // Generate a new apple
            GenerateApple();

            // Adjust the timer interval based on the trackbar value
            timer->Interval = CalculateTimerInterval();

            // Add the new segment to the snake
            snake->Add(snake[snake->Count - 1]);
        }
    }


    bool CheckCollisions()
    {
        // Check for collisions with walls
        int headX = snake[0].X;
        int headY = snake[0].Y;
        if (headX < 0 || headX >= gameWidth || headY < 0 || headY >= gameHeight)
        {
            return true;
        }
        // Check for collisions with the snake's body
        for (int i = 1; i < snake->Count; i++)
        {
            if (snake[i].IntersectsWith(snake[0]))
            {
                return true;
            }
        }
        return false;
    }
};

[STAThread]
void main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    SnakeGameForm form;
    Application::Run(% form);
}
