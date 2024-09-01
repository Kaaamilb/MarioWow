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
} TObject;

char map[mapHeight][mapWidth+1];
TObject mario;
TObject *brick = NULL;
int brickLength;
int level = 1;

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

}

BOOL IsCollision(TObject o1, TObject o2);

void CreateLevel(int lvl);
void MoveObjectVertical(TObject *object)
{
    (*object).isFly = TRUE;
    (*object).vertSpeed += 0.05;
    SetObjectPosition(object, (*object).x, (*object).y + (*object).vertSpeed );

    for(int i = 0; i < brickLength; i++)
        if(IsCollision(*object, brick[i]))
        {
            (*object).y -= (*object).vertSpeed;
            (*object).vertSpeed = 0;
            (*object).isFly = FALSE;
            if(brick[i].cType == '+')
            {
                system("cls");
                printf("\n\n\n\n\n\n\n\n\n\n \t\t\t\t  Ashal");
                Sleep(1000);
                level++;
                if (level > 2) level = 1;
                CreateLevel(level);
            }
            break;
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

void MoveMapHorizontal(float dx)
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
}

BOOL IsCollision(TObject o1, TObject o2)
{
    return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
           ((o1.y + o1.height) > o2.y) && ( o1.y < (o2.y + o2.height));
}

void CreateLevel(int lvl)
{
    InitObject(&mario, 39, 10, 3, 3, '@');

    if (lvl == 1)
    {
        brickLength = 6;
        brick = realloc(brick, sizeof(*brick) * brickLength );
        InitObject(brick+0, 20, 20, 40, 5, '#');
        InitObject(brick+1, 60, 15, 10, 10, '#');
        InitObject(brick+2, 80, 20, 20, 5, '#');
        InitObject(brick+3, 120, 15, 10, 10, '#');
        InitObject(brick+4, 150, 20, 40, 5, '#');
        InitObject(brick+5, 210, 15, 10, 10, '+');
    }
    if (lvl == 2)
    {
        brickLength = 4;
        brick = realloc(brick, sizeof(*brick) * brickLength );
        InitObject(brick+0, 20, 20, 40, 5, '#');
        InitObject(brick+1, 80, 15, 15, 5, '#');
        InitObject(brick+2, 120, 15, 15, 10, '#');
        InitObject(brick+3, 160, 10, 15, 15, '+');

    }
}

int main()
{



    CreateLevel(level);

    do
    {
        ClearMap();

        if ( (mario.isFly == FALSE) && (GetKeyState(VK_SPACE) < 0)) mario.vertSpeed = -1;
        if(GetKeyState('A') < 0) MoveMapHorizontal(1);
        if(GetKeyState('D') < 0) MoveMapHorizontal(-1);

        if(mario.y > mapHeight)
        {
            system("cls");
            printf("\n\n\n\n\n\n\n\n\n\n \t\t\t\tIDITE LESOM \n \t\t\t\t    wow ");

            Sleep(1000);
            CreateLevel(level);
        }


        MoveObjectVertical(&mario);
        for(int i = 0; i < brickLength; i++)
            PutObjectOnMap(brick[i]);
        PutObjectOnMap(mario);

        setcur (0, 0);
        ShowMap();

        Sleep(10);
    }
    while (GetKeyState(VK_ESCAPE)>= 0);
	
    free(brick);
    return 0;
}










