#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>


#define mapWidth 80
#define mapHeight 25

typedef struct SObject
{
    float x,y;
    float width, height;
    float vertSpeed;
    BOOL isFly;
    char cType;
    float horizontalSpeed;
} TObject;

char map[mapHeight][mapWidth+1];
TObject mario;

TObject *brick = NULL;
int brickLength;

TObject *moving = NULL;
int movingLength;

int level = 1;
int maxLvl;
int score;


void ClearMap()
{
    for (int i = 0; i < mapWidth; i++)
        map[0][i] = ' ';
    map[0][mapWidth] = '\0';
    for(int j = 1; j < mapHeight; j++)
        sprintf( map[j], map[0]);
}

void ShowMap()
{
    map[mapHeight - 1][mapWidth - 1] = '\0';
    for(int j = 0; j < mapHeight; j++)
        printf("%s", map[j]);
}

void SetObjectPosition(TObject *object, float xPos, float yPos)
{
    (*object).x = xPos;
    (*object).y= yPos;
}

void InitObject(TObject *object, float xPos, float yPos, float oWidth, float oHeight, char inType)
{
    SetObjectPosition(object, xPos, yPos);
    (*object).width = oWidth;
    (*object).height = oHeight;
    (*object).vertSpeed = 0;
    (*object).cType = inType;
    (*object).horizontalSpeed = 0.2;

}

void playerDead()
{
    system("cls");
    system("color       0C");
    printf("\n\n\n\n\n\n\n\n\n\n \t\t\t\tIDITE LESOM \n \t\t\t\t    wow ");

    Sleep(1000);
    CreateLevel(level);
}

BOOL IsCollision(TObject o1, TObject o2);

void *GetNewMoving();
void CreateLevel(int lvl);


void VerticalMoveMap(TObject *object)
{
    (*object).isFly = TRUE;
    (*object).vertSpeed += 0.05;
    SetObjectPosition(object, (*object).x, (*object).y + (*object).vertSpeed );

    for(int i = 0; i < brickLength; i++)
        if(IsCollision(*object, brick[i]))
        {
            if (object[0].vertSpeed > 0)
                object[0].isFly = FALSE;

            if( (brick[i].cType == '?') && (object[0].vertSpeed < 0) && (object == &mario) )
            {
                brick[i].cType = '-';
                InitObject(GetNewMoving(), brick[i].x, brick[i].y-3, 3, 2, '$');
                moving[movingLength - 1].vertSpeed = -0.7;
            }

            (*object).y -= (*object).vertSpeed;
            (*object).vertSpeed = 0;

            if(brick[i].cType == '+')
            {
                system("cls");
                system("color 0A");
                printf("\n\n\n\n\n\n\n\n\n\n \t\t\t\t  Ashal");
                Sleep(1000);
                level++;
                if (level > maxLvl) level = 1;
                CreateLevel(level);
            }
            break;
        }

}

void DeleteMoving (int i)
{
    movingLength--;
    moving[i] = moving[movingLength];
    moving = realloc( moving, sizeof(*moving) * movingLength);
}

void MarioCollision()
{
    for (int i = 0; i < movingLength; i++)
        if(IsCollision(mario, moving[i]))
        {
            if(moving[i].cType == 'o')
            {


                if (   (mario.isFly == TRUE)
                        && (mario.vertSpeed > 0)
                        && (mario.y + mario.height < moving[i].y + moving[i].height * 0.5 )
                   )
                {
                    score += 50;
                    DeleteMoving(i);
                    i--;
                    continue;
                }
                else
                    playerDead();
            }
            if(moving[i].cType == '$')
            {
                score += 100;
                DeleteMoving(i);
                i--;
                continue;
            }
        }
}

void HorizontalMoveObject(TObject *obj)
{
    obj[0].x += obj[0].horizontalSpeed;

    for (int i = 0; i< brickLength; i++)
        if (IsCollision(obj[0], brick[i]))
        {
            obj[0].x -= obj[0].horizontalSpeed;
            obj[0].horizontalSpeed = -obj[0].horizontalSpeed;
            return;
        }
    if(obj[0].cType == 'o')
    {


        TObject tmp = *obj;
        VerticalMoveMap(&tmp);
        if (tmp.isFly == TRUE)
        {
            obj[0].x -= obj[0].horizontalSpeed;
            obj[0].horizontalSpeed = - obj[0].horizontalSpeed;
        }
    }

}

BOOL IsPosInMap(int x, int y)
{
    return( (x>= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));
}


void PutObjectOnMap(TObject object)
{
    int ix = (int)round(object.x);
    int iy = (int)round(object.y);
    int iWidth = (int)round(object.width);
    int iHeight = (int)round(object.height);

    for (int i = ix; i < (ix + iWidth); i++)
        for (int j = iy; j < (iy + iHeight); j++)
            if(IsPosInMap(i, j))
                map[j][i] = object.cType;
}

void setcur (int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HorizontalMoveMap(float dx)
{
//    mario.x -= dx;
//    for(int i = 0; i < brickLength; i++)
//        if(IsCollision(mario, brick[i]))
//    {
//        mario.x += dx;
//        return;
//    }

    for(int i = 0; i < brickLength; i++)
        brick[i].x += dx;

    for(int i = 0; i < movingLength; i++)
        moving[i].x += dx;
}

BOOL IsCollision(TObject o1, TObject o2)
{
    return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
           ((o1.y + o1.height) > o2.y) && ( o1.y < (o2.y + o2.height));
}

void *GetNewBrick()
{
    brickLength++;
    brick = realloc(brick, sizeof(*brick) * brickLength );
    return brick + brickLength - 1;
}

void *GetNewMoving()
{
    movingLength++;
    moving = realloc(moving, sizeof(*moving) * movingLength );
    return moving + movingLength - 1;
}

void PutScoreOnMap()
{
    char c[30];
    sprintf(c, "Score: %d", score);
    int length = strlen(c);
    for (int i = 0; i < length; i++)
        map[1][i + 5] = c[i];
}

void CreateLevel(int lvl)
{
    system("color 0D");
    brickLength = 0;
    brick = realloc(brick, 0);
    movingLength = 0;
    moving = realloc(moving, 0);

    InitObject(&mario, 39, 10, 3, 3, '@');

    score = 0;

    if (lvl == 1)
    {

        InitObject(GetNewBrick(), 20, 20, 40, 5, '#');
        /* */InitObject(GetNewBrick(), 30, 10, 5, 3, '?');
        /* */InitObject(GetNewBrick(), 50, 10, 5, 3, '?');
        InitObject(GetNewBrick(), 60, 15, 40, 10, '#');
        /* */InitObject(GetNewBrick(), 60, 5, 10, 3, '-');
        /* */InitObject(GetNewBrick(), 70, 5, 5, 3, '?');
        /* */InitObject(GetNewBrick(), 75, 5, 5, 3, '-');
        /* */InitObject(GetNewBrick(), 80, 5, 5, 3, '?');
        /* */InitObject(GetNewBrick(), 85, 5, 10, 3, '-');
        InitObject(GetNewBrick(), 100, 20, 20, 5, '#');
        InitObject(GetNewBrick(), 120, 15, 10, 10, '#');
        InitObject(GetNewBrick(), 150, 20, 40, 5, '#');
        InitObject(GetNewBrick(), 210, 15, 10, 10, '+');

        InitObject(GetNewMoving(), 25, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 80, 10, 3, 2, 'o');
    }
    if (lvl == 2)
    {

        InitObject(GetNewBrick(), 20, 20, 40, 5, '#');
        InitObject(GetNewBrick(), 60, 15, 10, 10, '#');
        InitObject(GetNewBrick(), 80, 20, 20, 5, '#');
        InitObject(GetNewBrick(), 120, 15, 10, 10, '#');
        InitObject(GetNewBrick(), 150, 20, 40, 5, '#');
        InitObject(GetNewBrick(), 210, 15, 10, 10, '+');


        InitObject(GetNewMoving(), 25, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 65, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 80, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 120, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 160, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 175, 10, 3, 2, 'o');

    }
    if (lvl == 3)
    {

        InitObject(GetNewBrick(), 20, 20, 40, 5, '#');
        InitObject(GetNewBrick(), 80, 15, 15, 5, '#');
        InitObject(GetNewBrick(), 120, 15, 15, 10, '#');
        InitObject(GetNewBrick(), 160, 10, 15, 15, '+');


        InitObject(GetNewMoving(), 25, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 50, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 80, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 90, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 120, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 130, 10, 3, 2, 'o');
    }
    maxLvl = 3;
}

int main()
{



    CreateLevel(level);

    do
    {
        ClearMap();

        if ( (mario.isFly == FALSE) && (GetKeyState(VK_SPACE) < 0)) mario.vertSpeed = -1;
        if(GetKeyState('A') < 0) HorizontalMoveMap(1);
        if(GetKeyState('D') < 0) HorizontalMoveMap(-1);

        if(mario.y > mapHeight)
            playerDead();


        VerticalMoveMap(&mario);
        MarioCollision();

        for(int i = 0; i < brickLength; i++)
            PutObjectOnMap(brick[i]);
        for(int i = 0; i < movingLength; i++)
        {
            VerticalMoveMap(moving + i);
            HorizontalMoveObject(moving + i);
            if (moving[i].y > mapHeight)
            {
                DeleteMoving(i);
                i--;
                continue;
            }
            PutObjectOnMap(moving[i]);
        }
        PutObjectOnMap(mario);
        PutScoreOnMap();

        setcur (0, 0);
        ShowMap();

        Sleep(10);
    }
    while (GetKeyState(VK_ESCAPE)>= 0);

    free(brick);
    return 0;
}










