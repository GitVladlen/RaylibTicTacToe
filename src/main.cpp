#include "raylib.h"
#include <iostream>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
class TClickBox
{
public:
    TClickBox(const Rectangle& InBoxRect)
        : BoxRect(InBoxRect)
    {}

    bool CheckClick(const Vector2& Point)
    {
        return CheckClick(Point.x, Point.y);
    }

    bool CheckClick(float x, float y)
    {
        if (x < BoxRect.x || x > BoxRect.x + BoxRect.width)
        {
            return false;
        }

        if (y < BoxRect.y || y > BoxRect.y + BoxRect.height)
        {
            return false;
        }

        return true;
    }

    const Rectangle& GetBoxRect() const
    {
        return BoxRect;
    }

private:
    Rectangle BoxRect;
};
////////////////////////////////////////////////////////////////////////////////
class TTagedClickBox
    : public TClickBox
{
public:
    TTagedClickBox(int InTag, const Rectangle& InBoxRect)
        : TClickBox(InBoxRect)
        , Tag(InTag)
    {}

    int GetTag() const
    {
        return Tag;
    }

private:
    int Tag;
};
////////////////////////////////////////////////////////////////////////////////
class TGame
{
public:
    enum EClickBox
    {
        CLICK_BOX_1 = 0,
        CLICK_BOX_2,
        CLICK_BOX_3,
        CLICK_BOX_4,
        CLICK_BOX_5,
        CLICK_BOX_6,
        CLICK_BOX_7,
        CLICK_BOX_8,
        CLICK_BOX_9,
    };

    TGame(const Rectangle& InGameRect)
        : GameRect(InGameRect)
        , Cells(9)
        , CurrentPlayer(1)
        , bIsGameOver(false)
        , WinnerPlayer(0)
    {
        Recalc();
    }

public:
    void Recalc()
    {
        CellSize.x = GameRect.width / 3;
        CellSize.y = GameRect.height / 3;

        CellBorder.x = CellSize.x * CellBorderRatio;
        CellBorder.y = CellSize.y * CellBorderRatio;

        if (!ClickBoxes.empty())
        {
            ClickBoxes.clear();
        }

        ClickBoxes = {
            { CLICK_BOX_1, { GameRect.x + CellSize.x * 0, GameRect.y + CellSize.y * 0, CellSize.x, CellSize.y } },
            { CLICK_BOX_2, { GameRect.x + CellSize.x * 1, GameRect.y + CellSize.y * 0, CellSize.x, CellSize.y } },
            { CLICK_BOX_3, { GameRect.x + CellSize.x * 2, GameRect.y + CellSize.y * 0, CellSize.x, CellSize.y } },

            { CLICK_BOX_4, { GameRect.x + CellSize.x * 0, GameRect.y + CellSize.y * 1, CellSize.x, CellSize.y } },
            { CLICK_BOX_5, { GameRect.x + CellSize.x * 1, GameRect.y + CellSize.y * 1, CellSize.x, CellSize.y } },
            { CLICK_BOX_6, { GameRect.x + CellSize.x * 2, GameRect.y + CellSize.y * 1, CellSize.x, CellSize.y } },

            { CLICK_BOX_7, { GameRect.x + CellSize.x * 0, GameRect.y + CellSize.y * 2, CellSize.x, CellSize.y } },
            { CLICK_BOX_8, { GameRect.x + CellSize.x * 1, GameRect.y + CellSize.y * 2, CellSize.x, CellSize.y } },
            { CLICK_BOX_9, { GameRect.x + CellSize.x * 2, GameRect.y + CellSize.y * 2, CellSize.x, CellSize.y } },
        };
    }

    void RepositionTo(const Vector2& InPos)
    {
        GameRect.x = InPos.x;
        GameRect.y = InPos.y;
        Recalc();
    }

    void Resize(const Rectangle& InRect)
    {
        GameRect = InRect;
        Recalc();
    }

    void HandleKeyPress(int Key)
    {
        switch (Key)
        {
        case KEY_N:
            Reset();
            break;
        case KEY_R:
            Reset();
            break;
        case KEY_SPACE:
            Reset();
            break;
        default:
            break;
        }
    }

    void Reset()
    {
        std::cout << "New game\n";

        for (int& Cell : Cells)
        {
            Cell = 0;
        }

        CurrentPlayer = 1;
        bIsGameOver = false;
        WinnerPlayer = 0;
    }

    void DrawX(const Rectangle& InRect)
    {
        DrawLineEx({ InRect.x, InRect.y },
            { InRect.x + InRect.width, InRect.y + InRect.height }, 5.f, PlayerXColor);
        DrawLineEx({ InRect.x + InRect.width, InRect.y },
            { InRect.x, InRect.y + InRect.height }, 5.f, PlayerXColor);
    }

    void DrawO(const Rectangle& InRect)
    {
        Vector2 Center{ InRect.x + InRect.width / 2, InRect.y + InRect.height / 2 };
        float OuterRadius = InRect.width >= InRect.height ? InRect.height / 2 : InRect.width / 2;
        float InnerRadius = OuterRadius - 5.f;

        DrawRing(Center, InnerRadius, OuterRadius, 0.f, 360.f, 30, PlayerOColor);
    }

    void DrawPlayerX(int CellIdx)
    {
        const Rectangle& ClickBoxRect = ClickBoxes[CellIdx].GetBoxRect();

        DrawX({ ClickBoxRect.x + CellBorder.x, ClickBoxRect.y + CellBorder.y,
            ClickBoxRect.width - CellBorder.x * 2, ClickBoxRect.height - CellBorder.y * 2});
    }

    void DrawPlayerO(int CellIdx)
    {
        const Rectangle& ClickBoxRect = ClickBoxes[CellIdx].GetBoxRect();

        DrawO({ ClickBoxRect.x + CellBorder.x, ClickBoxRect.y + CellBorder.y,
            ClickBoxRect.width - CellBorder.x * 2, ClickBoxRect.height - CellBorder.y * 2 });
    }

    void DrawTextAligned(const char* InText, float InPosX, float InPosY, bool bIsHorizontalCenter, bool bIsVerticalCenter, float InFontSize, Color InColor)
    {
        int TextSize = MeasureText(InText, InFontSize);
        float TextPosX = InPosX - (bIsHorizontalCenter ? TextSize / 2 : 0);
        float TextPosY = InPosY - (bIsVerticalCenter ? InFontSize / 2 : 0);
        DrawText(InText, TextPosX, TextPosY, InFontSize, InColor);
    }

    void DrawWin(int PlayerId)
    {
        if (PlayerId == 1)
        {
            DrawTextAligned("Player X Wins",
                GameRect.x + GameRect.width / 2,
                GameRect.y + GameRect.height / 2,
                true, true, GameRect.height * 0.1f, ORANGE);
        }
        else if (PlayerId == 2)
        {
            DrawTextAligned("Player O Wins",
                GameRect.x + GameRect.width / 2,
                GameRect.y + GameRect.height / 2,
                true, true, GameRect.height * 0.1f, ORANGE);
        }
    }

    void DrawGameOver()
    {
        DrawTextAligned("Game Over",
            GameRect.x + GameRect.width / 2,
            GameRect.y + GameRect.height / 2,
            true, true, GameRect.height * 0.1f, ORANGE);
    }

    void DrawInstructions()
    {
        float FontSize = GameRect.height * 0.1f;
        DrawTextAligned("<Space> - New game",
            GameRect.x + GameRect.width / 2,
            GameRect.y - FontSize,
            true, false, FontSize, LIGHTGRAY);
    }

    void DrawBorders()
    {
        DrawLineEx({ GameRect.x + GameRect.width / 3 * 1, GameRect.y },
            { GameRect.x + GameRect.width / 3 * 1, GameRect.y + GameRect.height }, 5.f, BorderColor);
        DrawLineEx({ GameRect.x + GameRect.width / 3 * 2, GameRect.y },
            { GameRect.x + GameRect.width / 3 * 2, GameRect.y + GameRect.height }, 5.f, BorderColor);

        DrawLineEx({ GameRect.x, GameRect.y + GameRect.height / 3 * 1 },
            { GameRect.x + GameRect.width, GameRect.y + GameRect.height / 3 * 1 }, 5.f, BorderColor);
        DrawLineEx({ GameRect.x, GameRect.y + GameRect.height / 3 * 2 },
            { GameRect.x + GameRect.width, GameRect.y + GameRect.height / 3 * 2 }, 5.f, BorderColor);
    }

    void Draw()
    {
        //DrawInstructions();

        Rectangle BorderRect{ GameRect.x, GameRect.y, GameRect.width, GameRect.height };

        DrawRectangleLinesEx(BorderRect, 5.f, BorderColor);

        DrawBorders();

        for (size_t CellIdx = 0; CellIdx < Cells.size(); CellIdx++)
        {
            if (Cells[CellIdx] == 1)
            {
                DrawPlayerX(CellIdx);
            }
            else if (Cells[CellIdx] == 2)
            {
                DrawPlayerO(CellIdx);
            }
        }

        if (bIsGameOver)
        {
            if (WinnerPlayer != 0)
            {
                DrawWin(WinnerPlayer);
            }
            else
            {
                DrawGameOver();
            }
        }
    }

    void HandleClick(const TTagedClickBox& InClickBox)
    {
        std::cout << "Click on ClickBox with tag = " << InClickBox.GetTag() << "\n";

        switch (InClickBox.GetTag())
        {
        case CLICK_BOX_1:
            MakeTurn(0);
            break;
        case CLICK_BOX_2:
            MakeTurn(1);
            break;
        case CLICK_BOX_3:
            MakeTurn(2);
            break;
        case CLICK_BOX_4:
            MakeTurn(3);
            break;
        case CLICK_BOX_5:
            MakeTurn(4);
            break;
        case CLICK_BOX_6:
            MakeTurn(5);
            break;
        case CLICK_BOX_7:
            MakeTurn(6);
            break;
        case CLICK_BOX_8:
            MakeTurn(7);
            break;
        case CLICK_BOX_9:
            MakeTurn(8);
            break;
        default:
            break;
        }
    }

    bool HaveEmptyCells() const
    {
        for (const int Cell : Cells)
        {
            if (Cell == 0)
                return true;
        }
        return false;
    }

    bool IsPlayerWin(int PlayerId) const
    {
        return 
            (Cells[0] == PlayerId && Cells[1] == PlayerId && Cells[2] == PlayerId) ||
            (Cells[3] == PlayerId && Cells[4] == PlayerId && Cells[5] == PlayerId) || 
            (Cells[6] == PlayerId && Cells[7] == PlayerId && Cells[8] == PlayerId) ||

            (Cells[0] == PlayerId && Cells[3] == PlayerId && Cells[6] == PlayerId) ||
            (Cells[1] == PlayerId && Cells[4] == PlayerId && Cells[7] == PlayerId) || 
            (Cells[2] == PlayerId && Cells[5] == PlayerId && Cells[8] == PlayerId) || 

            (Cells[0] == PlayerId && Cells[4] == PlayerId && Cells[8] == PlayerId) || 
            (Cells[2] == PlayerId && Cells[4] == PlayerId && Cells[6] == PlayerId);
    }

    void CheckGameOver()
    {
        if (!HaveEmptyCells())
        {
            bIsGameOver = true;
            return;
        }

        if (IsPlayerWin(1))
        {
            WinnerPlayer = 1;
        }
        else if (IsPlayerWin(2))
        {
            WinnerPlayer = 2;
        }

        bIsGameOver = WinnerPlayer != 0;
    }

    void MakeTurn(int CellIdx)
    {
        if (bIsGameOver)
        {
            return;
        }

        if (Cells[CellIdx] != 0)
        {
            std::cout << "Cell " << CellIdx << " is already occupied with player " << Cells[CellIdx] << "\n";
            return;
        }
        
        Cells[CellIdx] = CurrentPlayer;
        NextPlayer();

        CheckGameOver();
    }

    void NextPlayer()
    {
        CurrentPlayer = CurrentPlayer == 1 ? 2 : 1;
    }

    void HandleWheelMove(float move)
    {
        const float MOVE_SIZE = 30.f;
        GameRect.width = GameRect.width + move * MOVE_SIZE;
        GameRect.height = GameRect.height + move * MOVE_SIZE;
        Recalc();
    }

    void Update()
    {
        //if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        //{
        //    RepositionTo(GetMousePosition());
        //}

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 MousePos = GetMousePosition();

            std::cout << "MOUSE_BUTTON_RIGHT Mouse pos = (" << MousePos.x << ", " << MousePos.y << ")\n";

            for (TTagedClickBox& ClickBox : ClickBoxes)
            {
                if (ClickBox.CheckClick(MousePos))
                {
                    Reset();
                    break;
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 MousePos = GetMousePosition();

            std::cout << "MOUSE_BUTTON_LEFT Mouse pos = (" << MousePos.x << ", " << MousePos.y << ")\n";

            for (TTagedClickBox& ClickBox : ClickBoxes)
            {
                if (ClickBox.CheckClick(MousePos))
                {
                    HandleClick(ClickBox);
                }
            }
        }

        //Vector2 WheelMovement = GetMouseWheelMoveV();

        //if (WheelMovement.x != 0.f)
        //{
        //    std::cout << "Wheel movement by X = " << WheelMovement.x << "\n";
        //    HandleWheelMove(WheelMovement.x);
        //}

        //if (WheelMovement.y != 0.f)
        //{
        //    std::cout << "Wheel movement by Y = " << WheelMovement.y << "\n";
        //    HandleWheelMove(WheelMovement.y);
        //}
    }

private:
    Color BorderColor = LIGHTGRAY;
    Color PlayerXColor = RED;
    Color PlayerOColor = BLUE;
    std::vector<TTagedClickBox> ClickBoxes;
    Rectangle GameRect;
    Vector2 CellSize;
    const float CellBorderRatio = 0.1f;
    Vector2 CellBorder;

    std::vector<int> Cells;
    int CurrentPlayer;
    bool bIsGameOver;
    int WinnerPlayer;
};
////////////////////////////////////////////////////////////////////////////////
std::vector<Rectangle> CalcGameRects(int HorizontalGameCount, int VerticalGameCount, int WindowWidth, int WindowHeight, bool bFillSpace = false)
{
    float GameWidth = 0;
    float GameHeight = 0;

    float OffsetX = 0;
    float OffsetY = 0;

    if (bFillSpace)
    {
        GameWidth = WindowWidth / HorizontalGameCount;
        GameHeight = WindowHeight / VerticalGameCount;

        OffsetX = 0;
        OffsetY = 0;
    }
    else
    {
        int GameSideSize = WindowWidth >= WindowHeight ? WindowHeight : WindowWidth;

        GameWidth = GameSideSize / HorizontalGameCount;
        GameHeight = GameSideSize / VerticalGameCount;

        OffsetX = (WindowWidth - GameSideSize) > 0 ? (WindowWidth - GameSideSize) / 2 : 0;
        OffsetY = (WindowHeight - GameSideSize) > 0 ? (WindowHeight - GameSideSize) / 2 : 0;
    }

    float BORDER_SIZE = GameWidth * 0.05f;
    Vector2 GAME_INIT_POS{ OffsetX + BORDER_SIZE, OffsetY + BORDER_SIZE };

    Vector2 GAME_INIT_SIZE{ GameWidth - BORDER_SIZE * 2, GameHeight - BORDER_SIZE * 2 };

    std::vector<Rectangle> GameRects;

    for (size_t RowIdx = 0; RowIdx < VerticalGameCount; RowIdx++)
    {
        for (size_t ColIdx = 0; ColIdx < HorizontalGameCount; ColIdx++)
        {
            GameRects.push_back({ GAME_INIT_POS.x + GameWidth * ColIdx, GAME_INIT_POS.y + GameHeight * RowIdx, GAME_INIT_SIZE.x, GAME_INIT_SIZE.y });
        }
    }

    return GameRects;
}
////////////////////////////////////////////////////////////////////////////////
int main() {
    int HORIZONTAL_GAME_COUNT = 3;
    int VERTICAL_GAME_COUNT = 3;

    int WINDOW_WIDTH = 500;
    int WINDOW_HEIGHT = 500;

    std::vector<Rectangle> GameRects = CalcGameRects(
        HORIZONTAL_GAME_COUNT, VERTICAL_GAME_COUNT, 
        WINDOW_WIDTH, WINDOW_HEIGHT, false);

    std::vector<TGame> Games;

    for (const Rectangle& GameRect : GameRects)
    {
        Games.push_back({ GameRect });
    }

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "[Raylib] Tic Tac Toe");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(300, 300);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (IsWindowResized())
        {
            std::cout << "Window resized\n";

            int NewWindowWidth = GetScreenWidth();
            int NewWindowHeight = GetScreenHeight();

            std::vector<Rectangle> NewGameRects = CalcGameRects(
                HORIZONTAL_GAME_COUNT, VERTICAL_GAME_COUNT, 
                NewWindowWidth, NewWindowHeight, false);

            for (size_t GameIdx = 0; GameIdx < Games.size(); GameIdx++)
            {
                Games[GameIdx].Resize(NewGameRects[GameIdx]);
            }
        }

        while (int Key = GetKeyPressed())
        {
            for (TGame& Game : Games)
            {
                Game.HandleKeyPress(Key);
            }
        }

        for (TGame& Game : Games)
        {
            Game.Update();
        }

        for (TGame& Game : Games)
        {
            Game.Draw();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
////////////////////////////////////////////////////////////////////////////////