#include <Servo.h>
#include <math.h>

Servo servog;
Servo servov;
#define LASER 8  // пин лазера
int PhotoRes = 0;  // номер пина после равно
const int l = 5;
int max = 0, x = 0, y = 1;
boolean centr = false, f = 0;
float ygolX = 0.0, ygolY = 0.0;

void setup()
{
    servog.attach(5);  // поменять номер пина
    servov.attach(6);  // поменять номер пина
    pinMode(LASER, OUTPUT);
    pinMode(PhotoRes, INPUT);
    Serial.begin(9600);  // функция для работы с портом, в скобках указывается скорость вывода на экран
}

void loop()
{
    if (f == 0)
    {
        delay(30000);
        digitalWrite(LASER, HIGH);
        delay(12000);
        digitalWrite(LASER, LOW);
        f = 1;
    }

    for (int i = 0; i < 3; i++)
    {
        while ((analogRead(PhotoRes) > max) && (x < 4))
        {
            max = analogRead(PhotoRes);
            x++;
            servog.write(atan2(x, l) - ygolX);
            ygolX = atan2(x, l);
            if ((x == 4) && (max < analogRead(PhotoRes)))
                max = analogRead(PhotoRes);
        }

        if (abs(max - analogRead(PhotoRes)) > 2)  // погрешность
        {
            x = x - 2;
            if (x >= 0)
            {
                servog.write(atan2(x, l) - ygolX);
                ygolX = atan2(x, l);
            }
            else
            {
                servog.write(-atan2(abs(x), l) - ygolX);
                ygolX = -atan2(abs(x), l);
            }
            centr = true;

            while ((analogRead(PhotoRes) > max) && (x > -4))
            {
                centr = false;
                max = analogRead(PhotoRes);
                x--;
                if (x >= 0)
                {
                    servog.write(atan2(x, l) - ygolX);
                    ygolX = atan2(x, l);
                }
                else
                {
                    servog.write(-atan2(abs(x), l) - ygolX);
                    ygolX = -atan2(abs(x), l);
                }
                if ((x == -4) && (max < analogRead(PhotoRes)))
                    max = analogRead(PhotoRes);
            }

            if ((abs(max - analogRead(PhotoRes)) > 2) || (centr))
            {
                x++;
                if (x >= 0)
                {
                    servog.write(atan2(x, l) - ygolX);
                    ygolX = atan2(x, l);
                }
                else
                {
                    servog.write(-atan2(abs(x), l) - ygolX);
                    ygolX = -atan2(abs(x), l);
                }
            }
        }

        max = analogRead(PhotoRes);
        y++;
        servov.write(atan2(y, l) - ygolY);
        ygolY = atan2(y, l);
        while ((analogRead(PhotoRes) > max) && (y < 4))
        {
            max = analogRead(PhotoRes);
            y++;
            servov.write(atan2(y, l) - ygolY);
            ygolY = atan2(y, l);
            if ((y == 4) && (max < analogRead(PhotoRes)))
                max = analogRead(PhotoRes);
        }

        if (abs(max - analogRead(PhotoRes)) > 2)
        {
            y--;
            servov.write(atan2(y, l) - ygolY);
            ygolY = atan2(y, l);
        }

        digitalWrite(LASER, HIGH);
        delay(12000);
        digitalWrite(LASER, LOW);
        servov.write(-ygolY);
        ygolY = 0.0;
        servog.write(-ygolX);
        ygolX = 0.0;
        x = 0;
        y = 1;
        max = 0;
        centr = false;
    }
}
