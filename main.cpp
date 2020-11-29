#include <iostream>
#include <ctime>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <locale>

using namespace std;

enum RequestType {firstType = 1, secType = 2}; // Тип заявки.

struct Request
{
    RequestType ReqType;
};

const double BegInterval = 0; // Начало каждого из интервалов генерации и обработки.

const int  MaxSReqPos = 4; // Не далее этого элемента заявка второго типа вернётся в очередь.


// Генерирует случайное число.
double RandTime()
{
    double Val = (rand() * 1.0) / RAND_MAX;
    return Val;
}

// Какое время меньше.
double MinTime(double T1, double T2)
{
    return ((T1 > T2) ? T2 : T1);
}

// Генерирует время прихода следующей заявки 1 типа.
double GenAddFReq(double EndGenerateFReq)
{
    double Val = RandTime();
    double Time = (EndGenerateFReq - BegInterval) * Val + BegInterval;
    return Time;
}

// Генерирует время обработки следующей заявки 1 типа.
double GenUseFReq(double UseFReq)
{
    double Val = RandTime();
    double Time = (UseFReq - BegInterval) * Val + BegInterval;
    return Time;
}

// Генерирует время обработки заявки 2 типа.
double GenUseSReq(double UseSReq)
{
    double Val = RandTime();
    double Time = (UseSReq - BegInterval) * Val + BegInterval;
    return Time;
}

int main()
{
    double EndGenerateFReq; // Максимальное время генерации заявки 1 типа.
    double UseFReq; // Максимальное время обработки заявки 1 типа.
    double UseSReq; // Максимальное время обработки заявки 2 типа.

    cout << "Maximum time generating request 1 type: ";
    cin >> EndGenerateFReq;

    cout << "Maximum time clear request 1 type: ";
    cin >> UseFReq;

    cout << "Maximum time clear request 2 type: ";
    cin >> UseSReq;

    srand(static_cast<unsigned int>(time(NULL)));

    list<RequestType> RequestQueue;

    RequestType STypeReq = secType;
    RequestType FTypeReq = firstType;

    RequestQueue.push_back(STypeReq);

    int CountFReqs = 0; // Количество обработанных заявок 1 типа.
    int CountUseFReqs = 0;
    int Enter2QueueFReqs = 0; // Количество заявок 1 типа вошедших в систему.
    double Time = 0.0;
    double TimeEnter = 0.0; // Время прихода заявки.
    double TimeClearService = 0.0; // Время обработки заявки
    double NextEventTime = 0.0; // Время следующего события.
    double TotalTime = 0.0; // Общее время моделирования.

    list<RequestType>::iterator PredSReq = RequestQueue.begin(); // Указывает на заявку, перед которой надо вставить заявку 2го типа.
    list<RequestType>::iterator tmp = RequestQueue.begin();
    int CountOfCalcs = 0; // Количество вычислений.
    double MiddleLenQueue = 0.0; // Средняя длина после каждого вычисления.
    int countSecReqs = 0; // Количество заявок 2 типа.
    int SumOfCalcs = 0;
    while( true )
    {

        if( CountUseFReqs == 1000)
            break;
        TimeEnter = GenAddFReq(EndGenerateFReq);
        if(RequestQueue.front() == 2)
            TimeClearService = GenUseSReq(UseSReq);
        else
            TimeClearService = GenUseFReq(UseFReq);
        Time = MinTime(TimeEnter, TimeClearService);

        if(Time == TimeEnter)
        {
            // Добавляем заявку первого типа.
            RequestQueue.push_back(FTypeReq);
            CountFReqs++;       // Увеличиваем счётчик заявок 1 типа
            Enter2QueueFReqs++; // Увеличиваем общее количество заявок вошедших в систему.
            continue;
        }
        else
        {
            // Обрабатываем заявку.
            if( RequestQueue.front() == secType)
            {
                // Обрабатываем заявку 2-го типа.
                TotalTime += Time;
                //cout << " 2 " << TimeClearService << " : " << TotalTime<<endl;
                countSecReqs++;
                RequestQueue.pop_front();
                if(MaxSReqPos >= RequestQueue.size())
                {
                    RequestQueue.push_back(STypeReq);
                    continue;
                }
                else
                {
                    PredSReq = RequestQueue.begin();
                    for(int i = 0; i != 4; i++) PredSReq++;

                    RequestQueue.insert(PredSReq, STypeReq);
                    continue;
                }
            }
            else
            {
                // Обрабатываем заявку 1 типа.
                TotalTime += Time;
                CountUseFReqs++;
                RequestQueue.pop_front();
                //cout << " 1 " << TimeClearService << " : " << TotalTime<< endl;
                SumOfCalcs += static_cast<int>(RequestQueue.size());
                CountOfCalcs++;

                if(( CountUseFReqs % 100 == 0 ) && (CountUseFReqs !=0))
                {
                    MiddleLenQueue = (SumOfCalcs * 1.0) / CountOfCalcs;
                    cout << endl;
                    cout << "Count of requests: " << CountUseFReqs << endl;
                    cout << "1 type requests enter: " << Enter2QueueFReqs << endl;
                    cout << "Count of second requests: " << countSecReqs << endl;
                    cout << "Now length of queue: " << static_cast<unsigned int>(RequestQueue.size()) << endl;
                    cout << "Now middle length of queue: " << MiddleLenQueue << endl;
                    cout << "Average Time: " << TotalTime/CountUseFReqs << endl;


                }
            }
            //cout << endl;
        }
    }
    cout << endl << "Total modeling time: " << TotalTime << endl;
    cout << endl << "Downtime: " << "0" << endl;
    cout << "1 type requests enter: " << Enter2QueueFReqs << endl;
    cout << "1 type requests out: " << CountUseFReqs << endl;
    cout << "Count of second requests: " << countSecReqs << endl;
    return 0;
}

